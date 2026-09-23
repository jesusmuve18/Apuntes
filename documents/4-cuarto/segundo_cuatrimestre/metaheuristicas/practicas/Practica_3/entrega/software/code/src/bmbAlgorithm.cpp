#include "bmbAlgorithm.h"

using namespace std;

ResultMH<int> BMBAlgorithm::optimize(Problem<int>& problem, int maxevals) {

    // Instancia de la Búsqueda Local
    LocalSearch bl;

    // Variables para registrar la mejor solución global
    tSolution<int> best_solution;
    tFitness best_fitness = std::numeric_limits<tFitness>::max();
    int total_evals  = 0;

    // Bucle principal BMB:
    for (int t = 0; t < n_arranques; ++t) {

        // Comprobamos que no hemos superado el número máximo de evaluaciones
        if (total_evals >= maxevals) break;

        // Número de evaluaciones restantes para esta ejecución de BL
        int evals_restantes = min(maxevals_bl, maxevals - total_evals);

        // Ejecutamos la Búsqueda Local
        ResultMH<int> result = bl.optimize(problem, evals_restantes);

        total_evals += result.evaluations;

        // Actualizamos la mejor solución global si la solución obtenida es mejor
        if (result.fitness < best_fitness) {
            best_fitness  = result.fitness;
            best_solution = result.solution;
        }
    }

    return ResultMH<int>(best_solution, best_fitness, total_evals);
}
