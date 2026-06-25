/**
 * ============================================================
 *  cec17_common.h — Infraestructura compartida para CEC2017
 * ============================================================
 *
 *  Este header centraliza todos los elementos reutilizables
 *  entre las distintas variantes del SCA implementadas en la
 *  práctica (SCA, SCA-LS, ASCA). Incluirlo en cada .cpp de
 *  algoritmo es suficiente para tener acceso a:
 *
 *    • Constantes del dominio (bounds, rango, parámetros globales)
 *    • Utilidades de bajo nivel (clamp, valid_dim)
 *    • Estructura Population (inicialización, élite, inyección)
 *    • Búsqueda local estocástica Solis-Wets (1981)
 *    • Función run_algorithm (bucle CEC2017 estándar)
 *
 *  Uso mínimo en cada algoritmo:
 *    #include "cec17_common.h"
 *
 * ============================================================
 *  Referencia benchmark:
 *    N. H. Awad et al., "Problem Definitions and Evaluation
 *    Criteria for the CEC 2017 Special Session on Real-Parameter
 *    Single Objective Optimization", Technical Report, NTU, 2016.
 *
 *  Referencia búsqueda local:
 *    F. J. Solis y R. J.-B. Wets, "Minimization by Random Search
 *    Techniques", Mathematics of Operations Research, 1981.
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 * ============================================================
 */

#pragma once

extern "C" {
#include "cec17.h"
}

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>


// ═════════════════════════════════════════════════════════════
//   DOMINIO Y PARÁMETROS GLOBALES
// ═════════════════════════════════════════════════════════════

/**
 * Límites del hipercubo de búsqueda comunes a las 30 funciones
 * del benchmark CEC2017. Todas las posiciones deben mantenerse
 * dentro del intervalo [LOWER_BOUND, UPPER_BOUND].
 */
inline constexpr double LOWER_BOUND = -100.0;
inline constexpr double UPPER_BOUND =  100.0;

/** Longitud del lado del hipercubo: UPPER_BOUND − LOWER_BOUND. */
inline constexpr double RANGE = UPPER_BOUND - LOWER_BOUND;

/**
 * Número de ejecuciones independientes por función.
 * El estándar CEC2017 usa 51; aquí se reducen a 10 para limitar
 * el tiempo de cómputo sin perder representatividad estadística.
 */
inline constexpr int T_RUNS = 10;

/**
 * Tamaño de la población (número de agentes de búsqueda).
 * Valor recomendado en la literatura para dimensiones ≤ 50.
 */
inline constexpr int POP_SIZE = 30;


// ═════════════════════════════════════════════════════════════
//   UTILIDADES DE BAJO NIVEL
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Proyecta un valor al intervalo [LOWER_BOUND, UPPER_BOUND].
 *
 * Se aplica tras cada actualización de posición para garantizar
 * que ningún agente salga del dominio factible del problema.
 *
 * @param  v  Valor a proyectar.
 * @return    v si ya estaba en el intervalo; el extremo más
 *            cercano en caso contrario.
 */
inline double clamp(double v) noexcept {
    return std::max(LOWER_BOUND, std::min(UPPER_BOUND, v));
}

/**
 * @brief  Comprueba si una dimensión es válida para CEC2017.
 *
 * El benchmark solo está definido para d ∈ {2, 5, 10, 30, 50, 100}.
 * En la práctica se evalúan d ∈ {10, 30, 50}.
 *
 * @param  dim  Dimensión a verificar.
 * @return      true si dim es una dimensión reconocida por el benchmark.
 */
inline bool valid_dim(int dim) noexcept {
    return dim ==  2 || dim ==  5 || dim == 10 ||
           dim == 30 || dim == 50 || dim == 100;
}


