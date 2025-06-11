#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <cstdlib>
class GerenciadorMemoria {
public:
    enum AlgoritmoSubstituicao { LRU, RELOGIO };

    struct Pagina {
        int id_pagina;
        int id_processo;
        bool referenciada;
        bool modificada;
        time_t ultimo_acesso;
        bool presente;
        int frame;
    };

    struct Frame {
        int id_frame;
        bool alocado;
        int id_pagina;
        int id_processo;
    };

    struct Processo {
        int id_processo;
        int tamanho;
        std::string estado;
        std::unordered_map<int, Pagina> tabela_paginas;
        int id_arquivo_swap;
    };

    struct OperacaoMemoria {
        int id_processo;
        char tipo_operacao; // 'R', 'W', 'P', 'I', 'C'
        unsigned long endereco;
        int tamanho;
        std::string dispositivo;
    };

    GerenciadorMemoria(int tamanho_pagina, int bits_endereco, int memoria_fisica, int memoria_secundaria, AlgoritmoSubstituicao algoritmo)
        : tamanho_pagina_(tamanho_pagina),
          bits_endereco_(bits_endereco),
          tamanho_memoria_fisica_(memoria_fisica),
          tamanho_memoria_secundaria_(memoria_secundaria),
          algoritmo_substituicao_(algoritmo) {

        validar_parametros();
        inicializar_memoria();
    }

    void simular(const std::string& arquivo_entrada) {
        std::ifstream arquivo(arquivo_entrada);
        if (!arquivo) {
            throw std::runtime_error("Falha ao abrir arquivo de entrada: " + arquivo_entrada);
        }

        std::string linha;
        while (std::getline(arquivo, linha)) {
            if (linha.empty() || linha[0] == '#') continue;

            try {
                OperacaoMemoria op = parse_operacao(linha);
                tratar_operacao(op);
                exibir_estado_sistema(op.id_processo);
            } catch (const std::exception& e) {
                std::cerr << "Erro ao processar linha: " << linha << "\n  Motivo: " << e.what() << std::endl;
            }
        }

        imprimir_resumo();
    }

private:
    // Configurações do sistema
    const int tamanho_pagina_;
    const int bits_endereco_;
    const int tamanho_memoria_fisica_;
    const int tamanho_memoria_secundaria_;
    const AlgoritmoSubstituicao algoritmo_substituicao_;

    // Estruturas de gerenciamento
    std::vector<Frame> memoria_fisica_;
    std::list<int> frames_livres_;
    std::unordered_map<int, Processo> processos_;
    std::queue<int> fila_prontos_;

    // Algoritmos de substituição
    std::list<int> lista_lru_;
    int ponteiro_relogio_ = 0;

    // Estatísticas
    int faltas_pagina_ = 0;
    int operacoes_swap_ = 0;

    void validar_parametros() {
        if (tamanho_pagina_ <= 0) throw std::invalid_argument("Tamanho da página deve ser positivo");
        if (bits_endereco_ <= 0) throw std::invalid_argument("Bits de endereço devem ser positivos");
        if (tamanho_memoria_fisica_ <= 0) throw std::invalid_argument("Tamanho da memória física deve ser positivo");
        if (tamanho_memoria_secundaria_ <= 0) throw std::invalid_argument("Tamanho da memória secundária deve ser positivo");
        if (tamanho_memoria_fisica_ % tamanho_pagina_ != 0) {
            throw std::invalid_argument("Tamanho da memória física deve ser múltiplo do tamanho da página");
        }
    }

    void inicializar_memoria() {
        const int num_frames = tamanho_memoria_fisica_ / tamanho_pagina_;
        memoria_fisica_.resize(num_frames);

        for (int i = 0; i < num_frames; ++i) {
            memoria_fisica_[i] = {i, false, -1, -1};
            frames_livres_.push_back(i);
        }
    }

    OperacaoMemoria parse_operacao(const std::string& linha) {
        std::istringstream iss(linha);
        OperacaoMemoria op;
        char temp;

        // Formato: P1 C 500 ou P1 R (0)2
        iss >> temp >> op.id_processo >> op.tipo_operacao;

        if (op.tipo_operacao == 'C') {
            iss >> op.tamanho;
        } else {
            iss >> temp; // '('
            if (op.tipo_operacao == 'I') {
                std::getline(iss, op.dispositivo, ')');
            } else {
                iss >> op.endereco;
                iss >> temp; // ')'
                iss >> temp; // '2' - indicador binário
            }
        }

        return op;
    }

