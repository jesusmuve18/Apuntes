#pragma once
#include <mh.h>

/**
 * Trayectory-based metaheuristic algorithm interface.
 */
template <typename tDomain>
class MHTrayectory : public MH<tDomain> {
public:
  virtual ~MHTrayectory() {}
  /**
   * Run the Trayectory-based metaheuristic algorithm to find the optimal
   * solution.
   *
   * @param problem  The problem to solve.
   * @param maxevals The maximum number of evaluations.
   * @see MHTrayectory::optimize()
   */
  ResultMH<tDomain> optimize(Problem<tDomain> *problem, int maxevals) override {
    tSolution<tDomain> initial = problem->createSolution();
    tFitness fitness = problem->fitness(initial);
    return optimize(problem, initial, fitness, maxevals);
  }

public:
  /**
   * Run the Trayectory-based metaheuristic algorithm to find the optimal
   * solution starting from a given solution.
   *
   * @param problem  The problem to solve.
   * @param current The initial solution.
   * @param fitness The fitness of the initial solution.
   * @param maxevals The maximum number of evaluations.
   */
  virtual ResultMH<tDomain> optimize(Problem<tDomain> *problem, const tSolution<tDomain> &current,
                            tFitness fitness, int maxevals) = 0;
};
