/**
 * ============================================================
 *  asca.cpp — Adaptive Sine Cosine Algorithm (ASCA)
 * ============================================================
 *  Propuesta de mejora sobre el SCA original. Se identificaron
 *  tres limitaciones en la descripción teórica (Part I de la
 *  memoria) y se diseña una mejora específica para cada una:
 *
 *  LIMITACIÓN 1 → MEJORA 1: Decaimiento no lineal de r1
 *  ──────────────────────────────────────────────────────────
 *  El SCA original usa r1(t) = a·(1 - t/T), que divide el
 *  presupuesto exactamente por la mitad entre exploración y
 *  explotación, independientemente del paisaje de aptitud.
 *  En funciones multimodales complejas (CEC2017 F6-F20) esta
 *  transición es demasiado agresiva: la explotación se activa
 *  antes de que la población haya convergido a regiones
 *  prometedoras.
 *
 *  Solución: sustituir el decaimiento lineal por uno basado
 *  en la función seno (coherente con la naturaleza del propio
 *  SCA), que extiende la fase exploratoria y comprime la
 *  transición hacia el final del presupuesto:
 *
 *    r1(t) = a · cos(π/2 · t/T)²
 *
 *  Esta curva permanece cerca de `a` durante la primera mitad
 *  del presupuesto y cae rápidamente en la segunda, dando más
 *  tiempo a los agentes para cubrir el espacio antes de
 *  concentrar la búsqueda.
 *
 *  LIMITACIÓN 2 → MEJORA 2: Topología de vecindario (ring)
 *  ──────────────────────────────────────────────────────────
 *  En el SCA original todos los agentes convergen al único
 *  mejor global P, lo que genera pérdida de diversidad
 *  temprana (efecto de "imán") y estancamiento en óptimos
 *  locales cuando P no está en la cuenca del óptimo global.
 *
 *  Solución: sustituir el destino global único por un destino
 *  local por vecindario (ring topology de radio K=2). Cada
 *  agente i converge al mejor de sus 2K+1 vecinos en el
 *  anillo {i-K, ..., i, ..., i+K}. El mejor global se
 *  mantiene como archivo separado y se usa para el reinicio.
 *
 *  Justificación: ampliamente validada en PSO (lbest vs gbest)
 *  — la topología local ralentiza la convergencia pero reduce
 *  drásticamente las caídas en óptimos locales.
 *
 *  LIMITACIÓN 3 → MEJORA 3: Reinicio parcial adaptativo
 *  ──────────────────────────────────────────────────────────
 *  Cuando la diversidad de la población colapsa (todos los
 *  agentes en la misma región), el SCA queda atrapado sin
 *  mecanismo de escape salvo que la búsqueda local (SCA-LS)
 *  logre salir por exploración local. En funciones con muchos
 *  óptimos locales esto no es suficiente.
 *
 *  Solución: medir la diversidad de la población como la
 *  distancia media al centroide. Si cae por debajo de un
 *  umbral adaptativo (DIV_THRESHOLD · RANGE), el 30% peor
 *  de la población se reinicializa con perturbación gaussiana
 *  alrededor del mejor global con radio decreciente. Esto
 *  inyecta diversidad sin perder la información del élite.
 *
 *  Umbral adaptativo: se escala con la dimensión del problema
 *  para evitar que en dimensiones altas (d=50) la distancia
 *  media sea siempre mayor que el umbral fijo.
 *
 * ============================================================
 *  Referencia base:
 *    S. Mirjalili, "SCA: A Sine Cosine Algorithm for solving
 *    optimization problems", Knowledge-Based Systems, 2016.
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 * ============================================================
 */

#include "cec17_common.h"

#include <numeric>
#include <cmath>

// ─────────────────────────────────────────────────────────────
// Parámetros del ASCA
// ─────────────────────────────────────────────────────────────
static constexpr double A_PARAM         = 2.0;   // Amplitud inicial r1
static constexpr int    RING_K          = 2;     // Radio del vecindario (2K+1 vecinos)
static constexpr double DIV_THRESHOLD   = 5e-3;  // Umbral de diversidad relativo
static constexpr double RESTART_SIGMA   = 0.1;   // Sigma inicial del reinicio (fracción de RANGE)
static constexpr double RESTART_RATIO   = 0.30;  // Fracción de la población que se reinicia

