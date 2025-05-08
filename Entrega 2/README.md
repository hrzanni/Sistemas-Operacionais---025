## 🧵 Problema Produtor-Consumidor — Simulação em C:
Essa etapa do projeto apresenta uma simulação da clássica solução de concorrência do problema Produtor-Consumidor, no qual múltiplos processos ou threads compartilham um recurso comum (normalmente um buffer), e os mesmos precisam coordenar o acesso a esse recurso entre si de forma que não ocorra conflito entre ambas as faces, podendo levar a problemas como Overflow e Underflow.

## 🎯 Objetivo:
Modelar um sistema onde múltiplos produtores e consumidores compartilham um buffer limitado, garantindo que haja:

- **Sincronização** correta entre as threads
- **Evitar condição de corrida** e acesso simultâneo
- **Controle de ciclos de produção/consumo**

## ⚙️ Funcionalidades:  

- Controle de **número de ciclos** de produção/consumo
- **Logs** das ações realizadas por produtores e consumidores
- **Exibição de ocupação** do buffer
- Finalização automática após 3 ciclos completos

## 📐 Estrutura da Solução:

- **Buffer compartilhado** implementado como vetor circular
- **Semáforos:**
  - `mutex`: exclusão mútua para acesso ao buffer
  - `empty`: controla quantos espaços estão livres
  - `full`: controla quantos itens estão disponíveis
- **Produtores:** geram números aleatórios e os colocam no buffer
- **Consumidores:** retiram elementos do buffer e processam
- **Contador de ciclos:** incrementado a cada esvaziamento total do buffer

# 🧪 Como Compilar e Executar:

Para rodar o nosso projeto é necessário baixar as pastas "code-system-calls" e "WebSite".  

Dentro da pasta WebSite, há o arquivo main.py, que contém as rotas do nosso projeto, ou seja, as abas de navagação do nosso site. Sendo assim, esse é o arquivo que precisa ser compilado.  

Após ser compilado, o programa vai liberar um link no terminal, basta copiar e colar o link em um navegador que aparecerá nosso site.

Essa é a tela inicial do nosso aplicativo web  
![Captura de tela de 2025-05-07 21-33-36](https://github.com/user-attachments/assets/6a015860-49b9-409b-8b37-4fb4631ab082)


Após isso, para acessar a Entrega 2 de nosso projeto há 2 jeitos. A primeira maneira é apertar no card que fizemos localizado no centro da tela e outra maneira é através da barra de navegação localizada na parte superior da guia.

Acessando a paginá da Entrega 2, o usuário perceberá uma pequena descrição sobre nosso projeto e nossa solução. Rolando um pouco para baixo, é possível perceber um formulário que levará ao nosso simulador Produtor/Consumidor. Nesse formulário, o usuário consegue decidir dinamicamente a quantidade de produtores, a quantidade de consumidores e o tamanho do buffer que será utilizado.
 
 
A partir disso, pode-se ver a interface do simulador do WebSite no qual o usuário pode inserir os parâmetros necessários para iniciar uma simulação do problema em questão.

![Captura de tela de 2025-05-07 21-39-07](https://github.com/user-attachments/assets/7a7b1f3a-b57e-4003-b18e-a8465fde3259)



Logo, inicia-se a simulação na qual é apresentada ao usuário o status completo da simulação, com o número de produtores, consumidores e o tamanho do buffer, selecionados pelo mesmo.

![Captura de tela de 2025-05-07 21-47-54](https://github.com/user-attachments/assets/1d83814b-2a93-4682-b02c-b6584a09f510)



Após a 1 interação, conseguimos perceber que o espaço reservado para o status do buffer é totalmente configurado, apresentando a quantidade de espaços livres no buffer, ocupados no buffer e a vizualização do buffer em tempo real. A cada interação, ou seja, a cada passo, esses status são atualizados.

Como apresentado abaixo, o simulador apresenta de forma interativa para o usúario todas as informações sobre o problema. São mostrados ao final o total de Produções, Consumos e o Tempo de execução que foi levado para completar o problema do caso escolhido pelo usuário. **Observação:** o tempo de duração da simulação é o tempo que o computador leva para fazer todo o processo e não o tempo que é necessário para a interface completa carregar para o usuário.

![Captura de tela de 2025-05-07 21-48-39](https://github.com/user-attachments/assets/203c8b3a-6c1b-4aca-805a-074cffac653d)


Dessa forma, tem-se a solução apresentada de forma de um simulador do WebSite simples e prático,  resolvendo a empecilho referido pela questão e evitando maiores complicações que já foram mencionadas nesse README com o passo a passo realizado pelo simulador, servindo como uma forma didática e de fácil acesso para o usuário que busca compreender como funciona a questão referida e ao mesmo tempo mostrando como a mesma pode ser resolvida interativamente.

![Captura de tela de 2025-05-07 21-52-25](https://github.com/user-attachments/assets/47e4c7c9-1f5f-463f-bc13-79c635762de4)


