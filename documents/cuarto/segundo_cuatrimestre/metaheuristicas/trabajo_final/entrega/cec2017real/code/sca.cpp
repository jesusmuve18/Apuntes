/**
 * ============================================================
 *  sca.cpp — Sine Cosine Algorithm (SCA)
 * ============================================================
 *
 *  Implementación de referencia del SCA para el benchmark CEC2017.
 *  Este fichero constituye la línea base contra la que se comparan
 *  las variantes SCA-LS y ASCA desarrolladas en la práctica.
 *
 * ─────────────────────────────────────────────────────────────
 *  DESCRIPCIÓN DEL ALGORITMO
 * ─────────────────────────────────────────────────────────────
 *  El SCA es una metaheurística poblacional basada en modelos
 *  matemáticos que actualiza las posiciones de los agentes mediante
 *  las funciones seno y coseno. La ecuación de actualización para
 *  la coordenada j del agente i en la iteración t es:
 *
 *    Si r₄ < 0.5:  X_i^{t+1}[j] = X_i[j] + r₁·sin(r₂)·|r₃·P[j] − X_i[j]|
 *    Si r₄ ≥ 0.5:  X_i^{t+1}[j] = X_i[j] + r₁·cos(r₂)·|r₃·P[j] − X_i[j]|
 *
 *  donde P es la mejor posición global encontrada hasta el momento y
 *  r₁, r₂, r₃, r₄ son parámetros estocásticos con roles específicos:
 *
 *    r₁ ∈ [0, a] decreciente → controla la amplitud del paso
 *                               (exploración → explotación)
 *    r₂ ∈ [0, 2π] aleatorio  → ángulo trigonométrico (dirección y signo)
 *    r₃ ∈ [0, 2]  aleatorio  → peso estocástico de la distancia al destino
 *    r₄ ∈ [0, 1]  aleatorio  → selección equiprobable de sin o cos
 *
 *  El parámetro r₁ decrece linealmente: r₁(t) = a·(1 − t/T), con a = 2.
 *  Cuando r₁ > 1 el agente puede sobrepasar el destino (EXPLORACIÓN);
 *  cuando r₁ < 1 se mueve dentro del segmento al destino (EXPLOTACIÓN).
 *  La transición ocurre exactamente en t = T/2.
 *
 * ─────────────────────────────────────────────────────────────
 *  LIMITACIONES IDENTIFICADAS (justifican SCA-LS y ASCA)
 * ─────────────────────────────────────────────────────────────
 *  1. Convergencia prematura: todos los agentes convergen al único P
 *     global, lo que elimina diversidad antes de explorar el espacio.
 *  2. Decaimiento lineal de r₁: la transición exploración/explotación
 *     es rígida e independiente del paisaje de aptitud del problema.
 *  3. Sin mecanismo de escape: el SCA no tiene forma de salir de un
 *     óptimo local una vez que la población ha convergido.
 *
 * ─────────────────────────────────────────────────────────────
 *  REFERENCIA
 * ─────────────────────────────────────────────────────────────
 *  S. Mirjalili, "SCA: A Sine Cosine Algorithm for solving
 *  optimization problems", Knowledge-Based Systems, vol. 96,
 *  pp. 120-133, 2016. https://doi.org/10.1016/j.knosys.2015.12.022
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 * ============================================================
 */

#include "cec17_common.h"


// ═════════════════════════════════════════════════════════════
//   PARÁMETROS DEL SCA
// ═════════════════════════════════════════════════════════════

/**
 * Amplitud inicial del factor r₁.
 *
 * Con a = 2, el umbral r₁ = 1 (que separa exploración de explotación)
 * se alcanza exactamente en t = T/2, dividiendo el presupuesto en dos
 * mitades iguales. También garantiza que el rango de movimiento inicial
 * sea el doble de la distancia al destino, cubriendo ambos lados del P.
 */
static constexpr double A_PARAM = 2.0;


// ═════════════════════════════════════════════════════════════
//   CLASE SCA
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Implementación del Sine Cosine Algorithm (Mirjalili, 2016).
 *
 * Gestiona el bucle principal del SCA: inicialización de la población,
 * cálculo del factor r₁ y actualización de posiciones por agente
 * y por dimensión. El criterio de parada es el agotamiento del
 * presupuesto de evaluaciones (max_evals).
 *
 * Diseñada para ser instanciada y ejecutada una sola vez mediante run().
 * El resultado es el error de aproximación al óptimo de la función actual,
 * listo para acumularse en las estadísticas del experimento.
 */
class SCA {
public:

