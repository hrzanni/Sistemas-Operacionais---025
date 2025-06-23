from flask import Flask, config, render_template, jsonify, request, redirect, url_for
from ctypes import CDLL, c_int, c_char_p, create_string_buffer, byref, POINTER
import re
import time
import subprocess
import os
import json


app = Flask(__name__)


# Página da home do projeto
@app.route('/')
def home():
    return render_template('home.html')
    
# Página da entrega 1 do projeto, contém a descrição da entrega e as system calls
@app.route('/entrega1')
def entrega1():
    return render_template('entrega1.html')

# Página da system call open()
@app.route("/entrega1/open_file", methods=["GET", "POST"])
def entrega1_open_file():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 1 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-open_file.html', resultado=resultado)


# Página da system call read()
@app.route("/entrega1/read_file", methods=["GET", "POST"])
def entrega1_read_file():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 2 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-read_file.html', resultado=resultado)


# Página da system call write()
@app.route("/entrega1/write_file", methods=["GET", "POST"])
def entrega1_write_file():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 3 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-write_file.html', resultado=resultado)


# Página da system call mmap()
@app.route("/entrega1/mmap", methods=["GET", "POST"])
def entrega1_mmap():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 4 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-mmap.html', resultado=resultado)


# Página da system call mprotect()
@app.route("/entrega1/mprotect", methods=["GET", "POST"])
def entrega1_mprotect():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 5 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-mprotect.html', resultado=resultado)


# Página da system call munmap()
@app.route("/entrega1/munmap", methods=["GET", "POST"])
def entrega1_munmap():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 6 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-munmap.html', resultado=resultado)


# Página da system call fork()
@app.route("/entrega1/fork", methods=["GET", "POST"])
def entrega1_fork():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 7 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-fork.html', resultado=resultado)


# Página da system call pause()
@app.route("/entrega1/pause", methods=["GET", "POST"])
def entrega1_pause():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 8 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-pause.html', resultado=resultado)


# Página da system call sleep()
@app.route("/entrega1/sleep", methods=["GET", "POST"])
def entrega1_sleep():
    resultado = None

    if request.method == "POST":
        try:
            base_dir = os.getcwd()
            path_executavel = os.path.join(base_dir, "webSite", "teste")
            comando = f'echo 8 | "{path_executavel}"'

            resultado = subprocess.getoutput(comando)
        except Exception as e:
            resultado = f"Erro ao executar: {e}"

    return render_template('entrega1-sleep.html', resultado=resultado)
    

# Página do processo I/O bound()
@app.route("/entrega1/Processos_I/O-bound")
def entrega1_IOBound():
    return render_template('entrega1-IOBound.html')


# Página do processo CPU bound()
@app.route("/entrega1/Processos_CPU-bound")
def entrega1_CPUBound():
    return render_template('entrega1-CPUBound.html')


# FIM DA ENTREGA 1


# COMEÇO DA ENTREGA 2


# biblioteca para o problema produtor-consumidor 
lib = CDLL('./webSite/ProjectSite/libpc_lib.so')


# Define tipos das funções
lib.main.argtypes = [c_int, c_int, c_int]
lib.get_step_count.restype = c_int
lib.get_step.argtypes = [c_int]
lib.get_step.restype = c_char_p

lib.get_simulation_log.argtypes = [c_char_p, c_int]
lib.get_simulation_log.restype = c_int

lib.get_status.argtypes = [POINTER(c_int), POINTER(c_int), POINTER(c_int), c_int]
lib.get_status.restype = None


# página que contém a descrição da entrega 2 e o formulário para ir para o simulador do probblema produtor/consumidor 
@app.route('/entrega2', methods=['GET', 'POST'])
def entrega2():

    buffer_status = None  

    if request.method == 'POST':
        # Receber os dados do formulário
        num_produtores = int(request.form['num_produtores'])
        num_consumidores = int(request.form['num_consumidores'])
        buffer_size = int(request.form['buffer_size'])

        # Definir as variáveis de status
        empty = c_int(buffer_size)
        full = c_int(0)
        buffer = (c_int * buffer_size)()

        # Iniciar produtores e consumidores com os parâmetros recebidos
        lib.start_producer(num_produtores)
        lib.start_consumer(num_consumidores)

        # Obter o status do buffer
        lib.get_status(byref(empty), byref(full), byref(buffer), buffer_size)

        # Passar o status para a template
        buffer_status = {
            'empty': empty.value,
            'full': full.value,
            'buffer': [buffer[i] for i in range(buffer_size)]
        }

    return render_template('entrega2.html', buffer_status=buffer_status)


