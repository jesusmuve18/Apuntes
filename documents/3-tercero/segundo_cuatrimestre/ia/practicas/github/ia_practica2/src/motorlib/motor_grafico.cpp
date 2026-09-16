#include "motorlib.hpp"

#include <glui/glui.h>
#include <dirent.h>

MonitorJuego monitor;

// variables que determninan la posicion y tamaño de la ventana X
const int IU_ancho_ventana = 1024;
const int IU_alto_ventana = 768;

// Variables que controlan las ventanas y vistas
GLuint ventanaPrincipal, vistaPrincipal, vistaMiniMapa, vistaIU;
GLUI *panelSelecMapa, *panelSelecMapaConfig;
GLUI_Listbox *listbox;
GLUI_RadioGroup *group;
GLUI_Button *botonElegirMapa, *botonConfigurar, *botonPaso, *botonEjecucion, *botonEjecutar, *botonSalir;
GLUI_EditText *editPosColumna, *editPosFila, *editTextPasos, *editTextRetardo, *editGravedad;
GLUI_Checkbox *drawMM, *drawMMVista, *drawMMWho, *check_Gravedad;
GLUI_StaticText *lineaVacia, *info0, *info1, *info2, *info3, *info4, *info5, *info6, *info7, *info8, *info9, *info10;
GLUI_Spinner *setup1, *setup20, *setup21, *setup30, *setup31, *setup4, *setup5, *setup4_5, *setup60, *setup61;
// GLUI_Spinner *editPosColumna, *editPosFila;

int nPasos = 10, tRetardo = 1, MMmode = 0, MMmode2 = 0, MMmode3 = 0, PosColumna = 1, PosFila = 1, tMap = 100, colisiones = 0;
int Gravedad;
int objtiveSelected = 0;
int ultimomapaPos = 0, ultimonivel = 0;
int ObjFil1 = -1, ObjFil2 = -1, ObjFil3, ObjCol1 = -1, ObjCol2, ObjCol3;

void irAlJuego(int valor);

/*void keyboard(unsigned char key, int x, int y) {
  float a;
  switch(key) {
    case 'w': // UP
      a = monitor.getMapa()->getAngle();
      a = a + 0.01;
      cout << "Y1: " << a << endl;
      monitor.getMapa()->putAngle(a);
      break;

    case 's': // DCHA
      a = monitor.getMapa()->getParamX();
      a = a + 0.01;
      cout << "Y2: " << a << endl;
      monitor.getMapa()->PutParamX(a);
      break;

    case 'a': // IZQDA
      a = monitor.getMapa()->getParamZ();
      a = a + 0.01;
      cout << "Y3: " << a << endl;
      monitor.getMapa()->PutParamZ(a);
      break;

    case 'd': // IZQDA
      a = monitor.getMapa()->getParamZ();
      a = a - 0.01;
      cout << "Y3: " << a << endl;
      monitor.getMapa()->PutParamZ(a);
      break;

    default: // Paranoia
      cout << key << " " << "ERROR\n";
      break;
  }
}*/

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion: nuevo ancho, nuevo alto
//***************************************************************************

void reshape(int ancho, int alto)
{
  if ((ancho > 1) and (alto > 1))
  {
    // parametros de la ventana principal

    float ratio = ((float)ancho * 0.7) / (float)alto;

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, ancho, alto);
    glLoadIdentity();
    gluOrtho2D(0, ancho, alto, 0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    // Display vista principal
    glutSetWindow(vistaPrincipal);
    glutPositionWindow(0, 0);
    glutReshapeWindow(ancho * 0.7, alto);
    glutPostRedisplay();

    // Display vista mini-mapa

    glutSetWindow(vistaMiniMapa);
    glutPositionWindow(ancho * 0.7, 0);
    glutReshapeWindow((ancho * 0.3) + 1, alto * 0.3);
    glutPostRedisplay();

    // Display interfaz
    glutSetWindow(vistaIU);
    glutPositionWindow(ancho * 0.7, alto * 0.3);
    glutReshapeWindow((ancho * 0.3) + 1, (alto * 0.7) + 1);
    lineaVacia->set_w((ancho * 0.3) + 1);
    lineaVacia->set_h((alto * 0.7) + 1);
    glutPostRedisplay();
  }
}

