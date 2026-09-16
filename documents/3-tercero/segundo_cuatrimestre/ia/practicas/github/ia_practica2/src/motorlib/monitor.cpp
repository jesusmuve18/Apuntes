#include "motorlib/monitor.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

void MonitorJuego::cambiarOrientacion(unsigned char entidad, unsigned char orientacion)
{
  Orientacion aux;
  switch (orientacion)
  {
  case 0:
    aux = norte;
    break;
  case 1:
    aux = noreste;
    break;
  case 2:
    aux = este;
    break;
  case 3:
    aux = sureste;
    break;
  case 4:
    aux = sur;
    break;
  case 5:
    aux = suroeste;
    break;
  case 6:
    aux = oeste;
    break;
  case 7:
    aux = noroeste;
    break;
  }
  entidades[entidad]->setOrientacion(aux);
}

void MonitorJuego::setMapa(const char *file)
{
  ifstream ifile;
  ifile.open(file, ios::in);

  unsigned int colSize, filSize;
  ifile >> colSize;
  ifile >> filSize;

  vector<vector<unsigned char>> mapAuxTerreno;
  vector<vector<unsigned char>> mapAuxAlturas;

  vector<unsigned char> colAuxTerreno(colSize);
  vector<unsigned char> colAuxAlturas(colSize);

  for (unsigned int i = 0; i < filSize; i++)
  {
    mapAuxTerreno.push_back(colAuxTerreno);
    mapAuxAlturas.push_back(colAuxAlturas);
  }

  // Lee la parte de terreno del fichero
  for (unsigned int i = 0; i < filSize; i++)
  {
    for (unsigned int j = 0; j < colSize; j++)
    {
      ifile >> mapAuxTerreno[i][j];
    }
  }

  // Lee la parte de alturas del terreno del fichero
  for (unsigned int i = 0; i < filSize; i++)
  {
    for (unsigned int j = 0; j < colSize; j++)
    {
      ifile >> mapAuxAlturas[i][j];
    }
  }
  ifile.close();

  mapa = new Mapa(mapAuxTerreno, mapAuxAlturas, &entidades);
}

bool MonitorJuego::inicializarJuego()
{
  bool aux = empezarJuego;
  if (empezarJuego)
  {
    empezarJuego = false;
    resultados = false;
  }
  return aux;
}

void MonitorJuego::generate_a_valid_cell(int &pos_fila, int &pos_col, int &ori)
{
  pos_col = -1;
  pos_fila = -1;
  char celdaRand = '_';
  do
  {
    pos_fila = aleatorio(getMapa()->getNFils() - 1);
    pos_col = aleatorio(getMapa()->getNCols() - 1);

    celdaRand = getMapa()->getCelda(pos_fila, pos_col);
  } while ((celdaRand == 'P' or celdaRand == 'M'));
  ori = aleatorio(7);
}

bool MonitorJuego::is_a_valid_cell_like_goal(int f, int c)
{
  if (f < 3 or f + 3 >= getMapa()->getNFils())
    return false;
  if (c < 3 or c + 3 >= getMapa()->getNCols())
    return false;
  char cell = getMapa()->getCelda(f, c);
  if (cell == 'M' or cell == 'P')
    return false;
  return true;
}

void MonitorJuego::generate_a_objetive()
{
  int pos_col = -1, pos_fila = -1;
  char celdaRand = '_';
  do
  {
    pos_fila = aleatorio(getMapa()->getNFils() - 1);
    pos_col = aleatorio(getMapa()->getNCols() - 1);

    celdaRand = getMapa()->getCelda(pos_fila, pos_col);
  } while ((celdaRand == 'P' or celdaRand == 'M' or celdaRand == 'B'));

  pair<pair<int, int>, bool> punto;
  punto.first.first = pos_fila;
  punto.first.second = pos_col;
  punto.second = aleatorio(2);
  objetivos.push_back(punto);
}

void MonitorJuego::put_a_new_objetivo_front(int fila, int columna, int gravedad)
{
  pair<pair<int, int>, bool> punto;
  punto.first.first = fila;
  punto.first.second = columna;
  punto.second = gravedad;
  objetivos.push_front(punto);
}

