/**
 * ============================================================
 *  asca.cpp — Adaptive Sine Cosine Algorithm (ASCA)
 * ============================================================
 *
 *  Segunda variante de mejora sobre el SCA original. En lugar de
 *  hibridar con una búsqueda local, el ASCA modifica la estructura
 *  interna del propio SCA para corregir tres limitaciones de diseño
 *  identificadas en el análisis teórico (Parte I de la memoria).
 *  Cada limitación se trata con una mejora específica e independiente.
 *
 * ─────────────────────────────────────────────────────────────
 *  LIMITACIÓN 1 → MEJORA 1: Decaimiento no lineal de r₁
 * ─────────────────────────────────────────────────────────────
 *  Problema: el SCA usa r₁(t) = a·(1 − t/T), que divide el
 *  presupuesto exactamente por la mitad entre exploración y
 *  explotación, sin adaptarse al paisaje de aptitud. En funciones
 *  multimodales complejas (CEC2017 F6–F20) la transición es demasiado
 *  agresiva: la fase de explotación se activa antes de que la
 *  población haya cubierto el espacio suficientemente.
 *
 *  Solución: sustituir el decaimiento lineal por uno basado en el
 *  coseno cuadrado (coherente con la naturaleza trigonométrica del SCA):
 *
 *    r₁(t) = a · cos²(π/2 · t/T)
 *
 *  Esta curva permanece próxima a `a` durante la primera mitad
 *  del presupuesto y cae abruptamente al final, extendiendo la
 *  fase de exploración sin modificar el rango inicial ni final.
 *
 * ─────────────────────────────────────────────────────────────
 *  LIMITACIÓN 2 → MEJORA 2: Topología de vecindario (ring lbest)
 * ─────────────────────────────────────────────────────────────
 *  Problema: en el SCA todos los agentes convergen al único mejor
 *  global P, lo que genera pérdida de diversidad temprana (efecto
 *  "imán") y estancamiento cuando P no está en la cuenca del óptimo
 *  global. Esta arquitectura "gbest" es la más susceptible a los
 *  óptimos locales en PSO y sus derivados.
 *
 *  Solución: sustituir el destino global único por un destino local
 *  por vecindario (ring topology con radio K = RING_K). El agente i
 *  converge al mejor de sus 2K+1 vecinos en el anillo circular
 *  { i−K, …, i, …, i+K } (con aritmética módulo POP_SIZE).
 *  El mejor global se mantiene en el archivo élite de Population
 *  y se emplea únicamente para el reinicio parcial (Mejora 3).
 *
 *  Fundamento: la topología local (lbest) ralentiza la convergencia
 *  respecto a gbest, pero reduce drásticamente las caídas en óptimos
 *  locales, tal y como se ha validado extensamente en la literatura PSO.
 *
 * ─────────────────────────────────────────────────────────────
 *  LIMITACIÓN 3 → MEJORA 3: Reinicio parcial adaptativo
 * ─────────────────────────────────────────────────────────────
 *  Problema: cuando la diversidad de la población colapsa (todos los
 *  agentes se concentran en la misma región), el SCA no dispone de
 *  mecanismo de escape. En funciones con muchos óptimos locales esto
 *  conlleva estancamiento permanente.
 *
 *  Solución: medir la diversidad como la distancia media euclidiana
 *  de cada agente al centroide de la población. Si cae por debajo de
 *  un umbral adaptativo (DIV_THRESHOLD · RANGE · √dim), se reinicializa
 *  el RESTART_RATIO de los peores agentes con perturbación gaussiana
 *  centrada en best_pos y desviación típica decreciente con el progreso:
 *
 *    σ(t) = RESTART_SIGMA · RANGE · (1 − t/T)
 *
 *  El umbral escala con √dim para que el criterio sea comparable en
 *  todas las dimensiones (la distancia media al centroide crece con d).
 *  La σ decreciente produce perturbaciones amplias al inicio (exploración)
 *  y finas al final (refinamiento), sin perder la información del élite.
 *
 * ─────────────────────────────────────────────────────────────
 *  REFERENCIA BASE
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

#include <numeric>
#include <cmath>


// ═════════════════════════════════════════════════════════════
//   PARÁMETROS DEL ASCA
// ═════════════════════════════════════════════════════════════

/**
 * Amplitud inicial del factor r₁ (igual que en el SCA base).
 * Con a = 2, el movimiento inicial puede ser hasta el doble de
 * la distancia al destino, cubriendo el espacio en ambos sentidos.
 */
