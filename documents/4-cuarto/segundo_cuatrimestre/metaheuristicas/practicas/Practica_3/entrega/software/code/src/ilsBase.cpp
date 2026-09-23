#include "ilsBase.h"
#include <cmath>
#include <vector>

using namespace std;

void ILSBase::mutate(tSolution<int>& solution, int k_min, int k_max, Problem<int>& problem) {
    int n = static_cast<int>(solution.size());

    // Número de genes a mutar
    int n_mutar = static_cast<int>(std::round(p_mutacion * n));
    if (n_mutar < 1) n_mutar = 1;

    // Generamos una permutación aleatoria de índices y tomamos los primeros n_mutar
    vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    Random::shuffle(indices);

    int k_range = k_max - k_min + 1; // Número de clusters disponibles

    for (int t = 0; t < n_mutar; ++t) {
        int pos = indices[t];
        int cur_cluster = solution[pos];

        // Elegimos un cluster distinto al actual de forma aleatoria
        int new_cluster;
        do {
            new_cluster = Random::get<int>(k_min, k_max);
        } while (new_cluster == cur_cluster && k_range > 1);

        solution[pos] = new_cluster;
    }

    // Reparamos la solución si algún cluster ha quedado vacío tras la mutación
    if (!problem.isValid(solution)) problem.fix(solution);
}

ResultMH<int> ILSBase::optimize(Problem<int>& problem, int maxevals) {
    // Inicialización de parámetros

    // Rango del dominio de la solución
    auto range = problem.getSolutionDomainRange();
    int  k_min = range.first;
    int  k_max = range.second;

    int total_evals = 0;

    // Generamos solución inicial aleatoria y aplicamos BL
    int evals_restantes = min(maxevals_algorithm, maxevals - total_evals);
    ResultMH<int> result_inicial = optimizeAlgorithm(problem, evals_restantes);   // Genera la propia solución inicial aleatoria
    total_evals += result_inicial.evaluations;

    // La mejor solución conocida se inicializa con el resultado de la primera BL
    tSolution<int> best_solution = result_inicial.solution;
    tFitness       best_fitness  = result_inicial.fitness;

    /********************************************************************************/
    // Mutamos la mejor solución y aplicamos BL sobre la mutación
    for (int t = 1; t < n_arranques; ++t) {

        if (total_evals >= maxevals) break;

        // Mutamos la mejor solución encontrada hasta ahora
        tSolution<int> mutated = best_solution;
        mutate(mutated, k_min, k_max, problem);

        // Aplicamos BL sobre la solución mutada
        evals_restantes = min(maxevals_algorithm, maxevals - total_evals);
        ResultMH<int> result_bl = optimizeAlgorithm(problem, evals_restantes, mutated);
        
        total_evals += result_bl.evaluations;

        // Actualizamos la mejor solución si la nueva es mejor
        if (result_bl.fitness < best_fitness) {
            best_fitness  = result_bl.fitness;
            best_solution = result_bl.solution;
        }
    }

    return ResultMH<int>(best_solution, best_fitness, total_evals);
}