// Pasa de la lista de objetivos al vector de objetivos activos.
// 'number' establece el número de objetivos que se fijan simultáneamente
void MonitorJuego::put_active_objetivos(int number)
{
  vector<unsigned int> v;

  if (mapa != 0)
  {
    while (objetivos.size() < number)
    {
      generate_a_objetive();
    }
    for (int i = 0; i < number; i++)
    {
      auto it = objetivos.begin();
      if (this->get_entidades()->size() > 1)
      {
        while ((this->get_entidad(0)->getFil() == it->first.first and this->get_entidad(0)->getCol() == it->first.second) or
               (this->get_entidad(1)->getFil() == it->first.first and this->get_entidad(1)->getCol() == it->first.second))
        {
          objetivos.erase(objetivos.begin());
          it = objetivos.begin();
        }
      }
      v.push_back(it->first.first);
      v.push_back(it->first.second);
      v.push_back(it->second);

      objetivos.erase(objetivos.begin());
    }
  }
  else
  {
    v.push_back(3);
    v.push_back(3);
    v.push_back(0);
  }
  objetivosActivos = v;
}

void MonitorJuego::get_n_active_objetivo(int n, int &posFila, int &posCol, int &gravedad)
{

  posFila = objetivosActivos[0];
  posCol = objetivosActivos[1];
  gravedad = objetivosActivos[2];
}

void MonitorJuego::set_n_active_objetivo(int n, int posFila, int posCol, int gravedad)
{

  objetivosActivos[0] = posFila;
  objetivosActivos[1] = posCol;
  objetivosActivos[2] = gravedad;

  if (numero_entidades() > 0)
    get_entidad(0)->setObjetivos(objetivosActivos);
}

bool MonitorJuego::there_are_active_objetivo()
{
  return (objetivosActivos.size() > 0);
}

int MonitorJuego::get_number_active_objetivos()
{
  return (objetivosActivos.size() / 3);
}

vector<unsigned int> MonitorJuego::get_active_objetivos()
{
  vector<unsigned int> copia = objetivosActivos;
  return copia;
}

void MonitorJuego::decPasos()
{
  if (get_entidad(0)->fin() or get_entidad(1)->fin())
  {
    if (nivel < 4)
    {
      jugando = false;
      resultados = true;
    }
    else
    {
      // Nivel 4
      if (!get_entidad(0)->vivo() or
          !get_entidad(1)->vivo() or
          get_entidad(0)->getBateria() == 0 or
          get_entidad(1)->getBateria() == 0 or
          get_entidad(0)->getTiempo() + get_entidad(1)->getTiempo() > get_entidad(0)->getTiempoMaximo())
      {
        jugando = false;
        resultados = true;
      }
      else
      {
        // Nuevo destino
        if (get_entidad(0)->allObjetivosAlcanzados())
        {
          /*put_active_objetivos(3);
          get_entidad(0)->anularAlcanzados();
          get_entidad(0)->setObjetivos(get_active_objetivos());*/
          // setObjCol(pos_col);
          // setObjFil(pos_fila);
          // if (editPosC != NULL)
          // editPosC->set_int_val(pos_col);
          // if (editPosF != NULL)
          // editPosF->set_int_val(pos_fila);
          // cout << "Nuevo objetivo: (" << pos_fila << "," << pos_col << ")" << endl;
          get_entidad(0)->resetFin();
        }

        if (pasos > 0)
        {
          pasos--;
          pasosTotales++;
        }
      }
    }
  }
  else
  {
    if (pasos > 0)
    {
      pasos--;
      pasosTotales++;
    }

    if (!get_entidad(0)->vivo() or get_entidad(0)->getBateria() == 0 or !get_entidad(1)->vivo() or get_entidad(1)->getBateria() == 0)
    {
      jugando = false;
      resultados = true;
    }
  }
}

bool MonitorJuego::CanHeSeesThisCell(int num_entidad, int fil, int col)
{
	bool salida = false;
  int agenteF = get_entidad(num_entidad)->getFil();
  int agenteC = get_entidad(num_entidad)->getCol();
  Orientacion agenteRumbo = get_entidad(num_entidad)->getOrientacion();

	int df = agenteF - fil;
	int dc = agenteC - col;
	int absf = (df < 0 ? -df : df);
	int absc = (dc < 0 ? -dc : dc);

	if (absf > 3 or absc > 3)
		return false;

	switch (agenteRumbo)
	{
	case norte:
		if (agenteF > fil and absc <= absf)
		{
			salida = true;
		}
		break;
	case noreste:
		if (agenteF >= fil and dc <= 0)
		{
			salida = true;
		}
		break;
	case este:
		if (agenteC < col and absf <= absc)
		{
			salida = true;
		}
		break;
	case sureste:
		if (agenteF <= fil and dc <= 0)
		{
			salida = true;
		}
		break;
	case sur:
		if (agenteF < fil and absc <= absf)
		{
			salida = true;
		}
		break;
	case suroeste:
		if (agenteF <= fil and dc >= 0)
		{
			salida = true;
		}
		break;
	case oeste:
		if (agenteC > col and absf <= absc)
		{
			salida = true;
		}
		break;
	case noroeste:
		if (agenteF >= fil and dc >= 0)
		{
			salida = true;
		}
		break;
	}
	return salida;
}

