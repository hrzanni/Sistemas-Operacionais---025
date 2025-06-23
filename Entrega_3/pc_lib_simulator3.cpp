#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cctype> 
#include <regex>


class GerenciadorMemoria {
public:
    enum AlgoritmoSubstituicao { RELOGIO = 0, LRU = 1 };

    struct Pagina {
        int id_pagina;
        int id_processo;
        bool referenciada = false;
        bool modificada = false;
        std::chrono::high_resolution_clock::time_point ultimo_acesso;
        bool presente = false;
        int frame = -1;
    };

    struct Frame {
        int id_frame;
        bool alocado = false;
        int id_pagina = -1;
        int id_processo = -1;
        bool referenciada = false;
        bool modificada = false;
    };

    struct Processo {
        int id_processo;
        int tamanho_bytes;
        int num_paginas;
        std::string estado;
        std::unordered_map<int, Pagina> tabela_paginas;
    };

    struct OperacaoMemoria {
        int id_processo;
        char tipo_operacao; // 'C','R','W','P','I'
        unsigned long endereco;
        int tamanho;
        std::string dispositivo;
    };

    GerenciadorMemoria(int tamanho_pagina, int bits_endereco,
                       int memoria_fisica, int memoria_secundaria,
                       AlgoritmoSubstituicao algoritmo)
        : tamanho_pagina_(tamanho_pagina),
          bits_endereco_(bits_endereco),
          memoria_fisica_bytes_(memoria_fisica),
          memoria_secundaria_bytes_(memoria_secundaria),
          algoritmo_(algoritmo) {
        validar_parametros();
        inicializar_memoria();
    }

    void simular(const std::string &arquivo_entrada) {
        std::ifstream arquivo(arquivo_entrada);
        if (!arquivo) throw std::runtime_error("Não foi possível abrir " + arquivo_entrada);
    
        std::string linha;
        int step = 0;
    
        while (std::getline(arquivo, linha)) {
            if (linha.empty() || linha[0] == '#') continue;
    
            // 1) Parse, log e execute
            auto op = parse_operacao(linha);
            log_op(op);

            std::string trimmed = std::regex_replace(linha, std::regex("^\\s+|\\s+$"), "");
            std::cout << ">>> Processando: " << trimmed << "\n";
            
            executar_op(op);
            // 2) Emite o estado **com** o processo em “Executando”
            emitir_estado(step++);
    
            if (op.tipo_operacao == 'P') {
                processos_[op.id_processo].estado = "Pronto";
                eventos_.push_back(
                  "Processo " + std::to_string(op.id_processo)
                  + " voltou a Pronto após CPU"
                );
                
                
            }
        }
    
        emitir_resumo();
    }
    

private:
    int tamanho_pagina_;
    int bits_endereco_;
    int memoria_fisica_bytes_;
    int memoria_secundaria_bytes_;
    AlgoritmoSubstituicao algoritmo_;

    int num_frames_;
    int capacidade_swap_paginas_;
    int usadas_swap_paginas_ = 0;

    std::vector<Frame> memoria_fisica_;
    std::list<int> frames_livres_;
    std::unordered_map<int, Processo> processos_;
    std::queue<int> fila_prontos_;

    // Substituição
    std::list<int> lista_lru_;
    int ponteiro_relogio_ = 0;

    // Estatísticas
    int faltas_pagina_ = 0;
    int operacoes_swap_ = 0;
    std::vector<std::string> eventos_;

    void validar_parametros() {
        if (tamanho_pagina_ <= 0 || bits_endereco_ <= 0 ||
            memoria_fisica_bytes_ <= 0 || memoria_secundaria_bytes_ <= 0)
            throw std::invalid_argument("Parâmetros devem ser > 0");
        if (memoria_fisica_bytes_ % tamanho_pagina_ != 0)
            throw std::invalid_argument("Memória física deve ser múltipla do tamanho da página");
        num_frames_ = memoria_fisica_bytes_ / tamanho_pagina_;
        capacidade_swap_paginas_ = memoria_secundaria_bytes_ / tamanho_pagina_;
    }

