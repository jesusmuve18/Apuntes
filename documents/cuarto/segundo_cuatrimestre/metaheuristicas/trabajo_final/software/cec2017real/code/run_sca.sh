#!/usr/bin/env bash
# ============================================================
#  run_sca.sh — Compila y ejecuta el SCA para las dimensiones
#  exigidas por el enunciado: 10, 30, 50
#
#  Uso: ./run_sca.sh
#       (ejecutar desde la raíz del repositorio cec2017real/)
# ============================================================

set -euo pipefail

DIMS=(10 30 50)
SEED=42
BINARY="./sca"

# ── 1. Compilación ─────────────────────────────────────────
echo "=== Compilando SCA ==="
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make sca -j"$(nproc)"
cd ..
cp build/sca .

# ── 2. Asegurarse de que input_data está en el directorio de trabajo
if [ ! -d "input_data" ]; then
    echo "[ERROR] El directorio input_data/ no existe en $(pwd)."
    echo "        Cópialo desde el repositorio original antes de ejecutar."
    exit 1
fi

# ── 3. Ejecución para cada dimensión ───────────────────────
for dim in "${DIMS[@]}"; do
    echo ""
    echo "========================================"
    echo "  SCA — Dimension ${dim}"
    echo "========================================"
    "$BINARY" "$dim" "$SEED"
done

# ── 4. Generar Excel para TacoLab ──────────────────────────
echo ""
echo "=== Generando fichero Excel para TacoLab ==="

# El script extract.py de Molina espera el nombre del directorio results_<alg>
if [ -f "extract.py" ]; then
    python3 extract.py results_sca
    echo "[OK] Fichero Excel generado."
else
    echo "[AVISO] No se encontró extract.py en el directorio actual."
    echo "        Ejecútalo manualmente: python3 extract.py results_sca"
fi

echo ""
echo "=== Listo. Sube el Excel a https://tacolab.org/bench ==="