    void tratar_operacao(const OperacaoMemoria& op) {
        switch (op.tipo_operacao) {
            case 'C': criar_processo(op.id_processo, op.tamanho); break;
            case 'R':
            case 'W': tratar_acesso_memoria(op); break;
            case 'P': tratar_instrucao_cpu(op); break;
            case 'I': tratar_operacao_io(op); break;
            default:
                throw std::invalid_argument("Tipo de operação inválido: " + std::string(1, op.tipo_operacao));
        }
    }

    void criar_processo(int pid, int tamanho) {
        if (processos_.count(pid)) {
            throw std::runtime_error("Processo " + std::to_string(pid) + " já existe");
        }

        Processo proc;
        proc.id_processo = pid;
        proc.tamanho = tamanho;
        proc.estado = "pronto";
        proc.id_arquivo_swap = -1;

        const int num_paginas = (tamanho + tamanho_pagina_ - 1) / tamanho_pagina_;
        for (int i = 0; i < num_paginas; ++i) {
            proc.tabela_paginas[i] = {
                i,       // id_pagina
                pid,     // id_processo
                false,   // referenciada
                false,   // modificada
                time(nullptr), // ultimo_acesso
                false,   // presente
                -1       // frame
            };
        }

        processos_[pid] = proc;
        fila_prontos_.push(pid);

        std::cout << "Processo " << pid << " criado com " << num_paginas << " páginas\n";
    }

    void tratar_acesso_memoria(const OperacaoMemoria& op) {
        auto& proc = obter_processo(op.id_processo);
        const int num_pagina = op.endereco / tamanho_pagina_;

        if (!proc.tabela_paginas.count(num_pagina)) {
            throw std::runtime_error("Número de página inválido " + std::to_string(num_pagina) +
                                   " para processo " + std::to_string(op.id_processo));
        }

        auto& pagina = proc.tabela_paginas[num_pagina];
        const int frame = alocar_frame(proc, pagina);

        // Atualizar bits da página
        pagina.referenciada = true;
        pagina.ultimo_acesso = time(nullptr);
        if (op.tipo_operacao == 'W') {
            pagina.modificada = true;
        }

        // Atualizar estruturas do algoritmo de substituição
        atualizar_estruturas_substituicao(frame);

        std::cout << (op.tipo_operacao == 'R' ? "\n\nLeitura" : "\n\nEscrita")
                  << " de memória no endereço " << op.endereco << " (página " << num_pagina
                  << ", frame " << frame << ")\n";
    }

    int alocar_frame(Processo& proc, Pagina& pagina) {
        if (pagina.presente) return pagina.frame;

        if (!frames_livres_.empty()) {
            int frame = frames_livres_.front();
            frames_livres_.pop_front();
            alocar_frame_para_pagina(proc, pagina, frame);
            carregar_pagina_da_secundaria(proc.id_processo, pagina.id_pagina, frame);
            return frame;
        }

        // Tentar liberar memória suspendendo processo
        try {
            swap_out_processo();
        } catch (...) {
            std::cerr << "AVISO: Falha ao liberar memória com swapper\n";
        }

        // Depois do swap, tentar alocar novamente
        if (!frames_livres_.empty()) {
            int frame = frames_livres_.front();
            frames_livres_.pop_front();
            alocar_frame_para_pagina(proc, pagina, frame);
            carregar_pagina_da_secundaria(proc.id_processo, pagina.id_pagina, frame);
            return frame;
        }

        // Se ainda assim não houver frame, aplicar substituição
        int frame_vitima = selecionar_frame_vitima();
        substituir_pagina(frame_vitima, proc, pagina);
        return frame_vitima;
    }

    int selecionar_frame_vitima() {
        switch (algoritmo_substituicao_) {
            case LRU: return selecionar_vitima_lru();
            case RELOGIO: return selecionar_vitima_relogio();
            default: throw std::logic_error("Algoritmo de substituição inválido");
        }
    }

    int selecionar_vitima_lru() {
        if (lista_lru_.empty()) {
            throw std::runtime_error("Lista LRU vazia");
        }

        const int vitima = lista_lru_.back();
        lista_lru_.pop_back();
        return vitima;
    }