// ─────────────────────────────────────────────────────────────
// Clase ASCA
// ─────────────────────────────────────────────────────────────
class ASCA {
public:
    ASCA(int dim, long max_evals, unsigned int seed)
        : dim_(dim), max_evals_(max_evals), gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND),
          dis_r2_(0.0, 2.0 * M_PI),
          dis_r3_(0.0, 2.0),
          dis_r4_(0.0, 1.0),
          dis_unit_(0.0, 1.0)
    {}

    double run() {
        // ── 1. Inicialización ──────────────────────────────────────
        Population pop(POP_SIZE, dim_, dis_pos_, gen_);

        // Sigma del reinicio parcial: decrece con el progreso
        double restart_sigma = RESTART_SIGMA * RANGE;

        while (pop.evals < max_evals_) {
            double progress = static_cast<double>(pop.evals) /
                              static_cast<double>(max_evals_);

            // ── MEJORA 1: r1 con decaimiento no lineal ──────────────
            // r1(t) = a · cos²(π/2 · t/T)
            // Permanece alta durante la primera mitad y cae
            // abruptamente al final, extendiendo la exploración.
            double phase = (M_PI / 2.0) * progress;
            double r1    = A_PARAM * std::cos(phase) * std::cos(phase);

            // ── MEJORA 2: Topología de vecindario (ring lbest) ──────
            // Precalcular el mejor de cada vecindario
            std::vector<int> nbest(POP_SIZE);
            for (int i = 0; i < POP_SIZE; ++i) {
                int best_nb = i;
                for (int k = -RING_K; k <= RING_K; ++k) {
                    int nb = (i + k + POP_SIZE) % POP_SIZE;
                    if (pop.fit[nb] < pop.fit[best_nb])
                        best_nb = nb;
                }
                nbest[i] = best_nb;
            }

            // ── Actualización de posiciones ─────────────────────────
            for (int i = 0; i < POP_SIZE; ++i) {
                if (pop.evals >= max_evals_) break;

                // Destino local: mejor del vecindario de i
                const std::vector<double> &dest = pop.pos[nbest[i]];

                for (int j = 0; j < dim_; ++j) {
                    double r2   = dis_r2_(gen_);
                    double r3   = dis_r3_(gen_);
                    double r4   = dis_r4_(gen_);
                    double dist = std::abs(r3 * dest[j] - pop.pos[i][j]);

                    if (r4 < 0.5) pop.pos[i][j] += r1 * std::sin(r2) * dist;
                    else          pop.pos[i][j] += r1 * std::cos(r2) * dist;

                    pop.pos[i][j] = clamp(pop.pos[i][j]);
                }

                pop.evaluate_and_update(i);
            }

            // ── MEJORA 3: Reinicio parcial adaptativo ───────────────
            if (pop.evals >= max_evals_) break;

            // Calcular diversidad: distancia media al centroide
            double diversity = compute_diversity(pop);

            // Umbral adaptativo: escala con sqrt(dim) para que sea
            // comparable en todas las dimensiones
            double threshold = DIV_THRESHOLD * RANGE * std::sqrt(dim_);

            if (diversity < threshold) {
                // Sigma del reinicio decrece con el progreso
                // (perturbaciones amplias al principio, finas al final)
                restart_sigma = RESTART_SIGMA * RANGE * (1.0 - progress);
                restart_sigma = std::max(restart_sigma, 1e-6);

                partial_restart(pop, restart_sigma);
            }
        }

        return cec17_error(pop.best_fit);
    }

private:

    // ── Diversidad: distancia media de cada agente al centroide ────
    double compute_diversity(const Population &pop) const {
        std::vector<double> centroid(dim_, 0.0);
        for (int i = 0; i < POP_SIZE; ++i)
            for (int j = 0; j < dim_; ++j)
                centroid[j] += pop.pos[i][j];
        for (int j = 0; j < dim_; ++j)
            centroid[j] /= POP_SIZE;

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

    // ── Reinicio parcial: el RESTART_RATIO peor se reinicializa ────
    // alrededor del mejor global con perturbación gaussiana de
    // radio restart_sigma.
    void partial_restart(Population &pop, double sigma) {
        // Ordenar índices por fitness (peor = mayor valor)
        std::vector<int> idx(POP_SIZE);
        std::iota(idx.begin(), idx.end(), 0);
        std::sort(idx.begin(), idx.end(),
                  [&pop](int a, int b){ return pop.fit[a] > pop.fit[b]; });

        int n_restart = static_cast<int>(POP_SIZE * RESTART_RATIO);
        std::normal_distribution<double> ndis(0.0, sigma);

        for (int k = 0; k < n_restart; ++k) {
            if (pop.evals >= max_evals_) break;
            int i = idx[k];
            for (int j = 0; j < dim_; ++j)
                pop.pos[i][j] = clamp(pop.best_pos[j] + ndis(gen_));
            pop.evaluate_and_update(i);
        }
    }

    int  dim_;
    long max_evals_;

    std::mt19937                           gen_;
    std::uniform_real_distribution<double> dis_pos_;
    std::uniform_real_distribution<double> dis_r2_;
    std::uniform_real_distribution<double> dis_r3_;
    std::uniform_real_distribution<double> dis_r4_;
    std::uniform_real_distribution<double> dis_unit_;
};

// ─────────────────────────────────────────────────────────────
// Main
// ─────────────────────────────────────────────────────────────
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