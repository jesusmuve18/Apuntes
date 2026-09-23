#!/bin/bash

DIR="./Ejercicios"
SOLVER="Gecode"
TIEMPO=300000 # 5 min
OUTPUT_FILE="resultados_minizinc.txt"

# Limpiar el archivo de salida
> "$OUTPUT_FILE"

# Variables fijas según el código
VAR_VOLUMEN="volumen_total_buscado_ml"  # Ejercicio 1
VAR_N="N"								# Ejercicio 4

# Función para ejecutar minizinc con cronómetro en la misma línea
ejecutar_con_cronometro() {
    local titulo="$1"
    shift
    local comando=("$@")

    echo "-------------------------------------------------" >> "$OUTPUT_FILE"
    echo "EJECUCIÓN: $titulo" >> "$OUTPUT_FILE"
    echo "-------------------------------------------------" >> "$OUTPUT_FILE"

    # Ejecutar en segundo plano
    "${comando[@]}" >> "$OUTPUT_FILE" 2>&1 &
    local pid=$!
    local segundos=0

    # Bucle de espera y actualización de línea
    while kill -0 $pid 2>/dev/null; do
        # Calcular min y seg para el display
        local min=$((segundos / 60))
        local seg=$((segundos % 60))
        
        # Mostrar tiempo en la misma línea (\r)
        printf "\rEjecutando %s: %02d:%02d (pulsa ENTER para cancelar) " "$titulo" "$min" "$seg"
        
        # Leer teclado durante 1 segundo. Si es ENTER (cadena vacía), cancelar.
        if read -t 1 -s -n 1 key && [[ $key == "" ]]; then
            kill -9 $pid 2>/dev/null
            wait $pid 2>/dev/null
            printf "\rEjecutando %s: %02d:%02d [CANCELADO POR USUARIO]          \n" "$titulo" "$min" "$seg"
            echo -e "\n[!] CANCELADO TRAS $min:$seg" >> "$OUTPUT_FILE"
            return 1
        fi
        ((segundos++))
    done

    wait $pid
    local min=$((segundos / 60))
    local seg=$((segundos % 60))
    printf "\rEjecutando %s: %02d:%02d [FINALIZADO]                      \n" "$titulo" "$min" "$seg"
    echo -e "\nFIN: Completado en $min:$seg" >> "$OUTPUT_FILE"
}

echo "Resultados en: $OUTPUT_FILE"
read -p "¿Qué ejercicio quieres ejecutar? (ej. 1a, 1b, 4, o Enter para todos): " EJ_NUM

if [ -n "$EJ_NUM" ]; then
    ARCHIVOS=$(find "$DIR" -maxdepth 1 -name "Ejercicio${EJ_NUM}*.mzn" -type f | sort -V)
else
    ARCHIVOS=$(find "$DIR" -maxdepth 1 -name "Ejercicio*.mzn" -type f | sort -V)
fi

for file in $ARCHIVOS; do
    filename=$(basename -- "$file")
    
    # --- Lógica para Ejercicio 1a ---
    if [[ "$filename" == *"Ejercicio1a"* ]]; then
        for val in 200 1000 5000 10000; do
            ejecutar_con_cronometro "Ej 1a ($val ml)" minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$VAR_VOLUMEN=$val;" "$file"
        done

    # --- Lógica para Ejercicio 1b ---
    elif [[ "$filename" == *"Ejercicio1b"* ]]; then
        for val in 200 1000 5000 10000; do
            ejecutar_con_cronometro "Ej 1b ($val ml)" minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$VAR_VOLUMEN=$val;" "$file"
        done

    # --- Lógica para Ejercicio 4 ---
    elif [[ "$filename" == *"Ejercicio4"* ]]; then
        for val in 3 4 5 6 7; do
            ejecutar_con_cronometro "Ej 4 (N=$val)" minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$VAR_N=$val;" "$file"
        done

    # --- Otros Ejercicios ---
    else
        ejecutar_con_cronometro "$filename" minizinc --solver "$SOLVER" -a --time-limit "$TIEMPO" -s "$file"
    fi
done

echo -e "\nProceso terminado. Revisa $OUTPUT_FILE"
