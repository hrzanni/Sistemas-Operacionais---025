## 🗄️ Módulo 3 — Simulador de Memória Virtual por Paginação
Essa etapa implementa um simulador de gerenciador de memória com paginação, que:

- Traduz endereços lógicos em páginas;
- Carrega quadros (frames) na memória física;
- Quando não há espaço, faz **swap out** ou **substituição** (Clock ou LRU);
- Registra faltas de página, operações de swap, estados de processos.

---

### 🎯 Objetivos

1. **Visualizar** passo a passo como um S.O. gerencia memória virtual.
2. Permitir escolher:
   - Tamanho das páginas (e quadros).
   - Bits de endereço lógico.
   - Tamanho da memória física (múltiplo do frame).
   - Tamanho da memória secundária (swap).
   - Algoritmo de substituição (Clock ou LRU).

---

### ⚙️ Funcionalidades

- **Leitura de arquivo de operações**  
  Formato:
P1 C 500 # criar processo P1, 500 bytes  
P1 R (1024)2 # leitura em endereço binário 1024₂  
P1 P (1)2 # “CPU burst”  

- **Passo a Passo**  
- `>>> Processando: …`  
- Grids de frames, tabelas de página, lista de processos, log de eventos.  
- **Resumo final**  
- Total de faltas de página.  
- Total de operações de swap.  
- Processos ainda “ativos” em memória.  

---

### 🏗️ Arquitetura

1. **C++**  
 - Classe `GerenciadorMemoria` faz toda a lógica de paginação, Clock/LRU e estatísticas.  
 - Imprime no stdout blocos como:
   ```text
   [FRAME] 0|1|0|1|0
   [PAGE_TABLE] 1|0|1|0|1|0
   [SWAP_SPACE] 2/64
   [PROCESS] 1|Pronto
   [EVENT] Falta de página em P1, pág 0
   ```
2. **Flask + Jinja2 (web)**  
 - Rota `/simuladorEntrega3`: recebe form, valida (memória física múltipla do frame, bits, etc.)   
 - Chama o executável C++, parseia o stdout e monta estrutura JSON.
 - Template `simuladorEntrega3.html` + JS renderizam grids e logs, com botões “← / →” para navegar steps.
3. **Front-end**  
 - CSS dark-mode, cards para frames, timeline de eventos, barra de swap.

---

### 📦 Como Compilar e Executar
Na raiz do projeto
```bash
g++ -std=c++17 -O2 -o simulador_memoria simulador_memoria.cpp
```

Porém, já deixamos o arquivo compilado para uso.  


Inicie o servidor Flask e vá para a tela da entrega3  

Lá, pode ser observado uma breve descrição do nosso desafio e o que fizemos para solucioná-lo. E após isso, há um formulário que contém os itens listados no **Objetivos**.

Após passar pelas 2 verificações, que são:  

-> Memória física (em bytes) deve ser múltipla de tamanho_pagina  
-> tamanho_pagina não pode exceder o espaço de endereçamento lógico 2^bits_endereco  

O usuário será redirecionado para a tela do simulador, onde irá ter a seguinte vizualização:  
![Captura de tela de 2025-06-23 00-09-00](https://github.com/user-attachments/assets/0bb085da-5180-4e2f-8ec9-a8ca9944de10)
![Captura de tela de 2025-06-23 00-09-14](https://github.com/user-attachments/assets/1ab80b72-17bf-4686-9a6f-2d533d92d12b)  

Após isso, basta ir vizualizando os próximos passos e conferindo como a simulação de como um sistema operacional faz para gerenciar memória  

####Aqui vai o exemplo do passo 7 deste mesmo exemplo:  

![Captura de tela de 2025-06-23 00-09-00](https://github.com/user-attachments/assets/1f67a096-b569-4819-8543-fcc2be31fe8e)
![Captura de tela de 2025-06-23 00-09-14](https://github.com/user-attachments/assets/0a469655-8168-46d9-b427-82111e611bc4)



