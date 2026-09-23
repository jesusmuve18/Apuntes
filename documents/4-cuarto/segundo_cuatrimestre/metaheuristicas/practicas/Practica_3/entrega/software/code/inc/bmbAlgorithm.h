#pragma once
#include "mh.h"
#include "localsearch.h"

using namespace std;

/**
 * @brief Implementación de la Búsqueda Multiarranque Básica (BMB).
 *
 * El algoritmo BMB genera un número fijo de soluciones aleatorias iniciales,
 * aplica la Búsqueda Local (BL) sobre cada una de ellas y devuelve la mejor
 * solución encontrada en todo el proceso.
 *
 * Parámetros (según guión):
 *   - Número de arranques: 5
 *   - Evaluaciones máximas por cada ejecución de BL: maxevals / n_arranques
 *     (con el límite adicional de 20000 evaluaciones por BL según el guión)
 */
class BMBAlgorithm : public MH<int> {

private:
    int n_arranques;    // Número de soluciones iniciales a generar y optimizar
    int maxevals_bl;    // Número máximo de evaluaciones por BL

public:
    /**
     * @brief Constructor.
     * @param n_arranques Número de arranques (por defecto 5, según el guión).
     */
    explicit BMBAlgorithm(int n_arranques, int maxevals_bl)
        : MH(), n_arranques(n_arranques), maxevals_bl(maxevals_bl) {}

    virtual ~BMBAlgorithm() {}

    /**
     * @brief Ejecuta la Búsqueda Multiarranque Básica sobre el problema dado.
     *
     * @param problem   Problema de optimización a resolver.
     * @param maxevals  Número máximo de evaluaciones totales permitidas.
     * @return          ResultMH<int> con la mejor solución encontrada,
     *                  su fitness y el número de evaluaciones usadas.
     */
    virtual ResultMH<int> optimize(Problem<int>& problem, int maxevals) override;
};
