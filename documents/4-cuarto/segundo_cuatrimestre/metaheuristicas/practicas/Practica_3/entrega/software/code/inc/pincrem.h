#pragma once
#include <problem.h>

class ProblemIncrem : public Problem<int> {
  size_t	size;

public:
  ProblemIncrem(size_t size) : Problem<int>() { this->size = size; }
  virtual size_t getSolutionSize() override { return size; }
  virtual std::pair<int, int> getSolutionDomainRange() override {
    return std::make_pair(false, true);
  }

  tFitness fitness(const tSolution<int> &solution) override {
    tFitness count = 0;

    for (int i = 0; i < solution.size(); i++) {
      if (solution[i]) {
        if (i % 2 == 0) {
          count += 1;
        } else {
          count -= 1;
        }
      }
    }
    return count;
  }

  tSolution<int> createSolution() override {
    tSolution<int> solution(size);
    for (int i = 0; i < solution.size(); i++) {
      solution[i] = Random::get<bool>();
    }
    return solution;
  }

  virtual bool isValid(const tSolution<int> &solution) override { return true; }

  virtual void fix(tSolution<int> &solution) override {}
};
