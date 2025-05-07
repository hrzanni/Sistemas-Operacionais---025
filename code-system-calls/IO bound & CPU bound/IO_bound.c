#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FILENAME "IO_bound.txt"
#define FILE_SIZE_MB 1024
#define CHUNK_SIZE (4 * 1024 * 1024)

void generate_file() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        perror("Erro ao criar o arquivo");
        exit(1);
    }

    printf("Gerando arquivo de %d MB...\n", FILE_SIZE_MB);
    
    char *buffer = (char *)malloc(CHUNK_SIZE);
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        exit(1);
    }
    memset(buffer, 'a', CHUNK_SIZE);
    
    setvbuf(file, NULL, _IOFBF, CHUNK_SIZE);

    size_t total_chunks = (FILE_SIZE_MB * 1024 * 1024) / CHUNK_SIZE;
    
    for (size_t i = 0; i < total_chunks; i++) {
        fwrite(buffer, sizeof(char), CHUNK_SIZE, file);
    }

    fflush(file);
    fsync(fileno(file));

    fclose(file);
    free(buffer);
    printf("Arquivo gerado com sucesso!\n");
}

void read_file() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para leitura");
        exit(1);
    }

    printf("Lendo arquivo de %d MB...\n", FILE_SIZE_MB);
    
    char *buffer = (char *)malloc(CHUNK_SIZE);
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        fclose(file);
        exit(1);
    }
    
    setvbuf(file, NULL, _IOFBF, CHUNK_SIZE);

    size_t total_read = 0;

    while (fread(buffer, sizeof(char), CHUNK_SIZE, file) > 0) {
        total_read += CHUNK_SIZE;
    }

    fclose(file);
    free(buffer);

}

int main() {
    for (int i = 0; i < 3; i++) {
        printf("\n### Iteração %d ###\n", i + 1);
        generate_file();
        read_file();
    }
    return 0;
}