static constexpr double A_PARAM = 2.0;

/**
 * Radio del vecindario en la topología ring (Mejora 2).
 * Cada agente i tiene 2·RING_K + 1 = 5 vecinos en el anillo.
 * Un radio pequeño (K = 2) mantiene grupos locales bien definidos
 * sin fragmentar demasiado la información entre agentes.
 */
static constexpr int RING_K = 2;

/**
 * Umbral de diversidad relativo para activar el reinicio parcial
 * (Mejora 3). Se compara con la distancia media al centroide
 * normalizada: diversity / (RANGE · √dim).
 *
 * Un valor de 5×10⁻³ activa el reinicio cuando todos los agentes
 * están muy próximos entre sí (convergencia prematura), sin
 * dispararse en fases de exploración normal.
 */
static constexpr double DIV_THRESHOLD = 5e-3;

/**
 * Desviación típica inicial del reinicio parcial, como fracción
 * de RANGE (Mejora 3). Se multiplica por (1 − progreso) para que
 * la perturbación sea decreciente a lo largo de la ejecución.
 */
static constexpr double RESTART_SIGMA = 0.1;

/**
 * Fracción de la población que se reinicializa cuando la diversidad
 * colapsa (Mejora 3). Se seleccionan los RESTART_RATIO·POP_SIZE
 * agentes con peor fitness (los más alejados del óptimo local actual).
 */
static constexpr double RESTART_RATIO = 0.30;


// ═════════════════════════════════════════════════════════════
//   CLASE ASCA
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Adaptive Sine Cosine Algorithm (ASCA).
 *
 * Extiende el SCA base con tres mejoras estructurales independientes:
 *
 *  1. Decaimiento coseno cuadrado de r₁ (exploración extendida).
 *  2. Topología de vecindario ring lbest (diversidad sostenida).
 *  3. Reinicio parcial adaptativo por detección de convergencia prematura.
 *
 * Las tres mejoras son independientes en diseño pero interaccionan en
 * la práctica: la topología local retarda la convergencia, lo que reduce
 * la frecuencia de los reinicios; y el decaimiento coseno mantiene r₁
 * alto más tiempo, reforzando el efecto de la topología local.
 */
class ASCA {
public:

