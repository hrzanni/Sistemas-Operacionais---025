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
#include <chrono>
#include <thread>


class GerenciadorMemoria {
public:
    enum AlgoritmoSubstituicao { LRU, RELOGIO };

    struct Pagina {
        int id_pagina;
        int id_processo;
        bool referenciada;
        bool modificada;
        std::chrono::high_resolution_clock::time_point ultimo_acesso;
        bool presente;
        int frame;
    };

    struct Frame {
        int id_frame;
        bool alocado;
        int id_pagina;
        int id_processo;
        bool referenciada;  // Adicionado para visualização
        bool modificada;    // Adicionado para visualização
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
        int step = 0;
        while (std::getline(arquivo, linha)) {
            if (linha.empty() || linha[0] == '#') continue;

            try {
                OperacaoMemoria op = parse_operacao(linha);
                
                // Log da operação atual
                std::cout << ">>> Processando: " 
                          << op.id_processo << " " 
                          << op.tipo_operacao << " ";
                
                if (op.tipo_operacao == 'R' || op.tipo_operacao == 'W') {
                    std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << op.endereco;
                } else if (op.tipo_operacao == 'C') {
                    std::cout << op.tamanho;
                } else if (op.tipo_operacao == 'I') {
                    std::cout << op.dispositivo;
                }
                std::cout << "\n";
                
                tratar_operacao(op);
                emitir_estado_sistema(op.id_processo, step++);
            } catch (const std::exception& e) {
                std::cerr << "[ERRO] " << e.what() << std::endl;
            }
        }

