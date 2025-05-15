#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("\nSignal %d received\n", sig);
    fflush(stdout);  // Garante que o printf dentro do signal handler seja exibido
}

int main() {
    setbuf(stdout, NULL);  // Desabilita buffer da saída padrão

    printf("Press (Ctrl+Z) to send a signal\n");

    signal(SIGTSTP, handle_signal);

    pause();

    return 0;
}
