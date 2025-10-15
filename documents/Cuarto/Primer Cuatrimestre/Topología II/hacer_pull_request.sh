#!/bin/bash

# --- Rutas ---
SRC="/home/jesusmuve/Escritorio/Apuntes/documents/Cuarto/Primer Cuatrimestre/Topología II"
DEST="/home/jesusmuve/Documentos/git clones/LosDelDGIIM.github.io/subjects/Topología II/Temario"
REPO_DIR="/home/jesusmuve/Documentos/git clones/LosDelDGIIM.github.io"

# --- Nombre de la rama ---
LOCAL_BRANCH="local"
UPSTREAM_REMOTE="upstream"
UPSTREAM_BRANCH="main"

# --- Paso 0: entrar al repo ---
cd "$REPO_DIR" || { echo "No se pudo entrar al repo"; exit 1; }

# --- Paso 1: asegurarse de tener upstream ---
if ! git remote | grep -q "$UPSTREAM_REMOTE"; then
    echo "Agregando remoto upstream..."
    git remote add upstream https://github.com/UsuarioOriginal/LosDelDGIIM.github.io.git
fi

# --- Paso 2: traer cambios del upstream ---
git fetch "$UPSTREAM_REMOTE"

# --- Paso 3: cambiar a la rama local y actualizar ---
git checkout "$LOCAL_BRANCH" || { echo "No existe la rama $LOCAL_BRANCH"; exit 1; }
git merge "$UPSTREAM_REMOTE/$UPSTREAM_BRANCH" || { echo "Error al merge upstream"; exit 1; }

# --- Paso 4: copiar archivos ---
echo "Copiando archivos .tex..."
cp -u "$SRC"/tema_*.tex "$DEST"/

# --- Paso 5: compilar el TeX ---
cd "$DEST" || { echo "No se pudo entrar a la carpeta de destino"; exit 1; }
echo "Compilando Temario_TopoII.tex..."
pdflatex Temario_TopoII.tex

# --- Paso 6: commit y push ---
cd "$REPO_DIR" || exit 1
git add "subjects/Topología II/Temario"
git commit -m "Actualizar Temario Topología II"
git push origin "$LOCAL_BRANCH"

echo "¡Todo listo! Ahora puedes hacer un pull request desde GitHub."