void ResetViewport()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-10.0, 10.0, -10.0, 10.0, 0.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void display_ventPrincipal(void)
{
  // Background Color
  glClearColor(0.8, 0.8, 0.8, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutSwapBuffers();
}

void display_vistPrincipal()
{
  ResetViewport();

  glMatrixMode(GL_PROJECTION);

  if (monitor.getMapa() != 0)
  {
    float ratio = ((float)IU_ancho_ventana * 0.7) / (float)IU_alto_ventana;
    gluPerspective(45, ratio, 1, 1000);
    glClearColor(60.0 / 256.0, 154.0 / 256.0, 201.0 / 256.0, 1);
  }
  else
  {
    glClearColor(0.0, 0.0, 0.0, 1);
  }

  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();
  if (monitor.jugar())
  {
    if (monitor.getMapa() != 0 and MMmode2 != 0)
    {
      monitor.getMapa()->drawFirstPerson(MMmode3);
    }
  }
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

void display_vistMiniMapa()
{
  ResetViewport();

  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
  if (monitor.jugar() /* or monitor.resultadosMostrados()*/)
  {
    if (MMmode == 1)
    {
      monitor.getMapa()->drawMM1(monitor.get_active_objetivos(), monitor.getLevel());
    }
    else
    {
      monitor.getMapa()->drawMM2(monitor.get_active_objetivos(), monitor.getLevel());
    }
  }
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

void Descomponer(string str, vector<string> &strs)
{
  while (str.length() != 0)
  {
    strs.push_back(str.substr(0, str.find('\n')));
    str = str.substr(str.find('\n') + 1, str.length());
    // cout << str << endl;
  }
}

void display_vistIU()
{
  ResetViewport();
  GLUI_Master.auto_set_viewport();
  std::vector<std::string> strs;
  string str;

  if (monitor.jugar() and monitor.numero_entidades() > 0)
  {
    str = monitor.toString();

    Descomponer(str, strs);

    int i = 0;
    for (vector<string>::const_iterator it = strs.begin(); it != strs.end(); it++)
    {
      switch (i)
      {
      case 0:
        info0->set_text(it->c_str());
        break;
      case 1:
        info1->set_text(it->c_str());
        break;
      case 2:
        info2->set_text(it->c_str());
        break;
      case 3:
        info3->set_text(it->c_str());
        break;
      case 4:
        info4->set_text(it->c_str());
        break;
      case 5:
        info5->set_text(it->c_str());
        break;
      case 6:
        info6->set_text(it->c_str());
        break;
      case 7:
        info7->set_text(it->c_str());
        break;
      case 8:
        info8->set_text(it->c_str());
        break;
      case 9:
        info9->set_text(it->c_str());
        break;
      case 10:
        info10->set_text(it->c_str());
        break;
      }
      i++;
    }
  }
  glutSwapBuffers();
}

void update(int valor)
{
  glutSetWindow(vistaPrincipal);
  glutPostRedisplay();

  glutSetWindow(vistaMiniMapa);
  glutPostRedisplay();

  glutSetWindow(vistaIU);
  glutPostRedisplay();
  // glutKeyboardFunc(keyboard);

  if (ObjFil1 != PosFila or ObjCol1 != PosColumna)
  {
    monitor.get_n_active_objetivo(0, PosFila, PosColumna, Gravedad);

    // editPosFila->set_int_val(PosFila);
    // editPosColumna->set_int_val(PosColumna);
    // check_Gravedad->set_int_val(Gravedad);

    ObjFil1 = PosFila;
    ObjCol1 = PosColumna;
    ObjFil2 = Gravedad;
  }

  if (check_Gravedad->get_int_val() != Gravedad)
  {
    // El usuario cambia la gravedad del accidentado
    int antiguaGravedad;
    monitor.get_n_active_objetivo(0, PosFila, PosColumna, antiguaGravedad);
    Gravedad = check_Gravedad->get_int_val();
    monitor.set_n_active_objetivo(0, PosFila, PosColumna, Gravedad);
  }

  if (monitor.getLevel()<2){
    editPosFila->disable();
    editPosColumna->disable();
    check_Gravedad->disable();
  }
  else {
    editPosFila->enable();
    editPosColumna->enable();
    check_Gravedad->enable();
  }

  Gravedad = check_Gravedad->get_int_val();
  // monitor.set_n_active_objetivo(0, PosFila, PosColumna, Gravedad);

  glutTimerFunc(1, irAlJuego, 0);
}

void irAlJuego(int valor)
{
  if (lanzar_motor_juego(colisiones, -1))
  {
    botonElegirMapa->enable();
    botonPaso->disable();
    botonEjecucion->disable();
    botonEjecutar->disable();
    botonSalir->disable();

    editTextPasos->disable();
    editTextRetardo->disable();
    botonSalir->enable();
    colisiones = 0;
  }
  // glutKeyboardFunc(keyboard);
  glutTimerFunc(1, update, 0);
}

void botonCancelarNuevoMapaCB(int valor)
{
  panelSelecMapa->close();

  botonElegirMapa->enable();
  botonSalir->enable();
}

void botonAceptarNuevoMapaCB(int valor)
{

  botonElegirMapa->enable();
  if (monitor.getLevel() == 2 or monitor.getLevel() == 3)
  {
    drawMM->disable();
  }

  drawMM->enable();
  botonPaso->enable();
  botonEjecucion->enable();
  botonEjecutar->enable();
  botonSalir->enable();
  if (monitor.getLevel() < 2)
  {
    editPosColumna->disable();
    editPosFila->disable();
    check_Gravedad->disable();
  }
  else
  {
    editPosColumna->enable();
    editPosFila->enable();
    check_Gravedad->enable();
  }
  editTextPasos->enable();
  editTextRetardo->disable();

  int posF, posC, orienta;
  int SposF, SposC, Sorienta;

  ultimomapaPos = listbox->get_int_val();

  if (ultimomapaPos >= 0)
  {
    char path[255];
    strcpy(path, "./mapas/");
    const char *file = listbox->curr_text.c_str();
    monitor.setMapa(strcat(path, file));

    monitor.reset_objetivos();

    // posicion del agente y del colaborador
    if (monitor.numero_entidades() > 1)
    {
      posC = monitor.get_entidad(0)->getCol();
      posF = monitor.get_entidad(0)->getFil();
      orienta = monitor.get_entidad(0)->getOrientacion();
      if (!monitor.is_a_valid_cell_like_goal(posF, posC))
      {
        monitor.generate_a_valid_cell(posF, posC, orienta);
      }

      SposC = monitor.get_entidad(1)->getCol();
      SposF = monitor.get_entidad(1)->getFil();
      Sorienta = monitor.get_entidad(1)->getOrientacion();
      if (!monitor.is_a_valid_cell_like_goal(SposF, SposC))
      {
        monitor.generate_a_valid_cell(SposF, SposC, Sorienta);
      }
    }
    else if (monitor.numero_entidades() > 0)
    {
      posC = monitor.get_entidad(0)->getCol();
      posF = monitor.get_entidad(0)->getFil();
      orienta = monitor.get_entidad(0)->getOrientacion();
      if (!monitor.is_a_valid_cell_like_goal(posF, posC))
      {
        monitor.generate_a_valid_cell(posF, posC, orienta);
      }
      monitor.generate_a_valid_cell(SposF, SposC, Sorienta);
    }
    else
    {
      monitor.generate_a_valid_cell(posF, posC, orienta);
      monitor.generate_a_valid_cell(SposF, SposC, Sorienta);
    }

    ultimonivel = group->get_int_val();

    int f, c;
    int g;
    monitor.get_n_active_objetivo(0, f, c, g);
    if (monitor.is_a_valid_cell_like_goal(f, c))
      monitor.put_a_new_objetivo_front(f, c, g);
    else
      monitor.generate_a_objetive();

    monitor.startGame(ultimonivel);
    if (monitor.inicializarJuego())
    {
      monitor.inicializar(posF, posC, orienta, SposF, SposC, Sorienta);
      tMap = monitor.juegoInicializado();
      if (ultimonivel == 0 or ultimonivel == 1 or ultimonivel == 4)
      {
        MMmode = 0;
        drawMM->set_int_val(0);
        drawMM->enable();
      }
    }
  }
  else
  {
    cout << "No se puede leer el fichero..." << endl;
  }

  panelSelecMapa->close();
}

vector<string> getFilesList(string name, string extension)
{
  vector<string> v;

  DIR *dirp = opendir(name.c_str());
  struct dirent *dp = readdir(dirp);
  while (dp)
  {
    string fname(dp->d_name);
    if (fname.find(extension, 0) < fname.size())
    {
      v.push_back(fname);
    }
    dp = readdir(dirp);
  }
  closedir(dirp);

  return v;
}

void botonConfigurarSimOK(int valor)
{
  botonElegirMapa->enable();
  // drawMM->disable();
  drawMMVista->enable();
  botonPaso->enable();
  botonEjecucion->enable();
  botonEjecutar->enable();
  botonSalir->enable();
  editTextPasos->enable();
  editTextRetardo->disable();
  if (ultimonivel > 1)
  {
    editPosFila->disable();
    editPosColumna->disable();
    check_Gravedad->disable();

  }

  srand(setup1->get_int_val());
  monitor.inicializar();

  int f, c;
  int g;

  if (ultimonivel > 1)
  {
    editPosFila->enable();
    editPosColumna->enable();
    check_Gravedad->enable();
  }
  monitor.set_n_active_objetivo(0, setup5->get_int_val(), setup4->get_int_val(), setup4_5->get_int_val());
  monitor.get_n_active_objetivo(0, f, c, g);
  ObjFil1 = PosFila = f;
  ObjFil1 = PosColumna = c;
  check_Gravedad->set_int_val(g);
  Gravedad = g;

  monitor.get_entidad(0)->setPosicion(setup30->get_int_val(), setup20->get_int_val());
  monitor.get_entidad(0)->setOrientacion(static_cast<Orientacion>(setup60->get_int_val()));
  monitor.get_entidad(1)->setPosicion(setup31->get_int_val(), setup21->get_int_val());
  monitor.get_entidad(1)->setOrientacion(static_cast<Orientacion>(setup61->get_int_val()));
}

void botonConfigurarSimCANCEL(int valor)
{
  botonConfigurarSimOK(1);
  MMmode = 0;
  panelSelecMapaConfig->close();
}

void botonConfigurarNuevoMapaCB(int valor)
{
  /* LLamada para cargar el mapa */
  panelSelecMapa->close();

  botonElegirMapa->disable();
  drawMM->disable();
  drawMMVista->disable();
  botonPaso->disable();
  botonEjecucion->disable();
  botonEjecutar->disable();
  botonSalir->enable();
  // editPosColumna->disable();
  // editPosFila->disable();
  editTextPasos->disable();
  editTextRetardo->disable();

  int item = listbox->get_int_val();
  int posC, posF, orienta, semilla;
  int SONposC, SONposF, SONorienta;

  if (item > 0)
  {
    char path[255];
    strcpy(path, "./mapas/");
    const char *file = listbox->curr_text.c_str();
    monitor.setMapa(strcat(path, file));

    int nivel = group->get_int_val();

    /* Paso las variables de monitor a variables locales a este metodo */
    semilla = monitor.get_semilla();

    if (monitor.numero_entidades() > 1)
    {
      posC = monitor.get_entidad(0)->getCol();
      posF = monitor.get_entidad(0)->getFil();
      orienta = monitor.get_entidad(0)->getOrientacion();
      if (!monitor.is_a_valid_cell_like_goal(posF, posC))
      {
        monitor.generate_a_valid_cell(posF, posC, orienta);
      }
      SONposC = monitor.get_entidad(1)->getCol();
      SONposF = monitor.get_entidad(1)->getFil();
      SONorienta = monitor.get_entidad(1)->getOrientacion();
      while (!monitor.is_a_valid_cell_like_goal(SONposC, SONposF) or (posF == SONposF and posC == SONposC))
      {
        monitor.generate_a_valid_cell(SONposC, SONposF, orienta);
      }
    }
    else
    {
      do
      {
        monitor.generate_a_valid_cell(posF, posC, orienta);
        monitor.generate_a_valid_cell(SONposF, SONposC, SONorienta);
      } while (posF == SONposF and posC == SONposC);
    }

    int numObj = 1;
    int f, c, kk;
    int g;

    monitor.get_n_active_objetivo(0, f, c, g);
    ObjFil1 = f;
    ObjCol1 = c;
    Gravedad = g;
    monitor.put_a_new_objetivo_front(f, c, g);

    monitor.startGame(nivel);
    if (monitor.inicializarJuego())
    {
      monitor.inicializar(posF, posC, orienta, SONposF, SONposC, SONorienta, semilla);
      tMap = monitor.juegoInicializado();
      if (nivel >= 4)
      {
        MMmode = 0;
        drawMM->set_int_val(0);
        drawMM->enable();
      }
      else if (nivel == 0 or nivel == 1)
      {
        MMmode = 0;
        drawMM->set_int_val(0);
        drawMM->enable();
      }
    }
  }
  else
  {
    cout << "No se puede leer el fichero..." << endl;
  }

  MMmode = 1;
  panelSelecMapaConfig = GLUI_Master.create_glui("Configurar Mapa");

  int num_filas = monitor.getMapa()->getNFils();
  int num_col = monitor.getMapa()->getNCols();

  GLUI_Panel *obj_panel = panelSelecMapaConfig->add_panel("Setup");
  setup1 = panelSelecMapaConfig->add_spinner_to_panel(obj_panel, "                Semilla ", GLUI_SPINNER_INT, &semilla);

  GLUI_Panel *obj_subpanel0 = panelSelecMapaConfig->add_panel_to_panel(obj_panel, "Pos Rescatador");
  setup30 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel0, "            Fila ", GLUI_SPINNER_INT, &posF);
  setup30->set_int_limits(0, num_filas, GLUI_LIMIT_WRAP);
  setup20 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel0, "     Columna ", GLUI_SPINNER_INT, &posC);
  setup20->set_int_limits(0, num_col, GLUI_LIMIT_WRAP);
  setup60 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel0, " Orientacion ", GLUI_SPINNER_INT, &orienta);
  setup60->set_int_limits(0, 8, GLUI_LIMIT_WRAP);

  GLUI_Panel *obj_subpanel1 = panelSelecMapaConfig->add_panel_to_panel(obj_panel, "Pos Auxiliar");
  setup31 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel1, "            Fila ", GLUI_SPINNER_INT, &SONposF);
  setup31->set_int_limits(0, num_filas, GLUI_LIMIT_WRAP);
  setup21 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel1, "     Columna ", GLUI_SPINNER_INT, &SONposC);
  setup21->set_int_limits(0, num_col, GLUI_LIMIT_WRAP);
  setup61 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel1, " Orientacion ", GLUI_SPINNER_INT, &SONorienta);
  setup61->set_int_limits(0, 8, GLUI_LIMIT_WRAP);

  GLUI_Panel *obj_subpanel2 = panelSelecMapaConfig->add_panel_to_panel(obj_panel, "Casilla Objetivo");
  setup5 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel2, "        Fil ", GLUI_SPINNER_INT, &ObjFil1);
  setup5->set_int_limits(0, num_filas, GLUI_LIMIT_WRAP);
  setup4 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel2, "    Col  ", GLUI_SPINNER_INT, &ObjCol1);
  setup4->set_int_limits(0, num_col, GLUI_LIMIT_WRAP);
  setup4_5 = panelSelecMapaConfig->add_spinner_to_panel(obj_subpanel2, "    Grav  ", GLUI_SPINNER_INT, &Gravedad);
  setup4_5->set_int_limits(0, 1, GLUI_LIMIT_WRAP);

  panelSelecMapaConfig->add_button("Ok", 1, botonConfigurarSimOK);
  panelSelecMapaConfig->add_button("Finish", 1, botonConfigurarSimCANCEL);
}

