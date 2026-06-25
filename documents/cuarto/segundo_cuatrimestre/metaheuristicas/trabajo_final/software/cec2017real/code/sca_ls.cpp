/**
 * ============================================================
 *  sca_ls.cpp — SCA-LS: Algoritmo Memético (SCA + Solis-Wets)
 * ============================================================
 *
 *  Primera variante de mejora sobre el SCA original. Combina el
 *  SCA como motor de exploración global con la búsqueda local
 *  estocástica de Solis-Wets como intensificador local, activado
 *  automáticamente cuando el algoritmo detecta estancamiento.
 *
 * ─────────────────────────────────────────────────────────────
 *  MOTIVACIÓN: LIMITACIÓN DEL SCA BASE
 * ─────────────────────────────────────────────────────────────
 *  El SCA carece de mecanismo de intensificación: cuando la
 *  población converge hacia el mejor global P, las actualizaciones
 *  se vuelven de amplitud reducida pero sin dirección informada,
 *  lo que limita el refinamiento fino de la solución. En el
 *  benchmark CEC2017, donde la distancia al óptimo puede ser
 *  muy pequeña, esta incapacidad de explotación precisa es crítica.
 *
 * ─────────────────────────────────────────────────────────────
 *  ESTRATEGIA DE HIBRIDACIÓN
 * ─────────────────────────────────────────────────────────────
 *  El SCA-LS sigue el esquema memético clásico:
 *
 *    1. [Exploración global] El SCA actualiza toda la población
 *       durante STAGNATION_GENS generaciones consecutivas.
 *
 *    2. [Detección de estancamiento] Si el mejor fitness no mejora
 *       durante STAGNATION_GENS generaciones, se activa la LS.
 *       El contador de estancamiento se reinicia tras cada mejora
 *       o tras cada activación de la búsqueda local.
 *
 *    3. [Intensificación local] Solis-Wets parte de best_pos
 *       y refina la solución durante un máximo de LS_BUDGET_RATIO
 *       del presupuesto total (o menos, si quedan pocas evaluaciones).
 *
 *    4. [Reintegración] La solución mejorada por la LS sustituye
 *       al peor agente de la población mediante inject_best(), de
 *       modo que la información local se comparte con la población
 *       en las iteraciones SCA siguientes.
 *
 * ─────────────────────────────────────────────────────────────
 *  PARÁMETROS CLAVE
 * ─────────────────────────────────────────────────────────────
 *    STAGNATION_GENS  = 10   generaciones sin mejora para activar LS.
 *    LS_BUDGET_RATIO  = 0.10 fracción del presupuesto total por llamada LS.
 *
 *  La fracción del 10% equilibra el coste de la LS con su beneficio:
 *  es suficiente para que Solis-Wets converja en la cuenca actual,
 *  sin consumir demasiado presupuesto en regiones ya exploradas.
 *
 * ─────────────────────────────────────────────────────────────
 *  REFERENCIAS
 * ─────────────────────────────────────────────────────────────
 *  SCA:  S. Mirjalili, Knowledge-Based Systems, 2016.
 *        https://doi.org/10.1016/j.knosys.2015.12.022
 *  LS:   F. J. Solis y R. J.-B. Wets, Mathematics of Operations
 *        Research, vol. 6, no. 1, pp. 19-30, 1981.
 *        https://doi.org/10.1287/moor.6.1.19
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 * ============================================================
 */

#include "cec17_common.h"


// ═════════════════════════════════════════════════════════════
//   PARÁMETROS DEL SCA-LS
// ═════════════════════════════════════════════════════════════

/**
 * Amplitud inicial del factor r₁ (igual que en el SCA base).
 * r₁(t) = A_PARAM · (1 − t/T); con A_PARAM = 2 el umbral r₁ = 1
 * se alcanza en t = T/2, separando las fases de exploración y explotación.
 */
static constexpr double A_PARAM = 2.0;

