import pandas as pd

def main():
    try:
        df = pd.read_csv("tabla_resumen_memoria.csv")
    except FileNotFoundError:
        print("Error: No se encuentra 'tabla_resumen_memoria.csv'.")
        return

    latex_code = [
        "% Ajustes de espaciado",
        "\\begingroup",
        "\\fontsize{9}{12}\\selectfont",
        "\\setlength{\\tabcolsep}{3pt}",
        "\\begin{longtable}{llcrrrrrrrrr}",
        # Eliminamos el caption de aquí
        "    \\toprule",
        "    \\multirow{2}{*}{\\textbf{Algoritmo}} & \\multirow{2}{*}{\\textbf{Dataset}} & \\multirow{2}{*}{\\textbf{Ratio}} & \\multicolumn{2}{c}{\\textbf{Fitness}} & \\multicolumn{2}{c}{\\textbf{Distancia}} & \\multicolumn{2}{c}{\\textbf{Incumplimiento}} & \\multirow{2}{*}{\\textbf{Evals}} & \\multicolumn{2}{c}{\\textbf{Tiempo (ms)}} \\\\",
        "    \\cmidrule(lr){4-5} \\cmidrule(lr){6-7} \\cmidrule(lr){8-9} \\cmidrule(lr){11-12}",
        "    & & & \\textbf{Media} & \\textbf{Std} & \\textbf{Media} & \\textbf{Std} & \\textbf{Media} & \\textbf{Std} & & \\textbf{Media} & \\textbf{Std} \\\\",
        "    \\midrule",
        "    \\endfirsthead",
        "",
        "    \\toprule",
        "    \\multirow{2}{*}{\\textbf{Algoritmo}} & \\multirow{2}{*}{\\textbf{Dataset}} & \\multirow{2}{*}{\\textbf{Ratio}} & \\multicolumn{2}{c}{\\textbf{Fitness}} & \\multicolumn{2}{c}{\\textbf{Distancia}} & \\multicolumn{2}{c}{\\textbf{Incumplimiento}} & \\multirow{2}{*}{\\textbf{Evals}} & \\multicolumn{2}{c}{\\textbf{Tiempo (ms)}} \\\\",
        "    \\cmidrule(lr){4-5} \\cmidrule(lr){6-7} \\cmidrule(lr){8-9} \\cmidrule(lr){11-12}",
        "    & & & \\textbf{Media} & \\textbf{Std} & \\textbf{Media} & \\textbf{Std} & \\textbf{Media} & \\textbf{Std} & & \\textbf{Media} & \\textbf{Std} \\\\",
        "    \\midrule",
        "    \\endhead",
        "",
        "    \\midrule",
        "    \\endfoot",
        "",
        "    \\bottomrule",
        # El caption lo ponemos aquí para que salga solo al final de la tabla
        "    \\noalign{\\vspace{10pt}}",
        "    \\caption{Resumen de métricas de rendimiento por algoritmo, dataset y ratio.} \\label{tab:metricas_resultados} \\\\",
        "    \\endlastfoot"
    ]

    algoritmo_actual = None
    for index, row in df.iterrows():
        algoritmo = str(row['Algorithm'])
        if algoritmo_actual is not None and algoritmo != algoritmo_actual:
            latex_code.append("    \\midrule")
        algoritmo_actual = algoritmo

        # Formateo de números (4 decimales para métricas, 1 para tiempo/evals)
        fila = (f"    {algoritmo} & {row['Dataset']} & {int(row['Ratio'])} & "
                f"{row['Fitness_mean']:.4f} & {row['Fitness_std']:.4f} & "
                f"{row['Distancia_mean']:.4f} & {row['Distancia_std']:.4f} & "
                f"{row['Incumplimiento_mean']:.4f} & {row['Incumplimiento_std']:.4f} & "
                f"{row['Evaluaciones_mean']:.0f} & {row['Time_ms_mean']:.1f} & {row['Time_ms_std']:.2f} \\\\")
        latex_code.append(fila)
    
    latex_code.append("\\end{longtable}")
    latex_code.append("\\endgroup")

    with open("latex/tablas/tabla_general.tex", "w") as f:
        f.write("\n".join(latex_code))
    print("¡Tabla larga generada con éxito!")

if __name__ == "__main__":
    main()