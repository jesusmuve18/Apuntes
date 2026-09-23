#!/bin/bash

# Comando: rclone copy NombreConfig:NombreCarpetaDrive /ruta/local

echo "Sincronizando desde Google Drive..."
rclone copy driveUGR:"AA-Practicas" ./ --progress
echo "¡Sincronización terminada!"
