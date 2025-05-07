## Esse README contém as estatísticas relacionadas ao uso de chamadas ao sistema em cada um dos programas desenvolvidos no diretório "codesProcess"

## Processo -> fork

Para criar um script executável do arquivo fork.c :
```bash
gcc -o <nome_arquivo_executavel> fork.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depois de criar o executável, aqui está um exeplo de como rodar o programa:

![fork-rodar](https://github.com/user-attachments/assets/31bf8099-13f0-4b95-929e-219040c77b76)

Nesse programa, cria um processo filho usando fork(). O processo pai exibe seu próprio ID e o ID do filho, depois chama a função wait() para esperar o filho terminar. O processo filho imprime seu próprio ID e o valor de pid. Ambos executam a função system("date"), então a data e hora são exibidas duas vezes (uma pelo pai e outra pelo filho).
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas:
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
![fork-strace](https://github.com/user-attachments/assets/cf1e95df-5167-49d6-a56a-e90e1ae07c87)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![fork-time-v](https://github.com/user-attachments/assets/33d3229b-a9d0-4ca5-bfc6-d3d098fbd701)

A chamada de sistema fork é tratada como CPU-bound porque ela envolve cálculos e manipulação ativa da CPU para criar um novo processo, como visto nas estatísticas fornecidas pela ferramenta 'time'. O fato de a maior parte do tempo ser consumida pela CPU (80%) e o pequeno tempo total de execução indicam que a criação de um novo processo exige uso significativo de CPU, sem envolvimento com operações de I/O, como leitura ou escrita em disco.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel: 

![fork-time](https://github.com/user-attachments/assets/a30f8c07-ff18-4ec2-8b4e-872cedf8c2da)

<br>
<br>

## Processo -> sleep

Para criar um script executável do arquivo sleep.c :
```bash
gcc -o <nome_arquivo_executavel> sleep.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depois de criar o executável, aqui está um exeplo de como rodar o programa: 

![sleep-rodar](https://github.com/user-attachments/assets/0d2348b4-7768-42f3-964c-65f9f093fb1b)

Esse programa funciona como um contador, ele começa imprimindo o número 1 na tela, depois de 1 segundo ele sobrescreve o 1 pelo número 2 e assim por diante até chegar no número 5.  
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas 
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```

![sleep-strace](https://github.com/user-attachments/assets/d2da192e-0014-4a19-aede-c5acd77840ed)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![sleep-time-v](https://github.com/user-attachments/assets/ac644cbb-9a86-4eb7-9f3f-f9b7047d74e7)

Nesse caso, a chamada de sistema sleep é considerada como I/O-bound por mais que não tenha o envolvimento com operações de escrita ou leitura durante sua execução. Isso ocorre por que o processo não faz uso ativo da CPU durante sua execução fazendo com que o mesmo fique ''aguardando'' por um determinado tempo sem realizar qualquer operação computacional, o que caracteriza um processo que não depende da CPU e se mantém em estado de inatividade ou espera. 

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel:

![sleep-time](https://github.com/user-attachments/assets/8c4cebeb-e475-4727-9169-da028cff3a7a)

<br>
<br>

## Processo -> pause

Para criar um script executável do arquivo pause.c :
```bash
gcc -o <nome_arquivo_executavel> pause.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depois de criar o executável, aqui está um exeplo de como rodar o programa:

![pause-rodar](https://github.com/user-attachments/assets/30b4d33d-f798-4e5a-a660-5071d52b4fcf)

O programa captura o sinal SIGINT (enviado ao pressionar Ctrl+C) e executa a função handle_signal(), que imprime a mensagem "Signal X received", onde X é o número do sinal. A função pause() faz o programa aguardar indefinidamente até receber um sinal<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas:
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
![pause-strace](https://github.com/user-attachments/assets/76901704-7a23-4bfc-8f94-42b8858cf610)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![pause-time-v](https://github.com/user-attachments/assets/6370ee8e-76a5-4414-a898-8d4f54235c91)

A chamada pause é classificada como I/O-bound porque, assim como a chamada sleep, o processo não utiliza a CPU durante sua execução e fica apenas aguardando por um sinal. Esse comportamento caracteriza um processo que não realiza operações ativas de cálculo ou I/O, mas sim fica em espera passiva até que um evento externo (o sinal) o interrompa.


<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel: 

![pause-time](https://github.com/user-attachments/assets/9876b96f-1518-433e-bfb7-761a4fb264d4)
