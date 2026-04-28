#include "motorlib.hpp"

#include <unistd.h>

extern MonitorJuego monitor;

bool actuacionRescatador(unsigned char celdaJ_inicial, unsigned char celdaJ_fin, int difAltJ, Action accion, unsigned int x, unsigned int y)
{
  int posibleElAvance;
  unsigned int mix, miy;
  unsigned char celdaRand;
  bool salida = false;
  int gasto;
  int error = 0; // 0 = NoError | 1 = NoEnergiaSuficiente | 2 = Colision

  // Primero evaluo si hay energía suficiente para realizar las acciones del rescatador.

  gasto = monitor.get_entidad(0)->fixBateria_sig_accion_jugador(celdaJ_inicial, difAltJ, accion);

  // std::cout << "Accion Rescatador: " << accion << "   Accion Colaborador: " << accionColaborador << endl;
  // std::cout << "Gasto Rescatador: " << gasto_Rescatador << "   Gasto Colaborador: " << gasto_colaborador << "   Gasto Total: " << gasto << endl;

  if (gasto > monitor.get_entidad(0)->getBateria())
  {
    cout << "La energía necesaria para las acciones seleccionadas superó a la energía disponible\n";
    error = 1;
    monitor.get_entidad(0)->setBateria(0);

    return false;
  }

  switch (accion)
  {
  case WALK:
    if (monitor.getMapa()->casillaOcupada(0) == -1 and ((abs(difAltJ) <= 1) or (monitor.get_entidad(0)->Has_Zapatillas() and (abs(difAltJ) <= 2)))) // Casilla destino desocupada        monitor.get_entidad(0)->seAostio();
    {
      switch (celdaJ_fin)
      {
      case 'B': // Arbol
      case 'M': // Muro
        monitor.get_entidad(0)->seAostio();
        if (celdaJ_fin == 'M')
          std::cout << "El rescatador ha chocado con un obstáculo\n";
        else
          std::cout << "El rescatador ha chocado con un árbol\n";
        break;
      case 'P': // Precipicio
        std::cout << "Se cayó por un precipicio\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
        break;
      case 'X': // Casilla Rosa (Puesto Base)
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(0)->Cogio_Zapatillas(true);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      default:
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.getLevel() > 1)
      {
        if (monitor.getLevel() == 2 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1)
        {
          // acaba de completar todos los objetivos.
          cout << "-----> Casilla objetivo alcanzada por el rescatador\n";
          // El rescatador llegó a la casilla objetivo.
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          monitor.get_entidad(0)->incrMisiones();

          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        }
        else if (monitor.getLevel() == 4 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and !monitor.get_entidad(0)->getObjPri(0))
        {
          cout << "Llega al objetivo y la prioridad no es grave\n";
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(2);
          monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
        else if (monitor.getLevel() == 4 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.CanHeSeesThisCell(1, monitor.get_entidad(0)->getObjFil(0), monitor.get_entidad(0)->getObjCol(0)))
        {
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(7);
          monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
      }
      // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    }
    else if (monitor.getMapa()->casillaOcupada(0) != -1) // Choca contra otro agente
    {
      // Choca contra una entidad
      monitor.get_entidad(0)->seAostio();
      if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == excursionista)
      {
        monitor.get_entidad(0)->perderPV(0);
        std::cout << "El Rescatador ha chocado con un excursionista\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == auxiliar)
      {
        monitor.get_entidad(0)->perderPV(0);
        std::cout << "El Rescatador ha chocado con un auxiliar\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == vandalo)
      {
        // Opcion reiniciarse en otro punto del mapa
        /*do
        {
          mix = aleatorio(monitor.getMapa()->getNFils() - 1);
          miy = aleatorio(monitor.getMapa()->getNCols() - 1);

          celdaRand = monitor.getMapa()->getCelda(mix, miy);
        } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setPosicion(mix, miy);
        monitor.get_entidad(0)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
        monitor.get_entidad(0)->setHitbox(true);
        monitor.get_entidad(0)->Cogio_Zapatillas(false);*/

        // Opcion simplemente choca contra el vandalo
        monitor.get_entidad(0)->perderPV(0);
        std::cout << "El Rescatador ha chocado con un vandalo\n";
      }
      salida = false;
    }
    else // Choca o cae por la diferencia de altura entre casilla inicial y final
    {
      if (difAltJ > 0) // Choca porque la casilla destino está demasiado alta.
      {
        monitor.get_entidad(0)->seAostio();
        std::cout << "El rescatador ha chocado. Casilla destino demasiado alta\n";
      }
      else // Se cae porque la casilla destino está demasiado baja.
      {
        std::cout << "El rescatador ha dado un salto al vacío. Demasiada diferencia de altura entre casillas\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
      }
    }
    break;

  case RUN:
    posibleElAvance = monitor.getMapa()->EsPosibleCorrer(0, 2);

    if (posibleElAvance != 0)
    { // No se ha podido avanzar.
      switch (posibleElAvance)
      {
      case 1: // Muro
        monitor.get_entidad(0)->seAostio();
        std::cout << "El Rescatador ha chocado con un muro\n";
        break;

      case 6: // Arbol
        monitor.get_entidad(0)->seAostio();
        std::cout << "El Rescatador ha chocado con un árbol\n";
        break;

      case 2: // Precipicio
        std::cout << "Se cayó por un precipicio\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
        break;

      case 3: // auxiliar
        monitor.get_entidad(0)->seAostio();
        std::cout << "El Rescatador ha chocado con el auxiliar\n";
        break;

      case 4: // excursionista
        monitor.get_entidad(0)->seAostio();
        std::cout << "El Rescatador ha chocado con un excursionista\n";
        break;

      case 5: // vandalo
        monitor.get_entidad(0)->seAostio();
        std::cout << "El Rescatador ha chocado con un vandalo\n";
        break;
      }
    }
    else if (posibleElAvance == 0 and ((abs(difAltJ) <= 1) or (monitor.get_entidad(0)->Has_Zapatillas() and (abs(difAltJ) <= 2))))
    { // Es posible correr
      switch (celdaJ_fin)
      {
      case 'X': // Puesto Base (Recarga)
        // monitor.get_entidad(0)->increaseBateria(10);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(0)->Cogio_Zapatillas(true);
        // pierdo el bikini
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'A': // Agua
      default:
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.getLevel() > 1)
      {
        if (monitor.getLevel() != 4 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1)
        {
          // acaba de completar todos los objetivos.
          cout << "-----> Casilla objetivo alcanzada por el rescatador\n";
          // El rescatador llegó a la casilla objetivo.
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          monitor.get_entidad(0)->incrMisiones();

          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        }
        else if (monitor.getLevel() == 4 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and !monitor.get_entidad(0)->getObjPri(0))
        {
          cout << "Llega al objetivo y la prioridad no es grave\n";
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(2);
          monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
        else if (monitor.getLevel() == 4 and monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.CanHeSeesThisCell(1, monitor.get_entidad(0)->getObjFil(0), monitor.get_entidad(0)->getObjCol(0)))
        {
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(7);
          monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
      }
    }
    else
    {                  // Se puede correr pero el desnivel de altura entre casillas es demasiado grande.
      if (difAltJ > 0) // Choca porque la casilla destino está demasiado alta.
      {
        monitor.get_entidad(0)->seAostio();
        std::cout << "El rescatador ha chocado. Casilla destino demasiado alta\n";
      }
      else // Se cae porque la casilla destino está demasiado baja.
      {
        std::cout << "El rescatador ha dado un salto al vacío. Demasiada diferencia de altura entre casillas\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
      }
    }
    break;

  case TURN_SR:
    monitor.get_entidad(0)->giro45Dch();
    monitor.girarJugadorDerecha(45);
    // monitor.get_entidad(1)->SetActionSent(IDLE);
    if (monitor.getLevel() == 4 and monitor.get_entidad(0)->getFil() == monitor.get_entidad(0)->getObjFil(0) and monitor.get_entidad(0)->getCol() == monitor.get_entidad(0)->getObjCol(0) and monitor.CanHeSeesThisCell(1, monitor.get_entidad(0)->getObjFil(0), monitor.get_entidad(0)->getObjCol(0)))
    {
      monitor.put_active_objetivos(1);
      monitor.get_entidad(0)->anularAlcanzados();
      monitor.get_entidad(0)->incrMisiones();
      monitor.get_entidad(0)->incrPuntuacion(7);
      monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

      for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
      {
        monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
      }
    }
    salida = false;
    break;

  case TURN_L:
    monitor.get_entidad(0)->giroIzq();
    monitor.girarJugadorIzquierda(90);
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    // monitor.get_entidad(1)->SetActionSent(IDLE);

    if (monitor.getLevel() == 4 and monitor.get_entidad(0)->getFil() == monitor.get_entidad(0)->getObjFil(0) and monitor.get_entidad(0)->getCol() == monitor.get_entidad(0)->getObjCol(0) and monitor.CanHeSeesThisCell(1, monitor.get_entidad(0)->getObjFil(0), monitor.get_entidad(0)->getObjCol(0)))
    {
      monitor.put_active_objetivos(1);
      monitor.get_entidad(0)->anularAlcanzados();
      monitor.get_entidad(0)->incrMisiones();
      monitor.get_entidad(0)->incrPuntuacion(7);
      monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar

      for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
      {
        monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
      }
    }

    salida = true;
    break;

  case PUSH:
    std::cout << "La acción PUSH no es aplicable al agente rescatador\n";
    break;

  case CALL_ON:
    monitor.get_entidad(1)->AsignarCall_ON(true);
    break;

  case CALL_OFF:
    monitor.get_entidad(1)->AsignarCall_ON(false);
    cout << "El rescatador anula la llamada al Auxiliar\n";
    if (monitor.getLevel() == 4 and monitor.get_entidad(0)->getFil() == monitor.get_entidad(0)->getObjFil(0) and monitor.get_entidad(0)->getCol() == monitor.get_entidad(0)->getObjCol(0))
    {
      cout << "El rescatador anula la operación de rescate\n";
      monitor.put_active_objetivos(1);
      monitor.get_entidad(0)->anularAlcanzados();
      monitor.get_entidad(0)->incrMisiones();
      monitor.get_entidad(0)->incrPuntuacion(1);
      for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
      {
        monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
      }
    }

    break;

  case IDLE:
    if (celdaJ_inicial == 'X' and monitor.getLevel() == 4 or monitor.getLevel() == 1)
    { // Casilla Rosa (Recarga)
      monitor.get_entidad(0)->increaseBateria(10);
    }
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    // monitor.get_entidad(1)->SetActionSent(IDLE);

    if (monitor.getLevel() == 4 and monitor.get_entidad(0)->getFil() == monitor.get_entidad(0)->getObjFil(0) and monitor.get_entidad(0)->getCol() == monitor.get_entidad(0)->getObjCol(0) and monitor.CanHeSeesThisCell(1, monitor.get_entidad(0)->getObjFil(0), monitor.get_entidad(0)->getObjCol(0)))
    {
      monitor.put_active_objetivos(1);
      monitor.get_entidad(0)->anularAlcanzados();
      monitor.get_entidad(0)->incrMisiones();
      monitor.get_entidad(0)->incrPuntuacion(7);
      monitor.get_entidad(1)->AsignarCall_ON(false); // Se deja de llamar al auxiliar
      for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
      {
        monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
      }
    }

    salida = true;
    break;
  }

  return salida;
}

bool actuacionAuxiliar(unsigned char celdaJ_inicial, unsigned char celdaJ_fin, int difAltJ, Action accion, unsigned int x, unsigned int y)
{
  int posibleElAvance;
  unsigned int mix, miy;
  unsigned char celdaRand;
  bool salida = false;
  int gasto;
  int error = 0; // 0 = NoError | 1 = NoEnergiaSuficiente | 2 = Colision

  // Primero evaluo si hay energía suficiente para realizar las acciones del rescatador.

  gasto = monitor.get_entidad(1)->fixBateria_sig_accion_jugador(celdaJ_inicial, difAltJ, accion);

  // std::cout << "Accion Rescatador: " << accion << "   Accion Colaborador: " << accionColaborador << endl;
  // std::cout << "Gasto Rescatador: " << gasto_Rescatador << "   Gasto Colaborador: " << gasto_colaborador << "   Gasto Total: " << gasto << endl;

  if (gasto > monitor.get_entidad(1)->getBateria())
  {
    cout << "La energía necesaria para las acciones seleccionadas superó a la energía disponible\n";
    error = 1;
    monitor.get_entidad(1)->setBateria(0);

    return false;
  }

  switch (accion)
  {
  case WALK:
    if (monitor.getMapa()->casillaOcupada(1) == -1 and abs(difAltJ) <= 1) // Casilla destino desocupada
    {
      switch (celdaJ_fin)
      {
      case 'M': // Muro
        monitor.get_entidad(1)->seAostio();
        std::cout << "El auxiliar ha chocado con un obstáculo\n";
        break;
      case 'B': // Arbol
        if (!monitor.get_entidad(1)->Has_Zapatillas())
        {
          monitor.get_entidad(1)->seAostio();
          std::cout << "El auxiliar ha chocado contra un árbol\n";
        }
        else
        {
          monitor.get_entidad(1)->setPosicion(x, y);
          salida = true;
        }
        break;
      case 'P': // Precipicio
        std::cout << "Se cayó por un precipicio\n";
        monitor.get_entidad(1)->resetEntidad();
        monitor.get_entidad(1)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
        break;
      case 'X': // Casilla Rosa (Puesto Base)
        monitor.get_entidad(1)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(1)->Cogio_Zapatillas(true);
        monitor.get_entidad(1)->setPosicion(x, y);
        salida = true;
        break;
      default:
        monitor.get_entidad(1)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.get_entidad(1)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(1)->allLessOneObjetivosAlcanzados())
      {
        if (monitor.getLevel() == 3)
        {
          // El auxiliar llegó a la casilla objetivo.
          cout << "-----> Casilla objetivo alcanzada por el auxiliar\n";
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        }
        else if(monitor.getLevel() == 2 or monitor.getLevel() == 4)
        {
          cout << "-----> En este nivel es el rescatador el que debe llegar a la casilla objetivo\n";
        }
      }
      // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    }
    else if (abs(difAltJ) > 1)
    {
      std::cout << "Demasiada altura entre las casillas\n"; // Hay demasiada altura entre casillas
      if (difAltJ > 0)                                      // Choca porque la casilla destino está demasiado alta.
      {
        monitor.get_entidad(1)->seAostio();
        std::cout << "El auxiliar ha chocado. Casilla destino demasiado alta\n";
      }
      else // Se cae porque la casilla destino está demasiado baja.
      {
        std::cout << "El auxiliar ha dado un salto al vacío. Demasiada diferencia de altura entre casillas\n";
        monitor.get_entidad(1)->resetEntidad();
        monitor.get_entidad(1)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
      }
    }
    else if (monitor.getMapa()->casillaOcupada(1) != -1) // Choca contra otro agente
    {
      // Choca contra una entidad
      monitor.get_entidad(1)->seAostio();
      if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(1))->getSubTipo() == excursionista)
      {
        monitor.get_entidad(1)->perderPV(0);
        std::cout << "El auxiliar ha chocado con un excursionista\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(1))->getTipo() == jugador)
      {
        monitor.get_entidad(1)->perderPV(0);
        std::cout << "El auxiliar ha chocado con el rescatador\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(1))->getSubTipo() == vandalo)
      {
        // Opcion reiniciarse en otro punto del mapa
        /*do
        {
          mix = aleatorio(monitor.getMapa()->getNFils() - 1);
          miy = aleatorio(monitor.getMapa()->getNCols() - 1);

          celdaRand = monitor.getMapa()->getCelda(mix, miy);
        } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

        monitor.get_entidad(1)->resetEntidad();
        monitor.get_entidad(1)->setPosicion(mix, miy);
        monitor.get_entidad(1)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
        monitor.get_entidad(1)->setHitbox(true);
        monitor.get_entidad(1)->Cogio_Zapatillas(false);*/

        // Opcion simplemente choca contra el vandalo
        monitor.get_entidad(1)->perderPV(0);
        std::cout << "El auxiliar ha chocado con un vandalo\n";
      }
      salida = false;
    }

    break;

  case RUN:
    std::cout << "La acción RUN no es aplicable al agente auxiliar\n";
    break;

  case TURN_SR:
    monitor.get_entidad(1)->giro45Dch();
    monitor.girarJugadorDerecha(45);
    salida = false;
    break;

  case TURN_L:
    std::cout << "La acción TURN_L no es aplicable al agente auxiliar\n";
    break;

  case PUSH:
    std::cout << "La acción PUSH no es aplicable al agente auxiliar\n";
    break;

  case CALL_ON:
    std::cout << "La acción CALL_ON no es aplicable al agente auxiliar\n";
    break;

  case CALL_OFF:
    std::cout << "La acción CALL_OFF no es aplicable al agente auxiliar\n";
    break;

  case IDLE:
    if (celdaJ_inicial == 'X' and (monitor.getLevel() == 1 or monitor.getLevel() == 4))
    { // Casilla Rosa (Recarga)
      monitor.get_entidad(1)->increaseBateria(10);
    }

    salida = true;
    break;
  }

  return salida;
}

bool actuacionNPC(unsigned int entidad, unsigned char celda, Action accion, unsigned int x, unsigned int y)
{
  bool out = false;
  int mix, miy;
  unsigned char celdaRand;

  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case excursionista: // Acciones para el excursionista
    switch (accion)
    {
    case WALK:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

      /*case TURN_R:
        monitor.get_entidad(entidad)->giroDch();
        out = true;
        break;*/

    case TURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case TURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

      /*case TURN_SL:
        monitor.get_entidad(entidad)->giro45Izq();
        out = true;
        break;*/
    }
    break;

  case vandalo: // Acciones para el vandalo
    switch (accion)
    {
    case WALK:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

      /*case TURN_R:
        monitor.get_entidad(entidad)->giroDch();
        out = true;
        break;*/

    case TURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case TURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

      /*case TURN_SL:
        monitor.get_entidad(entidad)->giro45Izq();
        out = true;
        break;*/

    case PUSH: // Esta accion para un vandalo es empujar equivalente a un actPUSH
      std::cout << "Recibido un empujón por un vandalo\n";
      bool esta_rescatadordelante = monitor.getMapa()->casillaOcupada(entidad) == 0;
      monitor.get_entidad(0)->seAostio();
      if (esta_rescatadordelante)
      {
        pair<int, int> casilla = monitor.getMapa()->NCasillasDelante(entidad, 2);
        if (monitor.getMapa()->QuienEnCasilla(casilla.first, casilla.second) == -1 and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'M' and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'P' and
            (monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'B')
          )
          if (aleatorio(1) == 0)
          { // Solo ocurre la mitad de las veces que el vandalo lo intenta.
            std::cout << "\tEl empujón ha sido efectivo\n";
            monitor.get_entidad(0)->setPosicion(casilla.first, casilla.second);
            monitor.get_entidad(0)->Increment_Empujones();
            monitor.get_entidad(entidad)->giroIzq();
            monitor.get_entidad(entidad)->giro45Izq();
          }
      }
      out = true;
      break;
    }
  }

  return out;
}

pair<int, int> NextCoordenadas(int f, int c, Orientacion brujula)
{
  switch (brujula)
  {
  case norte:
    f = f - 1;
    c = c;
    break;

  case noreste:
    f = f - 1;
    c = c + 1;
    break;

  case este:
    f = f;
    c = c + 1;
    break;

  case sureste:
    f = f + 1;
    c = c + 1;
    break;

  case sur:
    f = f + 1;
    c = c;
    break;

  case suroeste:
    f = f + 1;
    c = c - 1;
    break;

  case oeste:
    f = f;
    c = c - 1;
    break;

  case noroeste:
    f = f - 1;
    c = c - 1;
    break;
  }
  pair<int, int> coordenadas;
  coordenadas.first = f;
  coordenadas.second = c;
  return coordenadas;
}

bool actuacion(unsigned int entidad, Action accion)
{
  bool out = false;
  unsigned char celda_inicial, celda_fin, objetivo;
  unsigned int f, c;
  unsigned char altura_inicial, altura_fin;

  f = monitor.get_entidad(entidad)->getFil();
  c = monitor.get_entidad(entidad)->getCol();

  celda_inicial = monitor.getMapa()->getCelda(f, c);
  altura_inicial = monitor.getMapa()->alturaEnCelda(f, c);

  // Calculamos cual es la celda justo frente a la entidad
  pair<unsigned int, unsigned int> coord = NextCoordenadas(f, c, monitor.get_entidad(entidad)->getOrientacion());
  if (accion == RUN and monitor.get_entidad(entidad)->getSubTipo() == rescatador)
    coord = NextCoordenadas(coord.first, coord.second, monitor.get_entidad(entidad)->getOrientacion());
  f = coord.first;
  c = coord.second;
  celda_fin = monitor.getMapa()->getCelda(f, c);
  altura_fin = monitor.getMapa()->alturaEnCelda(f, c);

  // Dependiendo el tipo de la entidad actuamos de una forma u otra

  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case rescatador:
    out = actuacionRescatador(celda_inicial, celda_fin, altura_fin - altura_inicial, accion, f, c);
    break;

  case auxiliar:
    out = actuacionAuxiliar(celda_inicial, celda_fin, altura_fin - altura_inicial, accion, f, c);
    break;

  case excursionista:
    out = actuacionNPC(entidad, celda_fin, accion, f, c);
    break;

  case vandalo:
    out = actuacionNPC(entidad, celda_fin, accion, f, c);
    break;
  }

  return out;
}

void nucleo_motor_juego(MonitorJuego &monitor, int acc)
{
  Action accion;
  unsigned char celdaRand;

  // Para borrar despues
  /*  for (int fila = 0; fila < monitor.getMapa()->getNFils(); fila++){
      for (int col = 0; col < monitor.getMapa()->getNCols(); col++){
        if (monitor.getMapa()->getCelda(fila,col) == 'C')
          std::cout << monitor.getMapa()->alturaEnCelda(fila,col);
        else
          std::cout << " ";
      }
      cout << endl;
    }
    cout << endl;

    char ch;*/
  // cin >> ch;

  //====================================

  vector<vector<vector<unsigned char>>> estado;

  estado.clear();

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    estado.push_back(monitor.getMapa()->vision(i));
  }

  if (monitor.get_entidad(0)->ready())
  {

    monitor.init_casillas_especiales(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol(), monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol());

    clock_t t0 = clock();
    accion = monitor.get_entidad(0)->think(acc, estado[0], monitor.getLevel());
    clock_t t1 = clock();

    monitor.get_entidad(0)->addTiempo(t1 - t0);
    monitor.get_entidad(0)->setLastAction(accion);
    actuacion(0, accion);
  }
  else
  {
    monitor.get_entidad(0)->perderPV(0);
  }

  for (unsigned int i = 1; i < monitor.numero_entidades(); i++)
  {
    clock_t t0 = clock();
    accion = monitor.get_entidad(i)->think(acc, estado[i], monitor.getLevel());
    clock_t t1 = clock();

    monitor.get_entidad(i)->addTiempo(t1 - t0);
    monitor.get_entidad(i)->setLastAction(accion);
    actuacion(i, accion);
  }

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    monitor.get_entidad(i)->setVision(monitor.getMapa()->vision(i));
  }

  monitor.get_entidad(0)->decBateria_sig_accion();
  monitor.get_entidad(1)->decBateria_sig_accion();

  monitor.decPasos();

  if (acc != -1)
  {
    // sleep(monitor.getRetardo() / 10);
  }

  // Verificar si se dan las condiciones de salida con éxito de la simulacion
  if (!monitor.get_entidad(0)->fin() and !monitor.get_entidad(1)->fin())
  {
    // Reviso si se dan las condiciones de éxito en el nivel 0
    switch (monitor.getLevel())
    {
    case 0: // Nivel 0 -> Reactivo los dos en un puesto base
      if (monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) == 'X' and !monitor.getRecargaPisadaRescatador())
      {
        monitor.setRecargaPisadaRescatador(true);
        cout << "El rescatador ha alcanzado un puesto base." << endl;
      }
      if (monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) == 'X' and !monitor.getRecargaPisadaAuxiliar())
      {
        monitor.setRecargaPisadaAuxiliar(true);
        cout << "El auxiliar ha alcanzado un puesto base." << endl;
      }
      if (monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) != 'X' and monitor.getRecargaPisadaRescatador() and !monitor.getRecargaAbandonadaRescatador())
      {
        monitor.setRecargaAbandonadaRescatador(true);
        cout << "El rescatador ha abandonado el puesto base???" << endl;
      }
      if (monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) != 'X' and monitor.getRecargaPisadaAuxiliar() and !monitor.getRecargaAbandonadaAuxiliar())
      {
        monitor.setRecargaAbandonadaAuxiliar(true);
        cout << "El auxiliar ha abandonado el puesto base???" << endl;
      }

      if(monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) != 'X' and
         monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) != 'D' and
         monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) != 'C' and
         !monitor.getCaminoAbandonadoRescatador())
      {
        monitor.setCaminoAbandonadoRescatador(true);
        cout << "WARNING: El rescatador se ha salido del camino." << endl;
      }

      if (monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) != 'X' and
          monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) != 'D' and
          monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) != 'C' and
          (!monitor.get_entidad(1)->Has_Zapatillas() or  monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) != 'B') and
          !monitor.getCaminoAbandonadoAuxiliar())
      {
        monitor.setCaminoAbandonadoAuxiliar(true);
        cout << "WARNING: El auxiliar se ha salido del camino." << endl;
      }

      if (monitor.getMapa()->getCelda(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol()) == 'X' and
          monitor.getMapa()->getCelda(monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol()) == 'X')
      {
        // monitor.get_entidad(0)->setFin(true);
        // monitor.get_entidad(1)->setFin(true);
        cout << "Misión completada: ambos agentes han alcanzado el puesto base." << endl;
        if (monitor.getRecargaPisadaRescatador())
        {
          cout << "El rescatador ha alcanzado un puesto base." << endl;
        }
        if (monitor.getRecargaPisadaAuxiliar())
        {
          cout << "El auxiliar ha alcanzado un puesto base." << endl;
        }
        if (monitor.getRecargaAbandonadaRescatador())
        {
          cout << "El rescatador ha abandonado el puesto base???" << endl;
        }
        if (monitor.getRecargaAbandonadaAuxiliar())
        {
          cout << "El auxiliar ha abandonado el puesto base???" << endl;
        }

        if(monitor.getCaminoAbandonadoRescatador())
        {
          cout << "WARNING: El rescatador se ha salido del camino." << endl;
        }
        if (monitor.getCaminoAbandonadoAuxiliar())
        {
          cout << "WARNING: El auxiliar se ha salido del camino." << endl;
        }

        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
      }
      break;
    case 1: // Nivel 1 -> Termina cuando se agotan los recursos de ciclos, tiempo o energía.
      break;
    case 2: // Nivel 2 -> Termina cuando el agente rescatador llega a la casilla objetivo.
      break;
    case 3: // Nivel 3 -> Termina cuando el agente auxiliar llega a la casilla objetivo.
      break;
    case 4: // Nivel 4 -> Termina cuando se agontan los recursos de ciclos, tiempo o energia.
      break;
    }
  }
}

