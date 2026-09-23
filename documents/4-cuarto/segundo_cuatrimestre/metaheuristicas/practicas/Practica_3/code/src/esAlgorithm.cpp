#include <cmath>

#include "esAlgorithm.h"
#include "clusteringPloblem.h"
#include "random.hpp"

using namespace std;

double ESAlgorithm::nextT(double beta, double T)  {
    return T / (1.0 + beta * T);
}

ResultMHInt ESAlgorithm::optimize(Problem<int>& problem, int maxevals) {
    return optimize(problem, maxevals, problem.createSolution());
}

ResultMHInt ESAlgorithm::optimize(Problem<int>& problem, int maxevals, tSolution<int> initial_solution) {

    // Inicialización de parámetros
    const int n = problem.getSolutionSize();

    // Número máximo de vecinos generados y aceptados en cada iteración del bucle interno
    const int max_vecinos = 10 * n;
    const int max_exitos  =  1 * n;

    // Número de enfriamientos: M = maxevals / max_vecinos
    const int M = maxevals / max_vecinos;

    /********************************************************************************/

    // Generación y evaluación de la solución inicial
    tSolution<int> solution = initial_solution;
    if (!problem.isValid(solution)) problem.fix(solution);

    SolutionFactoringInfo<int>* info = problem.generateFactoringInfo(solution);
    int evals = 1;

    tFitness current_fitness = info->fitness;

    // Mejor solución encontrada hasta el momento
    tSolution<int> best_solution = solution;
    tFitness       best_fitness  = current_fitness;

    // Cálculo de T0, Tf y beta
    
    double T0 = (mu * current_fitness) / (-log(phi));
    double Tf = 1e-3;

    // Comprobamos que Tf que sea menor que T0
    if (Tf >= T0) Tf = T0 * 1e-3; // Si T0 es tan pequeña, usamos una fracción de T0

    double beta = (T0 - Tf) / (static_cast<double>(M) * T0 * Tf);

    double T = T0;  // Temperatura actual

    /********************************************************************************/

    // Dominio de la solución
    auto range  = problem.getSolutionDomainRange();
    int  k_min  = range.first;
    int  k_max  = range.second;

    /********************************************************************************/

    // Bucle principal de ES

    while (evals < maxevals) {

        int n_exitos  = 0;
        int n_vecinos = 0;

        // Construimos la lista completa de movimientos posibles (vecindario virtual)
        // y la barajamos para garantizar exploración sin repetición.
        vector<pair<int,int>> moves;
        moves.reserve(n * (k_max - k_min));

        for (int i = 0; i < n; ++i) {
            int current_cluster = solution[i];
            // Solo movimientos factibles (el cluster origen no queda vacío)
            if (info->n_puntos_cluster[current_cluster] > 1) {
                for (int j = k_min; j <= k_max; ++j) {
                    if (j != current_cluster) {
                        moves.push_back({i, j});
                    }
                }
            }
        }

        // Barajamos para escoger vecinos en orden aleatorio sin repetición
        Random::shuffle(moves);

        // Recorremos el vecindario actual
        for (const auto& move : moves) {
            if (evals >= maxevals)          break;
            if (n_vecinos >= max_vecinos)   break;
            if (n_exitos  >= max_exitos)    break;

            int pos = move.first;
            int new_clust = move.second;

            // Evaluamos el vecino usando la info factorizada
            tFitness neighbour_fitness = problem.fitness(solution, info, pos, new_clust);
            evals++;
            n_vecinos++;

            // Variación de fitness: positiva = empeoramiento (minimizamos)
            double delta_f = neighbour_fitness - current_fitness;

            // Criterio de aceptación de Metropolis
            bool accept = false;
            if (delta_f < 0.0) {
                // Mejora: aceptamos siempre
                accept = true;
            } else {
                // Empeoramiento: aceptamos con probabilidad exp(-delta_f / T)
                double prob = std::exp(-delta_f / T);
                accept = (Random::get<double>(0.0, 1.0) <= prob);
            }

            if (accept) {
                // Aplicamos el movimiento y actualizamos la info factorizada
                problem.updateSolutionFactoringInfo(info, solution, pos, new_clust);
                solution[pos]   = new_clust;
                current_fitness = info->fitness;
                n_exitos++;

                // Actualizamos la mejor solución global si procede
                if (current_fitness < best_fitness) {
                    best_fitness  = current_fitness;
                    best_solution = solution;
                }
            }
        }

        //Condición de parada por convergencia: ningún éxito en este enfriamiento
        if (n_exitos == 0) break;

        // Aplicamos el esquema de Cauchy
        T = nextT(beta, T);
    }

    delete info;

    return ResultMHInt(best_solution, best_fitness, evals);
}
