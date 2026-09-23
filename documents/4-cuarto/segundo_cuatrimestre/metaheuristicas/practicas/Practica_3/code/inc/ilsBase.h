#pragma once
#include "mh.h"

/**
 * @brief Implementación de la base para la Búsqueda Local Reiterada (ILS – Iterated Local Search)
 *        para el Problema de Asignación Con Restricciones (PAR).
 *
 * Esquema del algoritmo:
 *   1. Generar solución inicial aleatoria S0.
 *   2. Aplicar algoritmo_optimizacion sobre S0 → S.  Mejor_Solución ← S.
 *   3. Repetir (n_aplicaciones_bl - 1) veces:
 *        a. S' ← Mutar(Mejor_Solución)   [cambiar 20 % de los elementos]
 *        b. S'' ← algoritmo_optimizacion(S')
 *        c. Actualizar Mejor_Solución si S'' es mejor  (criterio del mejor)
 *   4. Devolver Mejor_Solución.
 *
 * Parámetros:
 *   - Número total de aplicaciones de algoritmo_optimizacion
 *   - Evaluaciones máximas por cada algoritmo_optimizacion.
 *   - Porcentaje de mutación.
 *   - Criterio de aceptación: criterio del mejor.
 */
class ILSBase : public MH<int> {
protected:
    int n_arranques;
    double p_mutacion;
    int maxevals_algorithm;

    /**
     * @brief Muta la solución dada cambiando un porcentaje (p_mutacion) de sus posiciones a un
     *        cluster distinto del actual, elegido aleatoriamente.
     *
     * @param solution  Solución a mutar (se modifica en local).
     * @param k_min     Valor mínimo del dominio de cluster.
     * @param k_max     Valor máximo del dominio de cluster.
     * @param problem   Referencia al problema (para poder reparar si es necesario).
     */
    void mutate(tSolution<int>& solution, int k_min, int k_max, Problem<int>& problem);

    // Métodos virtuales 
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals) = 0;
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals, tSolution<int> initial_solution) = 0;

public:
    ILSBase(int n_arranques, int maxevals_algorithm, double p_mutacion)
        : MH(), n_arranques(n_arranques), maxevals_algorithm(maxevals_algorithm), p_mutacion(p_mutacion) {}
        
    virtual ~ILSBase() {}

    /**
     * @brief Ejecuta ILS sobre el problema dado.
     *
     * @param problem   Problema de optimización a resolver.
     * @param maxevals  Número máximo de evaluaciones totales permitidas.
     * @return          ResultMH<int> con la mejor solución encontrada,
     *                  su fitness y el número de evaluaciones usadas.
     */
    virtual ResultMH<int> optimize(Problem<int>& problem, int maxevals) override;
};