import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

def main():
    archivo_bruto = "resultados_brutos.csv"
    output_dir = "latex/graficos"
    
    # 1. Comprobamos que el archivo existe
    if not os.path.exists(archivo_bruto):
        print(f"Error: No se encuentra '{archivo_bruto}'.")
        return

    print(f"Leyendo datos de {archivo_bruto}...")
    df = pd.read_csv(archivo_bruto)

    # Limpiamos los nombres de los datasets
    df['Dataset'] = df['Dataset'].str.replace('./code/data/', '')

    datasets = df['Dataset'].unique()
    ratios = df['Ratio'].unique()
    
    # Definimos unos colores agradables para las cajas (estilo pastel)
    colores = ['#8dd3c7', '#ffffb3', '#bebada']

    print("Generando gráficos de cajas con Matplotlib...")
    print("-" * 50)

    for dataset in datasets:
        for ratio in ratios:
            df_filtrado = df[(df['Dataset'] == dataset) & (df['Ratio'] == ratio)]
            
            if df_filtrado.empty:
                continue

            # Extraemos los algoritmos manteniendo el orden
            algoritmos = df_filtrado['Algorithm'].unique()
            
            # Preparamos una lista de listas con los valores de Fitness por cada algoritmo
            datos_por_algoritmo = [
                df_filtrado[df_filtrado['Algorithm'] == alg]['Fitness'].values 
                for alg in algoritmos
            ]

            fig, ax = plt.subplots(figsize=(8, 6))
            
            # 2. Generamos el Boxplot
            # patch_artist=True permite rellenar las cajas con color
            bp = ax.boxplot(
                datos_por_algoritmo, 
                positions=np.arange(1, len(algoritmos) + 1),
                widths=0.5,
                patch_artist=True,
                medianprops=dict(color="black", linewidth=1.5) # Línea de la mediana en negro
            )

            # Pintamos cada caja con un color distinto
            for patch, color in zip(bp['boxes'], colores):
                patch.set_facecolor(color)
                patch.set_alpha(0.8) # Un poco de transparencia

            # 3. Añadimos los puntos de las semillas individuales (Jitter manual)
            for i, valores in enumerate(datos_por_algoritmo):
                # Generamos un pequeño "ruido" horizontal para que los puntos no se pisen
                x_jitter = np.random.normal(i + 1, 0.04, size=len(valores))
                ax.plot(x_jitter, valores, 'k.', alpha=0.6, markersize=10)

            # 4. Títulos, etiquetas y formato de los ejes
            ax.set_xticks(np.arange(1, len(algoritmos) + 1))
            ax.set_xticklabels(algoritmos, fontsize=11)
            
            plt.title(f'Distribución del Fitness - Dataset {dataset.capitalize()} ({ratio}%)', fontsize=14, pad=15)
            plt.ylabel('Valor de Fitness (menor es mejor)', fontsize=12)
            plt.xlabel('Algoritmo', fontsize=12)
            
            # Cuadrícula sutil de fondo para facilitar la lectura
            ax.yaxis.grid(True, linestyle='--', which='major', color='lightgrey', alpha=0.7)
            ax.set_axisbelow(True) # Para que la cuadrícula quede por detrás de las cajas

            # 5. Guardamos en PDF
            nombre_archivo = f'{output_dir}/boxplot_{dataset}_{ratio}.pdf'
            plt.savefig(nombre_archivo, bbox_inches='tight', format='pdf')
            plt.close()
            
            print(f"  -> Guardado: {nombre_archivo}")

    print("-" * 50)
    print("¡Todos los gráficos generados con éxito!")

if __name__ == "__main__":
    main()