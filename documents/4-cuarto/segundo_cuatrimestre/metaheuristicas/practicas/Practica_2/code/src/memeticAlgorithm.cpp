/**
 * @file memeticAlgorithm.cpp
 * @brief Implementación de los Algoritmos Meméticos (AM-All, AM-Rand, AM-Best).
 *
 * Los AMs hibridan el mejor AGG con una Búsqueda Local Suave (BLS).
 * La BLS se aplica cada `n_gens_to_apply_bls` generaciones sobre
 * un subconjunto de cromosomas determinado por la variante:
 *
 *  - AM-All:  aplica BLS sobre todos los cromosomas de la población.
 *  - AM-Rand: aplica BLS sobre cada cromosoma con probabilidad P_LS.
 *  - AM-Best: aplica BLS sobre los mejores P_BEST·M cromosomas.
 */

#include "memeticAlgorithm.h"
#include <random>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// MUTACIÓN POR GEN (sobreescribe GeneticAlgorithm::mutate para los AM)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Operador de mutación uniforme por gen para los Algoritmos Meméticos.
 *
 * Implementación:
 *   Cada gen del cromosoma muta independientemente con probabilidad
 *   probMutacion / n, donde n = sol.size().
 *   Con probMutacion (valor heredado del AGG), la probabilidad por gen
 *   es probMutacion/n y la esperanza de mutaciones por cromosoma es exactamente probMutacion,
 *   la misma que en AGG/AGE, pero con semántica per-gen (varios genes pueden
 *   mutar en un mismo cromosoma).
 *
 * @param sol          Solución (cromosoma) a mutar, modificada in-place.
 * @param k_clusters   Número de clusters disponibles (valores 0..k-1).
 * @param probMutacion Probabilidad de referencia del cromosoma.
 *                     La probabilidad por gen se calcula como probMutacion/n.
 */