void botonElegirMapaCB(int valor)
{

  panelSelecMapa = GLUI_Master.create_glui("Elige mapa y nivel");

  botonElegirMapa->disable();
  botonPaso->disable();
  botonEjecucion->disable();
  botonEjecutar->disable();
  botonSalir->disable();
  // editPosColumna->disable();
  // editPosFila->disable();
  editTextPasos->disable();
  editTextRetardo->disable();

  listbox = panelSelecMapa->add_listbox("Mapa  ", &ultimomapaPos);

  int i = 1;
  vector<string> filesPaths = getFilesList("mapas/", ".map");
  vector<string>::const_iterator it = filesPaths.begin();
  while (it != filesPaths.end())
  {
    listbox->add_item(i++, (*it).c_str());
    it++;
  }

  listbox->set_int_val(ultimomapaPos);

  GLUI_Panel *obj_panel = panelSelecMapa->add_panel("Nivel");
  group = panelSelecMapa->add_radiogroup_to_panel(obj_panel, &ultimonivel);
  panelSelecMapa->add_radiobutton_to_group(group, "Nivel 0: [R] El Despertar Reactivo    ");
  panelSelecMapa->add_radiobutton_to_group(group, "Nivel 1: [R] La Cartografia de lo Desconocido  ");
  panelSelecMapa->add_radiobutton_to_group(group, "Nivel 2: [D] La Busqueda del Camino de Dijkstra   ");
  panelSelecMapa->add_radiobutton_to_group(group, "Nivel 3: [D] La Ascension del A*uxiliar       ");
  panelSelecMapa->add_radiobutton_to_group(group, "Nivel 4: Mision de rescate ");

  panelSelecMapa->add_button("Ok", 1, botonAceptarNuevoMapaCB);
  panelSelecMapa->add_button("Ok y Configurar", 2, botonConfigurarNuevoMapaCB);
  panelSelecMapa->add_button("Salir", 3, botonCancelarNuevoMapaCB);
}