# Página do simulador do problema produtor/consumidor
@app.route('/simuladorEntrega2', methods=['POST'])
def simulacao():
    
    try:
        num_produtores = int(request.form['num_produtores'])
        num_consumidores = int(request.form['num_consumidores'])
        buffer_size = int(request.form['buffer_size'])

        inicio = time.time()

        # Chama a simulação em C
        lib.main(num_produtores, num_consumidores, buffer_size)

        fim = time.time()

        # Calcula o tempo de execução da simulação
        tempo_execucao = fim - inicio

        # Cria o buffer para receber o log da simulação
        log_buffer = create_string_buffer(1024 * 10)  # 10 KB de buffer
        log_len = lib.get_simulation_log(log_buffer, len(log_buffer))

        # Converte o buffer C em uma string Python
        log_content = log_buffer.value.decode('utf-8')

        # Divide o log em passos
        simulation_steps = []
        total_producoes = 0
        total_consumos = 0

        empty_count = buffer_size
        full_count = 0
        buffer_content = []

        for i, line in enumerate(log_content.split('\n')):
            descricao = line.strip()
            if not descricao:
                continue

            if 'colocou' in descricao:
                total_producoes += 1
                if empty_count > 0:
                    # Tenta extrair o número que foi produzido 
                    match = re.search(r'colocou (\d+(?:\.\d+)?)', descricao)
                    if match:
                        valor = float(match.group(1))
                    else:
                        valor = '?'
                    buffer_content.append(valor)
                    empty_count -= 1
                    full_count += 1
            elif 'retirou' in descricao:
                total_consumos += 1
                if full_count > 0:
                    buffer_content.pop(0)  # Simula o consumo (FIFO)
                    full_count -= 1
                    empty_count += 1

            # Registra o estado nesse passo
            simulation_steps.append({
                'step': i + 1,
                'descricao': descricao,
                'buffer_status': {
                    'empty': empty_count,
                    'full': full_count,
                    'buffer': buffer_content.copy() 
                }
            })

    except Exception as e:
        print("[ERRO NA SIMULAÇÃO]", e)
        return render_template('error.html', error_message=str(e)), 500


    return render_template(
        'simuladorEntrega2.html',
        num_produtores=num_produtores,
        num_consumidores=num_consumidores,
        buffer_size=buffer_size,
        simulation_steps=simulation_steps,
        total_producoes=total_producoes,
        total_consumos=total_consumos,
        tempo_execucao=round(tempo_execucao, 4)
    )


# FIM DA ENTREGA 2


# COMEÇO DA ENTREGA 3


# página que contém a descrição da entrega 3 e o formulário para ir para o simulador de memória 
@app.route('/entrega3')
def entrega3():
    return render_template('entrega3.html')


def parse_memory_output(stdout: str):
    steps = []
    current = None

    for raw in stdout.splitlines():
        line = raw.strip()
        if not line:
            continue

        if line.startswith('>>> Processando:'):
            if current:
                steps.append(current)
            desc = line.split(':',1)[1].strip()
            current = {
                'descricao': desc,
                'frames': [],
                'page_tables': [],
                'processes': [],
                'events': [],
                'swap': { 'used': 0, 'cap': 0 }
            }
            continue

        if line.startswith('[FRAME]'):
            parts = line[len('[FRAME]'):].strip().split('|')
            fid, pid, page, ref, mod = parts
            current['frames'].append({
                'id':        int(fid),
                'pid':       int(pid),
                'page':      int(page),
                'referenced': ref == '1',
                'modified':   mod == '1'
            })
            continue

        if line.startswith('[SWAP_SPACE]'):
            used, cap = line[len('[SWAP_SPACE]'):].strip().split('/')
            current['swap']['used'] = int(used)
            current['swap']['cap']  = int(cap)
            continue

        if line.startswith('[PAGE_TABLE]'):
            parts = line[len('[PAGE_TABLE]'):].strip().split('|')
            pid_str, page_id_str, present, frame_str, ref, mod = parts

            # conversão segura de inteiros
            try:
                pid = int(pid_str)
            except ValueError:
                pid = -1
            try:
                page_id = int(page_id_str)
            except ValueError:
                page_id = -1
            try:
                frame = int(frame_str)
            except ValueError:
                frame = -1

            current['page_tables'].append({
                'pid':       pid,
                'page_id':   page_id,
                'present':   present == '1',
                'frame':     frame,
                'referenced':ref == '1',
                'modified':  mod == '1'
            })
            continue

        if line.startswith('[PROCESS]'):
            pid_str, state = line[len('[PROCESS]'):].strip().split('|')
            try:
                pid = int(pid_str)
            except ValueError:
                pid = -1
            current['processes'].append({'pid': pid, 'state': state})
            continue


        if line.startswith('[EVENT]'):
            msg = line[len('[EVENT]'):].strip()
            current['events'].append(msg)
            continue

        # ignoramos [STEP_END], [SUMMARY], etc.
    if current:
        steps.append(current)
    return steps


