## IO_Bound

vamos usar ocomando 'time' para obter tempo total de execução (em segundos), tempo gasto em modo usuário, tempo gasto em modo kernel, trocas de contexto voluntárias e trocas de contexto involuntárias

**Usando o comando:**
```bash
time ./<nome_arquivo_executável>
```
![IO-time](https://github.com/user-attachments/assets/10b48012-b401-49c6-afb1-8c063e487da5)

**Real:** 39.29s  
Tempo total desde o início até o fim do programa.

**User time:** 0.01s  
Tempo que a CPU gastou executando cálculos. Muito pouco tempo, indicando que o programa não exige processamento intenso.

**System time:** 7.756s  
Tempo gasto pelo sistema operacional manipulando chamadas de I/O (como fwrite e fread). Relativamente alto, mostrando que há muitas operações no disco.

**->** O tempo total (real) é muito maior que (user + sys), indicando que o programa está esperando o disco responder, assim, podemos perceber que o programa é I/O bound

<br>

**Agora usando o comando:**
```bash
/usr/bin/time -v ./<nome_arquivo_executável>
```
![IO-time-v](https://github.com/user-attachments/assets/515a770f-0c62-4c8e-a7c7-63b2f3523630)

Aqui nós também conseguimos notar que o programa é I/O-bound porque a maior parte do tempo total de execução foi gasta esperando operações de leitura/escrita no disco, e não na CPU, a CPU só está sendo usada 19% do tempo. 
O programa é limitado pela velocidade do disco, não pelo processamento da CPU. 
<br>

## CPU bound

vamos usar ocomando 'time' para obter tempo total de execução (em segundos), tempo gasto em modo usuário, tempo gasto em modo kernel, trocas de contexto voluntárias e trocas de contexto involuntárias

**Usando o comando:**
```bash
time ./<nome_arquivo_executável>
```
![CPU-time](https://github.com/user-attachments/assets/8ca0d38a-29f8-4246-8412-f240e8e843b8)

**Real:** 28.991s  
Tempo total desde o início até o fim do programa.

**User time:** 28.971s  
Tempo que a CPU gastou executando cálculos. Quase todo o tempo do programa a CPU estava diretamente ocupada executando cálculos, sem envolver interações com o sistema operacional, como chamadas de sistema ou I/O.

**System time:** 0.009s  
O tempo utilizado pelo sistema (chamadas de sistema, como I/O). Esse número é muito pequeno, o que significa que o programa não fez muitas operações de I/O ou interações com o sistema operacional.

**->** O tempo total (real) é quase igual ao tempo que a CPU gastou executando cálculos (user), indicando que o programa passou a maior parte do tempo calculando a soma, isso significa que o programa é CPU-bound (fortemente dependente da CPU), e a interação com o sistema foi mínima

<br>

**Agora usando o comando:**
```bash
/usr/bin/time -v ./<nome_arquivo_executável>
```
![CPU-time-v](https://github.com/user-attachments/assets/f9d95980-dca0-4583-8c18-f3a3fb0ace77)

O programa é CPU-bound, pois a maior parte do tempo foi gasta em cálculos intensivos pela CPU, com muito pouca interação com o sistema operacional (tempo de sistema de 0.00 segundos) e 99% de uso da CPU. O tempo total de execução foi praticamente igual ao tempo de CPU, o que significa que o programa não precisou esperar por outros recursos, como I/O. 
