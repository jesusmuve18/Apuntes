/**
 * @file geneticAlgorithm.cpp
 * @brief Implementación de los operadores comunes a todos los Algoritmos Genéticos
 *        (AGG y AGE) para el Problema de Agrupamiento con Restricciones (PAR).
 *
 * Contiene:
 *  - initializePopulation : generación aleatoria de la población inicial.
 *  - tournamentSelection  : selección por torneo de tamaño k .
 *  - uniformCrossover     : cruce uniforme .
 *  - fixedSegmentCrossover: cruce por segmento fijo.
 *  - mutate               : mutación uniforme POR CROMOSOMA para AGG y AGE.
 *                           Para los AM, MemeticAlgorithm sobreescribe este método
 *                           con mutación por gen
 */

#include "geneticAlgorithm.h"
#include "clusteringPloblem.h"
#include <random>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// INICIALIZACIÓN DE POBLACIÓN
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Genera aleatoriamente la población inicial y evalúa cada individuo.
 *
 * Para cada cromosoma:
 *  1. Se genera una solución aleatoria con problem.createSolution()
 *     (garantiza que cada cluster tiene al menos un elemento asignado).
 *  2. Se repara con problem.fix() por si la generación dejó algún cluster vacío.
 *  3. Se calcula el fitness y se incrementa el contador de evaluaciones.
 *  4. Se actualiza el mejor global si procede.
 *
 * @param problem   Referencia al problema PAR.
 * @param pop       Vector de soluciones (cromosomas) a inicializar.
 * @param fits      Vector de fitness correspondiente a cada cromosoma.
 * @param bestSol   [out] Mejor solución encontrada en la población inicial.
 * @param bestFit   [out] Fitness de la mejor solución inicial.
 */
