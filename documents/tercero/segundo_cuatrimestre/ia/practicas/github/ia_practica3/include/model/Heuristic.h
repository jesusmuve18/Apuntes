#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "NodeCounter.h"
#include <stdexcept>

class Parchis;

class Heuristic
{
public:
    // Con este se llama a la evaluación desde el exterior
    virtual float evaluate(const Parchis &estado, int jugador) final;
    

protected:
    Heuristic();

    // Aquí se define la heurística que se desee.
    virtual float getHeuristic(const Parchis &estado, int jugador) const = 0;

    virtual void startEvaluation() final;

    virtual void stopEvaluation() final;

private:
    bool started;
    bool stopped;

};

#endif // HEURISTIC_H