    /**
     * @brief  Constructor: configura dimensión, presupuesto y PRNG.
     *
     * @param dim        Dimensión del espacio de búsqueda.
     * @param max_evals  Presupuesto total de evaluaciones (= 10 000 × dim).
     * @param seed       Semilla del generador Mersenne Twister.
     */
    ASCA(int dim, long max_evals, unsigned int seed)
        : dim_(dim), max_evals_(max_evals), gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND),
          dis_r2_ (0.0, 2.0 * M_PI),
          dis_r3_ (0.0, 2.0),
          dis_r4_ (0.0, 1.0),
          dis_unit_(0.0, 1.0)
    {}

    /**
     * @brief  Ejecuta una ejecución completa del ASCA.
     *
     * @par Flujo de ejecución
     *  1. Inicializa la población (consume POP_SIZE evaluaciones).
     *  2. Calcula el σ inicial del reinicio parcial.
     *  3. Entra en el bucle principal hasta agotar max_evals:
     *     a. [Mejora 1] Calcula r₁ con decaimiento coseno cuadrado.
     *     b. [Mejora 2] Precalcula el mejor vecino de cada agente (ring).
     *     c. [SCA]      Actualiza posiciones usando el destino local.
     *     d. [Mejora 3] Comprueba la diversidad y aplica reinicio parcial
     *                   si ésta cae por debajo del umbral adaptativo.
     *
     * @return  Error de aproximación al óptimo: cec17_error(best_fit).
     */
    double run() {
        // ── Inicialización ────────────────────────────────────────────
        Population pop(POP_SIZE, dim_, dis_pos_, gen_);

        // σ del reinicio decrece con el progreso (se recalcula en cada ciclo).
        double restart_sigma = RESTART_SIGMA * RANGE;

        // ── Bucle principal ───────────────────────────────────────────
        while (pop.evals < max_evals_) {

            double progress = static_cast<double>(pop.evals) /
                              static_cast<double>(max_evals_);

            // ── MEJORA 1: r₁ con decaimiento coseno cuadrado ──────────
            // r₁(t) = a · cos²(π/2 · t/T)
            // Permanece próximo a `a` durante la primera mitad del
            // presupuesto y cae abruptamente al final, extendiendo la
            // fase de exploración respecto al decaimiento lineal del SCA.
            double phase = (M_PI / 2.0) * progress;
            double r1    = A_PARAM * std::cos(phase) * std::cos(phase);

            // ── MEJORA 2: Precálculo del mejor vecino (ring lbest) ────
            // Para cada agente i, se identifica el agente con mejor fitness
            // en la ventana [i−K, …, i+K] del anillo circular.
            // El agente i converge a su mejor vecino, no al global P.
            std::vector<int> nbest(POP_SIZE);
            for (int i = 0; i < POP_SIZE; ++i) {
                int best_nb = i;
                for (int k = -RING_K; k <= RING_K; ++k) {
                    int nb = (i + k + POP_SIZE) % POP_SIZE; // Aritmética circular.
                    if (pop.fit[nb] < pop.fit[best_nb])
                        best_nb = nb;
                }
                nbest[i] = best_nb;
            }

            // ── Actualización de posiciones con destino local ─────────
            for (int i = 0; i < POP_SIZE; ++i) {
                if (pop.evals >= max_evals_) break;

                // Destino del agente i: el mejor de su vecindario ring.
                const std::vector<double> &dest = pop.pos[nbest[i]];

                for (int j = 0; j < dim_; ++j) {
                    double r2   = dis_r2_(gen_);
                    double r3   = dis_r3_(gen_);
                    double r4   = dis_r4_(gen_);

                    // Distancia al destino local (no al global P).
                    double dist = std::abs(r3 * dest[j] - pop.pos[i][j]);

                    if (r4 < 0.5) pop.pos[i][j] += r1 * std::sin(r2) * dist;
                    else          pop.pos[i][j] += r1 * std::cos(r2) * dist;

                    pop.pos[i][j] = clamp(pop.pos[i][j]);
                }

                // Evalúa y actualiza el élite global (best_pos / best_fit).
                // El élite global sigue siendo el mejor de toda la población,
                // aunque no se usa como destino de movimiento (solo para reinicio).
                pop.evaluate_and_update(i);
            }

            // ── MEJORA 3: Reinicio parcial adaptativo ─────────────────
            if (pop.evals >= max_evals_) break;

            // Diversidad: distancia media euclidiana de cada agente al centroide.
            double diversity = compute_diversity(pop);

            // Umbral adaptativo: escala con √dim para que sea comparable
            // en todas las dimensiones (la distancia media crece con d).
            double threshold = DIV_THRESHOLD * RANGE * std::sqrt(dim_);

            if (diversity < threshold) {
                // σ decrece con el progreso: perturbaciones amplias al inicio,
                // finas al final. Se garantiza un mínimo de 1e-6 para evitar
                // degeneración numérica en las últimas iteraciones.
                restart_sigma = RESTART_SIGMA * RANGE * (1.0 - progress);
                restart_sigma = std::max(restart_sigma, 1e-6);

                partial_restart(pop, restart_sigma);
            }
        }

        return cec17_error(pop.best_fit);
    }

