/**
 * ============================================================
 *  cec17_common.h — Componentes reutilizables para CEC2017
 * ============================================================
 *  Contiene todo lo compartido entre variantes del SCA:
 *    - Constantes del dominio y parámetros comunes
 *    - Utilidades (clamp, valid_dim)
 *    - Clase Population (inicialización y gestión del élite)
 *    - Función solis_wets (búsqueda local estocástica)
 *    - Función run_experiment (bucle estándar CEC2017)
 *
 *  Uso: incluir este header en cada .cpp de algoritmo.
 *    #include "cec17_common.h"
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

// ─────────────────────────────────────────────────────────────
// Dominio (todas las funciones CEC2017)
// ─────────────────────────────────────────────────────────────
inline constexpr double LOWER_BOUND = -100.0;
inline constexpr double UPPER_BOUND =  100.0;
inline constexpr double RANGE       = UPPER_BOUND - LOWER_BOUND;

// ─────────────────────────────────────────────────────────────
// Parámetros comunes
// ─────────────────────────────────────────────────────────────
inline constexpr int T_RUNS   = 10;   // Repeticiones por función
inline constexpr int POP_SIZE = 30;   // Tamaño de población

// ─────────────────────────────────────────────────────────────
// Utilidades
// ─────────────────────────────────────────────────────────────
inline double clamp(double v) noexcept {
    return std::max(LOWER_BOUND, std::min(UPPER_BOUND, v));
}

inline bool valid_dim(int dim) noexcept {
    return dim == 2 || dim == 5  || dim == 10 ||
           dim == 30|| dim == 50 || dim == 100;
}

// ─────────────────────────────────────────────────────────────
// Population
//   Encapsula la población, sus fitness y el élite (mejor global).
//   Proporciona inicialización aleatoria y actualización del élite.
// ─────────────────────────────────────────────────────────────
struct Population {
    std::vector<std::vector<double>> pos;
    std::vector<double>              fit;
    std::vector<double>              best_pos;
    double                           best_fit = 1e300;
    long                             evals    = 0;

    /**
     * Inicializa N agentes aleatoriamente y evalúa cada uno.
     * Actualiza best_pos / best_fit con el mejor encontrado.
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

    /** Evalúa el agente i y actualiza el élite si mejora. */
    void evaluate_and_update(int i) {
        fit[i] = cec17_fitness(pos[i].data());
        ++evals;
        if (fit[i] < best_fit) {
            best_fit = fit[i];
            best_pos = pos[i];
        }
    }

    /** Reemplaza el peor agente con una solución externa (p.ej. resultado de LS). */
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

// ─────────────────────────────────────────────────────────────
// solis_wets
//   Búsqueda local estocástica (Solis & Wets, 1981).
//   Modifica sol y fit in-place. Consume hasta `budget`
//   evaluaciones, reportadas en evals_used.
//
//   Parámetros internos:
//     sigma_init = 0.2 * RANGE
//     max_success / max_fail = 5  (umbral de reajuste de sigma)
//     sigma *= 3  en éxito acumulado, sigma *= 0.5  en fallo
// ─────────────────────────────────────────────────────────────
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
    int    success = 0;
    int    fail    = 0;

    std::vector<double> bias(dim, 0.0);
    std::vector<double> xi(dim);
    std::vector<double> candidate(dim);

    while (evals_used < budget) {
        // Perturbación normal escalada + sesgo acumulado
        for (int j = 0; j < dim; ++j)
            xi[j] = sigma * ndis(gen);
        for (int j = 0; j < dim; ++j)
            candidate[j] = clamp(sol[j] + xi[j] + bias[j]);

        double cand_fit = cec17_fitness(candidate.data());
        ++evals_used;

        if (cand_fit < fit) {
            sol = candidate;
            fit = cand_fit;
            for (int j = 0; j < dim; ++j)
                bias[j] = 0.4 * bias[j] + 0.4 * xi[j];
            ++success; fail = 0;
        } else {
            // Intentar dirección opuesta si queda presupuesto
            if (evals_used >= budget) break;
            for (int j = 0; j < dim; ++j)
                candidate[j] = clamp(sol[j] - xi[j] - bias[j]);

            cand_fit = cec17_fitness(candidate.data());
            ++evals_used;

            if (cand_fit < fit) {
                sol = candidate;
                fit = cand_fit;
                for (int j = 0; j < dim; ++j)
                    bias[j] = 0.4 * bias[j] - 0.4 * xi[j];
                ++success; fail = 0;
            } else {
                for (int j = 0; j < dim; ++j)
                    bias[j] *= 0.5;
                ++fail; success = 0;
            }
        }

        // Reajuste adaptativo de sigma
        if (success >= 5) { sigma = std::min(sigma * 3.0, sigma_init); success = 0; }
        else if (fail >= 5) { sigma = std::max(sigma * 0.5, 1e-12);    fail    = 0; }
    }
}

// ─────────────────────────────────────────────────────────────
// run_algorithm
//   Bucle estándar CEC2017: 30 funciones × T_RUNS repeticiones.
//   Instancia el algoritmo ALG en cada run mediante la lambda
//   `make_alg`, que recibe (dim, max_evals, seed) y devuelve
//   un objeto con método run() → double (error).
//
//   Uso:
//     run_algorithm("sca", dim, [](int d, long e, unsigned s){
//         return SCA(d, e, s);
//     });
// ─────────────────────────────────────────────────────────────
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
            cec17_init(alg_name.c_str(), funcid, dim);

            unsigned int seed = static_cast<unsigned int>(
                seed_base + run * 1000 + funcid);

            auto alg   = make_alg(dim, max_evals, seed);
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
