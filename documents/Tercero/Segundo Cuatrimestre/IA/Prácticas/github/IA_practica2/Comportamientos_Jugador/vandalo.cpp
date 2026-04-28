#include "../Comportamientos_Jugador/vandalo.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <stdlib.h>

bool EstoyEnCasillaObjetivo(int fil, int col, const Sensores &sensores)
{
  return sensores.destinoF == fil and sensores.destinoC == col;
}

bool EsDelanteObjetivo(const Sensores &sensores)
{
  int fil = sensores.posF;
  int col = sensores.posC;
  switch (sensores.rumbo)
  {
  case norte:
    fil--;
    break;
  case noreste:
    fil--;
    col++;
    break;
  case este:
    col++;
    break;
  case sureste:
    fil++;
    col++;
    break;
  case sur:
    fil++;
    break;
  case suroeste:
    fil++;
    col--;
    break;
  case oeste:
    col--;
    break;
  case noroeste:
    fil--;
    col--;
    break;
  }
  return EstoyEnCasillaObjetivo(fil, col, sensores);
}

bool EsDelanteCasilla(const char casilla, const Sensores &sensores, const vector<vector<unsigned char>> &mapa)
{
  int fil = sensores.posF;
  int col = sensores.posC;
  switch (sensores.rumbo)
  {
  case norte:
    fil--;
    break;
  case noreste:
    fil--;
    col++;
    break;
  case este:
    col++;
    break;
  case sureste:
    fil++;
    col++;
    break;
  case sur:
    fil++;
    break;
  case suroeste:
    fil++;
    col--;
    break;
  case oeste:
    col--;
    break;
  case noroeste:
    fil--;
    col--;
    break;
  }
  return mapa[fil][col] == casilla;
}

Action ComportamientoVandalo::think(Sensores sensores)
{

  Action accion = IDLE;

  // Un lobo no puede estar en una casilla objetivo. Si está se tiene que salir.
  bool esta_en_casilla_objetivo = EstoyEnCasillaObjetivo(sensores.posF, sensores.posC, sensores);

  if (girando)
  {
    accion = TURN_SR;
    girando = false;
  }
  else if (last_action == PUSH)
  {
    accion = TURN_SR;
    girando = true;
  }
  else if ((esta_en_casilla_objetivo or sensores.superficie[0] == 'X') and
           (sensores.superficie[2] != 'P' and sensores.superficie[2] != 'M') and
           (sensores.agentes[2] == '_'))
  {
    accion = WALK;
  }
  else if (esta_en_casilla_objetivo)
  {
    accion = TURN_SR;
  }
  else
  {
    bool delanteEsCasillaObjetivo = EsDelanteObjetivo(sensores);

    if (sensores.agentes[2] == 'r')
    {
      accion = PUSH;
    }
    else if (sensores.agentes[2] == 'a')
    {
      accion = TURN_SR;
    }
    else if (sensores.agentes[2] == 'v')
    {
      accion = TURN_L;
    }
    else if (!delanteEsCasillaObjetivo and sensores.superficie[2] != 'P' and sensores.superficie[2] != 'M' and sensores.agentes[2] == '_')
    {
      if (sensores.agentes[6] == 'r' or sensores.agentes[12] == 'r')
      {
        accion = WALK;
      }
      else if (sensores.agentes[10] == 'r' or sensores.agentes[11] == 'r' or sensores.agentes[13] == 'r' or sensores.agentes[14] == 'r')
      {
        accion = WALK;
      }
    }
    else if (sensores.agentes[1] == 'r' or sensores.agentes[4] == 'r' or sensores.agentes[9] == 'r')
    {
      accion = TURN_L;
    }
    else if (sensores.agentes[3] == 'r' or sensores.agentes[8] == 'r' or sensores.agentes[15] == 'r')
    {
      accion = TURN_SR;
    }

    if (accion == IDLE)
    {
      int cual = aleatorio(20);
      switch (cual)
      {
      case 0:
        accion = IDLE;
        break;
      case 1:
      case 2:
        accion = TURN_L;
        break;
      case 3:
      case 4:
        accion = TURN_SR;
        break;
      default:
        if (!delanteEsCasillaObjetivo and sensores.superficie[2] != 'P' and sensores.superficie[2] != 'M' and sensores.agentes[2] == '_')
          accion = WALK;
        else
          accion = TURN_SR;
        break;
      }
    }
  }

  last_action = accion;
  return accion;
}

int ComportamientoVandalo::interact(Action accion, int valor)
{
  return 0;
}
