import os
import sys
import tty
import termios
import subprocess

# =============================
# CONFigURACIÓN
# =============================

EXCLUDE_PATTERNS = [
    "ia_practica2/",
    "ia_practica3/",
    ".ipynb_checkpoints",
    "_assets",
    ".vscode",
    ".git",
    "/documents/tercero/segundo_cuatrimestre/ise/practicas/Entrega",
    "/documents/tercero/segundo_cuatrimestre/ise/practicas/Entrega/Monitorizacion_de_API_Web_2/prometheus_data",
    "/documents/tercero/segundo_cuatrimestre/ise/practicas/ejercicio_grafana/grafana_prometheus_docker/",
    "/documents/tercero/segundo_cuatrimestre/ise/practicas/progra/",
    "/documents/tercero/segundo_cuatrimestre/ise/practicas/ejercicio_jmeter/",
    "/documents/cuarto/primer_cuatrimestre/ddsi/practicas/github/ddsi-practica3/src/",
    "/documents/cuarto/primer_cuatrimestre/ddsi/practicas/github/ddsi-seminario1/build/",
    "/documents/cuarto/primer_cuatrimestre/ddsi/practicas/github/ddsi-seminario1/src/",
    "/documents/cuarto/primer_cuatrimestre/ig/documentacion/godot-docs-html-master/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/tercera_entrega-v1/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_1/p-1/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/segunda_entrega-v1/p1/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/segunda_entrega-v1/p3/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/segunda_entrega-v1/p2/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/examenes/examen4-5/primera_entrega/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/examenes/examen4-5/p-3/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/examenes/examen4-5/segunda_entrega/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/examenes/examen1-3/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/primera_entrega-v1/p1/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/primera_entrega-v1/p2/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_2/p-2/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/cuarta_entrega-v1/p-5",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/cuarta_entrega-v1/p-4/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_3/p-3/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_4/Archivos-Texturas-Ejer-Adic-P4-20251124T145547Z-1-001/Archivos-Texturas-Ejer-Adic-P4",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_4/p-4/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_4/Archivos-Texturas-Ejer-Adic-P4-20251124T145547Z-1-001",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/practica_5/p-5/",
    "/documents/cuarto/primer_cuatrimestre/ig/practicas/plantilla/p-3/",
    "/documents/cuarto/primer_cuatrimestre/ig/prado/practicas/Archivos-Texturas-Ejer-Adic-P4",
]

def is_excluded(path):
    return any(pat in path for pat in EXCLUDE_PATTERNS)


def get_key():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

# =============================
# SLUGIFY
# =============================

def slugify(name, is_file=False):
    import unicodedata
    import re

    name = unicodedata.normalize('NFKD', name).encode('ascii', 'ignore').decode()
    name = name.lower()

    ext = ""
    if is_file and '.' in name:
        parts = name.split('.')
        ext = parts[-1]
        base = '.'.join(parts[:-1])
    else:
        base = name

    # -----------------------------
    # REGLAS SEMÁNTICAS
    # -----------------------------

    # ": " → "-"
    base = base.replace(": ", "-")

    # ":" → "-"
    base = base.replace(":", "-")

    # puntos entre números → "-"
    base = re.sub(r'(?<=\d)\.(?=\d)', '-', base)

    # espacios → "_"
    base = re.sub(r'\s+', '_', base)

    # limpieza de caracteres
    base = re.sub(r'[^a-z0-9_-]', '', base)

    # colapsar separadores
    base = re.sub(r'-+', '-', base)
    base = re.sub(r'_+', '_', base)

    base = base.strip('_-')

    if is_file and ext:
        return f"{base}.{ext}"
    return base


# =============================
# REFERENCIAS DETALLADAS
# =============================

def find_detailed_references(text):
    try:
        result = subprocess.run(
            ["grep", "-R", "-n", text, "."],
            capture_output=True,
            text=True
        )
        if result.stdout.strip():
            return result.stdout.strip().split("\n")
    except Exception:
        pass
    return []


# =============================
# RECOGIDA DE ARCHIVOS Y DIRS
# =============================

dirs = []
files = []

for root, dirnames, filenames in os.walk('.', topdown=False):
    if is_excluded(root):
        continue

    for d in dirnames:
        full_d = os.path.join(root, d)
        if not is_excluded(full_d):
            dirs.append(full_d)

    for f in filenames:
        full_f = os.path.join(root, f)
        if not is_excluded(full_f):
            files.append(full_f)


# =============================
# RENOMBRAR CARPETAS
# =============================

print("\n===== 🗂️ RENOMBRADO DE CARPETAS =====")

for path in dirs:
    if is_excluded(path):
        continue

    base = os.path.basename(path)
    parent = os.path.dirname(path)

    new_base = slugify(base, is_file=False)

    if base == new_base:
        continue

    new_path = os.path.join(parent, new_base)

    print("\n" + "="*80)
    print(f"📁 ORigINAL:   {path}")
    print(f"➡️ PROPUESTA:  {new_path}")

    refs = find_detailed_references(path)

    if refs:
        print("\n🔗 REFERENCIAS:")
        for r in refs[:30]:
            print(f"   {r}")
        if len(refs) > 30:
            print(f"   ... y {len(refs) - 30} más")
    else:
        print("\n🔗 Sin referencias")

    print("¿Renombrar? [y/N]: ", end="", flush=True)

    key = get_key().lower()

    print(key)  # para feedback visual

    if key == 'y':
        try:
            os.rename(path, new_path)
            print("✔ Renombrado")
        except Exception as e:
            print(f"❌ Error: {e}")
    else:
        print("⏭ Omitido")


# =============================
# RENOMBRAR ARCHIVOS
# =============================

print("\n===== 📄 RENOMBRADO DE ARCHIVOS =====")

for path in files:
    if is_excluded(path):
        continue

    base = os.path.basename(path)
    parent = os.path.dirname(path)

    new_base = slugify(base, is_file=True)

    if base == new_base:
        continue

    new_path = os.path.join(parent, new_base)

    print("\n" + "="*80)
    print(f"📄 ORigINAL:   {path}")
    print(f"➡️ PROPUESTA:  {new_path}")

    refs = find_detailed_references(path)

    if refs:
        print("\n🔗 REFERENCIAS:")
        for r in refs[:30]:
            print(f"   {r}")
        if len(refs) > 30:
            print(f"   ... y {len(refs) - 30} más")
    else:
        print("\n🔗 Sin referencias")

    resp = input("\n¿Renombrar archivo? [y/N]: ").strip().lower()

    if resp == 'y':
        try:
            os.rename(path, new_path)
            print("✔ Renombrado")
        except Exception as e:
            print(f"❌ Error: {e}")
    else:
        print("⏭ Omitido")