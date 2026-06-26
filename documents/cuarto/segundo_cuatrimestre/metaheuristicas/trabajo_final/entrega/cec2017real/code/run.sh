#!/usr/bin/env bash
# ============================================================
#  run.sh — Compila y ejecuta un algoritmo para dim 10, 30, 50
#
#  Uso: ./run.sh <target>  (ej: ./run.sh sca  o  ./run.sh sca_ls)
# ============================================================

set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Uso: $0 <target>  (ej: $0 sca  o  $0 sca_ls)"
    exit 1
fi

TARGET="$1"
DIMS=(10 30 50)
SEED=42

# ── 1. Compilación ─────────────────────────────────────────
echo "=== Compilando ${TARGET} ==="
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make "$TARGET" -j"$(nproc)"
cd ..
cp "build/$TARGET" .

# ── 2. Asegurarse de que input_data está en el directorio de trabajo
if [ ! -d "input_data" ]; then
    echo "[ERROR] El directorio input_data/ no existe en $(pwd)."
    echo "        Cópialo desde el repositorio original antes de ejecutar."
    exit 1
fi

# ── 3. Crear directorio de resultados ──────────────────────
mkdir -p "results_${TARGET}"

# ── 4. Ejecución para cada dimensión ───────────────────────
for dim in "${DIMS[@]}"; do
    echo ""
    echo "========================================"
    echo "  ${TARGET} — Dimension ${dim}"
    echo "========================================"
    "./${TARGET}" "$dim" "$SEED"
done

# ── 5. Generar Excel para TacoLab ──────────────────────────
echo ""
echo "=== Generando fichero Excel para TacoLab ==="

if [ -f "extract.py" ]; then
    python3 extract.py "results_${TARGET}"
    echo "[OK] Fichero Excel generado."
else
    echo "[AVISO] No se encontró extract.py en el directorio actual."
    echo "        Ejecútalo manualmente: python3 extract.py results_${TARGET}"
fi

echo ""
echo "=== Listo. Sube el Excel a https://tacolab.org/bench ==="