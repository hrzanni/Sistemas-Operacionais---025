## 🗄️ Módulo 3 — Simulador de Memória Virtual por Paginação
Essa etapa implementa um **mini‐S.O.**: um simulador de gerenciador de memória com paginação, que:

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

1. **C++ (baixo-nível)**  
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

Inicie o servidor Flask