void MemeticAlgorithm::mutate(tSolution<int>& sol, int k_clusters) {
    int    n          = sol.size();

    for (int i = 0; i < n; ++i) {
        if (Random::get<double>(0, 1) < expected_mutations_per_gen) {
            int old_val = sol[i];
            int new_val;
            // El nuevo valor debe ser distinto del actual.
            do {
                new_val = Random::get<int>(0, k_clusters - 1);
            } while (new_val == old_val);
            sol[i] = new_val;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// BÚSQUEDA LOCAL SUAVE (BLS)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Búsqueda Local Suave.
 *
 * Dado un cromosoma, recorre sus posiciones en orden aleatorio (RSI).
 * Para cada posición p:
 *   1. Si p es el único elemento de su cluster actual (oldvalue), mover p
 *      a cualquier otro cluster lo dejaría vacío, lo que viola las
 *      restricciones del problema.  En ese caso se cuenta como fallo y
 *      se pasa a la siguiente posición sin evaluar ningún vecino.
 *   2. En caso contrario, se prueban todos los valores de cluster posibles
 *      (excepto el actual) y se acepta el primero que mejore el fitness
 *      (estrategia first-improvement).
 *
 * Condiciones de parada (la primera que se alcance):
 *  - fallos >= ε : se han acumulado ε posiciones sin mejora (o inválidas).
 *  - i >= n      : se han visitado todas las posiciones del cromosoma.
 *  - bls_evals >= maxevals      : se ha alcanzado el límite LOCAL de evals.
 *  - evals >= maxevals_global   : se ha agotado el presupuesto GLOBAL.
 *
 *
 * @param problem          Referencia al problema PAR.
 * @param sol              [in/out] Solución a mejorar (modificada in-place).
 * @param bestfit          [in/out] Fitness de `sol` (actualizado si mejora).
 * @param maxevals         Límite de evaluaciones para ESTA llamada a BLS.
 * @param maxevals_global  Límite global de evaluaciones del algoritmo.
 * @param e                Número máximo de fallos permitidos.
 * @return                 Resultado con la solución, el fitness y las evals usadas.
 */
ResultMHInt MemeticAlgorithm::softLocalSearch(ProblemInt& problem,
                                               tSolution<int>& sol,
                                               tFitness& bestfit,
                                               int maxevals,
                                               int maxevals_global,
                                               int e) {
    int n = sol.size();
    auto range  = problem.getSolutionDomainRange();
    int  k_min  = range.first;   
    int  k_max  = range.second;  
    int  bls_evals = 0;

    // Generar un orden aleatorio de posiciones.
    std::vector<int> RSI(n);
    std::iota(RSI.begin(), RSI.end(), 0);
    Random::shuffle(RSI);

    int fallos = 0;
    int i      = 0;
    // newsol es la copia de trabajo sobre la que se prueban los vecinos.
    tSolution<int> newsol = sol;

    // Bucle principal de la BLS
    while ((fallos < e) && (i < n) && (bls_evals < maxevals) && (evals < maxevals_global)) {

        int p        = RSI[i];          // posición a explorar en esta iteración
        int oldvalue = sol[p];          // valor actual de esa posición

        // Probar todos los clusters alternativos
        bool no_more_evals = false;

        for (int val = k_min; val < k_max; ++val) {
            if (val == oldvalue) continue;   // no evaluar el valor ya asignado

            // Verificar evaluaciones antes de evaluar.
            if (bls_evals >= maxevals || evals >= maxevals_global) {
                no_more_evals = true;
                break;
            }

            newsol[p] = val;
            tFitness fit = problem.fitness(newsol);
            bls_evals++;
            evals++;   // Las evaluaciones de BLS descuentan del contador global.

            if (fit < bestfit) {
                // Mejora encontrada: actualizar solución y fitness.
                sol     = newsol;
                bestfit = fit;
            } else {
                // Sin mejora: deshacer el cambio en newsol.
                newsol[p] = sol[p];
            }
        }
       
        if (!no_more_evals && sol[p] == oldvalue) {
            fallos++;
        }

        i++;
    }

    return ResultMHInt(sol, bestfit, bls_evals);
}

// ─────────────────────────────────────────────────────────────────────────────
// MÉTODO DE OPTIMIZACIÓN PRINCIPAL
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Ejecuta el Algoritmo Memético.
 *
 * Estructura principal:
 *  1. Inicializar población (heredado del AGG).
 *  2. En cada generación, avanzar una generación del AGG (evolveOneGeneration).
 *  3. Cada `n_gens_to_apply_bls` generaciones, aplicar la BLS según la variante:
 *     - AM-All:  sobre todos los cromosomas.
 *     - AM-Rand: sobre cada cromosoma con probabilidad P_LS.
 *     - AM-Best: sobre los mejores P_BEST·M cromosomas.
 *  4. Devolver la mejor solución global encontrada.
 *
 * Las evaluaciones de la BLS se descuentan del presupuesto global,
 * por lo que el número real de generaciones del AGG es menor que en un AGG puro.
 *
 * @param problem   Referencia al problema PAR.
 * @param maxevals  Presupuesto máximo de evaluaciones.
 * @return          Resultado con la mejor solución, fitness y nº de evals.
 */
ResultMHInt MemeticAlgorithm::optimize(Problem<int>& problem, int maxevals) {

    // 1. Inicialización
    vector<tSolution<int>> population(populationSize);
    vector<tFitness>       fitnesses(populationSize);
    tSolution<int>         bestSol;
    tFitness               bestFit;

    initializePopulation(problem, population, fitnesses, bestSol, bestFit);

    int n = population[0].size();
    expected_mutations_per_gen = mutationProb / n;

    auto range      = problem.getSolutionDomainRange();
    int  k_clusters = range.second - range.first;

    int generation = 0;  // contador de generaciones (controla cuándo aplicar BLS)

    // Bucle evolutivo
    while (evals < maxevals) {

        //  2. Paso evolutivo: una generación del AGG con elitismo 
        evolveOneGeneration(problem, population, fitnesses, bestSol, bestFit, k_clusters, maxevals);
        generation++;

        // 3. Paso memético: aplicar BLS cada n_gens_to_apply_bls generaciones
        if (generation % n_gens_to_apply_bls == 0 && evals < maxevals) {

            if (am_type == amType::ALL) {
                // AM-All
                for (int i = 0; i < populationSize && evals < maxevals; ++i) {
                    softLocalSearch(problem, population[i], fitnesses[i],
                                    maxevals_bls, maxevals, max_bls_failures);
                    if (fitnesses[i] < bestFit) {
                        bestFit = fitnesses[i];
                        bestSol = population[i];
                    }
                }
            }
            else if (am_type == amType::RANDOM) {
                // AM_Random
                for (int i = 0; i < populationSize && evals < maxevals; ++i) {
                    if (Random::get<double>(0, 1) < P_LS) {
                        softLocalSearch(problem, population[i], fitnesses[i],
                                        maxevals_bls, maxevals, max_bls_failures);
                        if (fitnesses[i] < bestFit) {
                            bestFit = fitnesses[i];
                            bestSol = population[i];
                        }
                    }
                }
            }
            else if (am_type == amType::BEST) {
                // AM_Best
                vector<pair<double, int>> fit_idx(populationSize);
                for (int i = 0; i < populationSize; ++i) {
                    fit_idx[i] = {fitnesses[i], i};
                }
                // Orden ascendente: el índice 0 es el de menor fitness (mejor).
                std::sort(fit_idx.begin(), fit_idx.end());

                // Número de cromosomas sobre los que se aplica BLS (al menos 1).
                int limit = std::max(1, static_cast<int>(populationSize * P_BEST));

                for (int i = 0; i < limit && evals < maxevals; ++i) {
                    int idx = fit_idx[i].second;  // índice real en la población
                    softLocalSearch(problem, population[idx], fitnesses[idx],
                                    maxevals_bls, maxevals, max_bls_failures);
                    if (fitnesses[idx] < bestFit) {
                        bestFit = fitnesses[idx];
                        bestSol = population[idx];
                    }
                }
            }
        }
    }

    return ResultMHInt(bestSol, bestFit, evals);
}
