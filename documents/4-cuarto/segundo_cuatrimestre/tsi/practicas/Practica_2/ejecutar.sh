#!/bin/bash

DIR="./Ejercicios"
SOLVER="Gecode"
TIEMPO=300000 # 5 min (300.000 ms)

# Preguntamos qué ejercicio quiere ejecutar
read -p "¿Qué ejercicio quieres ejecutar? (ej. 1a, 1b, 4, o pulsa Enter para todos): " EJ_NUM

if [ -n "$EJ_NUM" ]; then
    ARCHIVOS=$(find "$DIR" -maxdepth 1 -name "Ejercicio${EJ_NUM}*.mzn" -type f | sort -V)
else
    ARCHIVOS=$(find "$DIR" -maxdepth 1 -name "Ejercicio*.mzn" -type f | sort -V)
fi

for file in $ARCHIVOS; do
    filename=$(basename -- "$file")
    
    echo "================================================="
    echo "Ejecutando: $filename"
    echo "================================================="
    
    # ---------------- TABLAS DEL EJERCICIO 1a ----------------
    if [[ "$filename" == *"Ejercicio1a"* ]]; then
    	VAR_VOLUMEN_NAME=volumen_total_buscado_ml
    
        for val in 200 1000 5000 10000; do
            echo -e "\n---> TABLA: Probando $filename con $VAR_VOLUMEN_NAME = $val <---"
            # Usamos -s para contar cuántas son y el runtime
            minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$VAR_VOLUMEN_NAME=$val;" "$file"
        done
        
    # ---------------- TABLAS DEL EJERCICIO 1b ----------------
    elif [[ "$filename" == *"Ejercicio1b"* ]]; then
    	VAR_VOLUMEN_NAME=volumen_total_buscado_ml
        for val in 200 1000 5000 10000; do
            echo -e "\n---> TABLA: Probando $filename con $VAR_VOLUMEN_NAME = $val <---"
            # Sin -a porque es minimización y solo queremos la ÓPTIMA. Usamos -s para el runtime.
            minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$VAR_VOLUMEN_NAME=$val;" "$file"
        done

    # ---------------- TABLAS DEL EJERCICIO 4 ----------------
    elif [[ "$filename" == *"Ejercicio4"* ]]; then
        N_NAME=N
        for val in 3 4 5 6 7; do
            echo -e "\n---> TABLA: Probando $filename con $N_NAME = $val <---"
            # Sin -a para sacar la óptima. Usamos -s para el runtime.
            minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s -D "$N_NAME=$val;" "$file"
        done
        
    # ---------------- RESTO DE EJERCICIOS ----------------
    else
        # Para el resto de ejercicios que no tienen tabla de valores de entrada
        minizinc --solver "$SOLVER" --time-limit "$TIEMPO" -s "$file"
    fi
    
    echo -e "\n"
done
