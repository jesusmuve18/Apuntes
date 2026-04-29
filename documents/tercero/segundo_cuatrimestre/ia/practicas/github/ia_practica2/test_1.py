#!/usr/bin/env python
import subprocess

EXEC = "./practica2SG"
MAP_DIR = "mapas"

tests = [

    # Tests nivel 1
    ("mapa30.map", [0, 1, 21, 9, 2, 15, 24, 6, 3, 3, 0], None),
    ("mapa50.map", [0, 1, 5, 3, 2, 36, 44, 6, 3, 3, 0], None),
    ("mapa75.map", [0, 1, 20, 9, 2, 15, 23, 6, 3, 3, 0], None),
    ("mapa100.map", [0, 1, 20, 9, 2, 80, 96, 0, 3, 3, 0], None),
    ("gemini2.map", [0, 1, 3, 10, 2, 3, 13, 6, 3, 3, 0], None),
    ("2ez.map", [0, 1, 14, 3, 4, 7, 7, 4, 3, 3, 0], None),
    ("mapa30.map", [0, 1, 16, 9, 2, 16, 14, 6, 3, 3, 0], None),
    ("mapaop.map", [0, 1, 16, 9, 2, 16, 14, 6, 3, 3, 0], None),
    ("chess.map", [1, 1, 45, 6, 1, 4, 43, 5, 3, 3, 0], None),
    ("bosque_prohibido.map", [1, 1, 5, 4, 2, 20, 15, 0, 3, 3, 0], None),
    ("islas25.map", [1, 1, 11, 3, 2, 12, 3, 2, 3, 3, 0], None),
    ("islas25.map", [1, 1, 11, 3, 2, 96, 88, 2, 3, 3, 0], None),
    ("islas_cambio_climatico.map", [1, 1, 11, 3, 2, 12, 3, 2, 3, 3, 0], None),
    ("islas_cambio_climatico.map", [1, 1, 11, 3, 2, 96, 88, 2, 3, 3, 0], None),
    ("paldea25.map", [1, 1, 91, 46, 2, 91, 48, 6, 3, 3, 0], None),
    ("paldea25.map", [1, 1, 30, 58, 4, 91, 48, 6, 3, 3, 0], None),
    ("luminalia25.map", [1, 1, 91, 44, 2, 9, 58, 6, 3, 3, 0], None),
    ("luminalia25.map", [1, 1, 45, 44, 3, 56, 55, 7, 3, 3, 0], None),
]

# --- Tests del Nivel 4 ---

tests4 = []



def parse_battery(output: str):
    for line in output.splitlines():
        if "Coste de Energía" in line:
            try:
                gasto = int(line.split(":")[1])
                return 3000 - gasto
            except ValueError:
                return None
    return None

def parse_discovery(output: str):
    for line in output.splitlines():
        if "Porcentaje descubierto de caminos y senderos" in line:
            try:
                return float(line.split(":")[1].strip())
            except ValueError:
                return None
    return None

def check_base_messages(output: str):
    rescatador_ok = "El rescatador ha alcanzado un puesto base." in output
    auxiliar_ok = "El auxiliar ha alcanzado un puesto base." in output
    return rescatador_ok, auxiliar_ok

def color_text(text, color_code):
    return f"\033[{color_code}m{text}\033[0m"