private:

    // ─────────────────────────────────────────────────────────────
    //   Cálculo de diversidad poblacional
    // ─────────────────────────────────────────────────────────────

    /**
     * @brief  Calcula la diversidad de la población como distancia media al centroide.
     *
     * El centroide es la media aritmética de todas las posiciones. La diversidad
     * es la distancia euclidiana media de cada agente al centroide, que tiende a
     * cero cuando todos los agentes están concentrados en la misma región.
     *
     * Complejidad: O(POP_SIZE × dim).
     *
     * @param pop  Estado actual de la población.
     * @return     Distancia euclidiana media al centroide (≥ 0).
     */
    double compute_diversity(const Population &pop) const {
        // Calcular el centroide de la población.
        std::vector<double> centroid(dim_, 0.0);
        for (int i = 0; i < POP_SIZE; ++i)
            for (int j = 0; j < dim_; ++j)
                centroid[j] += pop.pos[i][j];
        for (int j = 0; j < dim_; ++j)
            centroid[j] /= POP_SIZE;

        // Distancia media euclidiana al centroide.
        double total = 0.0;
        for (int i = 0; i < POP_SIZE; ++i) {
            double dist2 = 0.0;
            for (int j = 0; j < dim_; ++j) {
                double d = pop.pos[i][j] - centroid[j];
                dist2 += d * d;
            }
            total += std::sqrt(dist2);
        }
        return total / POP_SIZE;
    }

    // ─────────────────────────────────────────────────────────────
    //   Reinicio parcial adaptativo
    // ─────────────────────────────────────────────────────────────

    /**
     * @brief  Reinicializa el RESTART_RATIO peor de la población alrededor del élite.
     *
     * Selecciona los RESTART_RATIO·POP_SIZE agentes con peor fitness (los más
     * alejados del óptimo local actual) y reemplaza sus posiciones por muestras
     * gaussianas centradas en best_pos con desviación típica sigma. El resultado
     * se evalúa inmediatamente y el élite se actualiza si hay mejora.
     *
     * El radio sigma debe ser decreciente con el progreso (ver run()) para que
     * las perturbaciones sean amplias al inicio (exploración de nuevas cuencas)
     * y finas al final (refinamiento en la cuenca del élite).
     *
     * @param pop    Estado de la población (modificado in-place).
     * @param sigma  Desviación típica de la perturbación gaussiana (en unidades del dominio).
     */
    void partial_restart(Population &pop, double sigma) {
        // Ordenar agentes de peor a mejor (descendente por fitness).
        std::vector<int> idx(POP_SIZE);
        std::iota(idx.begin(), idx.end(), 0);
        std::sort(idx.begin(), idx.end(),
                  [&pop](int a, int b){ return pop.fit[a] > pop.fit[b]; });

        int n_restart = static_cast<int>(POP_SIZE * RESTART_RATIO);
        std::normal_distribution<double> ndis(0.0, sigma);

        for (int k = 0; k < n_restart; ++k) {
            if (pop.evals >= max_evals_) break;
            int i = idx[k]; ///< Índice del k-ésimo peor agente.
            // Nueva posición: best_pos + perturbación gaussiana, proyectada al dominio.
            for (int j = 0; j < dim_; ++j)
                pop.pos[i][j] = clamp(pop.best_pos[j] + ndis(gen_));
            pop.evaluate_and_update(i);
        }
    }

    int  dim_;        ///< Dimensión del espacio de búsqueda.
    long max_evals_;  ///< Presupuesto total de evaluaciones.

    std::mt19937                           gen_;
    std::uniform_real_distribution<double> dis_pos_;   ///< U[LOWER_BOUND, UPPER_BOUND].
    std::uniform_real_distribution<double> dis_r2_;    ///< U[0, 2π].
    std::uniform_real_distribution<double> dis_r3_;    ///< U[0, 2].
    std::uniform_real_distribution<double> dis_r4_;    ///< U[0, 1].
    std::uniform_real_distribution<double> dis_unit_;  ///< U[0, 1] (reservada para extensiones).
};


// ═════════════════════════════════════════════════════════════
//   PUNTO DE ENTRADA
// ═════════════════════════════════════════════════════════════

/**
 * @brief  Punto de entrada del ejecutable ASCA.
 *
 * Acepta opcionalmente dos argumentos de línea de comandos:
 *   argv[1] — Dimensión del problema (por defecto: 10).
 *   argv[2] — Semilla base del PRNG   (por defecto: 42).
 *
 * Imprime un resumen de las tres mejoras activas con sus parámetros,
 * valida la dimensión y delega en run_algorithm() para el protocolo
 * CEC2017 completo (30 funciones × T_RUNS ejecuciones).
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
              << "  Adaptive SCA (ASCA)\n"
              << "  Mejoras sobre SCA original:\n"
              << "    1. Decaimiento coseno cuadratico de r1\n"
              << "    2. Topologia de vecindario ring (K=" << RING_K << ")\n"
              << "    3. Reinicio parcial adaptativo ("
              <<        static_cast<int>(RESTART_RATIO * 100) << "% peor)\n"
              << "  Dimension   : " << dim << "\n"
              << "  Max. evals  : " << 10000L * dim << "\n"
              << "  Runs/func   : " << T_RUNS << "\n"
              << "  Pop. size   : " << POP_SIZE << "\n"
              << "========================================\n\n";

    run_algorithm("asca", dim, seed_base,
        [](int d, long e, unsigned s){ return ASCA(d, e, s); });
}