bool lanzar_motor_juego(int &colisiones, int acc)
{
  bool out = false;

  if (monitor.continuarBelkan())
  {
    if (monitor.jugar())
    {
      if ((monitor.getPasos() != 0) and (!monitor.finJuego()))
      {
        nucleo_motor_juego(monitor, acc);
      }
    }

    if (monitor.mostrarResultados())
    {
      std::cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
      std::cout << "Tiempo Consumido: " << (1.0 * monitor.get_entidad(1)->getTiempo() + monitor.get_entidad(1)->getTiempo()) / CLOCKS_PER_SEC << endl;
      std::cout << "Nivel Final de Energía (Rescatador): " << monitor.get_entidad(0)->getBateria() << endl;
      std::cout << "Nivel Final de Energía (Auxiliar): " << monitor.get_entidad(1)->getBateria() << endl;
      std::cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() + monitor.get_entidad(1)->getColisiones() << endl;
      std::cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
      std::cout << "Porcentaje descubierto de caminos y senderos: " << monitor.CoincidenciaConElMapaCaminosYSenderos() << endl;
      std::cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
      std::cout << "Objetivos encontrados: (" << monitor.get_entidad(0)->getMisiones() << ") " << monitor.get_entidad(0)->getPuntuacion() << endl;
      monitor.setMostrarResultados(false);

      out = true;
    }
  }
  return out;
}

