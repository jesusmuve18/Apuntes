#pragma once
#include "geneticAlgorithm.h"

using namespace std;

// Enumerado para elegir la variante del algoritmo
enum class ageType { UNIFORM, FIXED_SEGMENT };

class AGEAlgorithm : public GeneticAlgorithm {
protected:
    ageType age_type;
public:
    // Constructor donde podemos inyectar el tipo de algoritmo y las probabilidades
    AGEAlgorithm(ageType t, int popSize, double pc, double pm) : GeneticAlgorithm(popSize, pc, pm), age_type(t) {}
        
    virtual ~AGEAlgorithm() {}
    virtual ResultMH<int> optimize(Problem<int> &problem, int maxevals) override;
};