
## Esse README contém as estatísticas relacionadas ao uso de chamadas ao sistema em cada um dos programas desenvolvidos no diretório "codesMemory".

## Memória -> mmap

Para criar um script executável do arquivo mmap.c
```bash
gcc -o <nome_arquivo_executavel> mmap.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depos de criar o executável, aqui está um exeplo de como rodar o programa  

![mmpa-rodar](https://github.com/user-attachments/assets/d9513898-7473-43ac-9feb-c2949486c83c)

Este programa aloca dinamicamente memória utilizando mmap(), armazena os números de 1 a 5 nessa memória e os imprime na tela. Em seguida, libera a memória com munmap().

<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas 
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
![mmap-strace](https://github.com/user-attachments/assets/efd82491-866f-40f2-8ed3-cdb9600b9f4b)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![mmap-time-v](https://github.com/user-attachments/assets/a11aae93-566d-44e5-93a1-ee29e74db360)

Nesse caso, a chamada de sistema mmap é classificada como I/O-bound, uma vez que a operação interage com o sistema de arquivos para acessar dados, e o uso da CPU foi moderado utilizando 40%, sem consumir recursos intensivos de processamento.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel  

![mmap-time](https://github.com/user-attachments/assets/ee7b511f-1509-4c06-bd8c-1a662481cfa6)

<br>
<br>

## Memória -> mprotect

Para criar um script executável do arquivo mprotect.c
```bash
gcc -o <nome_arquivo_executavel> mprotect.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depos de criar o executável, aqui está um exeplo de como rodar o programa  

![mprotect-rodar](https://github.com/user-attachments/assets/e8eccf06-5e65-4603-a682-eef895bfc92a)

O programa aloca um bloco de memória de 4 páginas e usa mprotect() para alterar as permissões de acesso na terceira página, tornando-a apenas leitura. Em seguida, entra em um loop tentando escrever na memória alocada. Quando tenta gravar na página protegida, ocorre um segmentation fault (erro de violação de segmento). O tratador de sinal captura o erro, imprime o endereço da falha e encerra o programa. O objetivo é demonstrar o uso de mprotect() para alterar permissões de memória e como tratar uma violação de memória ao tentar acessar uma área com permissões inadequadas.
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas 
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```

![mprotect-strace](https://github.com/user-attachments/assets/876d5fef-2e7d-49f6-a970-6c5324b9c5ff)
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![mprotect-time-v](https://github.com/user-attachments/assets/cf8df673-fa2b-4ed6-88e8-2e42118c9e90)

No caso aprensentado, utilizando a ferramenta time, pode-se confirmar que por mais que o mprotect() estaja manipulando permissões de memória, a leitura de dados do sistema de arquivos (32 entradas) e a baixa utilização da CPU (40%) indicam que o processo é I/O-bound. Além disso, o tempo de execução foi muito rápido, indicando que a operação foi concluída rapidamente, com o tempo sendo consumido mais pela leitura de arquivos e não por cálculos intensivos.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel  

![mprotect-time](https://github.com/user-attachments/assets/de13d684-f50e-4759-9243-cb57b2ae507f)

<br>
<br>

## Memória -> munmap

Para criar um script executável do arquivo munmap.c
```bash
gcc -o <nome_arquivo_executavel> munmap.c
```
Para rodar o script executável
```bash
./<nome_arquivo_executavel>
```
<br>

Depos de criar o executável, aqui está um exeplo de como rodar o programa  


Este programa aloca dinamicamente uma página de memória (4096 bytes) usando mmap(), permitindo leitura e escrita. Ele armazena uma string nessa região e imprime seu conteúdo. Após o uso, a memória é liberada com munmap(), garantindo que o sistema operacional recupere os recursos alocados.
<br>
<br>

Agora vamos utilizar a ferramenta 'strace', por ela conseguimos a relação de todas as system calls invocadas pela aplicação e tempo gasto por cada uma delas 
#### Usando o comando:
```bash
strace -c ./<nome_arquivo_executavel>
```
 
<br>
<br>

Agora vamos utilizar a ferramenta 'time' para obter as estatísticas de uso de recursos em cada um dos programas desenvolvidos
#### Usando o comando:
```bash
/usr/bin/time -v ./<nome_arquivo_executavel>
```

![munmap-time-v](https://github.com/user-attachments/assets/17182c65-cb99-4dd3-8286-bdbc7a373ac3)

A chamada munmap, nesse caso, pode ser classificada como I/O-Bound por motivos semelhantes aos das chamadas mmap e mprotect haja vista que a mesma não utiliza a CPU de forma intensiva (40%), sendo utilizada principalmente para a interação com o sistema de arquivos manipulando a memória mapeada.

<br>
E aqui temos um número mais preciso do tempo total decorrido desde o início até o fim da execução, do tempo gasto pelo processo em modo usuário e do tempo gasto no modo kernel  

![munmap-time](https://github.com/user-attachments/assets/2d739f16-6f3d-43ab-93d7-36d8eb4c95bb)
