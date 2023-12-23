import serial
import pyautogui
import math
import re


def centralizar_cursor():
    # Obtém o tamanho da tela
    largura, altura = pyautogui.size()

    # Calcula as coordenadas do centro da tela



    centro_x = largura // 2
    centro_y = altura // 2

    # Move o cursor para o centro da tela
    pyautogui.moveTo(centro_x, centro_y)

    print("largura:", largura)
    print("altura:", altura)

# Chamando a função para centralizar o cursor
centralizar_cursor()

def largura(angulo):
    # Convertendo o ângulo para radianos, pois a função tan em Python usa radianos
    angulo_radianos = math.radians(angulo)

    # Calculando a largura usando a função tangente
    resultado = math.tan(angulo_radianos)*20

    #if abs(resultado)>683:
    #    return 683

    # Arredondando para um inteiro antes de retornar
    return abs(int(resultado))

def altura(angulo):
    # Convertendo o ângulo para radianos, pois a função tan em Python usa radianos
    angulo_radianos = math.radians(angulo)

    # Calculando a largura usando a função tangente
    resultado = math.tan(angulo_radianos) * 10

    #if abs(resultado)>384:
    #    return (384)

    # Arredondando para um inteiro antes de retornar
    return abs(int(resultado))

import re

def extrair_numero_da_string_direita(entrada):
    # Expressão regular para encontrar o número após "Direita"
    padrao = r"Direita\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return numero
    else:
        return 0


    
def extrair_numero_da_string_esquerda(entrada):
    # Expressão regular para encontrar o número após "Esquerda"
    padrao = r"Esquerda\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return numero
    else:
        return 0

def extrair_numero_da_string_baixo(entrada):
    # Expressão regular para encontrar o número após "Baixo"
    padrao = r"Baixo\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return numero
    else:
        return 0
    
def extrair_numero_da_string_cima(entrada):
    # Expressão regular para encontrar o número após "Cima"
    padrao = r"Cima\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return numero
    else:
        return None

def extrair_numero_da_string_direita_enter(entrada):
    # Expressão regular para encontrar o número após "Direita eixo x"
    padrao = r"Direita eixo x\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return numero
    else:
        return 0

def extrair_numero_da_string_esquerda_click(entrada):
    # Expressão regular para encontrar o número após "Esquerda eixo x"
    padrao = r"Esquerda eixo x\s*(-?\d+\.\d+)"

    # Aplicar a expressão regular na string
    correspondencia = re.search(padrao, entrada)

    # Verificar se houve correspondência e extrair o número
    if correspondencia:
        numero = float(correspondencia.group(1))
        return abs(numero)  # Retorna o valor absoluto para garantir que seja positivo
    else:
        return None


# Configura a porta serial (ajuste a porta conforme necessário)
port = 'COM3'
baud_rate = 9600

# Inicializa a comunicação serial
ser = serial.Serial(port, baud_rate, timeout=1)

# Espera a inicialização da porta serial
ser.flush()

# Nome do arquivo de texto para salvar os dados
file_name = 'dados_serial.txt'
# Obtém o tamanho da tela
largura_tela, altura_tela = pyautogui.size()

# Calcula as coordenadas do centro da tela
x = largura_tela // 2
y = altura_tela // 2


#y=384
#x=683

try:
    with open(file_name, 'w') as file:

        # Obtém o tamanho da tela
        #largura, altura = pyautogui.size()

        
        
        while True:
            # Lê os dados da porta serial
            serial_data = ser.readline().decode('utf-8')

            if "Direita eixo x" in serial_data:
                ang=extrair_numero_da_string_direita_enter(serial_data)
                if abs(ang)> 20:
                    pyautogui.press('enter')
                    print("apertou enter")

            #elif "Esquerda eixo x" in serial_data:
            #    ang=extrair_numero_da_string_esquerda_click(serial_data)
            #    if abs(ang)> 40:
            #        pyautogui.doubleClick
            #        print("clicou")


            elif "Esquerda eixo x" in serial_data:
                ang=extrair_numero_da_string_esquerda_click(serial_data)
                if abs(ang)> 20:

                    pyautogui.click()
                    print("clicou")

            elif "Direita" in serial_data:
                angulo=float(extrair_numero_da_string_direita(serial_data))
                print("ângulo para direita: ", angulo)
                print("===============Função Largura====================")
                print(largura(angulo), "  ", type(largura(angulo)))
                print("=================================================")
                if angulo>150:
                    break
                complemento = largura(angulo)
                x+= complemento#largura(angulo)
                # Move o cursor para o centro da tela
                pyautogui.moveTo(x, y)
            elif "Esquerda" in serial_data:
                angulo=float(extrair_numero_da_string_esquerda(serial_data))
                print("===============Função Largura====================")
                print(largura(angulo), "  ", type(largura(angulo)))
                print("=================================================")
                decremento = largura(angulo)
                x-= decremento #largura(angulo)
                # Move o cursor para o centro da tela
                pyautogui.moveTo(x, y)
            elif "Baixo" in serial_data:
                #print("===============Função Altura====================")
                #print(altura(angulo), "  ", type(altura(angulo)))
                #print("=================================================")
                angulo=float(extrair_numero_da_string_baixo(serial_data))
                y+=altura(angulo)
                pyautogui.moveTo(x, y)
            elif "Cima" in serial_data:
                angulo=float(extrair_numero_da_string_cima(serial_data))
                y-=altura(angulo)
                pyautogui.moveTo(x, y)
            
            


            # Exibe os dados no console
            print(serial_data.strip())

            # Grava os dados no arquivo
            file.write(serial_data)

except KeyboardInterrupt:
    # Se o usuário pressionar Ctrl+C, encerra o programa
    ser.close()
