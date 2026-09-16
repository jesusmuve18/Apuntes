#include "motorlib/entidad.hpp"

void Entidad::resetEntidad()
{
  hitbox = false;
  if (tipo == jugador or subtipo == auxiliar)
  {
    desactivado = 0;
  }
  else
  {
    desactivado = aleatorio(7) + 3;
  }

  choque = false;
  colisiones = 0;
  reset = true;
  if (vida != 0)
    muertes_innecesarias++;
  // vida = 0;
  done = false;

  mensaje = true;
}

int Entidad::isMemberObjetivo(unsigned int paramF, unsigned int paramC)
{
  int pos = -1;
  for (int i = 0; i < num_destinos and pos == -1; i++)
  {
    if (destino[3 * i] == paramF and destino[3 * i + 1] == paramC)
    {
      pos = i;
    }
  }
  if (pos != -1 and alcanzados[pos])
    pos = -1;

  return pos;
}

bool Entidad::allObjetivosAlcanzados()
{
  bool conseguido = true;
  for (int i = 0; i < num_destinos and conseguido; i++)
  {
    conseguido = conseguido and alcanzados[i];
  }
  return conseguido;
}

bool Entidad::allLessOneObjetivosAlcanzados()
{
  int num = 0;
  for (int i = 0; i < num_destinos; i++)
    if (alcanzados[i])
      num++;

  return (num == num_destinos - 1);
}

void Entidad::actualizarAlcanzados()
{
  int pos = isMemberObjetivo(f, c);
  if (pos != -1 and !alcanzados[pos])
  {
    alcanzados[pos] = true;
  }
}

void Entidad::anularAlcanzados()
{
  for (int i = 0; i < num_destinos; i++)
  {
    alcanzados[i] = false;
  }
}

void Entidad::setObjetivos(vector<unsigned int> v)
{
  destino = v;
  num_destinos = destino.size() / 3;
  for (int i = 0; i < num_destinos; i++)
  {
    alcanzados[i] = false;
  }
}

unsigned int Entidad::getObjFil(int pos)
{
  if (pos < num_destinos)
    return destino[3 * pos];
  else
  {
    std::cout << "Error en getObjFil: intento de acceso a una posición de objetivo que no existe\n";
    exit(1);
  }
}

unsigned int Entidad::getObjCol(int pos)
{
  if (pos < num_destinos)
    return destino[3 * pos + 1];
  else
  {
    std::cout << "Error en getObjFil: intento de acceso a una posición de objetivo que no existe\n";
    exit(1);
  }
}

unsigned int Entidad::getObjPri(int pos)
{
  if (pos < num_destinos)
    return destino[3 * pos + 2];
  else
  {
    std::cout << "Error en getObjPri: intento de acceso a una posición de objetivo que no existe\n";
    exit(1);
  }
}


Action Entidad::think(int acc, vector<vector<unsigned char>> vision, int level)
{
  Action accion = IDLE;
  Sensores sensor;

  if (desactivado == 0)
  {
    vida--;
    hitbox = true;

    sensor.nivel = level;

    sensor.vida = vida;
    sensor.energia = energia;

    sensor.destinoF = -1;
    sensor.destinoC = -1;

    sensor.choque = choque;
    sensor.reset = reset;

    sensor.venpaca = venpaca;
    sensor.gravedad = false;

    sensor.posF = f;
    sensor.posC = c;
    sensor.rumbo = orient;

    if (tipo == jugador) // Poner los sensores que se activan al jugador
    {
      // Si estoy en un nivel deliberativo activo que haya un destino (un accidentado)
      if (level > 1)
      {
        sensor.destinoF = destino[0];
        sensor.destinoC = destino[1];
      }

      if (level == 4 and sensor.posF == destino[0] and sensor.posC == destino[1]){
        sensor.gravedad = (destino[2] == 1);
      }
    }
    else if (subtipo == auxiliar) // Poner los sensores que se le activan al auxiliar
    {
      if (level > 1){
        sensor.destinoF = destino[0];
        sensor.destinoC = destino[1];  
        if (level == 4 and !MeHasLLamado()){
          sensor.destinoF = -1;
          sensor.destinoC = -1;
        }      
      }
    }

    if (tipo == jugador or subtipo == auxiliar)
    {
      if (sensor.energia == 0)
      {
        done = true;
        cout << "Se agoto la energía!" << endl;
      }
      else if (sensor.vida == 0)
      {
        done = true;
        cout << "Se agotaron los instantes de simulacion" << endl;
      }
      else if (getTiempo() > getTiempoMaximo())
      {
        done = true;
        cout << "Se agoto el tiempo de deliberacion!" << endl;
      }
    }

    sensor.cota = vision[2];
    sensor.agentes = vision[1];
    sensor.superficie = vision[0];

    sensor.tiempo = getTiempo() / CLOCKS_PER_SEC;

    visionAux = vision;

    if (!done)
    {
      if (acc == -1)
        accion = comportamiento->think(sensor);
      else
        accion = static_cast<Action>(acc);
    }
    choque = false;
    reset = false;
    mensaje = false;
  }
  else
  {
    desactivado--;
  }

  return accion;
}

