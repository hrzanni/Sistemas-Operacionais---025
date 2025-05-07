#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int *ptr;
    int n = 5; 

    ptr = (int*)mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        perror("Erro ao alocar memória com mmap");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        ptr[i] = i + 1;
    }

    printf("Valores armazenados na memória:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    if (munmap(ptr, n * sizeof(int)) == -1) {
        perror("Erro ao liberar memória com munmap");
        return 1;
    }

    return 0;
}

