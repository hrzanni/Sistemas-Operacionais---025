// Produtor/Consumidor

// Inclusão das bibliotecas necessárias
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

// Definições de tamanho do log de passos
#define MAX_STEPS 300
#define STEP_MSG_SIZE 256

// Semáforos para sincronização
sem_t empty, full, mutex;

// Buffer circular
int *buffer, buffer_size;
int in = 0, out = 0;

// Registro de etapas da simulação
int step_count = 0;
char steps[MAX_STEPS][STEP_MSG_SIZE];

// Contador de ciclos
int ciclos = 0;
int ciclos_max = 3;

// Controle da produção
int produtores_ativos = 0;
int producao_finalizada = 0;

// Controle de ciclos de produção/consumo
int buffer_uso = 0;
int consumo_no_ciclo = 0;

// Mutexes auxiliares para garantir exclusão mútua em regiões críticas
pthread_mutex_t ciclo_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prod_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t uso_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ciclo_control_mutex = PTHREAD_MUTEX_INITIALIZER;

// Salva passo e buffer atualizado
void registrar_step(const char *msg) {
    if (step_count < MAX_STEPS) {
        strncpy(steps[step_count], msg, STEP_MSG_SIZE - 1);
        steps[step_count][STEP_MSG_SIZE - 1] = '\0';
        step_count++;

        if (step_count < MAX_STEPS) {
            int buf[buffer_size];
            pthread_mutex_lock(&uso_mutex);
            int full_slots = buffer_uso;
            int empty_slots = buffer_size - buffer_uso;
            pthread_mutex_unlock(&uso_mutex);

            for (int j = 0; j < buffer_size; j++) {
                buf[j] = buffer[j];
            }

            char buffer_log[STEP_MSG_SIZE];
            snprintf(buffer_log, STEP_MSG_SIZE, "[BUFFER] {\"empty\": %d, \"full\": %d, \"buffer\": [", empty_slots, full_slots);
            int len = strlen(buffer_log);
            for (int j = 0; j < buffer_size; j++) {
                if (buf[j] == -1) {
                    len += snprintf(buffer_log + len, STEP_MSG_SIZE - len, "null");
                } else {
                    len += snprintf(buffer_log + len, STEP_MSG_SIZE - len, "%d", buf[j]);
                }
                if (j < buffer_size - 1) {
                    len += snprintf(buffer_log + len, STEP_MSG_SIZE - len, ", ");
                }
            }
            snprintf(buffer_log + len, STEP_MSG_SIZE - len, "]}");

            strncpy(steps[step_count], buffer_log, STEP_MSG_SIZE - 1);
            steps[step_count][STEP_MSG_SIZE - 1] = '\0';
            step_count++;
        }
    }
}

// Função executada por cada thread produtora
void* produtor(void* arg) {
    int id = *((int*)arg);
    free(arg);
    printf("Produtor %d iniciado\n", id);

    // Marca produtor como ativo
    pthread_mutex_lock(&prod_mutex);
    produtores_ativos++;
    pthread_mutex_unlock(&prod_mutex);

    while (1) {
        // Verifica se os ciclos acabaram
        pthread_mutex_lock(&ciclo_mutex);
        if (ciclos >= ciclos_max) {
            pthread_mutex_unlock(&ciclo_mutex);
            break;
        }
        pthread_mutex_unlock(&ciclo_mutex);

        sem_wait(&empty);   // Espera buffer ter espaço
        sem_wait(&mutex);   // Região crítica

        int item = rand() % 100;
        buffer[in] = item;
        in = (in + 1) % buffer_size;

        pthread_mutex_lock(&uso_mutex);
        buffer_uso++;
        pthread_mutex_unlock(&uso_mutex);

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Produtor %d colocou %d no buffer", id, item);
        registrar_step(msg);

        sem_post(&mutex);   // Libera região crítica
        sem_post(&full);    // Sinaliza que há item disponível

        usleep(100000);     // Sinaliza que há item disponível
    }

    // Atualiza controle de produtores ativos
    pthread_mutex_lock(&prod_mutex);
    produtores_ativos--;
    if (produtores_ativos == 0) {
        producao_finalizada = 1;
    }
    pthread_mutex_unlock(&prod_mutex);

    printf("Produtor %d finalizado\n", id);
    return NULL;
}