# Função que executa o simulador em C++
def executar_simulador(config, caminho_arquivo):

    phys_bytes = config['memoria_fisica'] * 1024
    swap_bytes = config['memoria_secundaria'] * 1024 * 1024

    cmd = [
        "./webSite/ProjectSite/simulador_memoria",
        caminho_arquivo,
        str(config['tamanho_pagina']),
        str(config['bits_endereco']),
        str(phys_bytes),
        str(swap_bytes),
        '0' if config['algoritmo']=='RELOGIO' else '1'
    ]

    inicio = time.time()
    proc = subprocess.run(cmd, capture_output=True, text=True)

    # DEBUG: exibe na saída do Flask 
    print("=== SIM STDOUT ===")
    print(proc.stdout)
    print("=== SIM STDERR ===")
    print(proc.stderr)
    print("=================")

    if proc.returncode != 0:
        raise RuntimeError(f"Erro no simulador: {proc.stderr}")

    fim = time.time()

    if proc.returncode != 0:
        raise RuntimeError(f"Erro no simulador: {proc.stderr}")

    stdout = proc.stdout

    steps = parse_memory_output(stdout)

    resumo = {}
    for line in stdout.splitlines():
        if line.startswith('[SUMMARY]'):
            m = re.match(r'\[SUMMARY\]\s*(.*?):\s*(\d+)', line)
            if m:
                chave = m.group(1).strip().lower().replace(' ', '_').replace('á','a')
                resumo[chave] = int(m.group(2))
    resumo['tempo_execucao'] = round(fim - inicio, 4)

    return {'steps': steps, 'resumo': resumo}


