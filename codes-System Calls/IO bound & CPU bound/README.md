## IO_Bound
**Usando o comando:**
```bash
time ./<nome_arquivo_executável>
```
![IO-time](https://github.com/user-attachments/assets/10b48012-b401-49c6-afb1-8c063e487da5)

**Real:** 39.29s
➝ Tempo total desde o início até o fim do programa.

**User time:** 0.01s
➝ Tempo que a CPU gastou executando cálculos. Muito pouco tempo, indicando que o programa não exige processamento intenso.

**System time:** 7.756s
➝ Tempo gasto pelo sistema operacional manipulando chamadas de I/O (como fwrite e fread). Relativamente alto, mostrando que há muitas operações no disco.

O tempo total (real) é muito maior que (user + sys), indicando que o programa está esperando o disco responder, assim, podemos perceber que o programa é I/O bound

<br>

**Agora usando o comando:**
```bash
/usr/bin/time -v ./<nome_arquivo_executável>
```
![IO-time-v](https://github.com/user-attachments/assets/515a770f-0c62-4c8e-a7c7-63b2f3523630)

O programa é I/O-bound porque a maior parte do tempo total de execução foi gasta esperando operações de leitura/escrita no disco, e não na CPU, a CPU só está sendo usada 19% do tempo. 
O programa é limitado pela velocidade do disco, não pelo processamento da CPU. Isso é típico de processos I/O-bound, onde a maior parte do tempo é gasta esperando acessos ao sistema de arquivos.