// ═════════════════════════════════════════════════════════════
//   POBLACIÓN (Population)
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Contenedor de la población y archivo del élite global.
 *
 * Encapsula el estado completo de la población en cada instante:
 *   - pos      : posiciones actuales de todos los agentes (n × dim).
 *   - fit      : valores de aptitud (fitness) de cada agente.
 *   - best_pos : posición del mejor agente encontrado hasta ahora.
 *   - best_fit : fitness del mejor agente (élite, minimización).
 *   - evals    : contador acumulado de evaluaciones de la función objetivo.
 *
 * La inicialización evalúa inmediatamente todos los agentes y
 * establece el élite. Todas las actualizaciones posteriores deben
 * hacerse mediante evaluate_and_update() para mantener coherencia.
 */
struct Population {

    std::vector<std::vector<double>> pos;      ///< Posiciones: pos[i][j] = coordenada j del agente i.
    std::vector<double>              fit;      ///< Fitness actual de cada agente.
    std::vector<double>              best_pos; ///< Posición del mejor agente histórico (élite).
    double                           best_fit = 1e300; ///< Fitness del élite (valor grande inicial para minimización).
    long                             evals    = 0;     ///< Evaluaciones de la función objetivo consumidas.

    /**
     * @brief  Inicializa la población con posiciones aleatorias uniformes.
     *
     * Genera n agentes muestreando cada coordenada de dis(gen), evalúa
     * cada uno mediante cec17_fitness() y actualiza el élite con el mejor.
     *
     * @param n    Número de agentes (tamaño de población).
     * @param dim  Dimensión del espacio de búsqueda.
     * @param dis  Distribución uniforme en [LOWER_BOUND, UPPER_BOUND].
     * @param gen  Generador de números pseudoaleatorios (Mersenne Twister).
     *
     * @post  evals == n; best_pos y best_fit reflejan el mejor agente inicial.
     */
    Population(int n, int dim,
               std::uniform_real_distribution<double> &dis,
               std::mt19937 &gen)
        : pos(n, std::vector<double>(dim)),
          fit(n),
          best_pos(dim)
    {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < dim; ++j)
                pos[i][j] = dis(gen);
            fit[i] = cec17_fitness(pos[i].data());
            if (fit[i] < best_fit) {
                best_fit = fit[i];
                best_pos = pos[i];
            }
        }
        evals = n;
    }

    /**
     * @brief  Evalúa el agente i y actualiza el élite si mejora.
     *
     * Llama a cec17_fitness() sobre la posición actual de pos[i],
     * almacena el resultado en fit[i], incrementa el contador evals
     * y, si el nuevo fitness es mejor que best_fit, actualiza el élite.
     *
     * Debe invocarse después de cada modificación de pos[i] para
     * mantener coherentes el contador de evaluaciones y el archivo élite.
     *
     * @param i  Índice del agente a evaluar (0 ≤ i < n).
     */
    void evaluate_and_update(int i) {
        fit[i] = cec17_fitness(pos[i].data());
        ++evals;
        if (fit[i] < best_fit) {
            best_fit = fit[i];
            best_pos = pos[i];
        }
    }

    /**
     * @brief  Inyecta una solución externa en lugar del peor agente.
     *
     * Localiza el agente con mayor fitness (el peor en minimización),
     * sustituye su posición y fitness por los de la solución dada, y
     * actualiza el élite si la solución externa lo mejora.
     *
     * Se utiliza típicamente para incorporar a la población el resultado
     * de una búsqueda local (p. ej. Solis-Wets en SCA-LS).
     *
     * @param sol      Vector de coordenadas de la solución externa.
     * @param sol_fit  Fitness de la solución externa.
     */
    void inject_best(const std::vector<double> &sol, double sol_fit) {
        int worst = static_cast<int>(
            std::max_element(fit.begin(), fit.end()) - fit.begin());
        pos[worst] = sol;
        fit[worst] = sol_fit;
        if (sol_fit < best_fit) {
            best_fit = sol_fit;
            best_pos = sol;
        }
    }
};


