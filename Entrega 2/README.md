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
Buffer compartilhado implementado como vetor circular.

Semáforos:
*mutex*: Exclusão mútua para garantir que apenas um processo acesse o buffer por vez.
*empty*: Controla quantos espaços estão livres no buffer.
*full*: Controla quantos itens estão disponíveis para consumo no buffer.

Produtores: Geram números aleatórios e os colocam no buffer.
Consumidores: Retiram elementos do buffer e processam (simulando consumo).
Contador de ciclos: Incrementado a cada esvaziamento total do buffer, para gerenciar a quantidade de ciclos completos.