    int selecionar_vitima_relogio() {
        int tentativas = 0;
        while (tentativas < memoria_fisica_.size()) {
            Frame& frame = memoria_fisica_[ponteiro_relogio_];
            if (frame.alocado) {
                Pagina& pagina = processos_[frame.id_processo].tabela_paginas[frame.id_pagina];
                if (!pagina.referenciada) {
                    int vitima = ponteiro_relogio_;
                    ponteiro_relogio_ = (ponteiro_relogio_ + 1) % memoria_fisica_.size();
                    return vitima;
                }
                pagina.referenciada = false;
            }
            ponteiro_relogio_ = (ponteiro_relogio_ + 1) % memoria_fisica_.size();
            ++tentativas;
        }
        throw std::runtime_error("Nenhum frame encontrado para substituição usando Relógio");
    }

    void swap_out_processo() {
        int pid_alvo = -1;
        size_t max_paginas = 0;

        for (const auto& [pid, proc] : processos_) {
            size_t paginas_na_memoria = 0;
            for (const auto& [id_pagina, pagina] : proc.tabela_paginas) {
                if (pagina.presente) paginas_na_memoria++;
            }
            if (paginas_na_memoria > max_paginas) {
                max_paginas = paginas_na_memoria;
                pid_alvo = pid;
            }
        }

        if (pid_alvo == -1) throw std::runtime_error("Nenhum processo para swap out");

        Processo& proc = processos_[pid_alvo];
        for (auto& [id_pagina, pagina] : proc.tabela_paginas) {
            if (pagina.presente) {
                Frame& frame = memoria_fisica_[pagina.frame];
                if (pagina.modificada) {
                    salvar_pagina_na_secundaria(proc.id_processo, pagina.id_pagina, frame.id_frame);
                }
                frame.alocado = false;
                frame.id_pagina = -1;
                frame.id_processo = -1;
                pagina.presente = false;
                pagina.frame = -1;
                frames_livres_.push_back(frame.id_frame);
            }
        }
        proc.estado = "suspenso";
        std::cout << "Processo " << pid_alvo << " removido da memória\n";
    }

    void substituir_pagina(int frame_vitima, Processo& novo_proc, Pagina& nova_pagina) {
        Frame& frame = memoria_fisica_[frame_vitima];

        // Salvar página vítima se foi modificada
        if (frame.alocado) {
            auto& pagina_vitima = processos_[frame.id_processo].tabela_paginas[frame.id_pagina];

            if (pagina_vitima.modificada) {
                salvar_pagina_na_secundaria(frame.id_processo, frame.id_pagina, frame_vitima);
            }

            // Atualizar tabela de páginas do processo vítima
            pagina_vitima.presente = false;
            pagina_vitima.frame = -1;
        }

        // Alocar para a nova página
        alocar_frame_para_pagina(novo_proc, nova_pagina, frame_vitima);
        carregar_pagina_da_secundaria(novo_proc.id_processo, nova_pagina.id_pagina, frame_vitima);

        faltas_pagina_++;
        std::cout << "Falta de página - Frame " << frame_vitima << " substituído\n";
    }

    void alocar_frame_para_pagina(Processo& proc, Pagina& pagina, int frame) {
        pagina.presente = true;
        pagina.frame = frame;
        memoria_fisica_[frame] = {frame, true, pagina.id_pagina, proc.id_processo};
    }

    void tratar_instrucao_cpu(const OperacaoMemoria& op) {
        std::cout << "\n\nProcesso " << op.id_processo
                  << " executando instrução CPU: " << op.dispositivo << "\n";
    }

    void tratar_operacao_io(const OperacaoMemoria& op) {
        auto& proc = obter_processo(op.id_processo);
        proc.estado = "esperando_io";
        std::cout << "\n\nProcesso " << op.id_processo
                  << " realizando I/O no dispositivo: " << op.dispositivo << "\n";
    }

    Processo& obter_processo(int pid) {
        auto it = processos_.find(pid);
        if (it == processos_.end()) {
            throw std::runtime_error("Processo " + std::to_string(pid) + " não encontrado");
        }
        return it->second;
    }

    void carregar_pagina_da_secundaria(int pid, int id_pagina, int id_frame) {
        operacoes_swap_++;
        std::cout << "Carregando página " << id_pagina << " do processo " << pid
                  << " no Frame " << id_frame << "\n";
    }

    void salvar_pagina_na_secundaria(int pid, int id_pagina, int id_frame) {
        operacoes_swap_++;
        std::cout << "Salvando página " << id_pagina << " do processo " << pid
                  << " do Frame " << id_frame << " para armazenamento secundário\n";
    }

