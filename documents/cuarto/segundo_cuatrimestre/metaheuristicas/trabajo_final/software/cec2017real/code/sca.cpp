/**
 * ============================================================
 *  Sine Cosine Algorithm (SCA) — Implementación para CEC2017
 * ============================================================
 *  Referencia original:
 *    S. Mirjalili, "SCA: A Sine Cosine Algorithm for solving
 *    optimization problems", Knowledge-Based Systems, vol. 96,
 *    pp. 120-133, 2016.
 *
 *  Autor:  Jesús Muñoz Velasco
 *  Curso:  Metaheurísticas 2025-2026
 *
 *  Uso:
 *    Compilar con el CMakeLists.txt del repositorio (ver README)
 *    o manualmente:
 *      g++ -O2 -o sca sca.cpp -L.. -lcec17 -Wl,-rpath,..
 *    Ejecutar desde el directorio que contenga input_data/:
 *      ./sca [dim] [seed]
 *
 *  Parámetros del algoritmo:
 *    N  = 30   (tamaño de población)
 *    a  = 2.0  (amplitud inicial de r1)
 *    T  = 10000*dim evaluaciones (criterio de parada estándar)
 *    T_runs = 10 (repeticiones, estándar 50; usamos 10 para reducir tiempos)
 * ============================================================
 */

extern "C" {
#include "cec17.h"
}

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <string>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────
// Constantes del dominio de búsqueda (todas las funciones CEC2017)
// ─────────────────────────────────────────────────────────────
static constexpr double LOWER_BOUND = -100.0;
static constexpr double UPPER_BOUND =  100.0;

// ─────────────────────────────────────────────────────────────
// Parámetros del SCA
// ─────────────────────────────────────────────────────────────
static constexpr int    POP_SIZE  = 30;    // Tamaño de la población
static constexpr double A_PARAM   = 2.0;   // Amplitud inicial de r1
static constexpr int    T_RUNS    = 10;    // Repeticiones por función/dimensión

// ─────────────────────────────────────────────────────────────
//  Clase SCA
// ─────────────────────────────────────────────────────────────
class SCA {
public:
    /**
     * Constructor.
     * @param dim       Dimensión del problema.
     * @param max_evals Máximo número de evaluaciones (10000 * dim).
     * @param seed      Semilla para el generador de números aleatorios.
     */
    SCA(int dim, long max_evals, unsigned int seed)
        : dim_(dim),
          max_evals_(max_evals),
          gen_(seed),
          dis_pos_(LOWER_BOUND, UPPER_BOUND),  // Para inicialización
          dis_r2_(0.0, 2.0 * M_PI),            // r2 en [0, 2π]
          dis_r3_(0.0, 2.0),                   // r3 en [0, 2]
          dis_r4_(0.0, 1.0)                    // r4 en [0, 1]
    {}

