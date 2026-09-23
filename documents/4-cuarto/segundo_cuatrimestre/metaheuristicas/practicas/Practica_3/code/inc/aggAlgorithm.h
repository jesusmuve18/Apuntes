#pragma once
#include "geneticAlgorithm.h"

enum class aggType { UNIFORM, FIXED_SEGMENT };

class AGGAlgorithm : public GeneticAlgorithm {
protected:
    aggType agg_type;

    int expected_mutations;
    int expected_crossovers;

    void evolveOneGeneration(Problem<int>& problem, std::vector<tSolution<int>>& population, std::vector<tFitness>& fitnesses, tSolution<int>& bestSol, tFitness& bestFit, int k_clusters, int maxevals);

public:
    AGGAlgorithm(aggType t, int popSize, double pc, double pm) : 
        GeneticAlgorithm(popSize, pc, pm), agg_type(t), expected_mutations(pm*popSize), expected_crossovers(pc*(popSize/2)) {}
    virtual ResultMH<int> optimize(Problem<int> &problem, int maxevals) override;
};