#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("\nSignal %d received\n", sig);
}

int main() {
    printf("Press (Ctrl+C) to send a signal\n");
    
    signal(SIGINT, handle_signal);

    pause();

    return 0;
}
