#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    printf("O conteúdo do arquivo é:\n\n");

    // Abre o arquivo txt e define que o mesmo só poderá ser lido (utilizando 'O_RDONLY').
    int fd = open("dados/myFile.txt", O_RDONLY);

    // Caso o arquivo não exista, envia uma mensagem de erro para o usuário e encerra o programa.
    if (fd < 0) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    // Declara um buffer para armazenar os dados lidos.
    char buffer[BUFSIZ + 1];
    ssize_t bytesRead;

    // A função read retorna o número de bytes lidos e armazena os dados no buffer.
    while ((bytesRead = read(fd, buffer, BUFSIZ)) > 0){
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    // Verifica se houveram erros durante a leitura do arquivo. E em caso verdadeiro, imprime um erro na tela e finaliza o programa.
    if (bytesRead < 0) {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}
