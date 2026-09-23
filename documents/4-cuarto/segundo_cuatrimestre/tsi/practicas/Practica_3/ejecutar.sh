#!/bin/bash

# ==========================================
# CONFIGURACIÓN DE RUTAS
# ==========================================
# Ruta absoluta o relativa a la carpeta de Fast Downward
FD_DIR="./fast-downward-24.06.1"

# Ruta a la carpeta donde guardas tus archivos .pddl
PDDL_DIR="./Ejercicios"

# Alias del planificador (algoritmo) a usar
ALIAS="seq-opt-lmcut"
TIME_LIMIT_ARG=""  # Por defecto va vacío (sin límite de tiempo)
# ==========================================

# Verificar si se ha proporcionado el parámetro del ejercicio
if [ -z "$1" ]; then
    echo "❌ Error: Debes indicar el número del ejercicio."
    echo "Uso: $0 <número>"
    echo "Ejemplo: $0 1      (Ejecuta dominio1.pddl y problema1.pddl)"
    echo "Ejemplo: $0 5      (Ejecuta dominio5.pddl y problema5.pddl)"
    exit 1
fi

NUM_EJERCICIO=$1
TIME_LIMIT=200      # En segundos (para el ejercicio 4)

# usar seq-sat-lama-2011 en el ejercicio 4
if [ "$NUM_EJERCICIO" = "4" ]; then
    ALIAS="seq-sat-lama-2011"
    TIME_LIMIT=200s  # Tiempo en segundos
    TIME_LIMIT_ARG="--search-time-limit $TIME_LIMIT"
fi

# Definir las rutas de los archivos basados en el parámetro
DOMINIO="${PDDL_DIR}/dominio${NUM_EJERCICIO}.pddl"
PROBLEMA="${PDDL_DIR}/problema${NUM_EJERCICIO}.pddl"
FD_SCRIPT="${FD_DIR}/fast-downward.py"

PLAN_SALIDA="$PDDL_DIR/output_${NUM_EJERCICIO}.sas"
OUT_FILE="$PDDL_DIR/output_${NUM_EJERCICIO}.log"

# Validaciones de existencia de archivos y carpetas
if [ ! -f "$FD_SCRIPT" ]; then
    echo "❌ Error: No se encuentra fast-downward.py en: $FD_DIR"
    echo "Por favor, verifica la variable FD_DIR dentro del script."
    exit 1
fi

if [ ! -f "$DOMINIO" ]; then
    echo "❌ Error: No se encuentra el archivo de dominio: $DOMINIO"
    exit 1
fi

if [ ! -f "$PROBLEMA" ]; then
    echo "❌ Error: No se encuentra el archivo de problema: $PROBLEMA"
    exit 1
fi

# Ejecución de Fast Downward
echo "🚀 Iniciando Fast Downward para el Ejercicio $NUM_EJERCICIO..."
echo "📂 Dominio:  $DOMINIO"
echo "📂 Problema: $PROBLEMA"

echo "🏃 Ejecutando: python3 $FD_SCRIPT --alias $ALIAS $TIME_LIMIT_ARG --plan-file $PLAN_SALIDA $DOMINIO $PROBLEMA"

# Ejecución real del comando
python3 "$FD_SCRIPT" --alias "$ALIAS" $TIME_LIMIT_ARG --plan-file "$PLAN_SALIDA" "$DOMINIO" "$PROBLEMA" > "$OUT_FILE"

echo "--------------------------------------------------------"
echo "🏁 Proceso finalizado. Si se encontró un plan, revisa el archivo '${PLAN_SALIDA}'."