void botonPasoCB(int valor)
{
  monitor.setPasos(1);
}

void botonEjecucionCB(int valor)
{
  monitor.setPasos(3000);
}

void botonEjecutarCB(int valor)
{
  monitor.setPasos(nPasos);
}

void setRetardo(int valor)
{
  monitor.setRetardo(tRetardo);
}

void setPosColumna(int valor)
{
  // Cambiando la posición de la columna
  monitor.set_n_active_objetivo(0, PosFila, PosColumna, Gravedad);
  ObjFil1 = PosFila;
  ObjCol1 = PosColumna;
  ObjFil2 = Gravedad;
}

void setPosFila(int valor)
{
  // Cambiando la posición de la fila
  monitor.set_n_active_objetivo(0, PosFila, PosColumna, Gravedad);
  ObjFil1 = PosFila;
  ObjCol1 = PosColumna;
  ObjFil2 = Gravedad;
}

void setGravedad(int valor)
{
}

void botonSalirCB(int valor)
{
  monitor.cerrarBelkan();
  exit(0);
}

Orientacion Next(Orientacion x)
{
  switch (x)
  {
  case norte:
    return noreste;
    break;
  case noreste:
    return este;
    break;
  case este:
    return sureste;
    break;
  case sureste:
    return sur;
    break;
  case sur:
    return suroeste;
    break;
  case suroeste:
    return oeste;
    break;
  case oeste:
    return noroeste;
    break;
  case noroeste:
    return norte;
    break;
  }
}

