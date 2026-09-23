#pragma once
#include "ilsBase.h"
#include "esAlgorithm.h"

using namespace std;

/**
 * @brief Implementación de la hibridación ILS + ES (ILS-ES).
 *
 * El algoritmo ILS-ES sigue el mismo esquema que el ILS, pero sustituye
 * la Búsqueda Local como método de optimización interna por el algoritmo
 * de Enfriamiento Simulado (ES).
 */
class ILSESAlgorithm : public ILSBase {

private:
    ESAlgorithm es;

    // Implementación de métodos virtuales
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals) override {
        return es.optimize(problem, maxevals);
    }
    virtual ResultMH<int> optimizeAlgorithm(Problem<int>& problem, int maxevals, tSolution<int> initial_solution) override {
        return es.optimize(problem, maxevals, initial_solution);
    }

public:
    /**
     * @brief Constructor.
     * @param n_arranques        Número de veces que se realiza el bucle principal
     * @param n_aplicaciones_es  Número de veces que se aplica el ES.
     * @param p_mutacion         Proporción de genes a mutar.
     * @param phi                Probabilidad de aceptar una solución mu-veces peor en ES.
     * @param mu                 Factor de empeoramiento relativo aceptado con prob. phi en ES.
     */
    explicit ILSESAlgorithm(int n_arranques, int maxevals_es, double p_mutacion, double phi, double mu)
        : ILSBase(n_arranques, maxevals_es, p_mutacion), es(ESAlgorithm(phi, mu)) {}

    virtual ~ILSESAlgorithm() {}
};