    void inicializar_memoria() {
        memoria_fisica_.resize(num_frames_);
        for (int i = 0; i < num_frames_; ++i) {
            memoria_fisica_[i].id_frame = i;
            frames_livres_.push_back(i);
        }
    }

    OperacaoMemoria parse_operacao(const std::string &linha) {
        std::istringstream iss(linha);
        OperacaoMemoria op;
        char P;
        iss >> P >> op.id_processo >> op.tipo_operacao;
    
        if (op.tipo_operacao == 'C') {
            // criação de processo
            iss >> op.tamanho;
    
        } 
        else if (op.tipo_operacao == 'R' || op.tipo_operacao == 'W') {
            // leitura/gravação: token "(...)" seguido de 2
            std::string token;
            iss >> token;  // ex: "(1024)2"
    
            // debug opcional
            std::cout << "[DEBUG_PARSE] token=\"" << token << "\"\n";
    
            // extrai o que está entre '(' e ')'
            auto close = token.find(')');
            if (token.empty() || token.front() != '(' || close == std::string::npos) {
                throw std::runtime_error("Formato inválido de endereço: " + token);
            }
            std::string inside = token.substr(1, close - 1);
    
            // debug opcional
            std::cout << "[DEBUG_PARSE] inside=\"" << inside << "\"\n";
    
            // tenta converter como binário, senão como decimal
            try {
                op.endereco = std::stoul(inside, nullptr, 2);
            } catch (...) {
                op.endereco = std::stoul(inside, nullptr, 10);
            }
    
            // checa limites
            if (op.endereco >= (1u << bits_endereco_)) {
                throw std::runtime_error("Endereço excede espaço lógico: " + inside);
            }
    
        } 
        else if (op.tipo_operacao == 'P' || op.tipo_operacao == 'I') {
            // CPU ou I/O: simplesmente pega o que vier até ')'
            char c;
            iss >> c;  // deve ser '('
            std::getline(iss, op.dispositivo, ')');
    
        } 
        else {
            throw std::runtime_error(std::string("Tipo de operação inválido: ") + op.tipo_operacao);
        }
    
        return op;
    }



    std::string tipo_humana(const OperacaoMemoria &op) {
        switch (op.tipo_operacao) {
            case 'C': return "criação de processo (tamanho: " + std::to_string(op.tamanho) + ")";
            case 'R': return "leitura no endereço " + std::to_string(op.endereco);
            case 'W': return "gravação no endereço " + std::to_string(op.endereco);
            case 'P': return "de CPU";
            case 'I': return "operação de I/O (dispositivo: " + op.dispositivo + ")";
            default:  return "operação desconhecida";
        }
    }

    void log_op(const OperacaoMemoria &op) {

        eventos_.push_back("Processo " + std::to_string(op.id_processo) + " realizará operação " + tipo_humana(op));
        if (op.tipo_operacao == 'C') std::cout << " " << op.tamanho;
        else if (op.tipo_operacao=='R' || op.tipo_operacao=='W')
            std::cout << " " << op.endereco;
        else std::cout << " " << op.dispositivo;
        std::cout << "\n";
    }

    void executar_op(const OperacaoMemoria &op) {
        switch (op.tipo_operacao) {
            case 'C': criar_processo(op); break;
            case 'R': case 'W': acesso_memoria(op); break;
            case 'P': inst_cpu(op); break;
            case 'I': inst_io(op); break;
        }
    }

    void criar_processo(const OperacaoMemoria &op) {
        Processo pr;
        pr.id_processo = op.id_processo;
        pr.tamanho_bytes = op.tamanho;
        pr.num_paginas = std::ceil((double)pr.tamanho_bytes / tamanho_pagina_);
        pr.estado = "Pronto";
        for (int i = 0; i < pr.num_paginas; ++i) {
            Pagina pg;
            pg.id_processo = pr.id_processo;
            pg.id_pagina = i;
            pr.tabela_paginas[i] = pg;
        }
        processos_[op.id_processo] = pr;
        fila_prontos_.push(op.id_processo);
        eventos_.push_back(
            "Processo " + std::to_string(op.id_processo) + " criado com " + std::to_string(pr.num_paginas) + " páginas");
          
    }