    /**
     * @brief  Constructor: configura dimensión, presupuesto y PRNG.
     *
     * @param dim        Dimensión del espacio de búsqueda.
     * @param max_evals  Presupuesto total de evaluaciones (= 10 000 × dim).
     * @param seed       Semilla del generador Mersenne Twister (reproducibilidad).
     */
    SCA(int dim, long max_evals, unsigned int seed)
        : dim_(dim), max_evals_(max_evals), gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND), ///< Distribución para inicialización uniforme.
          dis_r2_ (0.0, 2.0 * M_PI),          ///< r₂ ~ U[0, 2π]: ángulo trigonométrico.
          dis_r3_ (0.0, 2.0),                  ///< r₃ ~ U[0, 2]:   peso de la distancia.
          dis_r4_ (0.0, 1.0)                   ///< r₄ ~ U[0, 1]:   selección sin/cos.
    {}

    /**
     * @brief  Ejecuta una ejecución completa del SCA.
     *
     * @par Flujo de ejecución
     *  1. Inicializa la población aleatoriamente (consume POP_SIZE evaluaciones).
     *  2. Entra en el bucle principal hasta agotar max_evals:
     *     a. Calcula r₁ con decaimiento lineal según el progreso actual.
     *     b. Para cada agente, actualiza todas sus coordenadas hacia P.
     *     c. Evalúa el agente y actualiza el élite si mejora.
     *
     * @return  Error de aproximación al óptimo de la función actual:
     *          cec17_error(best_fit) = best_fit − f*(funcid).
     */
    double run() {
        // ── Inicialización: agentes uniformes en [-100, 100]^dim ─────
        Population pop(POP_SIZE, dim_, dis_pos_, gen_);

        // ── Bucle principal ───────────────────────────────────────────
        while (pop.evals < max_evals_) {

            // Fracción del presupuesto consumida: progreso ∈ [0, 1].
            double progress = static_cast<double>(pop.evals) /
                              static_cast<double>(max_evals_);

            // r₁ decrece linealmente: a·(1 − t/T).
            // r₁ > 1 → exploración (el agente puede sobrepasar P).
            // r₁ < 1 → explotación (el agente se mueve hacia P).
            double r1 = A_PARAM * (1.0 - progress);

            // ── Actualización por agente ──────────────────────────────
            for (int i = 0; i < POP_SIZE; ++i) {
                if (pop.evals >= max_evals_) break;

                // ── Actualización por dimensión ───────────────────────
                for (int j = 0; j < dim_; ++j) {
                    double r2   = dis_r2_(gen_);  ///< Ángulo ~ U[0, 2π].
                    double r3   = dis_r3_(gen_);  ///< Peso   ~ U[0, 2].
                    double r4   = dis_r4_(gen_);  ///< Selector sin/cos ~ U[0,1].

                    // Distancia ponderada al destino (siempre positiva).
                    // r₃ > 1 amplifica la distancia (mayor salto);
                    // r₃ < 1 la reduce (menor paso hacia P).
                    double dist = std::abs(r3 * pop.best_pos[j] - pop.pos[i][j]);

                    // El signo real del movimiento lo determina sin(r₂) o cos(r₂):
                    // puede ser positivo o negativo, permitiendo moverse en ambos
                    // sentidos respecto al destino P.
                    if (r4 < 0.5) pop.pos[i][j] += r1 * std::sin(r2) * dist;
                    else          pop.pos[i][j] += r1 * std::cos(r2) * dist;

                    // Proyectar al dominio factible.
                    pop.pos[i][j] = clamp(pop.pos[i][j]);
                }

                // Evaluar el agente actualizado y actualizar el élite global.
                pop.evaluate_and_update(i);
            }
        }

        return cec17_error(pop.best_fit);
    }

private:
    int  dim_;        ///< Dimensión del espacio de búsqueda.
    long max_evals_;  ///< Presupuesto de evaluaciones.

    std::mt19937                           gen_;      ///< Generador Mersenne Twister.
    std::uniform_real_distribution<double> dis_pos_;  ///< U[LOWER_BOUND, UPPER_BOUND].
    std::uniform_real_distribution<double> dis_r2_;   ///< U[0, 2π].
    std::uniform_real_distribution<double> dis_r3_;   ///< U[0, 2].
    std::uniform_real_distribution<double> dis_r4_;   ///< U[0, 1].
};


// ═════════════════════════════════════════════════════════════
//   PUNTO DE ENTRADA
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Punto de entrada del ejecutable SCA.
 *
 * Acepta opcionalmente dos argumentos de línea de comandos:
 *   argv[1] — Dimensión del problema (por defecto: 10).
 *   argv[2] — Semilla base del PRNG   (por defecto: 42).
 *
 * Valida la dimensión, imprime la configuración del experimento y
 * delega en run_algorithm() para ejecutar el protocolo CEC2017
 * completo (30 funciones × T_RUNS ejecuciones).
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
              << "  Sine Cosine Algorithm (SCA)\n"
              << "  Dimension  : " << dim << "\n"
              << "  Max. evals : " << 10000L * dim << "\n"
              << "  Runs/func  : " << T_RUNS << "\n"
              << "  Pop. size  : " << POP_SIZE << "\n"
              << "========================================\n\n";

    run_algorithm("sca", dim, seed_base,
        [](int d, long e, unsigned s){ return SCA(d, e, s); });
}