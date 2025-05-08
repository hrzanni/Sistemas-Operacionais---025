#include <fcntl.h>     
#include <unistd.h>    
#include <stdio.h>     
#include <stdlib.h>    

int main() {
    int fd;

    fd = open("./dados/myFile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("file 'myFile.txt' opened successfully\n");

    close(fd);
    return 0;
}
