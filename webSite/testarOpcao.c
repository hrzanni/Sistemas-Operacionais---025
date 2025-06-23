#include <stdio.h>
#include <stdlib.h>

int main() {
    int opcao;

    scanf("%d", &opcao);


    switch(opcao) {
        case 1:
            system("docker exec sistemas-operacionais-025-met01-1 ./app"); 
            break;
        case 2:
            system("docker exec sistemas-operacionais-025-met02-1 ./app");
            break;
        case 3:
            system("docker exec sistemas-operacionais-025-met03-1 ./app");
            break;
        case 4:
            system("docker exec sistemas-operacionais-025-met04-1 ./app");
            break;
        case 5:
            system("docker exec sistemas-operacionais-025-met05-1 ./app");
            break;
        case 6:
            system("docker exec sistemas-operacionais-025-met06-1 ./app");
            break;
        case 7:
            system("docker exec sistemas-operacionais-025-met07-1 ./app");
            break;
        case 8:
            system("docker exec sistemas-operacionais-025-met08-1 ./app");
            break;
        case 9:
            system("docker exec sistemas-operacionais-025-met09-1 ./app");
            break;
        case 10:
            system("docker exec sistemas-operacionais-025-iobound-1 ./app");
            break;
        case 11:
            system("docker exec sistemas-operacionais-025-cpubound-1 ./app");
            break;
        default:
            printf("Opção inválida.\n");
    }

    return 0;
}
