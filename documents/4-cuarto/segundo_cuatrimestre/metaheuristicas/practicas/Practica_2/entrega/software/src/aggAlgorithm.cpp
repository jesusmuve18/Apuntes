/**
 * @file aggAlgorithm.cpp
 * @brief Implementación del Algoritmo Genético Generacional con elitismo (AGG).
 *
 * El AGG mantiene una población de tamaño M y en cada generación:
 *  1. Calcula el mejor individuo de la población actual (para el elitismo).
 *  2. Selecciona M padres mediante torneo.
 *  3. Cruza pares consecutivos con cierta probabilidad.
 *  4. Muta cada hijo con cierta probabilidad por cromosoma.
 *  5. Evalúa y reemplaza TODA la población con los hijos (modelo generacional).
 *  6. Aplica elitismo: si el mejor hijo es peor que el mejor padre,
 *     el mejor padre sustituye al peor hijo.
 */

#include "aggAlgorithm.h"
#include <random>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// AVANCE DE UNA GENERACIÓN
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Ejecuta una generación completa del AGG con elitismo.
 *
 * Flujo detallado:
 *  1. Antes de generar ningún hijo, se recorre la población actual para
 *     localizar su mejor individuo (best_old_idx).  Este recorrido previo
 *     es fundamental: si se calculase de forma incremental dentro del bucle
 *     evolutivo y el presupuesto se agotase a mitad de generación, se
 *     obtendría un índice que solo cubriría parte de la población, con el
 *     riesgo de perder el mejor individuo en el paso de elitismo.
 *
 *  2. Para cada par de posiciones (i, i+1) de la nueva población:
 *     2.1. Se seleccionan dos padres por torneo (de forma independiente).
 *     2.2. Con cierta probabilidad se aplica el operador de cruce
 *          seleccionado (uniforme o segmento fijo); si no cruza, los hijos
 *          son copias de los padres seleccionados.
 *     2.3. Se muta cada hijo con cierta probabilidad.
 *     2.4. Se repara y evalúa cada hijo; se actualiza el mejor global.
 *
 *  3. Elitismo (al final de la generación, solo si quedan evaluaciones):
 *     - Si el mejor individuo de la generación anterior supera al mejor
 *       de la nueva población, sustituye al peor de la nueva.
 *
 * @param problem     Referencia al problema PAR.
 * @param population  Población actual (se sustituye al final con la nueva).
 * @param fitnesses   Fitness actuales (se sustituyen al final).
 * @param bestSol     [in/out] Mejor solución global encontrada hasta ahora.
 * @param bestFit     [in/out] Fitness de la mejor solución global.
 * @param k_clusters  Número de clusters (necesario para el operador de mutación).
 * @param maxevals    Presupuesto máximo de evaluaciones.
 */
