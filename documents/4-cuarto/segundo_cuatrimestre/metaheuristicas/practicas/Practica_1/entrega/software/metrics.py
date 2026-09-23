import subprocess
import pandas as pd
import os

# ==========================================
# CONFIGURACIÓN DE LA EJECUCIÓN
# ==========================================
EXECUTABLE = "./code/build/main" 
DATAFILE = "./code/data/"

SEEDS = [12345, 67890, 13579, 24680, 98765, 43210, 10293, 84756, 97531, 8642]

CONFIGURACIONES = [
    ("zoo", 15, 7),
    ("zoo", 30, 7),
    ("glass", 15, 7),
    ("glass", 30, 7),
    ("bupa", 15, 16),
    ("bupa", 30, 16)
]

def main():
    if not os.path.exists(EXECUTABLE):
        print(f"Error: No se encuentra el ejecutable '{EXECUTABLE}'. Compila tu código C++ primero.")
        return

    todas_las_filas = []
    
    # SOLUCIÓN: Traducimos las columnas desde el principio para que coincidan 
    # con los nombres que estás usando en el .agg() más abajo
    columnas = [
        "Algorithm", "Seed", "Dataset", "Ratio", "K", 
        "Fitness", "Distancia", "Incumplimiento", "Evaluaciones", "Time_ms"
    ]

    print("Iniciando batería de pruebas...")
    print("-" * 50)

    # Bucle principal de ejecución
    for dataset, ratio, k in CONFIGURACIONES:
        for seed in SEEDS:
            print(f"Ejecutando -> Dataset: {dataset} | Ratio: {ratio}% | K: {k} | Semilla: {seed}")
            
            # Construimos el comando a ejecutar
            comando = [EXECUTABLE, str(seed), DATAFILE + dataset, str(ratio), str(k)]
            
            # Ejecutamos y capturamos la salida
            resultado = subprocess.run(comando, capture_output=True, text=True)
            
            if resultado.returncode != 0:
                print(f"¡ERROR en la ejecución [{' '.join(comando)}]")
                print(resultado.stderr)
                continue
            
            # Procesamos la salida del C++
            lineas = resultado.stdout.strip().split('\n')
            for linea in lineas:
                # Ignoramos la cabecera del C++ 
                if "Algorithm,Seed" in linea or not linea.strip():
                    continue
                
                # Separamos los valores por coma y los guardamos
                valores = linea.split(',')
                if len(valores) == len(columnas):
                    todas_las_filas.append(valores)

    print("-" * 50)
    print("Pruebas finalizadas. Procesando métricas...\n")

    # 1. Crear el DataFrame
    df = pd.DataFrame(todas_las_filas, columns=columnas)

    # 2. Convertir las columnas a numéricas (actualizado con los nombres en español)
    columnas_numericas = ["Seed", "Ratio", "K", "Fitness", "Distancia", "Incumplimiento", "Evaluaciones", "Time_ms"]
    df[columnas_numericas] = df[columnas_numericas].apply(pd.to_numeric)

    # 3. Guardar los resultados brutos
    df.to_csv("resultados_brutos.csv", index=False)
    
    # 4. Agrupar por Algoritmo, Dataset y Ratio (ahora sí encuentra las columnas)
    resumen = df.groupby(["Algorithm", "Dataset", "Ratio"]).agg({
        "Fitness": ["mean", "std"],
        "Distancia": ["mean", "std"],
        "Incumplimiento": ["mean", "std"],
        "Evaluaciones": "mean",
        "Time_ms": ["mean", "std"],
    }).reset_index()

    # --- APLANAR CABECERAS PARA QUE NO SE VEA FEO ---
    nuevas_columnas = []
    for col in resumen.columns:
        if col[1]: # Si tiene sub-nivel (ej. 'mean' o 'std')
            nuevas_columnas.append(f"{col[0]}_{col[1]}")
        else:      # Si no tiene (ej. 'Algorithm')
            nuevas_columnas.append(col[0])
    resumen.columns = nuevas_columnas

    # Redondeamos a 4 decimales
    resumen = resumen.round(4)

    # Mostrar por pantalla la tabla resumen
    print("================================================= RESUMEN DE MÉTRICAS =================================================")
    print(resumen.to_string(index=False, justify='center'))
    print("=======================================================================================================================")

    # Guardamos también la tabla final limpia
    resumen.to_csv("tabla_resumen_memoria.csv", index=False)

if __name__ == "__main__":
    main()