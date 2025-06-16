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
pthread_mutex_t ciclo_mutex = PTHREAD_MUTEX_INITIALIZER;

int produtores_ativos = 0;
int producao_finalizada = 0;
pthread_mutex_t prod_mutex = PTHREAD_MUTEX_INITIALIZER;

void registrar_step(const char *msg) {
    if (step_count < MAX_STEPS) {
        strncpy(steps[step_count], msg, STEP_MSG_SIZE - 1);
        steps[step_count][STEP_MSG_SIZE - 1] = '\0';
        step_count++;
    }
}

int get_buffer_occupancy() {
    int full_count;
    sem_getvalue(&full, &full_count);
    return full_count;
}

void* produtor(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&prod_mutex);
    produtores_ativos++;
    pthread_mutex_unlock(&prod_mutex);

    while (1) {
        pthread_mutex_lock(&ciclo_mutex);
        if (ciclos >= 3) {
            pthread_mutex_unlock(&ciclo_mutex);
            break;
        }
        pthread_mutex_unlock(&ciclo_mutex);

        sem_wait(&empty);
        sem_wait(&mutex);

        int item = rand() % 100;
        buffer[in] = item;
        in = (in + 1) % buffer_size;

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Produtor %d colocou %d no buffer", id, item);
        registrar_step(msg);

        if (get_buffer_occupancy() == buffer_size) {
            pthread_mutex_lock(&ciclo_mutex);
            if (ciclos < 3) {
            }
            pthread_mutex_unlock(&ciclo_mutex);
        }

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);  // Simula o tempo de produção
    }

    pthread_mutex_lock(&prod_mutex);
    produtores_ativos--;
    if (produtores_ativos == 0) {
        producao_finalizada = 1;
    }
    pthread_mutex_unlock(&prod_mutex);

    return NULL;
}

void* consumidor(void* arg) {
    int id = *((int*)arg);
    while (1) {
        pthread_mutex_lock(&ciclo_mutex);
        int fim_ciclos = (ciclos >= 3);
        pthread_mutex_unlock(&ciclo_mutex);

        int ocupado;
        sem_getvalue(&full, &ocupado);

        pthread_mutex_lock(&prod_mutex);
        int fim_producao = producao_finalizada;
        pthread_mutex_unlock(&prod_mutex);

        if (fim_ciclos && fim_producao && ocupado == 0) break;

        if (ocupado == 0) {
            usleep(50000);
            continue;
        }

        sem_wait(&full);
        sem_wait(&mutex);

        int item = buffer[out];
        out = (out + 1) % buffer_size;

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Consumidor %d retirou %d do buffer", id, item);
        registrar_step(msg);

        int ocupacao = get_buffer_occupancy();
        if (ocupacao == 0) {
            pthread_mutex_lock(&ciclo_mutex);
            if (ciclos < 3) {
                ciclos++;
            }
            pthread_mutex_unlock(&ciclo_mutex);
        }

        sem_post(&mutex);
        sem_post(&empty);
        usleep(150000);
    }
    return NULL;
}

void iniciar_simulacao(int num_prod, int num_cons, int tam_buf) {
    printf("🚀 imulação C iniciada!\n");  

    buffer_size = tam_buf;
    buffer = malloc(sizeof(int) * buffer_size);
    in = 0;
    out = 0;
    step_count = 0;
    ciclos = 0;
    producao_finalizada = 0;
    produtores_ativos = 0;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);

    pthread_t prod_threads[num_prod];
    pthread_t cons_threads[num_cons];
    int prod_ids[num_prod], cons_ids[num_cons];

    for (int i = 0; i < num_prod; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&prod_threads[i], NULL, produtor, &prod_ids[i]);
    }

    for (int i = 0; i < num_cons; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&cons_threads[i], NULL, consumidor, &cons_ids[i]);
    }

    for (int i = 0; i < num_prod; i++)
        pthread_join(prod_threads[i], NULL);
    for (int i = 0; i < num_cons; i++)
        pthread_join(cons_threads[i], NULL);
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
