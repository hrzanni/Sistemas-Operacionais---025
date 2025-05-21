#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("\nSignal %d received\n", sig);
    fflush(stdout);  
}

int main() {
    setbuf(stdout, NULL);  

    printf("Press (Ctrl+Z) to send a signal\n");

    signal(SIGTSTP, handle_signal);

    pause();

    return 0;
}
