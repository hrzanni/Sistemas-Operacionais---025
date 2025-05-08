#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função que exibe o cabeçalho de resposta HTTP
void print_html_header()
{
  printf("Content-type: text/html\n\n");
  printf("<html><head><title>Resposta</title></head><body>");
}

// Função que exibe o rodapé de resposta HTML
void print_html_footer()
{
  printf("</body></html>");
}

int main()
{
  // Cabeçalho para a resposta HTTP
  print_html_header();

  // Variáveis para capturar a opção
  char *query_string = getenv("QUERY_STRING");
  if (query_string != NULL)
  {
    // Aqui, vamos obter a opção escolhida pelo usuário
    int opcao;
    sscanf(query_string, "opcao=%d", &opcao);

    // Dependendo da opção, fazemos algo
    printf("<h1>Você escolheu a opção %d!</h1>", opcao);
    printf("<p>Agora, você pode ver o resultado baseado nessa escolha.</p>");

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
  }

  // Rodapé
  print_html_footer();

  return 0;
}