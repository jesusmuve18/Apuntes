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
// Instanciamos la plantilla con el tipo que nos interese
using MHInt = MH<int>;
using ProblemInt = Problem<int>;
using ResultMHInt = ResultMH<int>;

class GreedySearch : public MHInt {

public:
  GreedySearch() : MH() {}
  virtual ~GreedySearch() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  virtual ResultMH<int> optimize(Problem<int> &problem, int maxevals);
};
