#include <cassert>
#include <greedy.h>
#include <clusteringPloblem.h>
#include <iostream>
#include <random>
#include <numeric>
#include <limits>

using namespace std;

// g++ tiempos.cc fact.cc -pg -lm -o tiempos
// gprof ./tiempos | less
// g++ tiempos.cc fact.cc -O2 -lm -o tiempos
// g++ tiempos.cc fact.cc -g -lm -o tiempos

template <class T>
void print_vector(string name, const vector<T> &sol)
{
    cout << name << ": ";

    for (auto elem : sol)
    {
        cout << elem << ", ";
    }
    cout << endl;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMHInt GreedySearch::optimize(ProblemInt &problem, int maxevals)
{
    // Adaptamos el código a nuestro problema
    ClusteringProblem & cproblem = dynamic_cast<ClusteringProblem &>(problem);

    // Inicializamos los parámetros necesarios
    int n = cproblem.getSolutionSize();
    auto range = cproblem.getSolutionDomainRange();
    int k_min = range.first;
    int k_max = range.second;
    int k = k_max - k_min + 1;
    tFitness infinity = numeric_limits<double>::infinity();

    // Comenzamos generando los centroides iniciales
    int dim = cproblem.getPoint(0).coordinate.size();
    vector<double> min_dim(dim, numeric_limits<double>::max());
    vector<double> max_dim(dim, numeric_limits<double>::lowest());

    // Buscamos los límites del dominio
    for (int i = 0; i < n; i++) {
        Point p = cproblem.getPoint(i);
        for (int d = 0; d < dim; d++) {
            if (p.coordinate[d] < min_dim[d]) min_dim[d] = p.coordinate[d];
            if (p.coordinate[d] > max_dim[d]) max_dim[d] = p.coordinate[d];
        }
    }

    // Generamos las coordenadas aleatorias para los k centroides
    vector<Point> initial_centroids(k);
    for (int c = 0; c < k; c++) {
        initial_centroids[c].coordinate.resize(dim);
        for (int d = 0; d < dim; d++) {
            initial_centroids[c].coordinate[d] = Random::get<double>(min_dim[d], max_dim[d]);
        }
    }

    // Generamos una solución aleatoria
    tSolution<int> solution = cproblem.createSolution();
    if (!cproblem.isValid(solution)) cproblem.fix(solution);

    // Generamos el Factoring Info
    SolutionFactoringInfo<int> *info = cproblem.generateFactoringInfo(solution);
    int evals = 1;
    
    // Sobreescribimos lo centroides
    info->centroids = initial_centroids;

    // Generamos y barajamos los índices
    std::vector<int> RSI(n);
    std::iota(RSI.begin(), RSI.end(), 0);
    Random::shuffle(RSI);

    bool changed;

    do
    {
        changed = false;

        for (int i : RSI)
        {
            int currentCluster = solution[i];
            int bestCluster = currentCluster;
            int bestInfeasibility = std::numeric_limits<int>::max(); 
            double bestDistanceToCentroid = std::numeric_limits<double>::max();

            // Evaluar todos los posibles clústeres para el punto i
            for (int j = k_min; j <= k_max; j++)
            {
                int inf = cproblem.infeasibility(solution, info, i, j);
                double d = ClusteringProblem::distance(cproblem.getPoint(i), info->centroids[j]);

                if (inf < bestInfeasibility)
                {
                    bestInfeasibility = inf;
                    bestDistanceToCentroid = d;
                    bestCluster = j;
                } else if (inf == bestInfeasibility) 
                {
                    if (d < bestDistanceToCentroid)
                    {
                        bestDistanceToCentroid = d;
                        bestCluster = j;
                    }
                }
            }

            // Si hay un clúster mejor para este punto
            if (bestCluster != currentCluster)
            {
                solution[i] = bestCluster;
                info->infeasibility = bestInfeasibility; 
                changed = true;
            }
        }

        // Actualizamos la información
        if (changed) {
            // Re-generar la info desde cero actualizará de golpe las medias de todos los centroides
            SolutionFactoringInfo<int> *new_info = cproblem.generateFactoringInfo(solution);
            delete info;
            info = new_info;
        }

    } while (changed);

    // Guardo el valor a devolver
    tFitness current_fitness = info->fitness;
    delete info;

    return ResultMHInt(solution, current_fitness, evals);
}

    // do
    // {
    //     evals++;
    //     changed = false;

    //     for (int i : RSI)
    //     {
    //         int currentCluster = solution[i];
    //         int bestCluster = currentCluster;
    //         int bestInfeasibility = info->infeasibility; 
    //         double bestDistanceToCentroid = ClusteringProblem::distance(cproblem.getPoint(i), info->centroids[currentCluster]);

    //         // Evaluar todos los posibles clústeres para el punto i
    //         for (int j = k_min; j <= k_max; j++)
    //         {
    //             if (j == currentCluster) continue;

    //             int inf = cproblem.infeasibility(solution, info, i, j);

    //             // Si encontramos una infactibilidad menor
    //             if (inf < bestInfeasibility)
    //             {
    //                 bestInfeasibility = inf;
    //                 bestDistanceToCentroid = ClusteringProblem::distance(cproblem.getPoint(i), info->centroids[j]);
    //                 bestCluster = j;
    //             }

    //             // Si empatamos en infactibilidad, desempatamos por distancia al centroide
    //             else if (inf == bestInfeasibility) 
    //             {
    //                 double d = ClusteringProblem::distance(cproblem.getPoint(i), info->centroids[j]);
    //                 if (d < bestDistanceToCentroid)
    //                 {
    //                     bestDistanceToCentroid = d;
    //                     bestCluster = j;
    //                 }
    //             }
    //         }

    //         // Si se ha encontrado una mejora de la solución anterior
    //         if (bestCluster != currentCluster)
    //         {
    //             cproblem.updateSolutionFactoringInfo(info, solution, i, bestCluster);
    //             solution[i] = bestCluster;
    //             changed = true;
    //         }
    //     }

    // } while (changed && evals < maxevals);

    // // Guardo el valor a devolver
    // current_fitness = info->fitness;
    // delete info;

    // // Devolvemos el resultado empaquetado
    // return ResultMHInt(solution, current_fitness, evals);
// }