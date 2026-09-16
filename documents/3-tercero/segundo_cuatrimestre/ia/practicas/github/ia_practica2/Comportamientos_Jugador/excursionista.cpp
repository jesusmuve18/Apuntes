#include "../Comportamientos_Jugador/excursionista.hpp"
#include "motorlib/util.h"

Action ComportamientoExcursionista::think(Sensores sensores) {
  Action accion = IDLE;

/*  // Aldeano basico
  switch (paso){
      case 0: accion = actFORWARD;
              break;
      case 1: accion = actTURN_R;
	      break;
  }
  paso = (paso+1)%2;*/

  // Aldeano comun
  int cual = aleatorio(20);
  switch (cual) {
	case 0: accion = IDLE; break;
	case 1: 
  case 2: accion = TURN_L; break;
  case 3:
  case 4: accion = TURN_SR; break;
	default: 
		if (sensores.superficie[2] != 'P' and sensores.superficie[2] != 'M' and sensores.agentes[2] == '_') accion = WALK;
    else accion = TURN_SR;
		break;
  }

  return accion;
}

int ComportamientoExcursionista::interact(Action accion, int valor) {
    return 0;
}
