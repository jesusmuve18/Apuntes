#ifndef __PROBLEM_H
#define __PROBLEM_H

#include "solution.h"
#include "random.hpp"
#include <utility>

// get base random alias which is auto seeded and has static API and internal
// state it is not threads secure, you can also use ::random_thread_local
using Random = effolkronium::random_static;

/**
 * Class that represent information useful for factorized solution.
 *
 * The idea is to inherit from this class for specific problems.
 */
template<typename tDomain>
class SolutionFactoringInfo {
    virtual ~SolutionFactoringInfo() = default; // Default destructor
};

/**
 * Problem class to solve the given problem in metaheuristic.
 *
 * @author your_name
 * @version 1.0
 */
template<typename tDomain>
class Problem {
public:
  Problem() {}
  virtual ~Problem() {}
  /***
   * Evaluate the solution from scratch.
   * @param solution to evaluate.
   */
  virtual tFitness fitness(const tSolution<tDomain> &solution) = 0;
  /**
   * Evaluate the solution indicating the current position to change and the new
   * value.
   *
   * By default it run the previous one. However, it can be override to
   * factorize the fitness.
   *
   * @param solution to evaluate.
   * @param solution_info information of solution to calculate the factored
   * fitness.
   * @param pos_change position of the solution to change.
   * @param new_value to store in pos_previous.
   */
  virtual tFitness fitness(const tSolution<tDomain> &solution,
                           SolutionFactoringInfo<tDomain> *solution_info,
                           unsigned pos_change, tDomain new_value) {
    auto newsol(solution);
    newsol[pos_change] = new_value;
    return fitness(newsol);
  }

  /**
   * Generate the solution information to generate the factorized fitness.
   * @param solution to generate.
   *
   * @return the solution information.
   */
  virtual SolutionFactoringInfo<tDomain> *
  generateFactoringInfo(const tSolution<tDomain> &solution) {
    return new SolutionFactoringInfo<tDomain>();
  }

  /**
   * Update the Solution Factoring Information after a movement is applied.
   *
   * @param solution.
   * @param solution_info to update, it is updated.
   * @param pos_change position of the solution to change.
   * @param new_value to store in pos_previous.
   */
  virtual void updateSolutionFactoringInfo(SolutionFactoringInfo<tDomain> *solution_info,
                                           const tSolution<tDomain> &solution,
                                           unsigned pos_change,
                                           tDomain new_value) {}

  /**
   * Create a new solution.
   */
  virtual tSolution<tDomain> createSolution() = 0;
  /**
   * Return the current size of the solution.
   */
  virtual size_t getSolutionSize() = 0; // Get the size of each solution
  /** Return the range of domain of each element of the solution */
  virtual std::pair<tDomain, tDomain> getSolutionDomainRange() = 0;

  /**
   * Check if the solution is a valid solution
   * @param solution to check.
   * @return true if the solution is valid, false otherwise.
   */
  virtual bool isValid(const tSolution<tDomain> &solution) = 0;

  /**
   * Update the solution to fix it, if isValid returns false
   * @param solution to fix.
   */
  virtual void fix(tSolution<tDomain> &solution) = 0;
};

#endif
