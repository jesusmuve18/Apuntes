#ifndef COMPORTAMIENTOEXCURSIONISTA_H
#define COMPORTAMIENTOEXCURSIONISTA_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoExcursionista : public Comportamiento{

  public:
    ComportamientoExcursionista(unsigned int size = 0) : Comportamiento(size){
      paso=0;
    }
    ComportamientoExcursionista(const ComportamientoExcursionista & comport) : Comportamiento(comport){}
    ~ComportamientoExcursionista(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoExcursionista * clone(){return new ComportamientoExcursionista(*this);}

  private:
    int paso;

};



#endif
