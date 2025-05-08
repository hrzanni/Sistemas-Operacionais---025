# 🧵 Problema Produtor-Consumidor — Simulação em C

Essa etapa do projeto apresenta uma simulação da clássica solução de concorrência do problema Produtor-Consumidor, no qual múltiplos processos ou threads compartilham um recurso comum (normalmente um buffer), e os mesmos precisam coordenar o acesso a esse recurso entre si de forma que não ocorra conflito entre ambas as faces, podendo levar a problemas como Overflow e Underflow.

## 🎯 Objetivo

Modelar um sistema onde múltiplos produtores e consumidores compartilham um buffer limitado, garantindo que haja:

- **Sincronização** correta entre os processos e threads.
- **Evitar condição de corrida** e acesso simultâneo evitando a descordenação do acesso ao buffer.
- **Controle de ciclos de produção/consumo** fazendo com que cada um acesse o buffer na sua vez (sem conflitos).

## ⚙️ Funcionalidades

- Controle de **número de ciclos** de produção/consumo.
- **Logs** das ações realizadas por produtores e consumidores.
- **Exibição de ocupação** do buffer de forma interativa, mostrando para o usúario o passo a passo.
- Finalização automática após 3 ciclos completos.

## 📐 Estrutura da Solução

- **Buffer compartilhado** implementado como vetor circular.
- **Semáforos:**
  - `mutex`: exclusão mútua para acesso ao buffer.
  - `empty`: controla quantos espaços estão livres.
  - `full`: controla quantos itens estão disponíveis.
- **Produtores:** geram números aleatórios e os colocam no buffer.
- **Consumidores:** retiram elementos do buffer e processam.
- **Contador de ciclos:** incrementado a cada esvaziamento total do buffer.

## 🧪 Como Compilar e Executar

```bash
gcc -o simulador simulador.c -lpthread
./simulador
