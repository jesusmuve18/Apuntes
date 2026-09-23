#include <cassert>
#include <localsearch.h>
#include <clusteringPloblem.h>
#include <iostream>
#include <random>

using namespace std;

ResultMHInt LocalSearch::optimize(Problem<int>& problem, int maxevals) {
    return optimize(problem, maxevals, problem.createSolution());
}

ResultMHInt LocalSearch::optimize(Problem<int>& problem, int maxevals, tSolution<int> initial_solution) {
    
    // Inicializamos los parámetros necesarios
    int n = problem.getSolutionSize();
    auto range = problem.getSolutionDomainRange();
    int k_min = range.first;
    int k_max = range.second;

    // Inicializamos con la solucion inicial
    tSolution<int> solution = initial_solution;
    
    // Nos aseguramos de que es válida
    if(!problem.isValid(solution)) problem.fix(solution); 

    SolutionFactoringInfo<int>* info = problem.generateFactoringInfo(solution);
    int evals = 1;

    // Evaluamos la solución inicial
    tFitness best_fitness = info->fitness;
    
    bool improvement = true;

    // Creamos el vecindario virtual
    vector<Move> neighborhood;
    // Reservamos memoria para el máximo de vecinos posibles: N * (K-1)
    neighborhood.reserve(n * (k_max - k_min)); 

    // Bucle principal de la Búsqueda Local
    while (improvement && evals < maxevals) {
        improvement = false;
        
        // Generamos el vecindario actual
        neighborhood.clear();
        for (int i = 0; i < n; ++i) {
            int currentCluster = solution[i];
            
            // Sólo guardamos los factibles
            if (info->n_puntos_cluster[currentCluster] > 1) {
                for (int j = k_min; j <= k_max; ++j) {
                    if (j != currentCluster) {
                        neighborhood.push_back({i, j});
                    }
                }
            }
        }

        // Barajamos el vecindario actual
        Random::shuffle(neighborhood);

        // Exploramos el vecindario
        for (const Move& move : neighborhood) {
            if (evals >= maxevals) break;

            int i = move.point;
            int j = move.new_cluster;

            // Evaluamos el vecino
            tFitness neighbour_fitness = problem.fitness(solution, info, i, j);
            evals++;

            // Si encontramos un fitness mejor
            if (neighbour_fitness < best_fitness) {
                // Aplicamos el movimiento inmediatamente
                problem.updateSolutionFactoringInfo(info, solution, i, j);
                solution[i] = j;
                best_fitness = neighbour_fitness;
                improvement = true;
                
                // Detenemos la exploración de este vecindario
                break; 
            }
        }
    }

    // Liberar memoria
    delete info;

    return ResultMHInt(solution, best_fitness, evals);
}