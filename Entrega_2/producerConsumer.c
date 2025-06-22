
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define MAX_STEPS 100
#define STEP_MSG_SIZE 100

sem_t empty, full, mutex;
int *buffer, buffer_size;
int in = 0, out = 0;
int step_count = 0;
char steps[MAX_STEPS][STEP_MSG_SIZE];

int ciclos = 0;
int ciclos_max = 3;

int produtores_ativos = 0;
int producao_finalizada = 0;

int buffer_uso = 0;
int consumo_no_ciclo = 0;

pthread_mutex_t ciclo_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t prod_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t uso_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ciclo_control_mutex = PTHREAD_MUTEX_INITIALIZER;

void registrar_step(const char *msg) {
    if (step_count < MAX_STEPS) {
        strncpy(steps[step_count], msg, STEP_MSG_SIZE - 1);
        steps[step_count][STEP_MSG_SIZE - 1] = '\0';
        step_count++;
    }
}

void* produtor(void* arg) {
    int id = *((int*)arg);
    free(arg);
    printf("Produtor %d iniciado\n", id);

    pthread_mutex_lock(&prod_mutex);
    produtores_ativos++;
    pthread_mutex_unlock(&prod_mutex);

    while (1) {
        pthread_mutex_lock(&ciclo_mutex);
        if (ciclos >= ciclos_max) {
            pthread_mutex_unlock(&ciclo_mutex);
            break;
        }
        pthread_mutex_unlock(&ciclo_mutex);

        sem_wait(&empty);
        sem_wait(&mutex);

        int item = rand() % 100;
        buffer[in] = item;
        in = (in + 1) % buffer_size;

        pthread_mutex_lock(&uso_mutex);
        buffer_uso++;
        pthread_mutex_unlock(&uso_mutex);

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Produtor %d colocou %d no buffer", id, item);
        registrar_step(msg);

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);
    }

    pthread_mutex_lock(&prod_mutex);
    produtores_ativos--;
    if (produtores_ativos == 0) {
        producao_finalizada = 1;
    }
    pthread_mutex_unlock(&prod_mutex);

    printf("Produtor %d finalizado\n", id);
    return NULL;
}

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
        sem_getvalue(&full, &ocupado);

        if (fim_ciclos && fim_producao && ocupado == 0) break;

        if (ocupado == 0) {
            usleep(50000);
            continue;
        }

        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        out = (out + 1) % buffer_size;

        pthread_mutex_lock(&uso_mutex);
        buffer_uso--;
        pthread_mutex_unlock(&uso_mutex);

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

        sem_post(&mutex);
        sem_post(&empty);
        usleep(150000);
    }

    printf("Consumidor %d finalizado\n", id);
    return NULL;
}

void simular(int num_prod, int num_cons, int tam_buf) {
    printf("🚀 Simulação C iniciada!\n");

    buffer_size = tam_buf;
    buffer = malloc(sizeof(int) * buffer_size);
    in = out = step_count = ciclos = consumo_no_ciclo = 0;
    producao_finalizada = 0;
    produtores_ativos = 0;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);

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

    for (int i = 0; i < num_prod; i++)
        pthread_join(prod_threads[i], NULL);
    for (int i = 0; i < num_cons; i++)
        pthread_join(cons_threads[i], NULL);

    FILE *fp = fopen("dados/saidaProducer.txt", "w");
    if (fp) {
        for (int i = 0; i < step_count; i++) {
            fprintf(fp, "%s\n", steps[i]);
        }
        fclose(fp);
    }

    printf("✅ Simulação finalizada com sucesso!\n");
}

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

void get_status(int* empty_slots, int* full_slots, int* buffer_out, int buf_size) {
    sem_getvalue(&empty, empty_slots);
    sem_getvalue(&full, full_slots);
    for (int i = 0; i < buf_size; i++) {
        buffer_out[i] = buffer[i];
    }
}