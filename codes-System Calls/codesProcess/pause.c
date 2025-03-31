#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("\nSignal %d received\n", sig);
}

int main() {
    signal(SIGINT, handle_signal);

    printf("Press (Ctrl+C) to send a signal\n");

    pause();

    return 0;
}