// ═════════════════════════════════════════════════════════════
//   BÚSQUEDA LOCAL: SOLIS-WETS (1981)
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Búsqueda local estocástica de Solis y Wets (1981).
 *
 * Intensificador de propósito general que refina una solución
 * mediante perturbaciones gaussianas adaptativas con sesgo acumulado.
 * Especialmente efectivo cuando la solución de partida ya se encuentra
 * en una cuenca de atracción del óptimo.
 *
 * @par Algoritmo
 *  En cada iteración:
 *  1. Genera una perturbación ξ ∼ N(0, σ·I) y una corrección de sesgo b.
 *  2. Evalúa el candidato x' = clamp(x + ξ + b).
 *  3. Si mejora: acepta x', actualiza el sesgo hacia ξ.
 *     Si no mejora: prueba la dirección opuesta x'' = clamp(x − ξ − b).
 *     Si tampoco mejora: reduce el sesgo y registra fallo.
 *  4. Ajusta σ de forma adaptativa:
 *       • 5 éxitos consecutivos → σ ×= 3  (ampliar exploración local).
 *       • 5 fallos consecutivos → σ ×= 0.5 (concentrar la búsqueda).
 *
 * @par Parámetros internos
 *  - σ inicial  : 0.2 × RANGE (perturbación inicial del 20% del rango).
 *  - σ mínima   : 1e-12 (evita degeneración numérica).
 *  - σ máxima   : σ_inicial (no crece por encima del valor inicial).
 *  - Umbral de ajuste: 5 éxitos o 5 fallos consecutivos.
 *  - Factor de sesgo: b ← 0.4·b ± 0.4·ξ (memoria direccional suave).
 *
 * @param[in,out] sol        Solución de partida; se modifica in-place con la
 *                           mejor posición encontrada.
 * @param[in,out] fit        Fitness de sol; se actualiza si hay mejora.
 * @param[in]     budget     Número máximo de evaluaciones a consumir.
 * @param[in,out] evals_used Contador de evaluaciones; se incrementa en cada
 *                           llamada a cec17_fitness(). El llamante debe sumarlo
 *                           a su propio contador de evaluaciones globales.
 * @param[in,out] gen        Generador PRNG compartido con el algoritmo principal.
 *
 * @pre   sol.size() == dim del problema; budget > 0.
 * @post  evals_used ≤ budget; sol contiene la mejor solución visitada.
 */
inline void solis_wets(std::vector<double> &sol,
                       double              &fit,
                       long                 budget,
                       long                &evals_used,
                       std::mt19937        &gen)
{
    const int    dim        = static_cast<int>(sol.size());
    const double sigma_init = 0.2 * RANGE;

    std::normal_distribution<double> ndis(0.0, 1.0);

    double sigma   = sigma_init;
    int    success = 0;   ///< Contador de éxitos consecutivos para el ajuste de σ.
    int    fail    = 0;   ///< Contador de fallos consecutivos para el ajuste de σ.

    std::vector<double> bias(dim, 0.0); ///< Sesgo acumulado (memoria direccional).
    std::vector<double> xi(dim);        ///< Perturbación gaussiana del paso actual.
    std::vector<double> candidate(dim); ///< Solución candidata temporal.

    while (evals_used < budget) {

        // ── Generar perturbación gaussiana + sesgo acumulado ─────────
        for (int j = 0; j < dim; ++j)
            xi[j] = sigma * ndis(gen);
        for (int j = 0; j < dim; ++j)
            candidate[j] = clamp(sol[j] + xi[j] + bias[j]);

        double cand_fit = cec17_fitness(candidate.data());
        ++evals_used;

        if (cand_fit < fit) {
            // ── Mejora: aceptar y reforzar el sesgo en esta dirección ─
            sol = candidate;
            fit = cand_fit;
            for (int j = 0; j < dim; ++j)
                bias[j] = 0.4 * bias[j] + 0.4 * xi[j];
            ++success; fail = 0;

        } else {
            // ── Sin mejora: probar la dirección opuesta ───────────────
            if (evals_used >= budget) break;
            for (int j = 0; j < dim; ++j)
                candidate[j] = clamp(sol[j] - xi[j] - bias[j]);

            cand_fit = cec17_fitness(candidate.data());
            ++evals_used;

            if (cand_fit < fit) {
                // Mejora en dirección opuesta: aceptar y actualizar sesgo.
                sol = candidate;
                fit = cand_fit;
                for (int j = 0; j < dim; ++j)
                    bias[j] = 0.4 * bias[j] - 0.4 * xi[j];
                ++success; fail = 0;

            } else {
                // Ninguna dirección mejora: reducir el sesgo y registrar fallo.
                for (int j = 0; j < dim; ++j)
                    bias[j] *= 0.5;
                ++fail; success = 0;
            }
        }

        // ── Ajuste adaptativo de σ ────────────────────────────────────
        if      (success >= 5) { sigma = std::min(sigma * 3.0, sigma_init); success = 0; }
        else if (fail    >= 5) { sigma = std::max(sigma * 0.5, 1e-12);      fail    = 0; }
    }
}


