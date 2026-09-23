#pragma once
#include <mh.h>
#include <vector>

using namespace std;

using MHInt = MH<int>;
using ProblemInt = Problem<int>;
using ResultMHInt = ResultMH<int>;

class GeneticAlgorithm : public MH<int> {
protected:
    int populationSize; 
    double crossoverProb; 
    double mutationProb;  
    int evals;

    // Métodos compartidos por todos los AGs
    void initializePopulation(Problem<int>& problem, std::vector<tSolution<int>>& pop, std::vector<tFitness>& fits, tSolution<int>& bestSol, tFitness& bestFit);
    int tournamentSelection(const std::vector<tFitness>& fitnesses, int k = 3);
    
    // Operadores
    void uniformCrossover(const tSolution<int>& p1, const tSolution<int>& p2, tSolution<int>& c1, tSolution<int>& c2);
    void fixedSegmentCrossover(const tSolution<int>& p1, const tSolution<int>& p2, tSolution<int>& c1, tSolution<int>& c2);
    virtual void mutate(tSolution<int>& sol, int k_clusters);

public:
    GeneticAlgorithm(int popSize, double pc, double pm) : 
        populationSize(popSize), 
        crossoverProb(pc), mutationProb(pm), 
        evals(0) {}
    virtual ~GeneticAlgorithm() {}
};