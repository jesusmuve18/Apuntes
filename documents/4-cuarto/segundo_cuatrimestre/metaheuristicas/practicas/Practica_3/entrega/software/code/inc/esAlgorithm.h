#pragma once
#include "mh.h"

using namespace std;

// Alias de tipos para mayor claridad
using MHInt       = MH<int>;
using ProblemInt  = Problem<int>;
using ResultMHInt = ResultMH<int>;

/**
 * @brief Implementación del algoritmo de Enfriamiento Simulado (ES / Simulated Annealing)
 *        para el Problema de Asignación Con Restricciones (PAR).
 *
 * Utiliza el esquema de enfriamiento de Cauchy modificado:
 *   T_{k+1} = T_k / (1 + beta * T_k)
 *   beta    = (T0 - Tf) / (M * T0 * Tf)
 *
 * Temperatura inicial:
 *   T0 = (mu * Coste(S0)) / (-ln(phi))
 *   con phi = 0.3, mu = 0.2
 *
 * Temperatura final:  Tf = 1e-3  (comprobando que Tf < T0)
 * Bucle interno L(T): max_vecinos = 10 * n,  max_exitos = 1 * n
 * Número de enfriamientos:  M = maxevals / max_vecinos
 *
 * Condición de parada: evals >= maxevals  OR  exitos == 0 en el enfriamiento actual.
 */
class ESAlgorithm : public MHInt {
private:
    double phi;     // Probabilidad de aceptar una solución mu-veces peor
    double mu;     // Factor de empeoramiento relativo aceptado con prob. phi
    

    /**
     * @brief Calcula T_{k+1} a partir de T_k.
     * 
     * Utiliza el esquema de enfriamiento de Cauchy modificado:
     *   T_{k+1} = T_k / (1 + beta * T_k)
     * 
     * @param beta Constante para el enfriamiento
     * @param T Temperatura actual
     */
    double nextT(double beta, double T);

public:
    /**
     * @brief Constructor.
     * @param phi Probabilidad de aceptar una solución mu-veces peor
     */
    ESAlgorithm(double phi, double mu) : 
        MH(), phi(phi), mu(mu) {}

    virtual ~ESAlgorithm() {}

    /**
     * @brief Ejecuta el Enfriamiento Simulado sobre el problema dado.
     *
     * @param problem   Problema de optimización a resolver.
     * @param maxevals  Número máximo de evaluaciones de la función objetivo.
     * @return          ResultMH<int> con la mejor solución encontrada, su fitness y el número de evaluaciones usadas.
     */
    virtual ResultMH<int> optimize(Problem<int>& problem, int maxevals) override;

    /**
     * @brief Ejecuta el Enfriamiento Simulado sobre el problema dado a partir de una solución inicial.
     *
     * @param problem           Problema de optimización a resolver.
     * @param maxevals          Número máximo de evaluaciones de la función objetivo.
     * @param initial_solution  Solución inicial
     * @return          ResultMH<int> con la mejor solución encontrada, su fitness y el número de evaluaciones usadas.
     */
    virtual ResultMH<int> optimize(Problem<int>& problem, int maxevals, tSolution<int> initial_solution);
};