// ═════════════════════════════════════════════════════════════
//   BUCLE EXPERIMENTAL CEC2017 (run_algorithm)
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Ejecuta el protocolo experimental estándar de CEC2017.
 *
 * Itera sobre las 30 funciones del benchmark (F1–F30) y, para cada
 * una, realiza T_RUNS ejecuciones independientes del algoritmo
 * proporcionado. Al finalizar cada función imprime por stdout la
 * media, el mejor y el peor error de aproximación.
 *
 * @par Protocolo
 *  - Presupuesto de evaluaciones: 10 000 × dim por ejecución.
 *  - Error reportado: cec17_error(best_fit), es decir, best_fit − f*(funcid).
 *  - Semilla: seed_base + run * 1000 + funcid (reproducible, no correlada
 *    entre funciones ni entre runs de la misma función).
 *  - Resultados en disco: el propio cec17_init() genera los archivos en
 *    results_<alg_name>/ al llamar a la librería del benchmark.
 *
 * @par Uso típico
 * @code
 *   run_algorithm("sca", dim, seed_base,
 *       [](int d, long e, unsigned s){ return SCA(d, e, s); });
 * @endcode
 *
 * @tparam Factory  Callable con firma (int dim, long max_evals, unsigned seed)
 *                  que devuelve un objeto con método run() → double (error).
 *
 * @param alg_name  Identificador textual del algoritmo (usado en rutas de salida).
 * @param dim       Dimensión del espacio de búsqueda (debe ser válida para CEC2017).
 * @param seed_base Semilla base para garantizar reproducibilidad entre experimentos.
 * @param make_alg  Lambda o functor que instancia el algoritmo para cada run.
 */
template <typename Factory>
void run_algorithm(const std::string &alg_name,
                    int                dim,
                    int                seed_base,
                    Factory            make_alg)
{
    const long max_evals = 10000L * dim;

    for (int funcid = 1; funcid <= 30; ++funcid) {
        double sum_error   = 0.0;
        double best_error  =  1e300;
        double worst_error = -1.0;

        for (int run = 0; run < T_RUNS; ++run) {
            // Inicializa el benchmark para esta función y dimensión.
            // Crea el directorio de resultados results_<alg_name>/ si no existe.
            cec17_init(alg_name.c_str(), funcid, dim);

            // Semilla única por (run, funcid): evita correlación entre ejecuciones.
            unsigned int seed = static_cast<unsigned int>(
                seed_base + run * 1000 + funcid);

            auto   alg = make_alg(dim, max_evals, seed);
            double err = alg.run();

            sum_error   += err;
            best_error   = std::min(best_error,  err);
            worst_error  = std::max(worst_error, err);
        }

        std::cout << "F" << funcid
                  << " | Mean: "  << std::scientific << (sum_error / T_RUNS)
                  << " | Best: "  << best_error
                  << " | Worst: " << worst_error
                  << "\n";
    }

    std::cout << "\n[OK] Resultados en results_" << alg_name << "/\n";
}