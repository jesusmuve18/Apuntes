#pragma once
#include "aggAlgorithm.h"

enum class amType { ALL, RANDOM, BEST };

class MemeticAlgorithm : public AGGAlgorithm {
private:
    const double P_LS = 0.1;
    const double P_BEST = 0.1;

    amType am_type;

    int generationCount;
    int max_bls_failures;       // Máximo número de fallos permitido en bls
    int n_gens_to_apply_bls;    // Cada cuántas generaciones se aplicará BLS
    int maxevals_bls;          // Máximo de evaluaciones permitidas por cada ejecución de BLS

    double expected_mutations_per_gen;

    // Búsqueda local suave
    ResultMHInt softLocalSearch(Problem<int>& problem, tSolution<int>& sol, tFitness& solFit, int maxevals, int maxevals_global, int e);
    void mutate(tSolution<int>& sol, int k_clusters) override;
    
    public:
    MemeticAlgorithm(amType t, aggType aggT, int popSize, double pc, double pm, int e, int n_gens_bls=10, int max_bls_evals=100) 
        : AGGAlgorithm(aggT, popSize, pc, pm), am_type(t), generationCount(0), max_bls_failures(e), n_gens_to_apply_bls(n_gens_bls), maxevals_bls(max_bls_evals){}
    
    virtual ResultMH<int> optimize(Problem<int> &problem, int maxevals) override;
};