#include <fcntl.h>     
#include <unistd.h>    
#include <stdio.h>     
#include <stdlib.h>    

int main() {
    int fd; 

    printf("Tentando abrir: myFile.txt\n");

    // Tenta abrir o arquivo txt e, em caso de não existência, cria um arquivo através do 'O_CREATE'.
    // O arquivo, nesse caso, será aberto somente para a escrita (através do 'O_WRONLY') e qualquer conteúdo existente será apagado com o uso de 'O_TRUNC'.
    fd = open("dados/myFile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Imprime essa mensagem no caso de uma abertura de arquivo bem sucedida.
    printf("file 'myFile.txt' opened successfully\n");

    close(fd);
    return 0;
}
