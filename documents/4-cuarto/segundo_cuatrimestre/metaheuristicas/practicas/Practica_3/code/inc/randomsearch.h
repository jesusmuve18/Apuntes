#pragma once
#include <mh.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
template <typename tDomain> class RandomSearch : public MH<tDomain> {

public:
  RandomSearch() : MH<tDomain>() {}
  virtual ~RandomSearch() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH<tDomain> optimize(Problem<tDomain> &problem, int maxevals) override {
    assert(maxevals > 0);
    tSolution<tDomain> best;
    tFitness best_fitness = -1;

    for (int i = 0; i < maxevals; i++) {
      auto solution = problem.createSolution();
      tFitness fitness = problem.fitness(solution);

      if (fitness < best_fitness || best_fitness < 0) {
        best = solution;
        best_fitness = fitness;
      }
    }

    return ResultMH(best, best_fitness, maxevals);
  }
};