void AGGAlgorithm::evolveOneGeneration(Problem<int>& problem,
                                       vector<tSolution<int>>& population,
                                       vector<tFitness>& fitnesses,
                                       tSolution<int>& bestSol,
                                       tFitness& bestFit,
                                       int k_clusters,
                                       int maxevals) {

    // 1. Localizar el mejor individuo de la población actual
    int best_old_idx = 0;
    for (int i = 1; i < populationSize; ++i) {
        if (fitnesses[i] < fitnesses[best_old_idx]) {
            best_old_idx = i;
        }
    }

    // 2. Generar la nueva población (M hijos, de dos en dos)
    vector<tSolution<int>> new_population(populationSize);
    vector<tFitness>       new_fitnesses(populationSize);

    // Establecemos los hijos que van a mutar según la esperanza
    std::vector<bool> muta_hijo(populationSize, false);
    for(int i=0; i<expected_mutations; i++) muta_hijo[i] = true;
    Random::shuffle(muta_hijo);

    for (int i = 0; i < populationSize && evals < maxevals; i += 2) {

        // 2.1. Selección de dos padres por torneo
        int p1_idx = tournamentSelection(fitnesses);
        int p2_idx = tournamentSelection(fitnesses);

        // Los hijos parten como copias de los padres seleccionados.
        tSolution<int> hijo1 = population[p1_idx];
        tSolution<int> hijo2 = population[p2_idx];

        // 2.2. Cruce con esperanza matemática
        if ((i/2) < expected_crossovers) {
            if (agg_type == aggType::UNIFORM) {
                uniformCrossover(population[p1_idx], population[p2_idx],
                                 hijo1, hijo2);
            } else {
                fixedSegmentCrossover(population[p1_idx], population[p2_idx],
                                      hijo1, hijo2);
            }
        }

        // 2.3. y 2.4. Mutación, reparación, evaluación del hijo 1
        if(muta_hijo[i]) mutate(hijo1, k_clusters);
        problem.fix(hijo1);
        new_fitnesses[i] = problem.fitness(hijo1);
        evals++;
        new_population[i] = hijo1;

        if (new_fitnesses[i] < bestFit) {
            bestFit = new_fitnesses[i];
            bestSol = hijo1;
        }

        // Si se agotan las evaluaciones cedo el control
        if (evals >= maxevals) return; 

        // 2.3. y 2.4. Mutación, reparación, evaluación del hijo 2
        if (i + 1 < populationSize) {
            if(muta_hijo[i+1]) mutate(hijo2, k_clusters);
            problem.fix(hijo2);
            new_fitnesses[i + 1] = problem.fitness(hijo2);
            evals++;
            new_population[i + 1] = hijo2;

            if (new_fitnesses[i + 1] < bestFit) {
                bestFit = new_fitnesses[i + 1];
                bestSol = hijo2;
            }
            
            // Si se agotan las evaluaciones cedo el control
            if (evals >= maxevals) return;
        }
    }

    // 3. Elitismo y reemplazo de la población
    
    // Localizar el mejor y el peor de la nueva generación.
    int best_new_idx  = 0;
    int worst_new_idx = 0;

    for (int i = 1; i < populationSize; ++i) {
        if (new_fitnesses[i] < new_fitnesses[best_new_idx])  best_new_idx  = i;
        if (new_fitnesses[i] > new_fitnesses[worst_new_idx]) worst_new_idx = i;
    }

    // Si el mejor de la generación anterior supera al mejor hijo,
    // reemplaza al peor hijo (elitismo: no se pierde la mejor solución).
    if (fitnesses[best_old_idx] < new_fitnesses[best_new_idx]) {
        new_population[worst_new_idx] = population[best_old_idx];
        new_fitnesses[worst_new_idx]  = fitnesses[best_old_idx];
    }

    // La nueva generación pasa a ser la población actual.
    population = new_population;
    fitnesses  = new_fitnesses;
}

// ─────────────────────────────────────────────────────────────────────────────
// MÉTODO DE OPTIMIZACIÓN PRINCIPAL
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Ejecuta el AGG completo hasta agotar evaluaciones disponibles.
 *
 * Flujo:
 *  1. Inicializar la población aleatoriamente y evaluar todos los cromosomas.
 *  2. Iterar generaciones (evolveOneGeneration) hasta evals >= maxevals.
 *  3. Devolver la mejor solución encontrada a lo largo de toda la ejecución.
 *
 * @param problem   Referencia al problema PAR.
 * @param maxevals  Número máximo de evaluaciones de la función objetivo.
 * @return          Resultado con la mejor solución, su fitness y el nº de evals.
 */
ResultMHInt AGGAlgorithm::optimize(Problem<int>& problem, int maxevals) {
    vector<tSolution<int>> population(populationSize);
    vector<tFitness>       fitnesses(populationSize);
    tSolution<int>         bestSol;
    tFitness               bestFit;

    // 1. Inicialización de la población.
    initializePopulation(problem, population, fitnesses, bestSol, bestFit);

    // Número de etiquetas de cluster disponibles (valores 0..k-1).
    auto range      = problem.getSolutionDomainRange();
    int  k_clusters = range.second - range.first;

    // 2. Bucle evolutivo generacional hasta agotar evaluaciones.
    while (evals < maxevals) {
        evolveOneGeneration(problem, population, fitnesses,
                            bestSol, bestFit, k_clusters, maxevals);
    }

    // 3. Devolver mejor solución
    return ResultMHInt(bestSol, bestFit, evals);
}