void MonitorJuego::inicializar(int pos_filaJ, int pos_colJ, int brujJ, int pos_filaS, int pos_colS, int brujS, int seed)
{
  clear();
  // cout << "Estoy en MonitorJuego::inicializar\n";
  int tama = getMapa()->getNCols();

  int nexcursionistas = 4;
  int nvandalos = 4;
  // int nexcursionistas = tama / 10;
  // int nvandalos = tama / 10;
  unsigned char celdaRand;

  // Se construye una lisa con 300 objetivos
  if (nivel == 4)
  {
    while (objetivos.size() < 300)
    {
      generate_a_objetive();
    }
  }

  // Primero SIEMPRE se coloca al rescatador. SIEMPRE.
  if ((pos_filaJ == -1) or (pos_colJ == -1))
  {
    do
    {
      pos_filaJ = aleatorio(getMapa()->getNFils() - 1);
      pos_colJ = aleatorio(getMapa()->getNCols() - 1);
      celdaRand = getMapa()->getCelda(pos_filaJ, pos_colJ);
    } while ((celdaRand == 'P') or (celdaRand == 'M') or (celdaRand == 'B') or (getMapa()->entidadEnCelda(pos_filaJ, pos_colJ) != '_'));
  }
  else
  {
    // para hacer que la secuencia de recogida de numeros aleatorios sea independiente si da o no las coordenadas al principio
    int kkx = aleatorio(getMapa()->getNFils() - 1);
    int kky = aleatorio(getMapa()->getNCols() - 1);
  }

  // El segundo SIEMPRE es el auxiliar. Siempre. SIEMPRE.
  if ((pos_filaS == -1) or (pos_colS == -1))
  {
    do
    {
      pos_filaS = aleatorio(getMapa()->getNFils() - 1);
      pos_colS = aleatorio(getMapa()->getNCols() - 1);
      celdaRand = getMapa()->getCelda(pos_filaS, pos_colS);
    } while ((celdaRand == 'P') or (celdaRand == 'M') or (celdaRand == 'B') or (getMapa()->entidadEnCelda(pos_filaS, pos_colS) != '_'));
  }
  else
  {
    // para hacer que la secuencia de recogida de numeros aleatorios sea independiente si da o no las coordenadas al principio
    int kkx = aleatorio(getMapa()->getNFils() - 1);
    int kky = aleatorio(getMapa()->getNCols() - 1);
  }

  // Pongo los primeros objetivos objetivosActivos
  put_active_objetivos(1);

  // Para los niveles 2 y 3 hago visible el mapa
  if ((nivel == 2) or (nivel == 3))
  {
    vector<vector<unsigned char>> mAux(getMapa()->getNFils(), vector<unsigned char>(getMapa()->getNCols(), '?'));
    for (int i = 0; i < getMapa()->getNFils(); i++)
      for (int j = 0; j < getMapa()->getNCols(); j++)
        mAux[i][j] = getMapa()->getCelda(i, j);

    vector<vector<unsigned char>> mAuxC(getMapa()->getNFils(), vector<unsigned char>(getMapa()->getNCols(), 0));
    for (int i = 0; i < getMapa()->getNFils(); i++)
      for (int j = 0; j < getMapa()->getNCols(); j++)
        mAuxC[i][j] = getMapa()->alturaEnCelda(i, j);
    

    // Esto coloca a los agentes en el mapa

    nueva_entidad(new Entidad(jugador, rescatador, static_cast<Orientacion>(brujJ), pos_filaJ, pos_colJ, new Jugador3D(""), new ComportamientoRescatador(mAux, mAuxC), 1, objetivosActivos, 3000));
    nueva_entidad(new Entidad(npc, auxiliar, static_cast<Orientacion>(brujS), pos_filaS, pos_colS, new Colaborador3D(""), new ComportamientoAuxiliar(mAux, mAuxC), 1, objetivosActivos, 3000));
  }
  else
  {
    // Esto coloca al jugador en el mapa
    if (brujJ == -1)
      brujJ = rand() % 8;
    if (brujS == -1)
      brujS = rand() % 8;

    nueva_entidad(new Entidad(jugador, rescatador, static_cast<Orientacion>(brujJ), pos_filaJ, pos_colJ, new Jugador3D(""), new ComportamientoRescatador(getMapa()->getNFils()), 1, objetivosActivos, 3000));
    nueva_entidad(new Entidad(npc, auxiliar, static_cast<Orientacion>(brujS), pos_filaS, pos_colS, new Colaborador3D(""), new ComportamientoAuxiliar(getMapa()->getNFils()), 1, objetivosActivos, 3000));
  }

  ReAparicionesEntidad(0, pos_filaJ, pos_colJ, static_cast<Orientacion>(brujJ));
  ReAparicionesEntidad(1, pos_filaS, pos_colS, static_cast<Orientacion>(brujS));
  get_entidad(0)->setLastAction(IDLE);
  get_entidad(1)->setLastAction(IDLE);

  // Para el último nivel genero los excursionistas y vandalos
  int pos_filaO, pos_colO;
  if (nivel == 4)
  {
    for (int i = 0; i < nexcursionistas; i++)
    {
      do
      {
        pos_filaO = aleatorio(getMapa()->getNFils() - 1);
        pos_colO = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_filaO, pos_colO);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaO, pos_colO) != '_'));
      nueva_entidad(new Entidad(npc, excursionista, static_cast<Orientacion>(aleatorio(7)), pos_filaO, pos_colO, new Aldeano3D(""), new ComportamientoExcursionista(), 1, objetivosActivos, 3000));
    }

    for (int i = 0; i < nvandalos; i++)
    {
      do
      {
        pos_filaO = aleatorio(getMapa()->getNFils() - 1);
        pos_colO = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_filaO, pos_colO);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaO, pos_colO) != '_'));
      nueva_entidad(new Entidad(npc, vandalo, static_cast<Orientacion>(aleatorio(7)), pos_filaO, pos_colO, new Aldeano3D(""), new ComportamientoVandalo(), 1, objetivosActivos, 3000));
    }
  }

  get_entidad(0)->setVision(getMapa()->vision(0));
  get_entidad(1)->setVision(getMapa()->vision(1));

  // Se conoce en todos los niveles la posición y orientación de los dos.
  // if ((nivel == 0) or (nivel == 1) or (nivel == 2) or (nivel == 3))
  get_entidad(0)->notify();
  get_entidad(1)->notify();

  /* quitar esto una vez verificado */
  // PintaEstadoMonitor();

  // Avisos de colocación incorrecta de los agentes al principio del juego según los niveles
  switch (nivel)
  {
  case 0:
    if (getMapa()->getCelda(get_entidad(0)->getFil(), get_entidad(0)->getCol()) != 'C')
      std::cout << "Error en la colocación del agente Rescatador en el nivel 0\n";
    if (getMapa()->getCelda(get_entidad(1)->getFil(), get_entidad(1)->getCol()) != 'C')
      std::cout << "Error en la colocación del agente Auxiliar en el nivel 0\n";
    break;
  case 1:
    if (getMapa()->getCelda(get_entidad(0)->getFil(), get_entidad(0)->getCol()) != 'C' and getMapa()->getCelda(get_entidad(0)->getFil(), get_entidad(0)->getCol()) != 'S')
      std::cout << "Error en la colocación del agente Rescatador en el nivel 1\n";
    if (getMapa()->getCelda(get_entidad(1)->getFil(), get_entidad(1)->getCol()) != 'C' and getMapa()->getCelda(get_entidad(1)->getFil(), get_entidad(1)->getCol()) != 'S')
      std::cout << "Error en la colocación del agente Auxiliar en el nivel 1\n";
    break;
  }
}

