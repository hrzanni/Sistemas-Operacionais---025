#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    printf("O conteúdo do arquivo é:\n\n");

    int fd = open("dados/myFile.txt", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFSIZ + 1];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUFSIZ)) > 0){
        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    if (bytesRead < 0) {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }


    close(fd);
    return 0;
}
