import pandas as pd
import os

def main():
    # 1. Cargar los resultados
    try:
        df = pd.read_csv("resultados_brutos.csv")
    except FileNotFoundError:
        print("Error: No se encuentra 'resultados_brutos.csv'.")
        return

    # 2. Agrupar y calcular medias
    # sort=False impide que Pandas ordene alfabéticamente; respeta el orden del CSV
    resumen = df.groupby(['Dataset', 'Ratio', 'Algorithm', 'K'], sort=False).agg({
        'Fitness': 'mean',
        'Distancia': 'mean',
        'Incumplimiento': 'mean',
        'Evaluaciones': 'mean',
        'Time_ms': 'mean'
    }).reset_index()

    output_dir = "latex/tablas"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 3. Generar un archivo por cada combinación de Dataset y Ratio
    # Mantenemos sort=False para que el bucle siga el orden del CSV
    for (dataset, ratio), group in resumen.groupby(['Dataset', 'Ratio'], sort=False):
        k_val = int(group['K'].iloc[0])
        filename = f"tabla_{dataset}_{ratio}.tex"
        filepath = os.path.join(output_dir, filename)

        latex_content = []
        latex_content.append("\\begin{table}[H]")
        latex_content.append("    \\centering")
        latex_content.append("    \\begin{tabular}{lrrrrr}")
        latex_content.append("        \\hline")
        latex_content.append("        Algoritmo & Fitness & Distancia & Incumplimiento & Evaluaciones & Tiempo (ms)\\\\")
        latex_content.append("        \\hline")

        for _, row in group.iterrows():
            fila = (f"        {row['Algorithm']} & {row['Fitness']:.4f} & {row['Distancia']:.4f} & "
                    f"{row['Incumplimiento']:.4f} & {row['Evaluaciones']:.1f} & {row['Time_ms']:.1f} \\\\")
            latex_content.append(fila)

        latex_content.append("        \\hline")
        latex_content.append("    \\end{tabular}")
        latex_content.append(f"    \\caption{{Resultados obtenidos para el dataset \\textbf{{{dataset.capitalize()}}} con {ratio}\\% de restricciones en  10 ejecuciones independientes (k={k_val}).}}")
        latex_content.append(f"    \\label{{tab:res_{dataset}_{ratio}}}")
        latex_content.append("\\end{table}")

        with open(filepath, "w") as f:
            f.write("\n".join(latex_content))
        
        print(f"Archivo generado: {filepath}")

if __name__ == "__main__":
    main()