        emitir_resumo();
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
    std::vector<std::string> eventos_;

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
            memoria_fisica_[i] = {i, false, -1, -1, false, false};
            frames_livres_.push_back(i);
        }
    }

    OperacaoMemoria parse_operacao(const std::string& linha) {
        std::istringstream iss(linha);
        OperacaoMemoria op;
        char temp;
        std::string token;
    
        // Formato: P1 C 500 ou P1 R (0)2
        if (!(iss >> temp >> op.id_processo >> op.tipo_operacao)) {
            throw std::runtime_error("Formato inválido: " + linha);
        }
    
        if (op.tipo_operacao == 'C') {
            if (!(iss >> op.tamanho)) {
                throw std::runtime_error("Faltando tamanho para operação C: " + linha);
            }
        } else {
            if (!(iss >> temp) || temp != '(') {
                throw std::runtime_error("Esperado '(' após tipo de operação: " + linha);
            }
            
            if (op.tipo_operacao == 'I' || op.tipo_operacao == 'P') {
                if (!std::getline(iss, op.dispositivo, ')')) {
                    throw std::runtime_error("Faltando dispositivo para operação I/P: " + linha);
                }
            } else {
                std::string endereco_str;
                if (!std::getline(iss, endereco_str, ')')) {
                    throw std::runtime_error("Faltando endereço: " + linha);
                }
                
                try {
                    // Alteração crucial: interpretar como decimal em vez de binário
                    op.endereco = std::stoul(endereco_str); // Conversão decimal
                } catch (const std::exception& e) {
                    throw std::runtime_error("Endereço inválido: " + endereco_str);
                }
            }
            
            // Consumir o '2' após o parêntese
            char next_char;
            if (iss >> next_char && next_char != '2') {
                // Se não for '2', colocar de volta no stream
                iss.putback(next_char);
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
                std::chrono::high_resolution_clock::now(),
                false,   // presente
                -1       // frame
            };
        }

        processos_[pid] = proc;
        fila_prontos_.push(pid);

        eventos_.push_back("Processo " + std::to_string(pid) + " criado com " + std::to_string(num_paginas) + " páginas");
    }

    void tratar_acesso_memoria(const OperacaoMemoria& op) {
        auto& proc = obter_processo(op.id_processo);
        const int num_pagina = op.endereco / tamanho_pagina_;

        if (op.endereco >= proc.tamanho) {
            eventos_.push_back("Erro: Endereço " + std::to_string(op.endereco) + 
                              " fora dos limites do processo " + std::to_string(op.id_processo));
            return;
        }

        if (!proc.tabela_paginas.count(num_pagina)) {
            eventos_.push_back("Erro: Página " + std::to_string(num_pagina) + 
                              " não alocada para processo " + std::to_string(op.id_processo));
            return;
        }

        auto& pagina = proc.tabela_paginas[num_pagina];
        const int frame = alocar_frame(proc, pagina);

        // Atualizar bits da página
        pagina.referenciada = true;
        pagina.ultimo_acesso = std::chrono::high_resolution_clock::now();
        if (op.tipo_operacao == 'W') {
            pagina.modificada = true;
        }

        // Atualizar frame correspondente
        if (frame != -1) {
            memoria_fisica_[frame].referenciada = pagina.referenciada;
            memoria_fisica_[frame].modificada = pagina.modificada;
        }

        // Atualizar estruturas do algoritmo de substituição
        atualizar_estruturas_substituicao(frame);
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
            eventos_.push_back("AVISO: Falha ao liberar memória com swapper");
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
        const int max_tentativas = 2 * memoria_fisica_.size();
        
        while (tentativas++ < max_tentativas) {
            Frame& frame = memoria_fisica_[ponteiro_relogio_];
            if (frame.alocado) {
                Pagina& pagina = processos_[frame.id_processo].tabela_paginas[frame.id_pagina];
                
                if (!pagina.referenciada) {
                    int vitima = ponteiro_relogio_;
                    ponteiro_relogio_ = (ponteiro_relogio_ + 1) % memoria_fisica_.size();
                    return vitima;
                }
                
                pagina.referenciada = false;
                frame.referenciada = false;
            }
            
            ponteiro_relogio_ = (ponteiro_relogio_ + 1) % memoria_fisica_.size();
        }
        
        throw std::runtime_error("Falha ao encontrar vítima no algoritmo do relógio");
    }

    void swap_out_processo() {
        int pid_alvo = -1;
        size_t max_paginas = 0;

        for (const auto& [pid, proc] : processos_) {
            if (proc.estado == "suspenso") continue;
            
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
        eventos_.push_back("Processo " + std::to_string(pid_alvo) + " removido da memória");
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
        eventos_.push_back("Falta de página - Frame " + std::to_string(frame_vitima) + " substituído");
    }

    void alocar_frame_para_pagina(Processo& proc, Pagina& pagina, int frame) {
        pagina.presente = true;
        pagina.frame = frame;
        memoria_fisica_[frame] = {
            frame, 
            true, 
            pagina.id_pagina, 
            proc.id_processo,
            pagina.referenciada,
            pagina.modificada
        };
    }

    void tratar_instrucao_cpu(const OperacaoMemoria& op) {
        auto& proc = obter_processo(op.id_processo);
        proc.estado = "executando";
        eventos_.push_back("Processo " + std::to_string(op.id_processo) + 
                           " executando instrução CPU: " + op.dispositivo);
    }

    void tratar_operacao_io(const OperacaoMemoria& op) {
        auto& proc = obter_processo(op.id_processo);
        proc.estado = "esperando_io";
        eventos_.push_back("Processo " + std::to_string(op.id_processo) + 
                           " realizando I/O no dispositivo: " + op.dispositivo);

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
        eventos_.push_back("Carregando página " + std::to_string(id_pagina) + 
                           " do processo " + std::to_string(pid) + 
                           " no Frame " + std::to_string(id_frame));
    }

    void salvar_pagina_na_secundaria(int pid, int id_pagina, int id_frame) {
        operacoes_swap_++;
        eventos_.push_back("Salvando página " + std::to_string(id_pagina) + 
                           " do processo " + std::to_string(pid) + 
                           " do Frame " + std::to_string(id_frame) + 
                           " para armazenamento secundário");
    }

    void atualizar_estruturas_substituicao(int frame) {
        if (algoritmo_substituicao_ == LRU) {
            lista_lru_.remove(frame);
            lista_lru_.push_front(frame);
        }
    }

    void emitir_estado_sistema(int pid_atual, int step) {
        // Emitir apenas frames alocados
        for (const auto& frame : memoria_fisica_) {
            if (frame.alocado) {
                std::cout << "[FRAME] "
                      << frame.id_frame << "|"
                      << frame.id_processo << "|"
                      << frame.id_pagina << "|"
                      << (frame.referenciada ? 1 : 0) << "|"
                      << (frame.modificada ? 1 : 0) << "\n";
            }
        }

        // Emitir tabelas de páginas
        for (const auto& [pid, proc] : processos_) {
            for (const auto& [id_pagina, pagina] : proc.tabela_paginas) {
                std::cout << "[PAGE_TABLE] "
                          << pid << "|"
                          << id_pagina << "|"
                          << (pagina.presente ? 1 : 0) << "|"
                          << (pagina.presente ? pagina.frame : -1) << "|"
                          << (pagina.referenciada ? 1 : 0) << "|"
                          << (pagina.modificada ? 1 : 0) << "\n";
            }
        }

        // Emitir estado dos processos
        for (const auto& [pid, proc] : processos_) {
            std::cout << "[PROCESS] "
                      << pid << "|"
                      << proc.estado << "\n";
        }

        // Emitir eventos
        for (const auto& evento : eventos_) {
            std::cout << "[EVENT] " << evento << "\n";
        }
        eventos_.clear();

        std::cout << "[STEP_END] " << step << "\n";
    }

    void emitir_resumo() {
        std::cout << "[SUMMARY] Faltas de página: " << faltas_pagina_ << "\n";
        std::cout << "[SUMMARY] Operações de swap: " << operacoes_swap_ << "\n";
        std::cout << "[SUMMARY] Processos ativos: " << processos_.size() << "\n";
        std::cout << "[END_SIMULATION]\n";
    }
};


