#include <stdio.h>
#include <stdlib.h>

int main() {
    int opcao;

    printf("Escolha uma opção (1-9): ");
    scanf("%d", &opcao);

    printf("Você escolheu a opção %d!\n", opcao);

    switch(opcao) {
        case 1:
            system("docker exec sistemas-operacionais-025_met01_1 ./app");
            break;
        case 2:
            system("docker exec sistemas-operacionais-025_met02_1 ./app");
            break;
        case 3:
            system("docker exec sistemas-operacionais-025_met03_1 ./app");
            break;
        case 4:
            system("docker exec sistemas-operacionais-025_met04_1 ./app");
            break;
        case 5:
            system("docker exec sistemas-operacionais-025_met05_1 ./app");
            break;
        case 6:
            system("docker exec sistemas-operacionais-025_met06_1 ./app");
            break;
        case 7:
            system("docker exec sistemas-operacionais-025_met07_1 ./app");
            break;
        case 8:
            system("docker exec sistemas-operacionais-025_met08_1 ./app");
            break;
        case 9:
            system("docker exec sistemas-operacionais-025_met09_1 ./app");
            break;
        default:
            printf("Opção inválida.\n");
    }

    return 0;
}
