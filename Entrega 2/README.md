## 🧵 Problema Produtor-Consumidor — Simulação em C
Essa etapa do projeto apresenta uma simulação da clássica solução de concorrência do problema Produtor-Consumidor, no qual múltiplos processos ou threads compartilham um recurso comum (normalmente um buffer), e os mesmos precisam coordenar o acesso a esse recurso entre si de forma que não ocorra conflito entre ambas as faces, podendo levar a problemas como Overflow e Underflow.

## 🎯 Objetivo
Modelar um sistema onde múltiplos produtores e consumidores compartilham um buffer limitado, garantindo que haja:

Sincronização correta entre os processos e threads.

Evitar condição de corrida e acesso simultâneo, evitando a descoordenação do acesso ao buffer.

Controle de ciclos de produção/consumo, fazendo com que cada um acesse o buffer na sua vez (sem conflitos).

## ⚙️ Funcionalidades
Controle de número de ciclos de produção/consumo.

Logs das ações realizadas por produtores e consumidores.

Exibição de ocupação do buffer de forma interativa, mostrando para o usuário o passo a passo.

Finalização automática após 3 ciclos completos.

Sincronização eficiente com uso de semaforos, evitando condições de corrida e garantindo que os processos sejam executados de forma coordenada.

## 📐 Estrutura da Solução  

- **Buffer compartilhado** implementado como vetor circular
- **Semáforos:**
  - `mutex`: exclusão mútua para acesso ao buffer
  - `empty`: controla quantos espaços estão livres
  - `full`: controla quantos itens estão disponíveis
- **Produtores:** geram números aleatórios e os colocam no buffer
- **Consumidores:** retiram elementos do buffer e processam
- **Contador de ciclos:** incrementado a cada esvaziamento total do buffer

# Como executar

formulario  

![Captura de tela de 2025-05-07 21-39-07](https://github.com/user-attachments/assets/7a7b1f3a-b57e-4003-b18e-a8465fde3259)


tela inicial  


![Captura de tela de 2025-05-07 21-47-54](https://github.com/user-attachments/assets/1d83814b-2a93-4682-b02c-b6584a09f510)



após a 1 interação  

![Captura de tela de 2025-05-07 21-48-39](https://github.com/user-attachments/assets/203c8b3a-6c1b-4aca-805a-074cffac653d)


Resultados  

![Captura de tela de 2025-05-07 21-52-25](https://github.com/user-attachments/assets/47e4c7c9-1f5f-463f-bc13-79c635762de4)


