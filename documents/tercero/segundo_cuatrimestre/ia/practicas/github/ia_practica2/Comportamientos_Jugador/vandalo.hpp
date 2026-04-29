#ifndef ComportamientoVandalo_H
#define ComportamientoVandalo_H

#include "comportamientos/comportamiento.hpp"

class ComportamientoVandalo : public Comportamiento{

  public:
    ComportamientoVandalo(unsigned int size = 0) : Comportamiento(size){
      last_action = IDLE;
      girando = false;
    }
    ComportamientoVandalo(const ComportamientoVandalo & comport) : Comportamiento(comport){}
    ~ComportamientoVandalo(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoVandalo * clone(){return new ComportamientoVandalo(*this);}

  private:
    Action last_action;
    bool girando;
};


#endif
