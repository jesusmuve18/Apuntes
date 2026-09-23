/**
 * @file ageAlgorithm.cpp
 * @brief Implementación del Algoritmo Genético Estacionario (AGE).
 *
 * El AGE es una variante del AG en la que en cada iteración:
 *  1. Se seleccionan dos padres mediante torneo.
 *  2. Se cruzan con la probabilidad inicializada.
 *  3. Se mutan ambos hijos con cierta probabilidad por cromosoma.
 *  4. Los dos hijos compiten contra los dos peores de la población actual;
 *     solo reemplazan si son estrictamente mejores (modelo estacionario).
 */

#include "ageAlgorithm.h"
#include <random>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// MÉTODO DE OPTIMIZACIÓN PRINCIPAL
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Ejecuta el AGE completo hasta agotar evaluaciones.
 *
 * Flujo de cada iteración:
 *  1. Selección: dos padres independientes por torneo binario.
 *  2. Cruce: con el operador configurado (uniforme o SF).
 *  3. Mutación: cada hijo con cierta probabilidad.
 *  4. Reparación: problem.fix() sobre ambos hijos.
 *  5. Evaluación: hijo1 siempre; hijo2 solo si quedan evaluaciones disponibles.
 *  6. Reemplazo estacionario:
 *     - Se localizan los dos peores de la población actual.
 *     - hijo1 reemplaza al peor absoluto si lo mejora.
 *     - Se recalcula el nuevo peor (puede haber cambiado en el paso anterior).
 *     - hijo2 reemplaza al siguiente peor.
 *  7. Actualización del mejor global con cualquier hijo que mejore.
 *
 * @param problem   Referencia al problema PAR.
 * @param maxevals  Número máximo de evaluaciones de la función objetivo.
 * @return          Resultado con la mejor solución, su fitness y el nº de evals.
 */
ResultMHInt AGEAlgorithm::optimize(Problem<int>& problem, int maxevals) {

    // Inicialización de la población
    vector<tSolution<int>> population(populationSize);
    vector<tFitness>       fitnesses(populationSize);
    tSolution<int>         bestSol;
    tFitness               bestFitness;

    

    initializePopulation(problem, population, fitnesses, bestSol, bestFitness);

    // Número de etiquetas de cluster disponibles (valores 0..k-1).
    auto range      = problem.getSolutionDomainRange();
    int  k_clusters = range.second - range.first;

    // Bucle evolutivo estacionario
    while (evals < maxevals) {

        // 1. Selección: dos padres por torneo independientes
        int p1_idx = tournamentSelection(fitnesses);
        int p2_idx = tournamentSelection(fitnesses);

        tSolution<int> hijo1 = population[p1_idx];
        tSolution<int> hijo2 = population[p2_idx];

        // 2. Cruce con esperanza matemática
        if (Random::get<double>(0, 1) < crossoverProb) {
            if (age_type == ageType::UNIFORM) {
                uniformCrossover(population[p1_idx], population[p2_idx],
                                hijo1, hijo2);
            } else {
                fixedSegmentCrossover(population[p1_idx], population[p2_idx],
                                    hijo1, hijo2);
            }
        }

        // 3. Mutación
        if (Random::get<double>(0, 1) < mutationProb)  mutate(hijo1, k_clusters);
        if (Random::get<double>(0, 1) < mutationProb) mutate(hijo2, k_clusters);

        // 4. Reparación
        problem.fix(hijo1);
        problem.fix(hijo2);

        // 5. Evaluación de los hijos
        tFitness fit1 = problem.fitness(hijo1);
        evals++;

        // hijo2 solo se evalúa si aún quedan evaluaciones disponibles.
        tFitness fit2       = std::numeric_limits<double>::max();
        bool     hijo2_eval = false;
        if (evals < maxevals) {
            fit2       = problem.fitness(hijo2);
            evals++;
            hijo2_eval = true;
        }

        // 6. Reemplazo estacionario
        // Los dos hijos compiten contra los dos peores de la población actual.
        //
        // 6.1 Localizar el peor y el segundo peor de la población.
        //     Inicializamos con el menor double representable para que cualquier
        //     fitness real (≥ 0 para el PAR) supere al valor inicial.
        int    worst1_idx = 0;
        int    worst2_idx = 1;
        double max_fit1   = std::numeric_limits<double>::lowest();
        double max_fit2   = std::numeric_limits<double>::lowest();

        for (int i = 0; i < populationSize; ++i) {
            if (fitnesses[i] > max_fit1) {
                // El peor actual pasa a ser el segundo peor.
                max_fit2  = max_fit1;
                worst2_idx = worst1_idx;
                // El nuevo individuo es el peor.
                max_fit1  = fitnesses[i];
                worst1_idx = i;
            } else if (fitnesses[i] > max_fit2) {
                max_fit2  = fitnesses[i];
                worst2_idx = i;
            }
        }

        // 6.2 hijo1 compite con el peor absoluto.
        if (fit1 < fitnesses[worst1_idx]) {
            population[worst1_idx] = hijo1;
            fitnesses[worst1_idx]  = fit1;
        }

        // 6.3 hijo2 compite contra el siguiente peor .
        if (hijo2_eval && fit2 < fitnesses[worst2_idx]) {
            population[worst2_idx] = hijo2;
            fitnesses[worst2_idx]  = fit2;
        }

        // 7. Actualizar el mejor global
        if (fit1 < bestFitness) {
            bestFitness = fit1;
            bestSol     = hijo1;
        }
        // fit2 solo es válido si hijo2 fue evaluado (evita comparar max_double).
        if (hijo2_eval && fit2 < bestFitness) {
            bestFitness = fit2;
            bestSol     = hijo2;
        }
    }

    return ResultMHInt(bestSol, bestFitness, evals);
}
