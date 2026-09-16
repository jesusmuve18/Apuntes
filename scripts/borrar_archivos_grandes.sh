#!/bin/bash

echo "🔍 Escaneando archivos pesados (>90MB)..."

# Detectar archivos grandes y añadirlos a .gitignore
find . -type f -size +90M -not -path '*/.git/*' | while read -r file; do
    clean_file="${file#./}"
    if ! grep -qsF "$clean_file" .gitignore 2>/dev/null; then
        echo "$clean_file" >> .gitignore
        echo "➕ Añadido a .gitignore: $clean_file"
    fi
done

# Quitar del rastreo de Git los archivos que estén en .gitignore
git rm -r --cached . > /dev/null 2>&1

# Preparar, confirmar y subir
git add .

FECHA=$(date +"%d.%m.%Y %H:%M")
git commit -m "Actualizado $FECHA"
git push