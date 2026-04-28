#!/usr/bin/env python3
import os
import csv
import random
import subprocess
from datetime import datetime

# Configuración
NUM_GENERACIONES = 10000
ID_11 = 4
ID_12 = 5
FICHERO_PESOS_11 = "pesos.csv"
FICHERO_PESOS_12 = "pesos2.csv"
LOG_PATH = "log_evolucion.csv"

# Rangos de cada parámetro
rangos = [
    (0, 100),   # Máximo beneficio distancia
    (0, 100),   # Mínimo beneficio distancia
    (0, 100),   # Máximo Comer Ficha
    (0, 100),   # Mínimo Comer Ficha
    (-100,100), # Comer propia
    (-100, 0),  # Ficha en Casa
    (0, 100),   # Ficha en Casilla Segura
    (0, 100),   # Ficha en Recta Final
    (0, 100),   # Ficha en Casilla Final
    (-100, 0),  # Rebote
    (0, 100),   # Barrera
]

# === Funciones auxiliares ===

def crear_individuo():
    return [round(random.uniform(r[0], r[1]), 2) for r in rangos]

# def mutar(individuo, tasa_mutacion=0.3, desviacion=2.5):
#     nuevo = individuo[:]
#     # for i in range(len(nuevo)):
#     #     if random.random() < tasa_mutacion:
#     #         min_val, max_val = rangos[i]
#     #         nuevo[i] = round(random.uniform(min_val, max_val), 2)
#     if random.random() < tasa_mutacion:
#             min_val, max_val = rangos[i]
#             cambio = random.uniform(-desviacion, desviacion)
#             nuevo_valor = nuevo[i] + cambio
#             # Asegurar que el nuevo valor esté dentro de los rangos
#             nuevo[i] = round(max(min_val, min(max_val, nuevo_valor)), 2)
#     return nuevo

def mutar(individuo, contador_mejoras, tasa_mutacion=0.3, desviacion_max=50.0, desviacion_min=2.5):
    """
    Mutación adaptativa: cuanto más mejora, más suave es la mutación.

    :param individuo: lista de floats (pesos)
    :param contador_mejoras: cuántas veces ha ganado consecutivamente
    :param tasa_mutacion: probabilidad de mutar cada peso
    :param desviacion_max: desviación inicial (cuando no ha ganado nada)
    :param desviacion_min: desviación mínima (si ha ganado muchas veces)
    """
    nuevo = individuo[:]

    # A partir de 10 victorias seguidas la desviación será constante
    contador_mejoras=min(contador_mejoras,10)
    desviacion = ((desviacion_min - desviacion_max)/10)*contador_mejoras + desviacion_max

    for i in range(len(nuevo)):
        if random.random() < tasa_mutacion:
            min_val, max_val = rangos[i]
            cambio = random.uniform(-desviacion, desviacion)
            nuevo_valor = nuevo[i] + cambio
            nuevo[i] = round(max(min_val, min(max_val, nuevo_valor)), 2)
    return nuevo

def guardar_pesos(pesos, fichero):
    with open(fichero, "w") as f:
        f.write(",".join(map(str, pesos)) + "\n")

def cargar_pesos(fichero):
    with open(fichero, "r") as f:
        return [float(x) for x in f.readline().strip().split(",")]

# guardar_log(generacion, hijo, "Victoria" if resultado_2 == 1 else "Derrota", ID_12)
def guardar_log(generacion, pesos, resultado, empieza):
    encabezado = [
        "generacion",
        "empieza",
        "min_distancia",
        "max_distancia",
        "comer_propia",
        "max_comer",
        "min_comer",
        "ficha_casa",
        "casilla_segura",
        "recta_final",
        "casilla_final",
        "rebote",
        "barrera",
        "resultado",
        "timestamp"
    ]
    existe = os.path.exists(LOG_PATH)
    with open(LOG_PATH, mode="a", newline="") as f:
        writer = csv.writer(f)
        if not existe:
            writer.writerow(encabezado)
        writer.writerow([generacion, empieza] + pesos + [resultado, datetime.now().isoformat()])

def ejecutar_partida(id1, id2):
    resultado = subprocess.run(
        ["./ParchisGame", "--p1", "AI", str(id1), "jugador1", "--p2", "AI", str(id2), "jugador2", "--no-gui"],
        capture_output=True, text=True
    )
    salida = resultado.stdout

    if "Ha ganado el jugador 2" in salida:
        return 2
    elif "Ha ganado el jugador 1" in salida:
        return 1
    else:
        return 0  # Empate o error

# === Bucle principal ===

if __name__ == "__main__":

    # Cargo la información del 
    try:
        campeon = cargar_pesos(FICHERO_PESOS_11)
        print("📥 Campeón inicial cargado desde pesos.csv")
    except FileNotFoundError:
        campeon = crear_individuo()
        guardar_pesos(campeon, FICHERO_PESOS_11)
        print("🆕 No se encontró pesos.csv. Generado nuevo campeón aleatorio.")

    contador_victorias=0

    for generacion in range(1, NUM_GENERACIONES + 1):

        hijo = mutar(campeon, contador_mejoras=contador_victorias)
        guardar_pesos(hijo, FICHERO_PESOS_12)

        resultado_1 = ejecutar_partida(ID_11, ID_12)
        guardar_log(generacion, hijo, "Victoria" if resultado_1 == 2 else "Derrota", ID_12)

        puntuacion_id12 = 0

        if resultado_1 == 2:
            print(f"✔️ Gen {generacion}, Empieza id {ID_11}: ¡Nuevo campeón!")
            puntuacion_id12 += 0.5

            # Solo juega la segunda si gana la primera
            resultado_2 = ejecutar_partida(ID_12, ID_11)
            guardar_log(generacion, hijo, "Victoria" if resultado_2 == 1 else "Derrota", ID_12)

            if resultado_2 == 1:
                print(f"✔️ Gen {generacion}, Empieza id {ID_12}: ¡Nuevo campeón!")
                puntuacion_id12 += 0.5
            else:
                print(f"❌ Gen {generacion}, Empieza id {ID_12}: Derrota o empate")
        else:
            print(f"❌ Gen {generacion}, Empieza id {ID_11}: Derrota o empate")

        guardar_log(generacion, hijo, "Victoria" if resultado_1 == 2 else "Derrota", ID_11)


        # Si gana 1: 0.5, si gana las 2: 1 y si pierde las 2: 0


        if puntuacion_id12 == 1: # Que gane las 2
            contador_victorias=0
            campeon = hijo
            guardar_pesos(campeon, FICHERO_PESOS_11)
        elif puntuacion_id12 == -1: # Se quita esta opción
            contador_victorias=0
            for i in range(len(hijo)):
                campeon[i] = (campeon[i]+hijo[i])/2   # La media
            guardar_pesos(campeon, FICHERO_PESOS_11)            
        else:
            contador_victorias+=1
            

    print("🏁 Optimización finalizada.")
