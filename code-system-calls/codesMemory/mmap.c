#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int *ptr;
    int n = 5; 

    // Aloca um bloco de memória de tamanho 'n' com acesso para leitura e escrita. 
    // MAP_PRIVATE e MAP_ANONYMOUS define que a memória é privada e anônima (não associada a nenhum arquivo).
    ptr = (int*)mmap(NULL, n * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    // Verifica a alocação.
    if (ptr == MAP_FAILED) {
        perror("Erro ao alocar memória com mmap");
        return 1;
    }

    // Preenche o bloco de memória mapeada com valores de 1 a n.
    for (int i = 0; i < n; i++) {
        ptr[i] = i + 1;
    }

    printf("Valores armazenados na memória:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");

    // Libera o espaço da memória mapeada.
    if (munmap(ptr, n * sizeof(int)) == -1) {
        perror("Erro ao liberar memória com munmap");
        return 1;
    }

    return 0;
}

