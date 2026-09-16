#!/usr/bin/env python3
import os
import csv
import random
import subprocess
from datetime import datetime

# Configuración
ID_11 = 4

# 2: Gana a ninja 1
# 3: Gana a ninja 2
# 1: No gana a nada (siuuu)

def ejecutar_partida_empiezo_yo(id1, id2):
    resultado = subprocess.run(
        ["./ParchisGame", "--p1", "AI", str(id1), "jugador1", "--p2", "ninja", str(id2), "jugador2", "--no-gui"],
        capture_output=True, text=True
    )
    salida = resultado.stdout

    if "Ha ganado el jugador 2" in salida:
        return 2
    elif "Ha ganado el jugador 1" in salida:
        return 1
    else:
        return 0  # Empate o error
    
def ejecutar_partida_empieza_ninja(id1, id2):
    resultado = subprocess.run(
        ["./ParchisGame", "--p2", "AI", str(id1), "jugador1", "--p1", "ninja", str(id2), "jugador2", "--no-gui"],
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

    for i in {1,2,3}:
        ninja = i

        print(f"Enfrentando a local {ID_11} contra ninja {ninja} (empieza {ID_11})...")
        resultado = ejecutar_partida_empiezo_yo(ID_11,ninja) 
        if(resultado == 1):
            print("✔️ Victoria")
        else: 
            print("❌ Derrota")
        
        print(f"Enfrentando a local {ID_11} contra ninja {ninja} (empieza ninja)...")
        resultado = ejecutar_partida_empieza_ninja(ID_11,ninja) 
        if(resultado == 2):
            print("✔️ Victoria")
        else:
            print("❌ Derrota")

