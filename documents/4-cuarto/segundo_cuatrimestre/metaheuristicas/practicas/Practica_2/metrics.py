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
    
    columnas = [
        "Algorithm", "Seed", "Dataset", "Ratio", "K", 
        "Fitness", "Distancia", "Incumplimiento", "Evaluaciones", "Time_ms"
    ]

    print("Iniciando batería de pruebas...")
    print("-" * 50)

    for dataset_arg, ratio, k in CONFIGURACIONES:
        for seed in SEEDS:
            print(f"Ejecutando -> Dataset: {dataset_arg} | Ratio: {ratio}% | K: {k} | Semilla: {seed}")
            
            comando = [EXECUTABLE, str(seed), DATAFILE + dataset_arg, str(ratio), str(k)]
            resultado = subprocess.run(comando, capture_output=True, text=True)
            
            if resultado.returncode != 0:
                print(f"¡ERROR en la ejecución [{' '.join(comando)}]")
                continue
            
            lineas = resultado.stdout.strip().split('\n')
            for linea in lineas:
                if "Algorithm,Seed" in linea or not linea.strip():
                    continue
                
                valores = linea.split(',')
                if len(valores) == len(columnas):
                    # --- CAMBIO AQUÍ: Limpiar la ruta del dataset ---
                    # Suponiendo que el dataset está en la posición 2 de la lista 'valores'
                    # (según el orden de la lista 'columnas')
                    valores[2] = os.path.basename(valores[2])
                    
                    todas_las_filas.append(valores)

    print("-" * 50)
    print("Pruebas finalizadas. Procesando métricas...\n")

    if not todas_las_filas:
        print("Error: No se capturaron datos. Revisa la salida de tu ejecutable C++.")
        return

    df = pd.DataFrame(todas_las_filas, columns=columnas)

    columnas_numericas = ["Seed", "Ratio", "K", "Fitness", "Distancia", "Incumplimiento", "Evaluaciones", "Time_ms"]
    df[columnas_numericas] = df[columnas_numericas].apply(pd.to_numeric)

    # Ahora el CSV se guardará con nombres limpios (ej: 'bupa' en lugar de './code/data/bupa')
    df.to_csv("resultados_brutos.csv", index=False)

    # Definir el orden categórico deseado
    orden_algoritmos = ["AGG-UN", "AGG-SF", "AGE-UN", "AGE-SF", "AM-All", "AM-Rand", "AM-Best"]
    
    # Convertir a categoría con el orden especificado
    df['Algorithm'] = pd.Categorical(df['Algorithm'], categories=orden_algoritmos, ordered=True)

    # Al agrupar, pandas respetará el orden de la categoría
    resumen = df.groupby(["Algorithm", "Dataset", "Ratio"]).agg({
        "Fitness": ["mean", "std"],
        "Distancia": ["mean", "std"],
        "Incumplimiento": ["mean", "std"],
        "Evaluaciones": "mean",
        "Time_ms": ["mean", "std"],
    }).reset_index()

    nuevas_columnas = []
    for col in resumen.columns:
        if col[1]:
            nuevas_columnas.append(f"{col[0]}_{col[1]}")
        else:
            nuevas_columnas.append(col[0])
    resumen.columns = nuevas_columnas

    resumen = resumen.round(4)

    print("================================================= RESUMEN DE MÉTRICAS =================================================")
    print(resumen.to_string(index=False, justify='center'))
    print("=======================================================================================================================")

    resumen.to_csv("tabla_resumen_memoria.csv", index=False)

if __name__ == "__main__":
    main()