Orientacion Previous(Orientacion x)
{
  switch (x)
  {
  case norte:
    return noroeste;
    break;
  case noroeste:
    return oeste;
    break;
  case oeste:
    return suroeste;
    break;
  case suroeste:
    return sur;
    break;
  case sur:
    return sureste;
    break;
  case sureste:
    return este;
    break;
  case este:
    return noreste;
    break;
  case noreste:
    return norte;
    break;
  }
}

void mouseClick(int button, int state, int x, int y)
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  int nx = round(x * tMap * 1.0 / tw);
  int ny = round(y * tMap * 1.0 / th);

  if ((button == GLUT_LEFT_BUTTON) and (state == GLUT_DOWN))
  {
    if ((nx >= 0) and (nx <= tMap - 1) and (ny >= 0) and (ny <= tMap - 1))
    {
      editPosFila->set_int_val(ny);
      editPosColumna->set_int_val(nx);
      setPosFila(ny);
      setPosColumna(nx);
    }
  }
  else if ((button == GLUT_RIGHT_BUTTON) and (state == GLUT_DOWN))
  {
    if ((nx >= 0) and (nx <= tMap - 1) and (ny >= 0) and (ny <= tMap - 1))
    {
      monitor.get_entidad(0)->setPosicion(ny, nx);
    }
  }
  else if ((button == GLUT_MIDDLE_BUTTON) and (state == GLUT_DOWN))
  {
    if ((nx >= 0) and (nx <= tMap - 1) and (ny >= 0) and (ny <= tMap - 1))
    {
      Orientacion bru = monitor.get_entidad(0)->getOrientacion();
      bru = (Orientacion)((bru + 1) % 8);
      monitor.get_entidad(0)->setOrientacion(bru);
    }
  }
}