/**
 * Número de generaciones consecutivas sin mejora del élite que
 * deben ocurrir antes de activar la búsqueda local Solis-Wets.
 * Un valor de 10 permite al SCA unos ciclos completos de actualización
 * antes de asumir que está atrapado en una cuenca local.
 */
static constexpr int STAGNATION_GENS = 10;

/**
 * Fracción del presupuesto total (max_evals) reservada para cada
 * invocación de Solis-Wets. Se calcula una sola vez en el constructor:
 *   ls_budget_ = LS_BUDGET_RATIO × max_evals
 *
 * El presupuesto real de cada llamada se limita además por el
 * presupuesto restante, para no sobrepasar max_evals.
 */
static constexpr double LS_BUDGET_RATIO = 0.10;


// ═════════════════════════════════════════════════════════════
//   CLASE SCA_LS
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Algoritmo memético SCA + Solis-Wets (SCA-LS).
 *
 * Extiende el SCA base con un mecanismo de intensificación local
 * activado por estancamiento. La búsqueda local se lanza sobre el
 * mejor global (best_pos) cada vez que ningún agente logra mejorar
 * el élite durante STAGNATION_GENS generaciones consecutivas.
 *
 * La clase sigue la misma interfaz que SCA para su uso en run_algorithm().
 */
class SCA_LS {
public:

    /**
     * @brief  Constructor: configura dimensión, presupuesto y PRNG.
     *
     * Calcula el presupuesto fijo por llamada a la LS como una fracción
     * del presupuesto total, para que el balance exploración/intensificación
     * sea independiente de la dimensión del problema.
     *
     * @param dim        Dimensión del espacio de búsqueda.
     * @param max_evals  Presupuesto total de evaluaciones (= 10 000 × dim).
     * @param seed       Semilla del generador Mersenne Twister.
     */
    SCA_LS(int dim, long max_evals, unsigned int seed)
        : dim_(dim), max_evals_(max_evals),
          ls_budget_(static_cast<long>(max_evals * LS_BUDGET_RATIO)),
          gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND),
          dis_r2_ (0.0, 2.0 * M_PI),
          dis_r3_ (0.0, 2.0),
          dis_r4_ (0.0, 1.0)
    {}

    /**
     * @brief  Ejecuta una ejecución completa del SCA-LS.
     *
     * @par Flujo de ejecución
     *  1. Inicializa la población (consume POP_SIZE evaluaciones).
     *  2. Entra en el bucle principal hasta agotar max_evals:
     *     a. [SCA] Calcula r₁ y actualiza todos los agentes.
     *     b. [Detección] Compara el mejor fitness antes y después:
     *        si no mejora, incrementa el contador de estancamiento.
     *     c. [LS] Si stagnation ≥ STAGNATION_GENS, lanza Solis-Wets
     *        partiendo de best_pos, reintegra el resultado con
     *        inject_best() y reinicia el contador de estancamiento.
     *
     * @return  Error de aproximación al óptimo: cec17_error(best_fit).
     */
    double run() {
        // ── Inicialización ────────────────────────────────────────────
        Population pop(POP_SIZE, dim_, dis_pos_, gen_);
        int stagnation = 0; ///< Generaciones consecutivas sin mejora del élite.

        // ── Bucle principal ───────────────────────────────────────────
        while (pop.evals < max_evals_) {

            double prev_best = pop.best_fit; ///< Fitness del élite al inicio de la generación.

            double progress = static_cast<double>(pop.evals) /
                              static_cast<double>(max_evals_);
            double r1 = A_PARAM * (1.0 - progress); ///< Decaimiento lineal de r₁.

            // ── Fase SCA: actualización de todos los agentes ──────────
            for (int i = 0; i < POP_SIZE; ++i) {
                if (pop.evals >= max_evals_) break;

                for (int j = 0; j < dim_; ++j) {
                    double r2   = dis_r2_(gen_);
                    double r3   = dis_r3_(gen_);
                    double r4   = dis_r4_(gen_);
                    double dist = std::abs(r3 * pop.best_pos[j] - pop.pos[i][j]);

                    if (r4 < 0.5) pop.pos[i][j] += r1 * std::sin(r2) * dist;
                    else          pop.pos[i][j] += r1 * std::cos(r2) * dist;

                    pop.pos[i][j] = clamp(pop.pos[i][j]);
                }

                pop.evaluate_and_update(i);
            }

            // ── Detección de estancamiento ────────────────────────────
            // El élite mejoró → reiniciar; no mejoró → incrementar.
            if (pop.best_fit < prev_best) stagnation = 0;
            else                          ++stagnation;

            // ── Fase LS: Solis-Wets sobre el mejor global ─────────────
            if (stagnation >= STAGNATION_GENS && pop.evals < max_evals_) {

                // El presupuesto real está limitado por lo que queda disponible.
                long ls_budget = std::min(ls_budget_, max_evals_ - pop.evals);
                long ls_evals  = 0;

                // Copia de trabajo: la LS parte del mejor conocido.
                std::vector<double> ls_sol = pop.best_pos;
                double              ls_fit = pop.best_fit;

                // Ejecutar Solis-Wets (modifica ls_sol y ls_fit in-place).
                solis_wets(ls_sol, ls_fit, ls_budget, ls_evals, gen_);

                // Contabilizar las evaluaciones consumidas por la LS.
                pop.evals += ls_evals;

                // Reintegrar la solución mejorada en la población,
                // sustituyendo al peor agente actual.
                pop.inject_best(ls_sol, ls_fit);

                // Reiniciar el contador para la siguiente fase SCA.
                stagnation = 0;
            }
        }

        return cec17_error(pop.best_fit);
    }

