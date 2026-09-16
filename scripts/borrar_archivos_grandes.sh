#!/bin/bash

# Función para escanear y des-rastrear archivos pesados (>90MB)
limpiar_archivos_grandes() {
    echo "🔍 Escaneando archivos pesados (>90MB)..."
    touch .gitignore

    find . -type f -size +90M -not -path '*/.git/*' | while read -r file; do
        clean_file="${file#./}"
        
        # Añadir a .gitignore si no está presente
        if ! grep -qsF "$clean_file" .gitignore 2>/dev/null; then
            echo "$clean_file" >> .gitignore
            echo "➕ Añadido a .gitignore: $clean_file"
        fi
        
        # Quitar del índice de Git sin borrar del disco
        git rm --cached "$clean_file" 2>/dev/null || true
    done
}

# 1. Limpieza preventiva
limpiar_archivos_grandes

# 2. Preparar y confirmar cambios locales
git add .
FECHA=$(date +"%d.%m.%Y %H:%M")

if git diff-index --quiet HEAD -- 2>/dev/null; then
    echo "ℹ️ No hay cambios pendientes para confirmar."
else
    git commit -m "Actualizado $FECHA"
fi

# 3. Intentar envío a GitHub
echo "📤 Subiendo cambios a GitHub..."
if git push; then
    echo "✅ ¡Subida completada con éxito!"
    exit 0
else
    echo "⚠️ El push falló. Se detectaron commits pasados con archivos pesados."
    echo "🛠️ Ejecutando autorreparación del historial local..."

    # Deshace los commits locales no subidos manteniendo todos tus archivos en disco
    git reset origin/main

    # Re-escanea e ignora los archivos pesados
    limpiar_archivos_grandes

    # Re-confirma y re-intenta el envío
    git add .
    git commit -m "Actualizado $FECHA (Tras autorreparación)"

    echo "🔄 Reintentando subida..."
    if git push; then
        echo "🎉 ¡Reparado y subido con éxito!"
        exit 0
    else
        echo "❌ Error persistente. Comprueba tu conexión o posibles conflictos de red."
        exit 1
    fi
fi