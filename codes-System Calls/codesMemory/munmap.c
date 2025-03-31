#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
   
    size_t length = 4096;
    void *addr;

    addr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    strcpy((char*)addr, "Memória alocada com mmap() e manipulada!");

    printf("Conteúdo na memória mapeada: %s\n", (char*)addr);

    if (munmap(addr, length) == -1) {
        perror("munmap");
        exit(1);
    }

    printf("Memória desalocada com sucesso!\n");

    return 0;
}