void GeneticAlgorithm::initializePopulation(ProblemInt& problem,
                                            vector<tSolution<int>>& pop,
                                            vector<tFitness>& fits,
                                            tSolution<int>& bestSol,
                                            tFitness& bestFit) {
    bestFit = std::numeric_limits<double>::max();

    for (int i = 0; i < populationSize; ++i) {
        pop[i] = problem.createSolution();
        problem.fix(pop[i]);
        fits[i] = problem.fitness(pop[i]);
        this->evals++;

        if (fits[i] < bestFit) {
            bestFit = fits[i];
            bestSol = pop[i];
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// SELECCIÓN POR TORNEO
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Selección mediante torneo de tamaño k.
 *
 * Algoritmo:
 *  1. Elegir el primer contendiente al azar de [0, populationSize-1].
 *  2. Repetir k-1 veces: elegir otro individuo al azar y conservar el de
 *     MENOR fitness (estamos minimizando la función objetivo).
 *
 * @param fitnesses Vector de fitness de la población actual.
 * @param k         Tamaño del torneo.
 * @return          Índice del individuo ganador en la población.
 */
int GeneticAlgorithm::tournamentSelection(const std::vector<tFitness>& fitnesses,
                                          int k) {
    int best_idx = Random::get<int>(0, populationSize - 1);

    for (int i = 1; i < k; ++i) {
        int candidate = Random::get<int>(0, populationSize - 1);
        // Minimización: nos quedamos con el individuo de menor fitness.
        if (fitnesses[candidate] < fitnesses[best_idx]) {
            best_idx = candidate;
        }
    }
    return best_idx;
}

// ─────────────────────────────────────────────────────────────────────────────
// CRUCE UNIFORME
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Operador de cruce uniforme.
 *
 * Para cada posición i del cromosoma:
 *   - Con probabilidad 0.5: c1[i] = p1[i], c2[i] = p2[i]
 *   - Con probabilidad 0.5: c1[i] = p2[i], c2[i] = p1[i]
 *
 *
 * @param p1  Primer padre.
 * @param p2  Segundo padre.
 * @param c1  [out] Primer hijo resultante.
 * @param c2  [out] Segundo hijo resultante.
 */
void GeneticAlgorithm::uniformCrossover(const tSolution<int>& p1,
                                        const tSolution<int>& p2,
                                        tSolution<int>& c1,
                                        tSolution<int>& c2) {
    int n = p1.size();
    c1.resize(n);
    c2.resize(n);

    for (int i = 0; i < n; ++i) {
        if (Random::get<double>(0, 1) < 0.5) {
            c1[i] = p1[i];
            c2[i] = p2[i];
        } else {
            c1[i] = p2[i];
            c2[i] = p1[i];
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// CRUCE POR SEGMENTO FIJO
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Operador de cruce por segmento fijo.
 *
 * Procedimiento:
 *  1. Generar el inicio del segmento `start` ∈ [0, N-1] y la longitud
 *     FN ∈ [1, N/3].  El segmento recorre posiciones de forma CIRCULAR,
 *     es decir, las posiciones del segmento son
 *       { (start + j) % N  |  j = 0, 1, ..., FN-1 }.
 *  2. Para las posiciones DENTRO del segmento (inSegment[i] = true):
 *       c1[i] = p1[i]  (c1 hereda el segmento de p1 → padre portador de c1)
 *       c2[i] = p2[i]  (c2 hereda el segmento de p2 → padre portador de c2)
 *     Cada hijo usa un padre portador distinto.
 *  3. Para las posiciones FUERA del segmento → cruce uniforme:
 *       Con r() < 0.5: c1[i] = p1[i], c2[i] = p2[i]
 *       Con r() ≥ 0.5: c1[i] = p2[i], c2[i] = p1[i]
 *
 *
 * @param p1  Primer padre.
 * @param p2  Segundo padre.
 * @param c1  [out] Primer hijo (segmento tomado de p1).
 * @param c2  [out] Segundo hijo (segmento tomado de p2).
 */
void GeneticAlgorithm::fixedSegmentCrossover(const tSolution<int>& p1,
                                             const tSolution<int>& p2,
                                             tSolution<int>& c1,
                                             tSolution<int>& c2) {
    int n = p1.size();
    c1.resize(n);
    c2.resize(n);

    // 1. Longitud y posición de inicio del segmento
    // FN ∈ [1, N/3], inicio ∈ [0, N-1]
    int length = Random::get<int>(1, n / 3);
    int start  = Random::get<int>(0, n - 1);

    // Marcar las posiciones del segmento (recorrido circular módulo N).
    std::vector<bool> inSegment(n, false);
    for (int j = 0; j < length; ++j) {
        inSegment[(start + j) % n] = true;
    }

    // 2. y 3. Asignación gen a gen
    for (int i = 0; i < n; ++i) {
        if (inSegment[i]) {
            // Dentro del segmento: c1 hereda de p1, c2 hereda de p2.
            // Cada hijo usa un padre portador diferente.
            c1[i] = p1[i];
            c2[i] = p2[i];
        } else {
            // Fuera del segmento: cruce uniforme.
            if (Random::get<double>(0, 1) < 0.5) {
                c1[i] = p1[i];
                c2[i] = p2[i];
            } else {
                c1[i] = p2[i];
                c2[i] = p1[i];
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// MUTACIÓN UNIFORME (AGG y AGE) — PROBABILIDAD POR CROMOSOMA
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Operador de mutación uniforme para AGG y AGE.
 *
 * Semántica: probabilidad `probMutacion` de que el cromosoma completo sufra
 * exactamente UNA mutación de gen.
 *
 * Implementación:
 *   Con probabilidad `probMutacion` se elige aleatoriamente un gen y se le
 *   asigna un valor de cluster distinto al actual.
 *
 * @param sol          Solución (cromosoma) a mutar, modificada in-place.
 * @param k_clusters   Número de clusters disponibles (valores 0..k-1).
 */
void GeneticAlgorithm::mutate(tSolution<int>& sol, int k_clusters) {

    // Seleccionar aleatoriamente el gen que muta.
    int n = sol.size();
    int gen_idx = Random::get<int>(0, n - 1);

    int old_val = sol[gen_idx];
    int new_val;

    // El nuevo valor debe ser distinto del actual.
    do {
        new_val = Random::get<int>(0, k_clusters - 1);
    } while (new_val == old_val);

    sol[gen_idx] = new_val;
}
