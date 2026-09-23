#pragma once
#include "ilsBase.h"
#include "localsearch.h"

using namespace std;

/**
 * @brief Implementación de la Búsqueda Local Reiterada (ILS – Iterated Local Search)
 *        para el Problema de Asignación Con Restricciones (PAR).
 */
class ILSAlgorithm : public ILSBase {
protected:

    LocalSearch bl;

    // Implementación de métodos virtuales
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals) override {
        return bl.optimize(problem, maxevals);
    }
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals, tSolution<int> initial_solution) override {
        return bl.optimize(problem, maxevals, initial_solution);
    }
    
public:
    /**
     * @brief Constructor.
     * @param n_arranques        Número de veces que se realiza el bucle principal
     * @param n_aplicaciones_bl  Número de veces que se aplica la BL.
     * @param p_mutacion         Proporción de genes a mutar.
     */
    explicit ILSAlgorithm(int n_arranques, int maxevals_bl, double p_mutacion)
        : ILSBase(n_arranques, maxevals_bl, p_mutacion){}

    virtual ~ILSAlgorithm() {}
};
