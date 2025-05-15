from flask import Flask, render_template, jsonify, request
from ctypes import CDLL, c_int, c_char_p, create_string_buffer, byref, POINTER
import re
import time

app = Flask(__name__)

#Load the C library
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

@app.route("/entrega1/open_file")
def entrega1_open_file():
    return render_template('entrega1-open_file.html')

@app.route("/entrega1/write_file")
def entrega1_write_file():
    return render_template('entrega1-write_file.html')

@app.route("/entrega1/read_file")
def entrega1_read_file():
    return render_template('entrega1-read_file.html')

@app.route("/entrega1/mmap")
def entrega1_mmap():
    return render_template('entrega1-mmap.html')

@app.route("/entrega1/mprotect")
def entrega1_mprotect():
    return render_template('entrega1-mprotect.html')

@app.route("/entrega1/munmap")
def entrega1_munmap():
    return render_template('entrega1-munmap.html')

@app.route("/entrega1/fork")
def entrega1_fork():
    return render_template('entrega1-fork.html')

@app.route("/entrega1/pause")
def entrega1_pause():
    return render_template('entrega1-pause.html')

@app.route("/entrega1/sleep")
def entrega1_sleep():
    return render_template('entrega1-sleep.html')
    
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
                'buffer': buffer_content.copy()  # MUITO IMPORTANTE!
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

#@app.route('/entrega3')
#def entrega3():
    #return render_template('entrega3.html')


if __name__ == '__main__':
    app.run(port=5001, debug=True)