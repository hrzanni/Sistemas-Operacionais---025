## Esse README contém as estatísticas relacionadas ao uso de chamadas ao sistema em cada um dos programas desenvolvidos no diretório "codesFiles"

## Arquivo -> open_file

Para criar um script executável do arquivo open_file.c:
```bash
gcc -o <nome_arquivo_executavel> open_file.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depos de criar o executável, aqui está um exeplo de como rodar o programa: 

![open_file-rodar](https://github.com/user-attachments/assets/0513aeab-cd8f-411d-97ba-0742862ae191)

Nesse programa, o arquivo 'myFile.txt' foi aberto pela função open() e é mostrado um texto na tela avisando se o arquivo foi aberto com sucesso. E a função close() é utilizada para fechar o arquivo.
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas:
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
![open c](https://github.com/user-attachments/assets/9bdb2c75-f9fa-4e71-823f-1bcbac8228fb)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![Captura de tela de 2025-03-30 19-27-37](https://github.com/user-attachments/assets/035c6770-469b-4773-974f-56596c6eb706)

Perante os dados acima, conseguimos afirmar que é um comportamento **I/O-bound**. A métrica "File system inputs: 32", indicando que a leitura do arquivo envolveu acessos reais ao disco. Também podemos falar que a CPU foi pouca utilizada (40%), o que sugere que o processo passou mais tempo aguardando o disco do que executando cálculos e que pode ser considerado I/O-bound.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel:

![Captura de tela de 2025-03-30 19-29-51](https://github.com/user-attachments/assets/600dfd37-c55d-4fdf-b625-2f4830a65c69)

<br>
<br>

## Arquivo -> write_file

Para criar um script executável do arquivo write_file.c:
```bash
gcc -o <nome_arquivo_executavel> write_file.c
```
Para rodar o script executável:
```bash
./<nome_arquivo_executavel>
```
<br>

Depois de criar o executável, aqui está um exeplo de como rodar o programa:

![write_file-rodar](https://github.com/user-attachments/assets/10caf3b4-4190-4663-8331-903d49b916f0)

Nesse programa, o arquivo 'myFile.txt' foi aberto pelo função open(), logo após isso, a função write escreve uma mensagem no arquivo e aparece uma mensagem na tela avisando que a mensagem foi escrita no arquivo. E a função close() é utilizada para fechar o arquivo.
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas:
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```

![Captura de tela de 2025-03-30 20-02-01](https://github.com/user-attachments/assets/2a26d179-134c-4799-82e9-a585bfbeb7dc)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![Captura de tela de 2025-03-30 19-48-50](https://github.com/user-attachments/assets/5841fe69-b7f4-4445-9f37-e2441026f18a)  

Pelos resultados, conseguimos afirmar que é um comportamento **I/O-bound**. A métrica "File system outputs: 8" que indica que o programa realizou operações de escrita no disco. Também podemos falar que a CPU foi pouca utilizada (40%), o que sugere que o processo passou mais tempo aguardando o disco do que executando cálculos.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel: 

![Captura de tela de 2025-03-30 19-47-54](https://github.com/user-attachments/assets/268ff14e-5c66-43aa-ac6c-2546bde20d06)

<br>
<br>

## Arquivo -> read_file

Para criar um script executável do arquivo read_file.c:
```bash
gcc -o <nome_arquivo_executavel> read_file.c
```
Para rodar o script executável:
```bash
./<nome_arquivo_executavel>
```
<br>

Depois de criar o executável, aqui está um exeplo de como rodar o programa:

![read_file-rodar](https://github.com/user-attachments/assets/618be321-dd8e-4102-928d-e8352f7204a6)

Nesse programa, o arquivo 'myFile.txt' foi aberto pelo função open() e após isso, a função read() é utilizada para ler o que foi escrito dentro do arquivo(o que foi escrito pelo arquivo write_file.c). E a função close() é utilizada para fechar o arquivo.
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas>
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
![write-strace](https://github.com/user-attachments/assets/51fcfff5-8838-4a2e-9ea5-2228744643fa)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos:
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![read_file-time-v](https://github.com/user-attachments/assets/cd5d5a4d-2727-49f4-9b9b-5da37e734e02)

Diante do analisado conclui-se que, mesmo que a chamada read_file não tenha mostrado I/O diretamente, o comportamento do processo, com uso moderado de CPU e operações típicas de leitura de arquivo, faz com que ele seja considerado I/O-bound, uma vez que a principal limitação do desempenho vem do acesso ao arquivo (ou dispositivo) e não do uso da CPU.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel: 

![read_file-time](https://github.com/user-attachments/assets/ea1b360f-b7f8-4e7f-b68a-f096f9d2cd8c)
