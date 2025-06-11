from flask import Flask, render_template, jsonify, request
from ctypes import CDLL, c_int, c_char_p, create_string_buffer, byref, POINTER
import re
import time
import subprocess
import os

app = Flask(__name__)

# biblioteca para o problema produtor-consumidor 
lib = CDLL('./webSite/ProjectSite/libpc_lib.so')


# Define tipos das funções
lib.iniciar_simulacao.argtypes = [c_int, c_int, c_int]
lib.get_step_count.restype = c_int
lib.get_step.argtypes = [c_int]
lib.get_step.restype = c_char_p
lib.get_simulation_log.argtypes = [c_char_p, c_int]
lib.get_simulation_log.restype = c_int

# Definir os tipos de argumentos corretamente
lib.get_status.argtypes = [POINTER(c_int), POINTER(c_int), POINTER(c_int), c_int]
lib.get_status.restype = None


@app.route('/')
def home():
    return render_template('home.html')
    

@app.route('/entrega1')
def entrega1():
    return render_template('entrega1.html')

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
    
@app.route("/entrega1/Processos_I/O-bound")
def entrega1_IOBound():
    return render_template('entrega1-IOBound.html')

@app.route("/entrega1/Processos_CPU-bound")
def entrega1_CPUBound():
    return render_template('entrega1-CPUBound.html')


@app.route('/entrega2', methods=['GET', 'POST'])
def entrega2():

    buffer_status = None  # Define a variável antes do bloco condicional

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


@app.route('/simuladorEntrega2', methods=['POST'])
def simulacao():
    num_produtores = int(request.form['num_produtores'])
    num_consumidores = int(request.form['num_consumidores'])
    buffer_size = int(request.form['buffer_size'])

    inicio = time.time()

    # Chama a simulação em C
    lib.iniciar_simulacao(num_produtores, num_consumidores, buffer_size)

    fim = time.time()
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



    # Passa para o template
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


@app.route('/entrega3')
def entrega3():
    return render_template('entrega3.html')


def parse_memory_output(output):
    """Converte a saída do simulador C++ em estrutura de dados Python"""
    frames = []
    current_frame = None
    
    for line in output.split('\n'):
        if not line.strip():
            continue
            
        # Identificar frames: [FRAME] id|pid|pagina|referenciado|modificado
        if line.startswith('[FRAME]'):
            if current_frame:
                frames.append(current_frame)
            parts = line.split(maxsplit=1)[1].split('|')
            current_frame = {
                'id': int(parts[0]),
                'alocado': True,
                'pid': parts[1],
                'pagina': int(parts[2]),
                'referenciado': parts[3] == '1',
                'modificado': parts[4] == '1'
            }
        elif line.startswith('[PAGE_TABLE]'):
            # Esta parte seria para tabelas de páginas
            pass
        elif line.startswith('[EVENT]'):
            # Esta parte seria para eventos
            pass
            
    if current_frame:
        frames.append(current_frame)
        
    return frames

def executar_simulador(config, caminho_arquivo):
    """Executa o simulador C++ e processa os resultados"""
    # Construir comando para o simulador
    comando = [
        "./simulador_memoria",  # Nome do executável C++
        caminho_arquivo,
        str(config['tamanho_pagina']),
        str(config['bits_endereco']),
        str(config['memoria_fisica'] * 1024),  # Convertendo KB para bytes
        str(config['memoria_secundaria'] * 1024 * 1024),  # Convertendo MB para bytes
        '0' if config['algoritmo'] == 'LRU' else '1'
    ]
    
    # Executar e capturar saída
    inicio = time.time()
    resultado = subprocess.run(comando, capture_output=True, text=True)
    fim = time.time()
    
    if resultado.returncode != 0:
        raise RuntimeError(f"Erro no simulador: {resultado.stderr}")
    
    # Processar saída para formato de simulação passo a passo
    steps = []
    buffer = []
    current_step = None
    memoria_atual = []
    
    for line in resultado.stdout.split('\n'):
        if not line.strip():
            continue
            
        # Operação detectada
        if line.startswith('>>> Processando:'):
            if current_step:
                steps.append(current_step)
                
            parts = line.split(':', 1)[1].strip().split()
            pid = parts[0]
            tipo = parts[1]
            endereco = parts[2] if len(parts) > 2 else None
            
            current_step = {
                'descricao': f"{pid} {tipo} {endereco}" if endereco else f"{pid} {tipo}",
                'memoria_fisica': memoria_atual.copy(),
                'evento': None
            }
            
        # Frame atualizado
        elif line.startswith('[FRAME]'):
            parts = line.split(maxsplit=1)[1].split('|')
            frame_id = int(parts[0])
            frame_data = {
                'id': frame_id,
                'alocado': parts[1] == '1',
                'pid': parts[2] if parts[2] != '-' else None,
                'pagina': int(parts[3]) if parts[3] != '-' else -1,
                'referenciado': parts[4] == '1',
                'modificado': parts[5] == '1'
            }
            
            # Atualizar frame na memória
            for i, frame in enumerate(memoria_atual):
                if frame['id'] == frame_id:
                    memoria_atual[i] = frame_data
                    break
            else:
                memoria_atual.append(frame_data)
                
        # Evento detectado
        elif line.startswith('[EVENT]'):
            event_type = "info"
            if "FALTA" in line:
                event_type = "falta"
            elif "SWAP" in line:
                event_type = "swap"
                
            current_step['evento'] = {
                'tipo': event_type,
                'mensagem': line.split(']', 1)[1].strip()
            }
            
        # Estado do processo
        elif line.startswith('[PROCESS]'):
            # Formato: [PROCESS] P1:READY
            pass
            
    if current_step:
        steps.append(current_step)
    
    # Gerar relatório final
    resumo = {
        'faltas_pagina': sum(1 for step in steps if step.get('evento', {}).get('tipo') == 'falta'),
        'operacoes_swap': sum(1 for step in steps if step.get('evento', {}).get('tipo') == 'swap'),
        'tempo_execucao': round(fim - inicio, 4)
    }
    
    return {
        'steps': steps,
        'resumo': resumo
    }

@app.route('/simuladorEntrega3', methods=['POST'])
def simulador_entrega3():
    # Obter configurações do formulário
    config = {
        'tamanho_pagina': int(request.form['tamanho_pagina']),
        'bits_endereco': int(request.form['bits_endereco']),
        'memoria_fisica': int(request.form['memoria_fisica']),
        'memoria_secundaria': int(request.form['memoria_secundaria']),
        'algoritmo': request.form['algoritmo']
    }
    
    # Obter arquivo
    arquivo = request.files['arquivo_operacoes']
    
    # Salvar arquivo temporariamente
    upload_dir = os.path.join(os.getcwd(), 'uploads')
    os.makedirs(upload_dir, exist_ok=True)
    caminho_arquivo = os.path.join(upload_dir, arquivo.filename)
    arquivo.save(caminho_arquivo)
    
    # Executar simulador
    try:
        resultado = executar_simulador(config, caminho_arquivo)
        return render_template(
            'simuladorEntrega3.html',
            dados_simulacao=resultado,
            config=config
        )
    except Exception as e:
        return render_template('error.html', error_message=str(e))
    
    # Remover arquivo temporário após uso
    finally:
        if os.path.exists(caminho_arquivo):
            os.remove(caminho_arquivo)




if __name__ == '__main__':
    app.run(port=5001, debug=True)