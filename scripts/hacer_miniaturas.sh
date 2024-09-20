#!/bin/bash

# Verifica que el archivo PDF existe
if [ $# -eq 0 ]; then
    echo "Uso: $0 archivo.pdf [output.png]"
    exit 1
fi

# Nombre del archivo PDF y del archivo de salida
PDF_FILE=$1
OUTPUT_FILE=./miniaturas/${2:-thumbnail}.png

# Genera la miniatura de la primera página del PDF
convert -thumbnail x300 "$PDF_FILE[0]" "$OUTPUT_FILE"

# Verifica si se generó correctamente
if [ $? -eq 0 ]; then
    echo "Miniatura generada correctamente: $OUTPUT_FILE"
else
    echo "Error al generar la miniatura"
    exit 1
fi