    /**
     * Ejecuta el SCA sobre la función activa de cec17.
     * @return Error de la mejor solución encontrada.
     */
    double run() {
        // ── 1. Inicialización de la población ──────────────────────
        std::vector<std::vector<double>> pop(POP_SIZE,
                                             std::vector<double>(dim_));
        std::vector<double> fitness(POP_SIZE);

        for (int i = 0; i < POP_SIZE; ++i) {
            for (int j = 0; j < dim_; ++j)
                pop[i][j] = dis_pos_(gen_);
            fitness[i] = cec17_fitness(pop[i].data());
        }

        long evals = POP_SIZE;

        // ── 2. Determinar la mejor solución inicial (P) ────────────
        //    P es el "destino" (destination point) al que converge la
        //    población, es decir, la mejor solución hasta el momento.
        int best_idx = static_cast<int>(
            std::min_element(fitness.begin(), fitness.end()) - fitness.begin());
        std::vector<double> best_pos = pop[best_idx];
        double best_fit = fitness[best_idx];

        // ── 3. Bucle principal ─────────────────────────────────────
        //    Iteramos hasta agotar el presupuesto de evaluaciones.
        //    En cada iteración t recalculamos r1 con respecto a las
        //    evaluaciones consumidas, no a un contador de generaciones,
        //    para que el decaimiento sea proporcional al presupuesto.
        while (evals < max_evals_) {
            // Fracción de presupuesto consumido ∈ [0, 1)
            double progress = static_cast<double>(evals) /
                              static_cast<double>(max_evals_);

            // r1 decrece linealmente de A a 0 conforme avanza la búsqueda
            //   r1(t) = A * (1 - t/T)
            double r1 = A_PARAM * (1.0 - progress);

            for (int i = 0; i < POP_SIZE; ++i) {
                if (evals >= max_evals_) break;

                // Actualizar cada dimensión del agente i
                for (int j = 0; j < dim_; ++j) {
                    double r2 = dis_r2_(gen_);   // ángulo en [0, 2π]
                    double r3 = dis_r3_(gen_);   // peso de la distancia
                    double r4 = dis_r4_(gen_);   // selección seno/coseno

                    // Distancia ponderada al destino (con signo implícito en |·|)
                    double dist = std::abs(r3 * best_pos[j] - pop[i][j]);

                    // Ecuación de actualización (Eq. 3.3 del artículo original)
                    if (r4 < 0.5)
                        pop[i][j] += r1 * std::sin(r2) * dist;
                    else
                        pop[i][j] += r1 * std::cos(r2) * dist;

                    // Reparación de límites (clamping)
                    pop[i][j] = std::max(LOWER_BOUND,
                                std::min(UPPER_BOUND, pop[i][j]));
                }

                // Evaluar el agente actualizado
                fitness[i] = cec17_fitness(pop[i].data());
                ++evals;

                // Actualizar el mejor global (destino P)
                if (fitness[i] < best_fit) {
                    best_fit = fitness[i];
                    best_pos = pop[i];
                    best_idx = i;
                }
            }
        }

        return cec17_error(best_fit);
    }

private:
    int  dim_;
    long max_evals_;

    std::mt19937                          gen_;
    std::uniform_real_distribution<double> dis_pos_;
    std::uniform_real_distribution<double> dis_r2_;
    std::uniform_real_distribution<double> dis_r3_;
    std::uniform_real_distribution<double> dis_r4_;
};


// ─────────────────────────────────────────────────────────────
//  Programa principal
// ─────────────────────────────────────────────────────────────
int main(int argc, char *argv[]) {

    // Lectura de dimensión y semilla base desde argumentos opcionales
    int dim  = 10;
    int seed_base = 42;

    if (argc >= 2) dim       = std::stoi(argv[1]);
    if (argc >= 3) seed_base = std::stoi(argv[2]);

    // Validación de dimensión (CEC2017 acepta 2, 5, 10, 30, 50, 100)
    if (dim != 2  && dim != 5  && dim != 10 &&
        dim != 30 && dim != 50 && dim != 100) {
        std::cerr << "[Error] Dimension invalida: " << dim
                  << ". Valores validos: 2, 5, 10, 30, 50, 100.\n";
        return 1;
    }

    const long max_evals = 10000L * dim;   // Criterio de parada estándar

    std::cout << "========================================\n"
              << "  Sine Cosine Algorithm (SCA)\n"
              << "  Dimension  : " << dim  << "\n"
              << "  Max. evals : " << max_evals << "\n"
              << "  Runs/func  : " << T_RUNS << "\n"
              << "  Pop. size  : " << POP_SIZE << "\n"
              << "========================================\n\n";

    // ── Bucle sobre las 30 funciones de CEC2017 ──────────────────
    for (int funcid = 1; funcid <= 30; ++funcid) {

        // Inicializar la función del benchmark.
        // El nombre "sca" determina el directorio de salida: results_sca/
        double sum_error = 0.0;
        double best_error = 1e300;
        double worst_error = -1.0;

        for (int run = 0; run < T_RUNS; ++run) {
            cec17_init("sca", funcid, dim);
            unsigned int seed = static_cast<unsigned int>(seed_base + run * 1000 + funcid);
            SCA sca(dim, max_evals, seed);
            double error = sca.run();

            sum_error   += error;
            best_error   = std::min(best_error,  error);
            worst_error  = std::max(worst_error, error);
        }

        double mean_error = sum_error / T_RUNS;

        std::cout << "F" << funcid
                  << " | Mean: "  << std::scientific << mean_error
                  << " | Best: "  << best_error
                  << " | Worst: " << worst_error
                  << "\n";
    }

    std::cout << "\n[OK] Resultados guardados en results_sca/\n";
    return 0;
}