void lanzar_motor_grafico(int argc, char **argv)
{
  glutInit(&argc, argv);
  // Mode Setting
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  // window size (+gap size)
  glutInitWindowSize(IU_ancho_ventana, IU_alto_ventana);
  float ratio = ((float)IU_ancho_ventana * 0.7) / (float)IU_alto_ventana;
  // Initial position
  glutInitWindowPosition(300, 0);

  // Main Window
  ventanaPrincipal = glutCreateWindow("Practica 2: Agentes Deliberativos/Reactivos. Curso 24/25");
  // Main Window callback function
  glutReshapeFunc(reshape);
  glutDisplayFunc(display_ventPrincipal);

  // World Window and Display
  vistaPrincipal = glutCreateSubWindow(ventanaPrincipal, 0, 0, IU_ancho_ventana * 0.7, IU_alto_ventana);
  glutDisplayFunc(display_vistMiniMapa);
  glutMouseFunc(mouseClick);
  // glutKeyboardFunc(keyboard);

  // screen Window and Display
  vistaMiniMapa = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana * 0.7, 0, (IU_ancho_ventana * 0.3) + 1, IU_alto_ventana * 0.3);
  glutDisplayFunc(display_vistPrincipal);

  vistaIU = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana * 0.7, IU_alto_ventana * 0.3, (IU_ancho_ventana * 0.3) + 1, (IU_alto_ventana * 0.7) + 1);
  glutDisplayFunc(display_vistIU);

  glutInitWindowPosition(IU_ancho_ventana * 0.7, 50);
  glutInitWindowSize(50, 50);

  GLUI *panelIU = GLUI_Master.create_glui_subwindow(vistaIU, GLUI_SUBWINDOW_TOP);
  panelIU->set_main_gfx_window(vistaIU);

  botonElegirMapa = panelIU->add_button("Nuevo Juego", 0, botonElegirMapaCB);
  botonElegirMapa->set_alignment(GLUI_ALIGN_CENTER);

  GLUI_Panel *obj_panel2 = panelIU->add_panel("Visualizacion");

  MMmode2 = 1;
  drawMMVista = panelIU->add_checkbox_to_panel(obj_panel2, "1ªPersona", &MMmode2);
  drawMMVista->set_alignment(GLUI_ALIGN_CENTER);
  drawMMVista->enable();
  panelIU->add_column_to_panel(obj_panel2, true);

  MMmode = 1;
  drawMM = panelIU->add_checkbox_to_panel(obj_panel2, "Mapa", &MMmode);
  drawMM->set_alignment(GLUI_ALIGN_CENTER);
  drawMM->enable();
  panelIU->add_column_to_panel(obj_panel2, true);

  MMmode3 = 0;
  drawMMWho = panelIU->add_checkbox_to_panel(obj_panel2, "Auxiliar", &MMmode3);
  drawMMWho->set_alignment(GLUI_ALIGN_CENTER);
  drawMMWho->enable();

  // lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *obj_panel = panelIU->add_panel("Ir a...");
  // editPosFila = panelIU->add_spinner_to_panel(obj_panel, "   Fila", GLUI_SPINNER_INT, &PosFila);
  // editPosColumna = panelIU->add_spinner_to_panel(obj_panel, "Columna", GLUI_SPINNER_INT, &PosColumna);

  if (!monitor.there_are_active_objetivo())
  {
    monitor.put_active_objetivos(1);
  }
  monitor.get_n_active_objetivo(0, PosFila, PosColumna, Gravedad);

  editPosFila = panelIU->add_edittext_to_panel(obj_panel, "Fila", GLUI_EDITTEXT_INT, &PosFila, -1, setPosFila);
  editPosFila->set_alignment(GLUI_ALIGN_LEFT);
  editPosFila->set_w(24);
  panelIU->add_column_to_panel(obj_panel, false);
  editPosColumna = panelIU->add_edittext_to_panel(obj_panel, "Col", GLUI_EDITTEXT_INT, &PosColumna, -1, setPosColumna);
  editPosColumna->set_alignment(GLUI_ALIGN_LEFT);
  editPosColumna->set_w(24);
  panelIU->add_column_to_panel(obj_panel, false);
  check_Gravedad = panelIU->add_checkbox_to_panel(obj_panel, "Gravedad", &Gravedad, -1, setGravedad);
  check_Gravedad->set_alignment(GLUI_ALIGN_CENTER);
  check_Gravedad->enable();
  panelIU->add_column_to_panel(obj_panel, false);

  // setPosColumna(PosColumna);
  // setPosFila(PosFila);
  // setGravedad(Gravedad);
  // cout << "Gravedad: " << Gravedad << endl;
  //   editPosFila->set_int_limits(0, 100, GLUI_LIMIT_WRAP);
  //   editPosColumna->set_int_limits(0, 100, GLUI_LIMIT_WRAP);

  // lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *run_panel = panelIU->add_panel("Control");

  editTextPasos = panelIU->add_edittext_to_panel(run_panel, "Pasos", GLUI_EDITTEXT_INT, &nPasos);
  editTextPasos->set_int_val(nPasos);
  editTextPasos->set_int_limits(1, 10000000, GLUI_LIMIT_CLAMP);
  editTextPasos->set_alignment(GLUI_ALIGN_CENTER);
  editTextPasos->set_w(24);

  editTextRetardo = panelIU->add_edittext_to_panel(run_panel, "Retardo", GLUI_EDITTEXT_INT, &tRetardo, -1, setRetardo);
  editTextRetardo->set_int_val(tRetardo);
  editTextRetardo->set_int_limits(0, 10000000, GLUI_LIMIT_CLAMP);
  editTextRetardo->set_alignment(GLUI_ALIGN_CENTER);
  editTextRetardo->set_w(24);
  setRetardo(tRetardo);

  panelIU->add_column_to_panel(run_panel, true);

  botonPaso = panelIU->add_button_to_panel(run_panel, "Paso", 0, botonPasoCB);
  botonPaso->set_alignment(GLUI_ALIGN_CENTER);
  botonEjecutar = panelIU->add_button_to_panel(run_panel, "Ciclo", 0, botonEjecutarCB);
  botonEjecutar->set_alignment(GLUI_ALIGN_CENTER);
  botonEjecucion = panelIU->add_button_to_panel(run_panel, "Ejecucion", 0, botonEjecucionCB);

  lineaVacia = panelIU->add_statictext("");

  botonSalir = panelIU->add_button("Salir", 0, botonSalirCB);
  botonSalir->set_alignment(GLUI_ALIGN_CENTER);

  lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *panelInfo = panelIU->add_panel("Informacion");
  panelInfo->set_alignment(GLUI_ALIGN_CENTER);

  info0 = panelIU->add_statictext_to_panel(panelInfo, "              No hay informacion disponible           ");
  info1 = panelIU->add_statictext_to_panel(panelInfo, "");
  info2 = panelIU->add_statictext_to_panel(panelInfo, "");
  info3 = panelIU->add_statictext_to_panel(panelInfo, "");
  info4 = panelIU->add_statictext_to_panel(panelInfo, "");
  info5 = panelIU->add_statictext_to_panel(panelInfo, "");
  info6 = panelIU->add_statictext_to_panel(panelInfo, "");
  info6->set_font(GLUT_BITMAP_8_BY_13);
  info7 = panelIU->add_statictext_to_panel(panelInfo, "");
  info7->set_font(GLUT_BITMAP_8_BY_13);
  info8 = panelIU->add_statictext_to_panel(panelInfo, "");
  info8->set_font(GLUT_BITMAP_8_BY_13);
  info9 = panelIU->add_statictext_to_panel(panelInfo, "");
  info9->set_font(GLUT_BITMAP_8_BY_13);
  info10 = panelIU->add_statictext_to_panel(panelInfo, "");
  info10->set_font(GLUT_BITMAP_8_BY_13);

  // panelIU->add_separator_to_panel(panelInfo);

  lineaVacia = panelIU->add_statictext("");

  glutTimerFunc(100, irAlJuego, 0);

  botonPaso->disable();
  botonEjecucion->disable();
  botonEjecutar->disable();
  editPosColumna->disable();
  editPosFila->disable();
  check_Gravedad->disable();
  editTextPasos->disable();
  editTextRetardo->disable();

  glutMainLoop();
}