bool Entidad::interact(Action accion, int valor)
{
  bool out = false;
  int retorno = comportamiento->interact(accion, valor);

  if (retorno == 1)
  { // Desaparición temporal
    resetEntidad();
    out = true;
  }

  if (retorno == 2)
  { // Muerte
    hitbox = false;
    desactivado = -1;
    out = true;
  }

  if (retorno == 3)
  { // Revivir
    hitbox = true;
    desactivado = 0;
    out = true;
  }

  return out;
}

unsigned char Entidad::getSubTipoChar()
{
  unsigned char out = ' ';

  switch (subtipo)
  {
  case rescatador:
    out = 'r';
    break;
  case excursionista:
    out = 'e';
    break;
  case auxiliar:
    out = 'a';
    break;
  case vandalo:
    out = 'v';
    break;
  }

  return out;
}

void Entidad::fixTiempo_sig_accion(unsigned char celda)
{
  tiempo_sig_accion = 1;
}

int Entidad::fixBateria_sig_accion_jugador(unsigned char celdaJugador, int difAltura, Action accion)
{
  bateria_sig_accion = 1;
  switch (accion)
  {
  case IDLE:

  case CALL_ON:
  case CALL_OFF:
    bateria_sig_accion = 0;
    break;
  case WALK:
    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 100;
      if (difAltura > 0)
        bateria_sig_accion += 10;
      else if (difAltura < 0)
        bateria_sig_accion -= 10;
      break;
    case 'T':
      bateria_sig_accion = 20;
      if (difAltura > 0)
        bateria_sig_accion += 5;
      else if (difAltura < 0)
        bateria_sig_accion -= 5;
      break;
    case 'S':
      bateria_sig_accion = 2;
      if (difAltura > 0)
        bateria_sig_accion += 1;
      else if (difAltura < 0)
        bateria_sig_accion -= 1;
      break;
    } // Fin switch celdaJugador
    break;
  case RUN:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 150;
      if (difAltura > 0)
        bateria_sig_accion += 15;
      else if (difAltura < 0)
        bateria_sig_accion -= 15;
      break;
    case 'T':
      bateria_sig_accion = 35;
      if (difAltura > 0)
        bateria_sig_accion += 5;
      else if (difAltura < 0)
        bateria_sig_accion -= 5;
      break;
    case 'S':
      bateria_sig_accion = 3;
      if (difAltura > 0)
        bateria_sig_accion += 2;
      else if (difAltura < 0)
        bateria_sig_accion -= 2;
      break;
    } // Fin switch celdaColaborador
    break;
  case TURN_L:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 30;
      break;
    case 'T':
      bateria_sig_accion = 5;
      break;
    case 'S':
      bateria_sig_accion = 1;
      break;
      break;
    } // Fin switch celdaJugador
    break;
  case TURN_SR:

    switch (celdaJugador)
    {
    case 'A':
      bateria_sig_accion = 16;
      break;
    case 'T':
      bateria_sig_accion = 3;
      break;
    } // Fin switch celdaJugador
    break;
  }

  return bateria_sig_accion;
}

void Entidad::decBateria_sig_accion()
{
  energia -= bateria_sig_accion;
  if (energia < 0)
    energia = 0;
}