void MonitorJuego::ReAparicionesEntidad(int Entidad, int fila, int columna, Orientacion brujula)
{
  get_entidad(Entidad)->setPosicion(fila, columna);
  get_entidad(Entidad)->setOrientacion(brujula);
  get_entidad(Entidad)->setHitbox(true);
  get_entidad(Entidad)->Cogio_Zapatillas(false);
  switch (getMapa()->getCelda(fila, columna))
  {
  case 'X': // Casilla Rosa (Recarga)
    get_entidad(Entidad)->increaseBateria(10);
    break;

  case 'D': // Casilla Morada (Zapatillas)
    get_entidad(Entidad)->Cogio_Zapatillas(true);
    break;
  }
}

void MonitorJuego::PintaEstadoMonitor()
{
  cout << "*********************************************\n";
  cout << "Pos Fil: " << get_entidad(0)->getFil() << endl;
  cout << "Pos Col: " << get_entidad(0)->getCol() << endl;
  cout << "Brujula: " << get_entidad(0)->getOrientacion() << endl;
  for (int i = 0; i < get_entidad(0)->getNumObj(); i++)
  {
    cout << "Obj " << i << " : F: " << get_entidad(0)->getObjFil(i) << "  C: " << get_entidad(0)->getObjCol(i) << endl;
  }
  cout << "excursionistas: " << numero_entidades() << endl;
  for (int i = 1; i < numero_entidades(); i++)
  {
    cout << "  Pos Fil: " << get_entidad(i)->getFil() << endl;
    cout << "  Pos Col: " << get_entidad(i)->getCol() << endl;
    cout << "  Brujula: " << get_entidad(i)->getOrientacion() << endl
         << endl;
  }
  cout << "*********************************************\n";
}