// === Funções auxiliares para log ===
void log_event(const std::string& msg) {
    std::cout << "[EVENT] " << msg << std::endl;
}

void log_frame(const std::vector<GerenciadorMemoria::Frame>& memoria_fisica) {
    for (const auto& f : memoria_fisica) {
        std::cout << "[FRAME] ID=" << f.id_frame
                  << " | PID=" << f.id_processo
                  << " | Página=" << f.id_pagina
                  << " | Ref=" << f.referenciada
                  << " | Mod=" << f.modificada << std::endl;
    }
}

void log_page_table(const GerenciadorMemoria::Processo& p) {
    for (const auto& [id, pg] : p.tabela_paginas) {
        std::cout << "[PAGE_TABLE] PID=" << p.id_processo
                  << " | Página=" << id
                  << " | Presente=" << pg.presente
                  << " | Frame=" << pg.frame
                  << " | Ref=" << pg.referenciada
                  << " | Mod=" << pg.modificada << std::endl;
    }
}

void log_process_state(int pid, const std::string& estado) {
    std::cout << "[PROCESS_STATE] " << pid << " | " << estado << std::endl;
}

void log_swap_out(int pid, int pagina) {
    std::cout << "[SWAP] To Disk: PID=" << pid << " | Página=" << pagina << std::endl;
}

void log_swap_in(int pid, int pagina) {
    std::cout << "[SWAP] From Disk: PID=" << pid << " | Página=" << pagina << std::endl;
}

void log_config(const std::string& algoritmo, int tam_pag, int bits, int mem_fis, int mem_sec) {
    std::cout << "[INFO] Algoritmo de substituição de páginas: " << algoritmo << std::endl;
    std::cout << "[INFO] Tamanho da página: " << tam_pag
              << " | Bits endereço: " << bits
              << " | Memória Física: " << mem_fis
              << " | Memória Secundária: " << mem_sec << std::endl;
}
// === Fim das funções auxiliares ===


int main(int argc, char* argv[]) {
    try {
        // Verificar argumentos
        if (argc < 7) {
            std::cerr << "Uso: " << argv[0] 
                      << " <arquivo_entrada>"
                      << " <tamanho_pagina>"
                      << " <bits_endereco>"
                      << " <memoria_fisica>"
                      << " <memoria_secundaria>"
                      << " <algoritmo>\n";
            std::cerr << "Algoritmo: 0 (Relógio) ou 1 (LRU)\n";
            return 1;
        }

        const std::string arquivo_entrada = argv[1];
        const int tamanho_pagina = std::stoi(argv[2]);
        const int bits_endereco = std::stoi(argv[3]);
        const int memoria_fisica = std::stoi(argv[4]);
        const int memoria_secundaria = std::stoi(argv[5]);
        const int algoritmo = std::stoi(argv[6]);

        // Validar algoritmo
        if (algoritmo != 0 && algoritmo != 1) {
            std::cerr << "Algoritmo inválido. Use 0 (Relógio) ou 1 (LRU)\n";
            return 1;
        }

        // Configurar gerenciador
        GerenciadorMemoria::AlgoritmoSubstituicao algo =
            (algoritmo == 0) ? GerenciadorMemoria::RELOGIO : GerenciadorMemoria::LRU;

        GerenciadorMemoria gerenciador(
            tamanho_pagina,
            bits_endereco,
            memoria_fisica,
            memoria_secundaria,
            algo
        );

        gerenciador.simular(arquivo_entrada);
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}