void lanzar_motor_grafico_verOnline(int argc, char **argv, EnLinea &argumentos)
{

  // Poner los valores del juego antes de lanzar el bucle
  const char *dirMapa = argumentos.ubicacion_mapa.c_str();
  monitor.setMapa(dirMapa);
  tMap = monitor.getMapa()->getNFils();
  monitor.startGame(argumentos.level);
  MMmode3 = 0;
  if (argumentos.level == 4)
  {
    MMmode2 = 1;
    MMmode = 0;
  }
  else if (argumentos.level == 0 or argumentos.level == 1)
  {
    MMmode2 = 1;
    MMmode = 0;
  }
  else
  {
    MMmode2 = 1;
    MMmode = 1;
  }

  // Posicion inicial del agente
  monitor.setListObj(argumentos.listaObjetivos);
  monitor.inicializar(argumentos.posInicialRescatador.f, argumentos.posInicialRescatador.c, argumentos.posInicialRescatador.brujula, argumentos.posInicialAuxiliar.f,
                      argumentos.posInicialAuxiliar.c, argumentos.posInicialAuxiliar.brujula, argumentos.semilla);

  monitor.get_entidad(0)->setBateria(3000);
  monitor.setPasos(1);
  monitor.setRetardo(0);

  glutInit(&argc, argv);
  // Mode Setting
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  // window size (+gap size)
  glutInitWindowSize(IU_ancho_ventana, IU_alto_ventana);
  float ratio = ((float)IU_ancho_ventana * 0.7) / (float)IU_alto_ventana;
  // Initial position
  glutInitWindowPosition(300, 0);

  // Main Window
  ventanaPrincipal = glutCreateWindow("Practica 2: Agentes Deliberativos/Reactivos. Curso 24/25. Version BAJO PARAMETROS");
  // Main Window callback function
  glutReshapeFunc(reshape);
  glutDisplayFunc(display_ventPrincipal);

  // World Window and Display
  vistaPrincipal = glutCreateSubWindow(ventanaPrincipal, 0, 0, IU_ancho_ventana * 0.7, IU_alto_ventana);
  glutDisplayFunc(display_vistMiniMapa);
  glutMouseFunc(mouseClick);
  // glutKeyboardFunc(keyboard);

  // screen Window and Display
  vistaMiniMapa = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana * 0.7, 0, (IU_ancho_ventana * 0.3) + 1, IU_alto_ventana * 0.3);
  glutDisplayFunc(display_vistPrincipal);

  vistaIU = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana * 0.7, IU_alto_ventana * 0.3, (IU_ancho_ventana * 0.3) + 1, (IU_alto_ventana * 0.7) + 1);
  glutDisplayFunc(display_vistIU);

  glutInitWindowPosition(IU_ancho_ventana * 0.7, 50);
  glutInitWindowSize(50, 50);

  GLUI *panelIU = GLUI_Master.create_glui_subwindow(vistaIU, GLUI_SUBWINDOW_TOP);
  panelIU->set_main_gfx_window(vistaIU);

  botonElegirMapa = panelIU->add_button("Nuevo Juego", 0, botonElegirMapaCB);
  botonElegirMapa->set_alignment(GLUI_ALIGN_CENTER);

  GLUI_Panel *obj_panel2 = panelIU->add_panel("Visualizacion");

  drawMMVista = panelIU->add_checkbox_to_panel(obj_panel2, "1ªPersona", &MMmode2);
  drawMMVista->set_alignment(GLUI_ALIGN_CENTER);
  drawMMVista->enable();
  panelIU->add_column_to_panel(obj_panel2, true);

  drawMM = panelIU->add_checkbox_to_panel(obj_panel2, "Mapa", &MMmode);
  drawMM->set_alignment(GLUI_ALIGN_CENTER);
  drawMM->enable();
  panelIU->add_column_to_panel(obj_panel2, true);

  drawMMWho = panelIU->add_checkbox_to_panel(obj_panel2, "Auxiliar", &MMmode3);
  drawMMWho->set_alignment(GLUI_ALIGN_CENTER);
  drawMMWho->enable();

  // lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *obj_panel = panelIU->add_panel("Ir a...");
  // editPosFila = panelIU->add_spinner_to_panel(obj_panel, "   Fila", GLUI_SPINNER_INT, &PosFila);
  // editPosColumna = panelIU->add_spinner_to_panel(obj_panel, "Columna", GLUI_SPINNER_INT, &PosColumna);

  if (!monitor.there_are_active_objetivo())
  {
    monitor.put_active_objetivos(1);
  }
  monitor.get_n_active_objetivo(0, PosFila, PosColumna, Gravedad);

  editPosFila = panelIU->add_edittext_to_panel(obj_panel, "Fila", GLUI_EDITTEXT_INT, &PosFila, -1, setPosFila);
  editPosFila->set_alignment(GLUI_ALIGN_LEFT);
  editPosFila->set_w(24);
  panelIU->add_column_to_panel(obj_panel, false);
  editPosColumna = panelIU->add_edittext_to_panel(obj_panel, "Col", GLUI_EDITTEXT_INT, &PosColumna, -1, setPosColumna);
  editPosColumna->set_alignment(GLUI_ALIGN_LEFT);
  editPosColumna->set_w(24);
  panelIU->add_column_to_panel(obj_panel, false);
  check_Gravedad = panelIU->add_checkbox_to_panel(obj_panel, "Gravedad", &Gravedad);
  check_Gravedad->set_alignment(GLUI_ALIGN_CENTER);
  check_Gravedad->enable();
  panelIU->add_column_to_panel(obj_panel, false);

  // setPosColumna(PosColumna);
  // setPosFila(PosFila);
  // setGravedad(Gravedad);
  //  editPosFila->set_int_limits(0, 100, GLUI_LIMIT_WRAP);
  //  editPosColumna->set_int_limits(0, 100, GLUI_LIMIT_WRAP);

  // lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *run_panel = panelIU->add_panel("Control");

  editTextPasos = panelIU->add_edittext_to_panel(run_panel, "Pasos", GLUI_EDITTEXT_INT, &nPasos);
  editTextPasos->set_int_val(nPasos);
  editTextPasos->set_int_limits(1, 10000000, GLUI_LIMIT_CLAMP);
  editTextPasos->set_alignment(GLUI_ALIGN_CENTER);
  editTextPasos->set_w(24);

  editTextRetardo = panelIU->add_edittext_to_panel(run_panel, "Retardo", GLUI_EDITTEXT_INT, &tRetardo, -1, setRetardo);
  editTextRetardo->set_int_val(tRetardo);
  editTextRetardo->set_int_limits(0, 10000000, GLUI_LIMIT_CLAMP);
  editTextRetardo->set_alignment(GLUI_ALIGN_CENTER);
  editTextRetardo->set_w(24);
  setRetardo(tRetardo);

  panelIU->add_column_to_panel(run_panel, true);

  botonPaso = panelIU->add_button_to_panel(run_panel, "Paso", 0, botonPasoCB);
  botonPaso->set_alignment(GLUI_ALIGN_CENTER);
  botonEjecutar = panelIU->add_button_to_panel(run_panel, "Ciclo", 0, botonEjecutarCB);
  botonEjecutar->set_alignment(GLUI_ALIGN_CENTER);
  botonEjecucion = panelIU->add_button_to_panel(run_panel, "Ejecucion", 0, botonEjecucionCB);

  lineaVacia = panelIU->add_statictext("");

  botonSalir = panelIU->add_button("Salir", 0, botonSalirCB);
  botonSalir->set_alignment(GLUI_ALIGN_CENTER);

  lineaVacia = panelIU->add_statictext("");

  GLUI_Panel *panelInfo = panelIU->add_panel("Informacion");
  panelInfo->set_alignment(GLUI_ALIGN_CENTER);

  info0 = panelIU->add_statictext_to_panel(panelInfo, "              No hay informacion disponible           ");
  info1 = panelIU->add_statictext_to_panel(panelInfo, "");
  info2 = panelIU->add_statictext_to_panel(panelInfo, "");
  info3 = panelIU->add_statictext_to_panel(panelInfo, "");
  info4 = panelIU->add_statictext_to_panel(panelInfo, "");
  info5 = panelIU->add_statictext_to_panel(panelInfo, "");
  info6 = panelIU->add_statictext_to_panel(panelInfo, "");
  info6->set_font(GLUT_BITMAP_8_BY_13);
  info7 = panelIU->add_statictext_to_panel(panelInfo, "");
  info7->set_font(GLUT_BITMAP_8_BY_13);
  info8 = panelIU->add_statictext_to_panel(panelInfo, "");
  info8->set_font(GLUT_BITMAP_8_BY_13);
  info9 = panelIU->add_statictext_to_panel(panelInfo, "");
  info9->set_font(GLUT_BITMAP_8_BY_13);
  info10 = panelIU->add_statictext_to_panel(panelInfo, "");
  info10->set_font(GLUT_BITMAP_8_BY_13);
  // panelIU->add_separator_to_panel(panelInfo);

  lineaVacia = panelIU->add_statictext("");

  monitor.juegoInicializado();

  botonPaso->enable();
  botonEjecucion->enable();
  botonEjecutar->enable();
  if (monitor.getLevel() < 2)
  {
    editPosColumna->disable();
    editPosFila->disable();
    check_Gravedad->disable();
  }
  else
  {
    editPosColumna->enable();
    editPosFila->enable();
    check_Gravedad->enable();
  }

  if (monitor.getLevel() == 2 or monitor.getLevel() == 3)
  {
    drawMM->disable();
  }

  editTextPasos->enable();
  editTextRetardo->disable();

  // glutTimerFunc(100,update,0);
  glutTimerFunc(100, irAlJuego, 0);

  glutMainLoop();
}
