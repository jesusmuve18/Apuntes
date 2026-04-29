#include "../../include/model/Heuristic.h"

Heuristic::Heuristic() : started(true), stopped(true) {}

float Heuristic::evaluate(const Parchis & estado, int jugador)
{
    if(!started or !stopped){
        throw std::runtime_error("Intento de hacer trampas detectado. Se informará a los profesores de este incidente.");
    }
    started = false;
    stopped = false;
    startEvaluation();
    float result = getHeuristic(estado, jugador);
    stopEvaluation();
    return result;
}

void Heuristic::startEvaluation()
{
    if (started)
    {
        throw std::runtime_error("Intento de hacer trampas detectado. Se informará a los profesores de este incidente.");
    }
    NodeCounter::incrementEvaluated();
    NodeCounter::getInstance()->startTimer();
    started = true;

}

void Heuristic::stopEvaluation()
{
    if (!started or stopped)
    {
        throw std::runtime_error("Intento de hacer trampas detectado. Se informará a los profesores de este incidente.");
    }
    NodeCounter::getInstance()->stopTimer();
    stopped = true;
}