private:
    int  dim_;        ///< Dimensión del espacio de búsqueda.
    long max_evals_;  ///< Presupuesto total de evaluaciones.
    long ls_budget_;  ///< Presupuesto fijo por llamada a Solis-Wets.

    std::mt19937                           gen_;
    std::uniform_real_distribution<double> dis_pos_;
    std::uniform_real_distribution<double> dis_r2_;
    std::uniform_real_distribution<double> dis_r3_;
    std::uniform_real_distribution<double> dis_r4_;
};


// ═════════════════════════════════════════════════════════════
//   PUNTO DE ENTRADA
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Punto de entrada del ejecutable SCA-LS.
 *
 * Acepta opcionalmente dos argumentos de línea de comandos:
 *   argv[1] — Dimensión del problema (por defecto: 10).
 *   argv[2] — Semilla base del PRNG   (por defecto: 42).
 *
 * Valida la dimensión, imprime la configuración y delega en
 * run_algorithm() para el protocolo CEC2017 completo.
 *
 * @return 0 si todo va bien; 1 si la dimensión es inválida.
 */
int main(int argc, char *argv[]) {
    int dim       = 10;
    int seed_base = 42;
    if (argc >= 2) dim       = std::stoi(argv[1]);
    if (argc >= 3) seed_base = std::stoi(argv[2]);

    if (!valid_dim(dim)) {
        std::cerr << "[Error] Dimension invalida: " << dim
                  << ". Validos: 2, 5, 10, 30, 50, 100.\n";
        return 1;
    }

    std::cout << "========================================\n"
              << "  SCA-LS (SCA + Solis-Wets)\n"
              << "  Dimension   : " << dim << "\n"
              << "  Max. evals  : " << 10000L * dim << "\n"
              << "  Runs/func   : " << T_RUNS << "\n"
              << "  Pop. size   : " << POP_SIZE << "\n"
              << "  Stagnation  : " << STAGNATION_GENS << " gens\n"
              << "  LS budget % : " << (LS_BUDGET_RATIO * 100.0) << "%\n"
              << "========================================\n\n";

    run_algorithm("sca_ls", dim, seed_base,
        [](int d, long e, unsigned s){ return SCA_LS(d, e, s); });
}