    void acesso_memoria(const OperacaoMemoria &op) {
        auto &pr = processos_.at(op.id_processo);
        if (op.endereco >= pr.tamanho_bytes) {
            eventos_.push_back("Segmentation Fault P" + std::to_string(op.id_processo));
            processos_[op.id_processo].estado = "FALHOU";
            return;
        }

        int pgid = op.endereco / tamanho_pagina_;
        auto &pg = pr.tabela_paginas.at(pgid);

        if (pg.presente) {
            eventos_.push_back(
                "Acesso sem falta de página. Processo " + std::to_string(pr.id_processo) +
                " na página " + std::to_string(pgid)
            );
        } else {
            tratar_falta(pr, pg);
        }

        
        if (!pg.presente) tratar_falta(pr, pg);
        pg.referenciada = true;
        pg.ultimo_acesso = std::chrono::high_resolution_clock::now();
        if (op.tipo_operacao == 'W') pg.modificada = true;
        auto &fr = memoria_fisica_[pg.frame];
        fr.referenciada = pg.referenciada;
        fr.modificada = pg.modificada;
        if (algoritmo_ == LRU) {
            lista_lru_.remove(pg.frame);
            lista_lru_.push_front(pg.frame);
        }
    }

    void tratar_falta(Processo &pr, Pagina &pg) {
        faltas_pagina_++;
        eventos_.push_back("Falta de página no processo " + std::to_string(pr.id_processo) + ", página " + std::to_string(pg.id_pagina));        
        int frame;
        if (!frames_livres_.empty()) {
            frame = frames_livres_.front(); frames_livres_.pop_front();
        } else if (usar_swap_out()) {
            frame = frames_livres_.front(); frames_livres_.pop_front();
        } else {
            frame = selecionar_vitima();
            substituir(frame);
        }
        alocar(pr, pg, frame);
        carregar(pr.id_processo, pg.id_pagina, frame);
    }

    bool usar_swap_out() {
        int alvo=-1; size_t maxp=0;
        for (auto &kv : processos_) {
            auto &p=kv.second; if (p.estado=="Suspenso") continue;
            size_t cnt=0; for (auto &e:p.tabela_paginas) if (e.second.presente) cnt++;
            if (cnt>maxp) { maxp=cnt; alvo=p.id_processo; }
        }
        if (alvo<0) return false;
        auto &p = processos_[alvo];
        for (auto &e : p.tabela_paginas) {
            auto &pg=e.second;
            if (pg.presente) {
                auto &fr=memoria_fisica_[pg.frame];
                if (pg.modificada) salvar(alvo,pg.id_pagina,pg.frame);
                fr = Frame{fr.id_frame,false};
                frames_livres_.push_back(fr.id_frame);
                pg.presente=false; pg.frame=-1;
            }
        }
        p.estado = "Suspenso";
        eventos_.push_back("Processo " + std::to_string(alvo) + " foi movido para a memória secundária (swap out)");
        return true;
    }

    int selecionar_vitima() {
        if (algoritmo_ == LRU) {
            int v=lista_lru_.back(); lista_lru_.pop_back(); return v;
        }
        for (int i=0;i<2*num_frames_;++i) {
            auto &fr=memoria_fisica_[ponteiro_relogio_];
            auto &pg = processos_[fr.id_processo].tabela_paginas[fr.id_pagina];
            if (!pg.referenciada) { int v=ponteiro_relogio_; ponteiro_relogio_=(v+1)%num_frames_; return v; }
            pg.referenciada=false; fr.referenciada=false;
            ponteiro_relogio_=(ponteiro_relogio_+1)%num_frames_;
        }
        throw std::runtime_error("Vitima não encontrada");
    }

    void substituir(int frame) {
        auto &fr = memoria_fisica_[frame];
        auto &oldp = processos_[fr.id_processo].tabela_paginas[fr.id_pagina];
        if (oldp.modificada) salvar(fr.id_processo,oldp.id_pagina,frame);
        oldp.presente=false; oldp.frame=-1;
        eventos_.push_back("Substituindo página " + std::to_string(oldp.id_pagina) + " do processo " + std::to_string(oldp.id_processo) + " no frame " + std::to_string(frame));
    }