double MonitorJuego::CoincidenciaConElMapa()
{
  int aciertos = 0, totalCasillas = 0;
  vector<vector<unsigned char>> resultado = get_entidad(0)->getMapaResultado();
  getMapa()->JoinMapasSuperficie(resultado, get_entidad(1)->getMapaResultado());

  for (unsigned int i = 0; i < getMapa()->getNFils(); i++)
  {
    for (unsigned int j = 0; j < getMapa()->getNCols(); j++)
    {
      if (getMapa()->getCelda(i, j) == resultado[i][j])
      {
        aciertos++;
      }
      else if (resultado[i][j] != '?')
      { // Puso un valor distinto de desconocido en mapaResultado y no acertó
        aciertos--;
      }
      totalCasillas++;
    }
  }
  return (aciertos * 100.0 / totalCasillas);
}

double MonitorJuego::CoincidenciaConElMapaCaminosYSenderos()
{
  int aciertos = 0, totalCasillas = 0;
  vector<vector<unsigned char>> resultado = get_entidad(0)->getMapaResultado();
  getMapa()->JoinMapasSuperficie(resultado, get_entidad(1)->getMapaResultado());

  for (unsigned int i = 0; i < getMapa()->getNFils(); i++)
  {
    for (unsigned int j = 0; j < getMapa()->getNCols(); j++)
    {
      if (getMapa()->getCelda(i, j) == 'C' or getMapa()->getCelda(i, j) == 'S')
      {
        if (getMapa()->getCelda(i, j) == resultado[i][j])
        {
          aciertos++;
        }
        else if (resultado[i][j] != '?')
        { // Puso un valor distinto de desconocido en mapaResultado y no acertó
          aciertos--;
        }
        totalCasillas++;
      }
    }
  }
  return (aciertos * 100.0 / totalCasillas);
}

string MonitorJuego::strAccion(Action accion)
{
  string out = "";

  switch (accion)
  {
  case WALK:
    out = "WALK";
    break;
  case RUN:
    out = "RUN";
    break;
  case TURN_L:
    out = "TURN_L";
    break;
  case TURN_SR:
    out = "TURN_SR";
    break;
  case PUSH:
    out = "PUSH";
    break;
  case CALL_OFF:
    out = "CALL_OFF";
    break;
  case CALL_ON:
    out = "CALL_ON";
    break;
  case IDLE:
    out = "IDLE";
    break;
  }

  return out;
}

string paraDonde(const Orientacion &x)
{
  string aux;
  switch (x)
  {
  case norte:
    aux = "norte";
    break;
  case noreste:
    aux = "noreste";
    break;
  case este:
    aux = "este ";
    break;
  case sureste:
    aux = "sureste ";
    break;
  case sur:
    aux = "sur  ";
    break;
  case suroeste:
    aux = "suroeste  ";
    break;
  case oeste:
    aux = "oeste";
    break;
  case noroeste:
    aux = "noroeste";
    break;
  }
  return aux;
}

std::string justificarDerechaNumero(int numero, int longitud)
{
  std::stringstream ss;
  ss << std::setw(longitud) << std::right << numero;
  return ss.str();
}

std::string justificarDerechaCadena(std::string texto, int longitud)
{
  std::stringstream ss;
  ss << std::setw(longitud) << std::right << texto;
  return ss.str();
}

std::string formatearReal(double numero, int p) {
    std::stringstream ss;
    ss << std::right << std::setw(p) << numero;
    return ss.str();
}