void lanzar_motor_juego2(MonitorJuego &monitor)
{

  // monitor.get_entidad(0)->setObjetivo(monitor.getObjFil(), monitor.getObjCol());

  while (!monitor.finJuego() && monitor.jugar())
  {
    nucleo_motor_juego(monitor, -1);
  }

  if (monitor.mostrarResultados() and (monitor.getLevel() == 0))
  {
    std::cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
    std::cout << "Coste de Energía (Rescatador): " << 3000 - monitor.get_entidad(0)->getBateria() << endl;
    std::cout << "Coste de Energía (Auxiliar): " << 3000 - monitor.get_entidad(1)->getBateria() << endl;

    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados() and (monitor.getLevel() == 1))
  {
    std::cout << "Porcentaje descubierto de caminos y senderos: " << monitor.CoincidenciaConElMapaCaminosYSenderos() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados() and (monitor.getLevel() == 2))
  {
    std::cout << "Longitud del camino (Rescatador): " << 3000 - monitor.get_entidad(0)->getInstantesPendientes() << endl;
    std::cout << "Coste de Energía (Rescatador): " << 3000 - monitor.get_entidad(0)->getBateria() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados() and monitor.getLevel() == 3)
  {
    std::cout << "Longitud del camino (Auxiliar): " << 3000 - monitor.get_entidad(1)->getInstantesPendientes() << endl;
    std::cout << "Coste de Energía (Auxiliar): " << 3000 - monitor.get_entidad(1)->getBateria() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados())
  {
    std::cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
    std::cout << "Tiempo Consumido: " << (1.0 * monitor.get_entidad(1)->getTiempo() + monitor.get_entidad(1)->getTiempo()) / CLOCKS_PER_SEC << endl;
    std::cout << "Nivel Final de Energía (Rescatador): " << monitor.get_entidad(0)->getBateria() << endl;
    std::cout << "Nivel Final de Energía (Auxiliar): " << monitor.get_entidad(1)->getBateria() << endl;
    std::cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() + monitor.get_entidad(1)->getColisiones() << endl;
    std::cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
    std::cout << "Porcentaje descubierto de caminos y senderos: " << monitor.CoincidenciaConElMapaCaminosYSenderos() << endl;
    std::cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
    std::cout << "Objetivos encontrados: (" << monitor.get_entidad(0)->getMisiones() << ") " << monitor.get_entidad(0)->getPuntuacion() << endl;
    monitor.setMostrarResultados(false);
  }
}