    void alocar(Processo &pr, Pagina &pg, int frame) {
        pg.presente=true; pg.frame=frame;
        auto &fr=memoria_fisica_[frame];
        fr.alocado=true; fr.id_processo=pr.id_processo; fr.id_pagina=pg.id_pagina;
        if (algoritmo_==LRU) lista_lru_.push_front(frame);
    }

    void carregar(int pid,int pgid,int frame) {
        operacoes_swap_++;
        if (usadas_swap_paginas_<capacidade_swap_paginas_) usadas_swap_paginas_++;
        eventos_.push_back("Carregando página " + std::to_string(pgid) + " do processo " + std::to_string(pid) + " no frame " + std::to_string(frame));
    }

    void salvar(int pid,int pgid,int frame) {
        operacoes_swap_++;
        if (usadas_swap_paginas_>0) usadas_swap_paginas_--;
        eventos_.push_back("Salvando página " + std::to_string(pgid) + " do processo " + std::to_string(pid) + " do frame " + std::to_string(frame) + " para memória secundária");
    }

    void inst_cpu(const OperacaoMemoria &op) {
        processos_[op.id_processo].estado = "Executando";
        eventos_.push_back("Processo " + std::to_string(op.id_processo) + " executando instrução de CPU");
    }
    void inst_io(const OperacaoMemoria &op) {
        processos_[op.id_processo].estado = "Esperando I/O";
        eventos_.push_back("Processo " + std::to_string(op.id_processo) + " iniciando operação de I/O");
    }

    void emitir_estado(int step) {

        
        for (auto &fr : memoria_fisica_) if (fr.alocado)
            std::cout<<"[FRAME] "<<fr.id_frame<<"|"<<fr.id_processo<<"|"<<fr.id_pagina
                     <<"|"<<fr.referenciada<<"|"<<fr.modificada<<"\n";
        std::cout<<"[SWAP_SPACE] "<<usadas_swap_paginas_<<"/"<<capacidade_swap_paginas_<<"\n";
        for (auto &kv:processos_) for (auto &e:kv.second.tabela_paginas)
            std::cout<<"[PAGE_TABLE] "<<kv.first<<"|"<<e.first<<"|"<<e.second.presente
                     <<"|"<<e.second.frame<<"|"<<e.second.referenciada<<"|"<<e.second.modificada<<"\n";
        for (auto &kv:processos_)
            std::cout<<"[PROCESS] "<<kv.first<<"|"<<kv.second.estado<<"\n";
        for (auto &ev: eventos_) std::cout<<"[EVENT] "<<ev<<"\n";
        eventos_.clear();
        std::cout<<"[STEP_END] "<<step<<"\n";
    }

    void emitir_resumo() {
        // Sem acentos e com palavras exatas para gerar as chaves certas:
        std::cout << "[SUMMARY] Faltas de pagina: "  << faltas_pagina_   << "\n";
        std::cout << "[SUMMARY] Operacoes de swap: " << operacoes_swap_   << "\n";
        std::cout << "[SUMMARY] Processos ativos: "   << processos_.size() << "\n";
        std::cout << "[END_SIMULATION]\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc<7) {
        std::cerr<<"Uso: "<<argv[0]<<" <input> <page_size> <addr_bits> <phys_mem> <swap_mem> <algo>\n";
        return 1;
    }
    try {
        auto arqu = argv[1];
        int ps=std::stoi(argv[2]); int ab=std::stoi(argv[3]);
        int pm=std::stoi(argv[4]); int sm=std::stoi(argv[5]);
        int alg=std::stoi(argv[6]);
        GerenciadorMemoria::AlgoritmoSubstituicao algo=
            alg==0?GerenciadorMemoria::RELOGIO:GerenciadorMemoria::LRU;
        GerenciadorMemoria gm(ps,ab,pm,sm,algo);
        gm.simular(arqu);
    } catch(const std::exception&e) {
        std::cerr<<"[ERRO]"<<e.what()<<"\n";
        return 1;
    }
    return 0;
}
