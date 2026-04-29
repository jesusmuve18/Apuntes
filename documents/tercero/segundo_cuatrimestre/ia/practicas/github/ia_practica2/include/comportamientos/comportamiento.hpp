#ifndef COMPORTAMIENTO_H
#define COMPORTAMIENTO_H

#include <vector>

using namespace std;

enum Orientacion {norte, noreste, este, sureste, sur, suroeste, oeste, noroeste};
enum Action {WALK, RUN, TURN_L, TURN_SR, CALL_ON, CALL_OFF, IDLE, PUSH};

struct ubicacion{
    int f;
    int c;
    Orientacion brujula;

    bool operator==(const ubicacion &ub)const {
      return (f == ub.f and c == ub.c and brujula == ub.brujula);
    }    
};

struct Sensores {
  int nivel;
  int vida;
  int energia;
  bool choque;
  bool reset;
  int posF;
  int posC;
  Orientacion rumbo; // Orientacion
  double tiempo;
  vector< unsigned char> superficie;
  vector< unsigned char> agentes;
  vector< unsigned char> cota;
  int destinoF;
  int destinoC;
  bool venpaca;  // Avisa al auxiliar que vaya a las coordenadas destino
  bool gravedad;  // Indica la gravedad del accidentado: True => Necesita auxiliar 

};



class Comportamiento {
  public:
    vector< vector< unsigned char> > mapaResultado;
    vector< vector< unsigned char> > mapaEntidades;
    vector< vector< unsigned char > > mapaCotas;
    std::vector< std::vector<unsigned char> > mapaConPlan;

    Comportamiento(unsigned int size);
    Comportamiento(vector< vector< unsigned char> > mapaR, vector< vector< unsigned char> > mapaC );
    Comportamiento(const Comportamiento & comport) : mapaEntidades(comport.mapaEntidades), mapaResultado(comport.mapaResultado), mapaCotas(comport.mapaCotas), mapaConPlan(comport.mapaConPlan) {}
    Comportamiento * clone(){return new Comportamiento(*this);}
    virtual ~Comportamiento(){}

    virtual Action think(Sensores sensores);

    virtual int interact(Action accion, int valor);

    vector< vector< unsigned char> > getMapaResultado(){return mapaResultado;}
    vector< vector< unsigned char> > getMapaEntidades(){return mapaEntidades;}
    vector< vector< unsigned char> > getMapaCotas(){return mapaCotas;}
    vector< vector< unsigned char> > getMapaPlan(){return mapaConPlan;}

};

#endif
