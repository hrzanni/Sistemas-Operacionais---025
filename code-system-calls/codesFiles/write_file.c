#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    int fd;

    // Declara um ponteiro para a string que será escrita no arquivo.
    char *message = "writing in my file!!\n";

    // Abre o arquivo e define que será utilizado somente para a escrita.
    fd = open("dados/myFile.txt", O_WRONLY | O_APPEND);
    if (fd < 0) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    // O número de bytes escritos deve ser igual ao comprimento da String que foi dada. No caso de erro, retorna uma mensagem e fecha o programa.
    if (write(fd, message, strlen(message)) < 0) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Confirma o sucesso enviando essa mensagem para o usuário.
    printf("Message written to 'myFile.txt'.\n");

    close(fd);
    return 0;
}
