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

    // Aqui, você pode colocar a lógica para cada opção (1-9)
    if (opcao == 1)
    {
      system("docker start -ai met01");
      system("docker exec -it sistemas-operacionais_met01 ./app");
    }
    else if (opcao == 2)
    {
      system("docker start -ai met02");
      system("docker exec -it sistemas-operacionais_met02 ./app");
    }
    else if (opcao == 3)
    {
      system("docker start -ai met03");
      system("docker exec -it sistemas-operacionais_met03 ./app");
    }
    else if (opcao == 4)
    {
      system("docker start -ai met04");
      system("docker exec -it sistemas-operacionais_met04 ./app");
    }
    else if (opcao == 5)
    {
      system("docker start -ai met05");
      system("docker exec -it sistemas-operacionais_met05 ./app");
    }
    else if (opcao == 6)
    {
      system("docker start -ai met06");
      system("docker exec -it sistemas-operacionais_met06 ./app");
    }
    else if (opcao == 7)
    {
      system("docker start -ai met07");
      system("docker exec -it sistemas-operacionais_met07 ./app");
    }
    else if (opcao == 8)
    {
      system("docker start -ai met08");
      system("docker exec -it sistemas-operacionais_met08 ./app");
    }
    else if (opcao == 9)
    {
      system("docker start -ai met09");
      system("docker exec -it sistemas-operacionais_met09 ./app");
    }
    else
    {
      printf("<p>Opção inválida.</p>");
    }
  }
  else
  {
    printf("<h1>Erro: Nenhuma opção foi selecionada.</h1>");
  }

  // Rodapé
  print_html_footer();

  return 0;
}