string MonitorJuego::toString()
{
  string aux, aux1, saux;

  string str;

  aux = justificarDerechaNumero(get_entidad(0)->getInstantesPendientes(), 10);
  str += "Tiempo restante: " + aux + "\n";
  str += "                  Rescatador   |       Auxiliar\n";
  str += "Energia   " + justificarDerechaNumero(get_entidad(0)->getBateria(), 16) + "    | " + justificarDerechaNumero(get_entidad(1)->getBateria(), 16) + "\n";

  string paDondeR = paraDonde(get_entidad(0)->getOrientacion());
  string paDondeA = paraDonde(get_entidad(1)->getOrientacion());

  aux = "F:" + justificarDerechaNumero(get_entidad(0)->getFil(), 2) + " C:" + justificarDerechaNumero(get_entidad(0)->getCol(), 2) + " " + paDondeR;
  aux1 = "F:" + justificarDerechaNumero(get_entidad(1)->getFil(), 2) + " C:" + justificarDerechaNumero(get_entidad(1)->getCol(), 2) + " " + paDondeA;
  str += "Pos       " + justificarDerechaCadena(aux, 16) + " |" + justificarDerechaCadena(aux1, 16) + "\n";

  Action ac = get_entidad(0)->getLastAction();
  str += "Ultima Accion " + justificarDerechaCadena(strAccion(ac), 12);
  ac = get_entidad(1)->getLastAction();
  str += " | " + justificarDerechaCadena(strAccion(ac), 12) + "\n";

  str += "Tiempo        " + formatearReal(get_entidad(0)->getTiempo() / CLOCKS_PER_SEC, 13) + " | " + formatearReal(get_entidad(1)->getTiempo() / CLOCKS_PER_SEC, 12) + "\n";

  string saux2;
  if (get_entidad(0)->Has_Zapatillas())
  {
    saux2 = " ZAPATILLAS ";
  }
  else
  {
    saux2 = "----------- ";
  }

  string saux3;
  if (get_entidad(1)->Has_Zapatillas())
  {
    saux3 = " ZAPATILLAS ";
  }
  else
  {
    saux3 = "----------- ";
  }

  str += "     " + justificarDerechaCadena(saux2, 12) + " | " + justificarDerechaCadena(saux3, 12) + "\n";

  str += "Misiones: " + justificarDerechaNumero(get_entidad(0)->getMisiones(), 3) + "  Puntuacion:  " + justificarDerechaNumero(get_entidad(0)->getPuntuacion(), 3) + "\n";

  vector<vector<unsigned char>> visionAux = getMapa()->vision(0);
  vector<vector<unsigned char>> visionAux1 = getMapa()->vision(1);

  if (visionAux.size() > 0)
  {
    // str += "************ Vision **************\n";

    // Vector Sensorial que contiene a los agentes
    for (unsigned int i = 0; i < visionAux[1].size(); i++)
    {
      str += visionAux[1][i];
    }

    str += " | ";

    for (unsigned int i = 0; i < visionAux1[1].size(); i++)
    {
      str += visionAux1[1][i];
    }
    str += "\n";

    // Vector Sensorial que contiene el terreno
    for (unsigned int i = 0; i < visionAux[0].size(); i++)
    {
      str += visionAux[0][i];
    }

    str += " | ";

    for (unsigned int i = 0; i < visionAux1[0].size(); i++)
    {
      str += visionAux1[0][i];
    }

    str += "\n";

    // Vector Sensorial que contiene la altura
    for (unsigned int i = 0; i < visionAux[0].size(); i++)
    {
      str += visionAux[2][i];
    }

    str += " | ";

    for (unsigned int i = 0; i < visionAux1[0].size(); i++)
    {
      str += visionAux1[2][i];
    }

    str += "\n";
  }

  return str;
}

void MonitorJuego::init_casillas_especiales(unsigned int f, unsigned int c, unsigned int fcolab, unsigned int ccolab)
{
  unsigned char celda_inicial, celda_colab;
  celda_inicial = getMapa()->getCelda(f, c);
  celda_colab = getMapa()->getCelda(fcolab, ccolab);
  if (celda_inicial == 'D')
  {
    get_entidad(0)->Cogio_Zapatillas(true);
  }

  if (celda_colab == 'D')
  {
    get_entidad(1)->Cogio_Zapatillas(true);
  }
}

void MonitorJuego::reset_objetivos()
{
  if (!objetivos.empty())
  {
    objetivos.clear();
  }
  if (!objetivosActivos.empty())
  {
    objetivosActivos.clear();
  }
}