// Função executada por cada thread consumidora
void* consumidor(void* arg) {
    int id = *((int*)arg);
    free(arg);
    printf("Consumidor %d iniciado\n", id);

    while (1) {
        pthread_mutex_lock(&ciclo_mutex);
        int fim_ciclos = (ciclos >= ciclos_max);
        pthread_mutex_unlock(&ciclo_mutex);

        pthread_mutex_lock(&prod_mutex);
        int fim_producao = producao_finalizada;
        pthread_mutex_unlock(&prod_mutex);

        int ocupado;
        sem_getvalue(&full, &ocupado);  // Verifica se há itens para consumir


        // Condição de parada: produção acabou e buffer está vazio
        if (fim_ciclos && fim_producao && ocupado == 0) break;

        if (ocupado == 0) {
            usleep(50000);  // Espera um pouco antes de tentar novamente
            continue;
        }

        sem_wait(&full);    // Espera item disponível
        sem_wait(&mutex);   // Região crítica

        int item = buffer[out];
        buffer[out] = -1;  // marca como consumido
        out = (out + 1) % buffer_size;

        pthread_mutex_lock(&uso_mutex);
        buffer_uso--;
        pthread_mutex_unlock(&uso_mutex);

        // Controle de ciclo
        pthread_mutex_lock(&ciclo_control_mutex);
        consumo_no_ciclo++;
        if (consumo_no_ciclo == buffer_size) {
            pthread_mutex_lock(&ciclo_mutex);
            if (ciclos < ciclos_max) {
                ciclos++;
                printf("Ciclo atual: %d\n", ciclos);
            }
            pthread_mutex_unlock(&ciclo_mutex);
            consumo_no_ciclo = 0;
        }
        pthread_mutex_unlock(&ciclo_control_mutex);

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Consumidor %d retirou %d do buffer", id, item);
        registrar_step(msg);

        sem_post(&mutex);   // Libera buffer
        sem_post(&empty);   // Sinaliza que há espaço disponível
        usleep(150000);     // Simula tempo de consumo
    }

    printf("Consumidor %d finalizado\n", id);
    return NULL;
}

// Função principal da simulação
void simular(int num_prod, int num_cons, int tam_buf) {
    printf("🚀 Simulação C iniciada!\n");

    buffer_size = tam_buf;
    buffer = malloc(sizeof(int) * buffer_size);
    for (int i = 0; i < buffer_size; i++) buffer[i] = -1;

    // Zera variáveis globais
    in = out = step_count = ciclos = consumo_no_ciclo = 0;
    producao_finalizada = 0;
    produtores_ativos = 0;

    // Inicializa semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);

    // Cria threads produtoras e consumidoras
    pthread_t prod_threads[num_prod];
    pthread_t cons_threads[num_cons];

    for (int i = 0; i < num_prod; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&prod_threads[i], NULL, produtor, arg);
    }

    for (int i = 0; i < num_cons; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i + 1;
        pthread_create(&cons_threads[i], NULL, consumidor, arg);
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_prod; i++)
        pthread_join(prod_threads[i], NULL);
    for (int i = 0; i < num_cons; i++)
        pthread_join(cons_threads[i], NULL);

    // Grava log da simulação em arquivo
    FILE *fp = fopen("dados/saidaProducer.txt", "w");
    if (fp) {
        for (int i = 0; i < step_count; i++) {
            fprintf(fp, "%s\n", steps[i]);
        }
        fclose(fp);
    }

    printf("✅ Simulação finalizada com sucesso!\n");
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <num_produtores> <num_consumidores> <tam_buffer>\n", argv[0]);
        return 1;
    }

    int num_prod = atoi(argv[1]);
    int num_cons = atoi(argv[2]);
    int tam_buf = atoi(argv[3]);

    simular(num_prod, num_cons, tam_buf);
    return 0;
}

// Funções auxiliares para acesso externo aos dados da simulação
int get_step_count() {
    return step_count;
}

const char* get_step(int index) {
    if (index >= 0 && index < step_count)
        return steps[index];
    return "";
}

int get_simulation_log(char *log_buffer, int max_len) {
    int len = 0;
    for (int i = 0; i < step_count && len < max_len - 1; i++) {
        int step_len = strlen(steps[i]);
        if (len + step_len + 1 < max_len) {
            strcat(log_buffer, steps[i]);
            strcat(log_buffer, "\n");
            len += step_len + 1;
        }
    }
    return len;
}

// Retorna o estado atual do buffer
void get_status(int* empty_slots, int* full_slots, int* buffer_out, int buf_size) {
    sem_getvalue(&empty, empty_slots);
    sem_getvalue(&full, full_slots);
    for (int i = 0; i < buf_size; i++) {
        buffer_out[i] = buffer[i];
    }
}

// Mostra o estado atual do buffer no terminal
void log_buffer_status() {
    int empty_slots, full_slots;
    int buf[buffer_size];
    sem_getvalue(&empty, &empty_slots);
    sem_getvalue(&full, &full_slots);

    for (int i = 0; i < buffer_size; i++) {
        buf[i] = buffer[i];
    }

    printf("[BUFFER] {\"empty\": %d, \"full\": %d, \"buffer\": [", empty_slots, full_slots);
    for (int i = 0; i < buffer_size; i++) {
        printf("%d", buf[i]);
        if (i < buffer_size - 1) printf(", ");
    }
    printf("]}\n");
}