# Página do simulador de memória
@app.route('/simuladorEntrega3', methods=['POST'])
def simulador_entrega3():
    # Pega dados do formulario
    try:
        tamanho_pagina = int(request.form['tamanho_pagina'])
        bits_endereco = int(request.form['bits_endereco'])
        memoria_fisica_kb = int(request.form['memoria_fisica'])  # em KB
        memoria_secundaria = int(request.form['memoria_secundaria'])  # MB
        algoritmo = request.form['algoritmo']
    except Exception as e:
        return render_template('error.html', error_message="Parâmetros inválidos.")

    # Validações
    memoria_fisica_bytes = memoria_fisica_kb * 1024
    max_addressable = 2 ** bits_endereco

    if tamanho_pagina <= 0:
        return render_template('error.html', error_message="Tamanho de página inválido.")

   # Memória física deve ser múltiplo do tamanho da página
    if memoria_fisica_bytes % tamanho_pagina != 0:
        return render_template(
            'error.html',
            error_message=(
                f"A memória física ({memoria_fisica_bytes} bytes ou {memoria_fisica_kb} KB) "
                f"deve ser múltiplo do tamanho da página ({tamanho_pagina} bytes)."
            )
        )

    # Tamanho da página não pode ser maior que o espaço de endereçamento
    if tamanho_pagina > max_addressable:
        return render_template(
            'error.html',
            error_message=(
                f"Tamanho da página ({tamanho_pagina} bytes) não pode ser maior do que "
                f"o espaço de endereçamento lógico ({max_addressable} bytes)."
            )
        )

    # Monta o config corretamente após validação
    config = {
        'tamanho_pagina': tamanho_pagina,
        'bits_endereco': bits_endereco,
        'memoria_fisica': memoria_fisica_kb,
        'memoria_secundaria': memoria_secundaria,
        'algoritmo': algoritmo
    }

    # Recupera e salva o arquivo enviado
    arquivo = request.files.get('arquivo_operacoes')
    print(request.files)
    if not arquivo:
        return render_template('error.html', error_message="Nenhum arquivo enviado")

    upload_dir = os.path.join(os.getcwd(), 'uploads')
    os.makedirs(upload_dir, exist_ok=True)
    caminho_arquivo = os.path.join(upload_dir, arquivo.filename)

    print("Received file:", arquivo, "Filename:", arquivo.filename)

    arquivo.save(caminho_arquivo)

    try:
        # Executa o simulador C++
        resultado = executar_simulador(config, caminho_arquivo)
        steps = resultado['steps']
        resumo = resultado['resumo']

        # Monta dados_simulacao a partir do último step
        ultima = steps[-1]

        # Memória física
        memoria_fisica = ultima['frames']

        # Tabelas de páginas agrupadas por PID
        tabelas_paginas = {}
        for pt in ultima['page_tables']:
            pid = pt['pid']
            if pid not in tabelas_paginas:
                # encontra o estado do processo
                estado = next(p['state'] for p in ultima['processes'] if p['pid'] == pid)
                tabelas_paginas[pid] = {
                    'estado': estado,
                    'paginas': []
                }
            tabelas_paginas[pid]['paginas'].append({
                'numero':       pt['page_id'],
                'presente':     pt['present'],
                'frame':        pt['frame'],
                'referenciada': pt['referenced'],
                'modificada':   pt['modified']
            })

        # Log de eventos: timestamp = índice do step
        log_eventos = []
        for idx, step in enumerate(steps):
            for ev in step['events']:
                log_eventos.append({
                    'timestamp': idx,
                    'mensagem':  ev
                })

        # Prepara o dicionário para o template
        dados_simulacao = {
            'faltas_pagina':   resumo.get('faltas_de_pagina', 0),
            'operacoes_swap':  resumo.get('operacoes_de_swap', 0),
            'processos_ativos': len(ultima['processes']),
            'memoria_fisica':  memoria_fisica,
            'tabelas_paginas': tabelas_paginas,
            'log_eventos':     log_eventos,
            'swap_used':     ultima['swap']['used'],
            'swap_capacity': ultima['swap']['cap'],
        }

        return render_template(
            'simuladorEntrega3.html',
            config=config,
            dados_simulacao=dados_simulacao,
            steps=steps
        )

    finally:
        # Limpeza: remove o arquivo temporário
        if os.path.exists(caminho_arquivo):
            os.remove(caminho_arquivo)


# FIM DA ENTREGA 3


# Configuração da página da anotação dos professores


ARQUIVO = 'webSite/ProjectSite/data/anotacoes.json'

# Função para carregar anotações do arquivo
def carregar_anotacoes():
    if not os.path.exists(ARQUIVO):
        return []
    with open(ARQUIVO, 'r', encoding='utf-8') as f:
        return json.load(f)

# Função para salvar anotações no arquivo
def salvar_anotacoes(anotacoes):
    print(f"Salvando no arquivo: {ARQUIVO}")
    with open(ARQUIVO, 'w', encoding='utf-8') as f:
        json.dump(anotacoes, f, ensure_ascii=False, indent=2)


@app.route('/anotacoes_professor', methods=['GET', 'POST'])
def anotacoes():
    if request.method == 'POST':
        nome = request.form['nome'].strip()
        texto = request.form['anotacao'].strip()

        if nome and texto:
            nova = {'nome': nome, 'texto': texto}
            anotacoes = carregar_anotacoes()
            anotacoes.insert(0, nova)   # anotação mais recente primeiro
            salvar_anotacoes(anotacoes)
        return redirect(url_for('anotacoes'))
 
    anotacoes = carregar_anotacoes()
    return render_template('anotacoes.html', anotacoes=anotacoes)


if __name__ == '__main__':
    app.run(port=5005, debug=True)