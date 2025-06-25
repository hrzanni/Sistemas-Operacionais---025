    // Produtor/Consumidor

    // Inclusão das bibliotecas necessárias
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <semaphore.h>
    #include <string.h>

    // Definições de tamanho do log de passos
    #define MAX_STEPS 100
    #define STEP_MSG_SIZE 100

    // Número global de produtores
    int global_num_prod = 0;

    // Semáforos para sincronização
    sem_t empty, full, mutex;

    // Buffer circular
    int *buffer, buffer_size;
    int in = 0, out = 0;

    // Registro de etapas da simulação
    int step_count = 0;
    char steps[MAX_STEPS][STEP_MSG_SIZE];

    // Controle da produção
    int produtores_ativos = 0;
    int producao_finalizada = 0;

    // Mutexes adicionais
    pthread_mutex_t ciclo_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t prod_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Função que registra uma etapa da simulação no vetor steps
    void registrar_step(const char *msg) {
        if (step_count < MAX_STEPS) {
            strncpy(steps[step_count], msg, STEP_MSG_SIZE - 1);
            steps[step_count][STEP_MSG_SIZE - 1] = '\0';
            step_count++;
        }
    }

    // Função auxiliar que retorna quantos slots estão ocupados no buffer
    int get_buffer_occupancy() {
        int full_count;
        sem_getvalue(&full, &full_count);
        return full_count;
    }

    // Função executada por uma thread produtora
    void* produtor(void* arg) {
        int id = *((int*)arg);
        int total_to_produce = buffer_size;  // Cada produtor produz o tomanho buffer_size itens

        // Incrementa o número de produtores ativos com proteção por mutex
        pthread_mutex_lock(&prod_mutex);
        produtores_ativos++;
        pthread_mutex_unlock(&prod_mutex);

        // Loop de produção
        for (int i = 0; i < total_to_produce; i++) {

            sem_wait(&empty);   // Espera por um slot vazio no buffer
            sem_wait(&mutex);   // Garante acesso exclusivo ao buffer

            printf("[PRODUTOR %d] iniciou\n", id);

            // Gera um item aleatório, insere no buffer e atualiza o indice circular
            int item = rand() % 100;
            buffer[in] = item;
            in = (in + 1) % buffer_size;

            // Registra a ação no log
            char msg[STEP_MSG_SIZE];
            snprintf(msg, STEP_MSG_SIZE, "Produtor %d colocou %d no buffer", id, item);
            registrar_step(msg);

            sem_post(&mutex);   // Libera acesso ao buffer
            sem_post(&full);     // Indica que um novo item está disponível

            usleep(100000);  // Simula o tempo de produção
            printf("[PRODUTOR %d] terminou\n", id);

        }

        // Ao final, decrementa o número de produtores ativos
        pthread_mutex_lock(&prod_mutex);
        produtores_ativos--;
        if (produtores_ativos == 0) {
            producao_finalizada = 1;    // Marca fim da produção
        }
        pthread_mutex_unlock(&prod_mutex);

        return NULL;
    }

    // Função executada por uma thread consumidora
    void* consumidor(void* arg) {
        int id = *((int*)arg);

        while (1) {

            int ocupado;
            sem_getvalue(&full, &ocupado);  // Verifica se há itens a consumir

            pthread_mutex_lock(&prod_mutex);
            int fim_producao = producao_finalizada;
            pthread_mutex_unlock(&prod_mutex);

            // Se não há mais itens e a produção acabou, termina
            if (ocupado == 0 && fim_producao) break;

            // Se não há itens mas ainda há produção, espera
            if (ocupado == 0) {
                usleep(50000);
                continue;
            }

            sem_wait(&full);    // Espera por um item disponível
            sem_wait(&mutex);   // Garante acesso exclusivo ao buffer

            printf("[CONSUMIDOR %d] iniciou\n", id);

            // Consome item e atualiza índice circular
            int item = buffer[out];
            out = (out + 1) % buffer_size;

            // Registra a ação no log
            char msg[STEP_MSG_SIZE];
            snprintf(msg, STEP_MSG_SIZE, "Consumidor %d retirou %d do buffer", id, item);
            registrar_step(msg);


            sem_post(&mutex);   // Libera o buffer
            sem_post(&empty);   // Indica um novo slot vazio

            usleep(150000);      // Simula tempo de consumo
            printf("[CONSUMIDOR %d] terminou\n", id);

        }
        return NULL;
    }

    // Função que inicializa a simulação com o número de produtores, consumidores e tamanho do buffer
    void main(int num_prod, int num_cons, int tam_buf) {
        
        printf("🚀 Simulação do Produtor-Consumidor iniciada!\n"); 

        // Reset do log
        step_count = 0;
        for (int i = 0; i < MAX_STEPS; i++) {
            steps[i][0] = '\0';
        }

        // Inicializações
        global_num_prod = num_prod;
        buffer_size = tam_buf;
        buffer = malloc(sizeof(int) * buffer_size);
        in = 0;
        out = 0;
        producao_finalizada = 0;
        produtores_ativos = 0;

        // Inicialização dos semáforos
        sem_init(&mutex, 0, 1);
        sem_init(&empty, 0, buffer_size);
        sem_init(&full, 0, 0);

        // Arrays de threads e IDs
        pthread_t prod_threads[num_prod];
        pthread_t cons_threads[num_cons];
        int prod_ids[num_prod], cons_ids[num_cons];

        // Criação das threads produtoras
        for (int i = 0; i < num_prod; i++) {
            prod_ids[i] = i + 1;
            pthread_create(&prod_threads[i], NULL, produtor, &prod_ids[i]);
        }

        // Criação das threads consumidoras
        for (int i = 0; i < num_cons; i++) {
            cons_ids[i] = i + 1;
            pthread_create(&cons_threads[i], NULL, consumidor, &cons_ids[i]);
        }   

        // Espera o término das threads produtoras
        for (int i = 0; i < num_prod; i++)
            pthread_join(prod_threads[i], NULL);
        
        printf("✅ Todos os produtores terminaram\n");

        // Espera o término das threads consumidoras
        for (int i = 0; i < num_cons; i++)
            pthread_join(cons_threads[i], NULL);

        printf("✅ Todos os consumidores terminaram\n");

        // Liberação de recursos
        sem_destroy(&mutex);
        sem_destroy(&empty);
        sem_destroy(&full);
        free(buffer);
        buffer = NULL;

        // Reset de variáveis
        buffer_size = 0;
        in = out = 0;
        //step_count = 0;
        producao_finalizada = 0;
        produtores_ativos = 0;
    }


    // Funções auxiliares para acessar o log da simulação

    int get_step_count() {
        return step_count;
    }

    const char* get_step(int index) {
        if (index >= 0 && index < step_count)
            return steps[index];
        return "";
    }

    // Copia todo o log da simulação para um buffer externo
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

    // Obtém o estado atual do buffer e dos semáforos
    void get_status(int* empty_slots, int* full_slots, int* buffer_out, int buf_size) {
        sem_getvalue(&empty, empty_slots);
        sem_getvalue(&full, full_slots);
        for (int i = 0; i < buf_size; i++) {
            buffer_out[i] = buffer[i];
        }
    }
