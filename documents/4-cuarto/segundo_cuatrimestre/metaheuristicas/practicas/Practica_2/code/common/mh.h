#pragma once

#include "problem.h"
#include <utility>

template <typename tDomain>
struct ResultMH {
  tSolution<tDomain> solution;
  tFitness fitness;
  unsigned int evaluations;

  ResultMH(tSolution<tDomain> &sol, tFitness fit, unsigned evals)
      : solution(sol), fitness(fit), evaluations(evals) {}
};

/**
 * Metaheuristic algorithm interface.
 *
 * @author  author
 * @version 1.0
 * @since   2021-05-01
 */

template <typename tDomain>
class MH {
public:
  virtual ~MH() {}
  /**
   * Run the metaheuristic algorithm to find the optimal solution.
   *
   * @param problem  The problem to solve.
   * @param maxevals The maximum number of evaluations.
   * @return A pair containing the optimal solution and its fitness.
   * @throws Exception if the algorithm fails to converge.
   *
   * @since   2021-05-01
   * @author  author
   * @version 1.0
   */
  virtual ResultMH<tDomain> optimize(Problem<tDomain> &problem, int maxevals) = 0;
};