    void atualizar_estruturas_substituicao(int frame) {
        if (algoritmo_substituicao_ == LRU) {
            lista_lru_.remove(frame);
            lista_lru_.push_front(frame);
        }
    }

    void exibir_estado_sistema(int pid_atual) {
        exibir_estado_memoria();
        exibir_estado_processo(pid_atual);
        std::cout << std::string(40, '-') << "\n";
    }

    void exibir_estado_memoria() {
        std::cout << "\nEstado da Memória Fisica:\n";
        std::cout << "Frame | Aloc | PID  | Pag  | Ref | Mod\n";
        std::cout << "------|------|------|------|-----|----\n";

        for (const auto& frame : memoria_fisica_) {
            std::cout << std::setw(5) << frame.id_frame << " | ";
            std::cout << (frame.alocado ? "  S  | " : "  N  | ");

            if (frame.alocado) {
                const auto& pagina = processos_.at(frame.id_processo).tabela_paginas.at(frame.id_pagina);
                std::cout << std::setw(4) << frame.id_processo << " | ";
                std::cout << std::setw(4) << frame.id_pagina << " | ";
                std::cout << (pagina.referenciada ? " S | " : " N | ");
                std::cout << (pagina.modificada ? " S" : " N");
            } else {
                std::cout << "     |      |     |    ";
            }
            std::cout << "\n";
        }
    }

    void exibir_estado_processo(int pid) {
        const auto& proc = obter_processo(pid);

        std::cout << "\nEstado do Processo " << pid << ":\n";
        std::cout << "Tamanho: " << proc.tamanho << " bytes | Estado: " << proc.estado << "\n";

        std::cout << "\nTabela de Paginas do Processo " << pid << ":\n";
        std::cout << "Pag  | Presente | Frame | Ref | Mod | Ultimo Acesso\n";
        std::cout << "-----|----------|--------|-----|-----|--------------\n";

        for (const auto& [id_pagina, pagina] : proc.tabela_paginas) {
            std::cout << std::setw(4) << id_pagina << " | ";
            std::cout << (pagina.presente ? "    S    | " : "    N    | ");
            std::cout << (pagina.presente ? std::setw(6) : std::setw(6)) << pagina.frame << " | ";
            std::cout << (pagina.referenciada ? " S | " : " N | ");
            std::cout << (pagina.modificada ? " S | " : " N | ");
            std::cout << std::put_time(std::localtime(&pagina.ultimo_acesso), "%T");
            std::cout << "\n";
        }
    }

    void exibir_estado_memoria_secundaria() {
        std::cout << "\n[Memoria Secundaria - Simulada]\n";
        std::cout << "Operações de swap ate agora: " << operacoes_swap_ << "\n";
    }

    void imprimir_resumo() {
        std::cout << "\nResumo da Simulacao:\n";
        std::cout << "Total de faltas de página: " << faltas_pagina_ << "\n";
        std::cout << "Total de operações de swap: " << operacoes_swap_ << "\n";
        std::cout << "Processos ativos: " << processos_.size() << "\n";
        exibir_estado_memoria_secundaria();
    }
};

int main(int argc, char* argv[]) {
    try {
        // Verificar argumentos
        if (argc < 3) {
            std::cerr << "Uso: " << argv[0] << " <arquivo_entrada> <algoritmo>\n";
            std::cerr << "Algoritmo: 0 (Relógio) ou 1 (LRU)\n";
            return 1;
        }

        const std::string arquivo_entrada = argv[1];
        const int algoritmo = std::atoi(argv[2]);

        // Validar algoritmo
        if (algoritmo != 0 && algoritmo != 1) {
            std::cerr << "Algoritmo inválido. Use 0 (Relógio) ou 1 (LRU)\n";
            return 1;
        }

        // Configurar gerenciador
        GerenciadorMemoria::AlgoritmoSubstituicao algo =
            (algoritmo == 0) ? GerenciadorMemoria::RELOGIO : GerenciadorMemoria::LRU;

        GerenciadorMemoria gerenciador(
            4096,     // Tamanho da página (4KB)
            32,       // Bits de endereço (32-bit)
            16384,    // Memória física (16KB)
            1048576,  // Memória secundária (1MB)
            algo      // Algoritmo escolhido
        );

        gerenciador.simular(arquivo_entrada);
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