def run_test(map_name, params, expected_battery):
    result = subprocess.run(
        [EXEC, f"{MAP_DIR}/{map_name}"] + list(map(str, params)),
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        return (False, map_name, params, f"Error: {result.stderr.strip()}")

    level = params[1]
    if level == 0:
        rescatador_ok, auxiliar_ok = check_base_messages(result.stdout)
        if rescatador_ok and auxiliar_ok:
            return (True, map_name, None, "Bases alcanzadas")
        else:
            missing = []
            if not rescatador_ok:
                missing.append("Rescatador")
            if not auxiliar_ok:
                missing.append("Auxiliar")
            return (False, map_name, params, f"No alcanzado: {', '.join(missing)}")

    if level == 1:
        discovery = parse_discovery(result.stdout)
        if discovery is not None:
            score = round((discovery / 100) * 2.5, 3)
            return (True, map_name, None, f"Puntuación: {score} (descubierto {discovery:.4f}%)")
        else:
            return (False, map_name, params, "No se pudo obtener porcentaje descubierto")

    battery = parse_battery(result.stdout)
    if battery == expected_battery:
        return (True, map_name, None, battery)
    else:
        return (False, map_name, params, battery)

def format_score_line(test_id, color_code, score):
    icon = "\uf111"  # Unicode del círculo ''
    return f"{test_id} | \033[{color_code}m{icon}\033[0m {score:.3f} / 2.5"

def parse_level4_points(output: str):
    for line in output.splitlines():
        if "Objetivos encontrados" in line:
            try:
                return int(line.strip().split()[-1])
            except ValueError:
                return None
    return None

def calcular_nota_nivel4(puntos, u075, u15, u25):
    if puntos < u075:
        return 0.75 * (puntos / u075)
    elif puntos < u15:
        return 0.75 + 0.75 * ((puntos - u075) / (u15 - u075))
    elif puntos < u25:
        return 1.5 + 1.0 * ((puntos - u15) / (u25 - u15))
    else:
        return 2.5

if __name__ == "__main__":
    level1_results = []
    level1_scores = []
    total = len(tests)
    ok = 0
    failed_tests = []
    current_level = None

    for i, test in enumerate(tests):
        level = test[1][1]
        if level != current_level:
            if current_level == 1 and level1_results:
                # Mostrar resumen justo después de nivel 1
                print("\n")
                for row in level1_results:
                    print(row)
                avg = sum(level1_scores) / len(level1_scores)
                print(f"MEDIA = {avg:.3f} / 2.5\n")
                level1_results.clear()
                level1_scores.clear()

            current_level = level
            print(f"\n{color_text(f'Tests nivel {level}:', '34')}")

        success, map_name, params, info = run_test(*test)

        if level == 1 and isinstance(info, str) and "Puntuación:" in info:
            try:
                score_str, percent_str = info.split(":")[1].split("(")
                score = float(score_str.strip())
                percent = float(percent_str.strip().replace("descubierto", "").replace("%)", ""))

                if percent >= 99.9:
                    color_code = "34"  # Azul
                elif percent >= 80:
                    color_code = "32"  # Verde
                elif percent >= 50:
                    color_code = "33"  # Amarillo (oscuro)
                elif percent >= 30:
                    color_code = "93"  # Amarillo claro
                else:
                    color_code = "31"  # Rojo

                level1_results.append(format_score_line(f"1{i+1}", color_code, score))
                level1_scores.append(score)
            except Exception as e:
                print(color_text(f"⚠️ Error formateando puntuación: {e}", "33"))

        if success:
            print(color_text(f"✅ {map_name}", "32"))
            ok += 1
        else:
            if isinstance(info, str) and "Error" in info:
                print(color_text(f"❌ {map_name} {' '.join(map(str, params))}", "31"))
                print(color_text(f"   {info}", "31"))
            elif level == 1:
                print(color_text(f"❌ {map_name} {' '.join(map(str, params))}", "31"))
                print(color_text(f"   {info}", "31"))
            else:
                expected = test[2]
                print(color_text(f"❌ {map_name} {' '.join(map(str, params))}", "31"))
                print(color_text(f"   batería final = {info} (esperado {expected})", "31"))
            failed_tests.append((map_name, params))

    # Si los últimos tests fueron de nivel 1, imprimir resumen
    if current_level == 1 and level1_results:
        print("\n🗺 | 💯")
        print("-----------")
        for row in level1_results:
            print(row)
        avg = sum(level1_scores) / len(level1_scores)
        print("-----------")
        print(f"MEDIA = {avg:.3f} / 2.5\n")

    print("=" * 40)
    print(f"Total: {total} | OK: {ok} | Fails: {total - ok}")
    if failed_tests:
        print(color_text("\nTests fallidos:", "31"))
        for map_name, params in failed_tests:
            print(f"{EXEC} {MAP_DIR}/{map_name} {' '.join(map(str, params))}")

    # Tests del Nivel 4
    if tests4:
        print(f"\n{color_text('Tests nivel 4:', '34')}")
        for test_id, map_name, params, u075, u15, u25 in tests4:
            result = subprocess.run(
                [EXEC, f"{MAP_DIR}/{map_name}"] + list(map(str, params)),
                capture_output=True,
                text=True
            )
            if result.returncode != 0:
                print(color_text(f"❌ {test_id} Error: {result.stderr.strip()}", "31"))
                continue

            puntos = parse_level4_points(result.stdout)
            if puntos is None:
                print(color_text(f"❌ {test_id} No se pudo extraer puntuación", "31"))
                continue

            nota = calcular_nota_nivel4(puntos, u075, u15, u25)
            color_code = "31" if nota < 0.75 else "33" if nota < 1.5 else "32"
            print(format_score_line(test_id, color_code, nota))