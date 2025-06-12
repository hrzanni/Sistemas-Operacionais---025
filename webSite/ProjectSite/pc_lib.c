// simulador.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define MAX_STEPS 100
#define STEP_MSG_SIZE 100

int global_num_prod = 0;
sem_t empty, full, mutex;
int *buffer, buffer_size;
int in = 0, out = 0;
int step_count = 0;
char steps[MAX_STEPS][STEP_MSG_SIZE];

int produtores_ativos = 0;
int producao_finalizada = 0;

pthread_mutex_t ciclo_mutex = PTHREAD_MUTEX_INITIALIZER;
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
    printf(">>> Produtor %d: global_num_prod = %d\n", id, global_num_prod);
    int total_to_produce = buffer_size;


    pthread_mutex_lock(&prod_mutex);
    produtores_ativos++;
    pthread_mutex_unlock(&prod_mutex);

    for (int i = 0; i < total_to_produce; i++) {

        sem_wait(&empty);
        sem_wait(&mutex);

        printf("[PRODUTOR %d] iniciou\n", id);

        int item = rand() % 100;
        buffer[in] = item;
        in = (in + 1) % buffer_size;

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Produtor %d colocou %d no buffer", id, item);
        registrar_step(msg);

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);  // Simula o tempo de produção
        printf("[PRODUTOR %d] terminou\n", id);

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

        int ocupado;
        sem_getvalue(&full, &ocupado);

        pthread_mutex_lock(&prod_mutex);
        int fim_producao = producao_finalizada;
        pthread_mutex_unlock(&prod_mutex);

        if (ocupado == 0 && fim_producao) break;

        if (ocupado == 0) {
            usleep(50000);
            continue;
        }

        sem_wait(&full);
        sem_wait(&mutex);

        printf("[CONSUMIDOR %d] iniciou\n", id);

        int item = buffer[out];
        out = (out + 1) % buffer_size;

        char msg[STEP_MSG_SIZE];
        snprintf(msg, STEP_MSG_SIZE, "Consumidor %d retirou %d do buffer", id, item);
        registrar_step(msg);


        sem_post(&mutex);
        sem_post(&empty);

        usleep(150000);
        printf("[CONSUMIDOR %d] terminou\n", id);

    }
    return NULL;
}

void iniciar_simulacao(int num_prod, int num_cons, int tam_buf) {
    
    printf("🚀 Simulação do Produtor-Consumidor iniciada!\n"); 


    step_count = 0;
    for (int i = 0; i < MAX_STEPS; i++) {
        steps[i][0] = '\0';
    }
    global_num_prod = num_prod;
    buffer_size = tam_buf;
    buffer = malloc(sizeof(int) * buffer_size);
    in = 0;
    out = 0;
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
    
    printf("✅ Todos os produtores terminaram\n");

    for (int i = 0; i < num_cons; i++)
        pthread_join(cons_threads[i], NULL);

    printf("✅ Todos os consumidores terminaram\n");

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    free(buffer);
    buffer = NULL;

    buffer_size = 0;
    in = out = 0;
    //step_count = 0;
    producao_finalizada = 0;
    produtores_ativos = 0;
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
