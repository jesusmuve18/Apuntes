#pragma once
#include <vector>
/**
 * Typedef for the fitness function return type.
 *
 * This is an example, change it for your specific problem.
 *
 */
typedef float tFitness;

/**
 * Domain of each element
 */
template<typename tDomain>
using tSolution = std::vector<tDomain>;

/**
 * Represent the new option to select the heuristic
 */
typedef tFitness tHeuristic;
