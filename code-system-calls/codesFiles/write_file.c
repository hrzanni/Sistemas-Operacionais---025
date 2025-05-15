#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int fd;
    char *message = "writing in my file!!\n";

    fd = open("dados/myFile.txt", O_WRONLY | O_APPEND);
    if (fd < 0) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    if (write(fd, message, strlen(message)) < 0) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Message written to 'myFile.txt'.\n");

    close(fd);
    return 0;
}
