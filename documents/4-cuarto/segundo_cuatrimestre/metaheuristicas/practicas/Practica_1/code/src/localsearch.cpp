#include <cassert>
#include <localsearch.h>
#include <clusteringPloblem.h>
#include <iostream>
#include <random>

using namespace std;

ResultMHInt LocalSearch::optimize(Problem<int>& problem, int maxevals) {
    // Adamptamos el código a nuestro problema
    ClusteringProblem& cproblem = dynamic_cast<ClusteringProblem&>(problem);

    // Inicializamos los parámetros necesarios
    int n = cproblem.getSolutionSize();
    auto range = cproblem.getSolutionDomainRange();
    int k_min = range.first;
    int k_max = range.second;

    // Creamos la solución inicial
    tSolution<int> solution = cproblem.createSolution();
    
    // Nos aseguramos de que es válida
    if(!cproblem.isValid(solution)) cproblem.fix(solution); 

    SolutionFactoringInfo<int>* info = cproblem.generateFactoringInfo(solution);
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
            tFitness neighbor_fitness = cproblem.fitness(solution, info, i, j);
            evals++;

            // Si encontramos un fitness mejor
            if (neighbor_fitness < best_fitness) {
                // Aplicamos el movimiento inmediatamente
                cproblem.updateSolutionFactoringInfo(info, solution, i, j);
                solution[i] = j;
                best_fitness = neighbor_fitness;
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