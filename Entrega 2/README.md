## Explicação do código 

O sem_wait() serve para evitar a espera ociosa (busy waiting).
Quando um produtor chama sem_wait(&empty) e o buffer está cheio (contador do semáforo empty == 0), a thread é bloqueada automaticamente pelo sistema operacional. O mesmo ocorre para consumidores quando chamam sem_wait(&full) em um buffer vazio (full == 0).
Quando um consumidor libera um espaço (sem_post(&empty)), o sistema operacional reativa uma thread produtora bloqueada (se houver). Analogamente, quando um produtor adiciona um item (sem_post(&full)), um consumidor bloqueado é despertado.
Assim, podemos concluir que não ocorrerá espera ociosa pois threads bloqueadas não consomem ciclos de CPU. Elas ficam em estado de espera até serem notificadas pelo sistema.
