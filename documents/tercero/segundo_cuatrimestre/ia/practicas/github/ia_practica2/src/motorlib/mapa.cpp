#include "motorlib/mapa.hpp"

void Mapa::colorCeldaMM(unsigned char celda, unsigned char altura)
{
  float h = altura - '0';
  float brillo = 0.5f + (h / 10.0f); // Escalado de brillo (0.5 a 1.0)
  switch (celda)
  {
  case 'P': // Precipicio
    glColor3f(0.15 * brillo, 0.15 * brillo, 0.15 * brillo);
    break;
  case 'B': // Árbol
    glColor3f(0.0, 1.0 * brillo, 0.0);
    break;
  case 'C': // Camino
    glColor3f(0.3 * brillo, 0.25 * brillo, 0.2 * brillo);
    break;
  case 'A': // Agua
    glColor3f(0.0, 0.0, 1.0 * brillo);
    break;
  case 'S': // Sendero
    glColor3f(0.6 * brillo, 0.6 * brillo, 0.6 * brillo);
    break;
  case 'M': // Obstáculo
    glColor3f(0.6 * brillo, 0.0, 0.0);
    break;
  case 'T': // Matorral
    glColor3f(0.4 * brillo, 0.8 * brillo, 0.0);
    break;
  case 'K': // Bikini
    glColor3f(1.0 * brillo, 1.0 * brillo, 0.0);
    break;
  case 'Z': // Zapatillas
    glColor3f(0.0, 1.0 * brillo, 0.3 * brillo);
    break;
  case 'D':
    glColor3f(0.3 * brillo, 0.15 * brillo, 0.5 * brillo);
    break;
  case 'X': // Puesto Base
    glColor3f(1.0 * brillo, 0.0, 1.0 * brillo);
    break;
  default:
    glColor3f(1.0, 1.0, 1.0);
    // glColor3f(1.0 * brillo, 1.0 * brillo, 1.0 * brillo);
    break;
  }
}

void Mapa::colorCeldaOpuestoMM(unsigned char celda)
{
  switch (celda)
  {
  case 'P':
    glColor3f(1, 0, 0);
    break;
  case 'B':
    glColor3f(1, 0, 0);
    break;
  case 'A':
    glColor3f(1.0, 0.0, 0.0);
    break;
  case 'S':
    glColor3f(1, 0, 0);
    break;
  case 'M':
    glColor3f(1, 1, 1);
    break;
  case 'T':
    glColor3f(1, 0, 0);
    break;
  case 'K':
    glColor3f(0, 0, 0);
    break;
  case 'Z':
    glColor3f(1, 1, 1);
    break;
  case 'D':
    glColor3f(1, 1, 1);
    break;
  case 'X':
    glColor3f(0, 0, 0);
    break;
  default:
  glColor3f(1,0,0);
  break;
  }
}

void Mapa::colorCeldaOpuestoMM2(unsigned char celda)
{
  switch (celda)
  {
  case 'P':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'B':
  case 'T':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'A':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'S':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'M':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  // case 'T':
  //   glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
  //   break;
  case 'K':
    glColor3f(0, 0, 0);
    break;
  case 'Z':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'D':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  case 'X':
    glColor3f(0, 0, 0);
    break;
  default:
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    break;
  }
}

void Mapa::OrientacionEntidadMM(Orientacion orienParam)
{
  switch (orienParam)
  {
  case norte:
    glRotatef(90.0, 0, 0, 1);
    break;
  case noreste:
    glRotatef(45.0, 0, 0, 1);
    break;
  case este:
    glRotatef(0.0, 0, 0, 1);
    break;
  case sureste:
    glRotatef(315.0, 0, 0, 1);
    break;
  case sur:
    glRotatef(270.0, 0, 0, 1);
    break;
  case suroeste:
    glRotatef(215.0, 0, 0, 1);
    break;
  case oeste:
    glRotatef(180.0, 0, 0, 1);
    break;
  case noroeste:
    glRotatef(135.0, 0, 0, 1);
    break;
  }
}

void Mapa::OrientacionEntidadFP(Orientacion orienParam)
{
  switch (orienParam)
  {
  case norte:
    glRotatef(0.0, 0, 1, 0);
    break;
  case noreste:
    glRotatef(315.0, 0, 1, 0);
    break;
  case este:
    glRotatef(270.0, 0, 1, 0);
    break;
  case sureste:
    glRotatef(215.0, 0, 1, 0);
    break;
  case sur:
    glRotatef(180.0, 0, 1, 0);
    break;
  case suroeste:
    glRotatef(135.0, 0, 1, 0);
    break;
  case oeste:
    glRotatef(90.0, 0, 1, 0);
    break;
  case noroeste:
    glRotatef(45.0, 0, 1, 0);
    break;
  }
}

void Mapa::complementosCelda(unsigned char celda, unsigned char altura)
{
  Arbol3D *arbol = new Arbol3D();
  unsigned char h = altura - '0';

  switch (celda)
  {
  case 'P':
    glColor3f(0.15, 0.15, 0.15);
    glTranslatef(0.0, 7.5, 0.0);
    glScalef(5.0, -15.0, 5.0);
    glutSolidCube(1.0);
    break;
  case 'B':
    glScalef(5.0, 10.0 + (1.0), 5.0);
    arbol->draw(1);
    break;
  case 'M':
    glColor3f(0.6, 0.0, 0.0);
    glTranslatef(0.0, 0.2, 0.0);
    glScalef(5.0, 15.0, 5.0);
    glutSolidCube(1.0);
    break;
  }
  delete arbol;
}

void Mapa::formaEntidad(unsigned char tipoParam)
{
  RevolucionObj3D *jug = new RevolucionObj3D("ply/cilindro.ply", 3);
  switch (tipoParam)
  {
  case 'r':
    glRotatef(90.0, 1, 0, 0);
    glScalef(1.0, 1.0, 0.75);
    jug->setColor(_vertex3<float>(1.0, 0.0, 0.0));
    jug->draw(1);
    break;

  case 'a':
    glRotatef(90.0, 1, 0, 0);
    glScalef(1.0, 1.0, 0.75);
    jug->setColor(_vertex3<float>(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0));
    jug->draw(1);
    break;

  case 'v':
    glColor3f(1.0, 5.0 / 255.0, 238 / 255.0);
    glutSolidSphere(0.25, 6, 2);
    break;
  case 'e':
    glColor3f(255.0 / 255.0, 159.0 / 255.0, 5.0 / 255.0);
    glutSolidCube(0.6);
    break;

  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
    glColor3f(1.0, 5.0 / 255.0, 238 / 255.0);
    glutSolidSphere(0.5, 6, 2);
    break;
  }
}

void Mapa::drawMM1(vector<unsigned int> objetivosActivos, int level)
{
  unsigned int colMed, filaMed;
  vector<vector<unsigned char>> mapaConPlan = (*entidades)[0]->getMapaPlan();
  JoinMapasPlan(mapaConPlan, (*entidades)[1]->getMapaPlan());

  colMed = (mapaCompleto.size() / 2);
  filaMed = (mapaCompleto[0].size() / 2);

  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);

  float ratio = 1.0;
  if (mapaCompleto.size() > 15)
  {
    ratio = 20.0 / (float)mapaCompleto.size();
  }

  for (unsigned int i = 0; i < mapaCompleto.size(); i++)
  {
    for (unsigned int j = 0; j < mapaCompleto[i].size(); j++)
    {
      glPushMatrix();

      glTranslatef(((GLfloat)i - (GLfloat)filaMed) * ratio, ((GLfloat)colMed - (GLfloat)j) * ratio, 0.0);
      glScalef(ratio, ratio, ratio);

      colorCeldaMM(mapaCompleto[j][i], mapaAlturas[j][i]);

      glBegin(GL_QUADS);
      glVertex3f(-0.5, -0.5, 0.0);
      glVertex3f(-0.5, 0.5, 0.0);
      glVertex3f(0.5, 0.5, 0.0);
      glVertex3f(0.5, -0.5, 0.0);
      glEnd();

      if (mapaConPlan[j][i] > 0)
      {
        /*glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
          glVertex3f(0.0, -0.5, 0.0);
          glVertex3f(-0.5, 0.0, 0.0);
          glVertex3f( 0.0, 0.5, 0.0);
          glVertex3f( 0.5, 0.0, 0.0);
        glEnd();*/

        // glColor3f(0.0,1.0,1.0);
        if (mapaConPlan[j][i] == 1 or mapaConPlan[j][i] == 2)
        {
          if (mapaConPlan[j][i] == 1)
            colorCeldaOpuestoMM(mapaCompleto[j][i]);
          else
            colorCeldaOpuestoMM2(mapaCompleto[j][i]);

          glBegin(GL_QUADS);
          glVertex3f(0.0, -0.3, 0.0);
          glVertex3f(-0.3, 0.0, 0.0);
          glVertex3f(0.0, 0.3, 0.0);
          glVertex3f(0.3, 0.0, 0.0);
          glEnd();
        }
        if (mapaConPlan[j][i] == 3)
        {
          colorCeldaOpuestoMM(mapaCompleto[j][i]);
          glBegin(GL_TRIANGLES);
          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);
          glVertex3f(0.0f, 0.20f, 0.0);

          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(-0.30f, -0.10f, 0.0);
          glVertex3f(-0.10f, -0.20f, 0.0);

          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.0f, -0.40f, 0.0);
          glVertex3f(0.10f, -0.20f, 0.0);

          glVertex3f(0.10f, -0.20f, 0.0);
          glVertex3f(0.30f, -0.10f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);

          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);

          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);
          glEnd();
        }
      }

      glPopMatrix();
    }
  }

  if (level > 1)
  {
    for (unsigned int i = 0; i < objetivosActivos.size(); i = i + 3)
    {
      glPushMatrix();
      glTranslatef(((GLfloat)objetivosActivos[i + 1] - (GLfloat)colMed) * ratio, ((GLfloat)filaMed - (GLfloat)objetivosActivos[i]) * ratio, 0);
      if (objetivosActivos[i + 2] == 0)
        glColor3f(0.0, 1.0, 1.0);
      else
        glColor3f(1.0, 0.0, 0.0);

      glScalef(ratio, ratio, ratio);
      glutSolidSphere(0.5, 6, 2);

      glColor3f(0.0, 0.0, 0.0);
      glScalef(7 * ratio / 8, 7 * ratio / 8, 7 * ratio / 8);
      glutSolidSphere(0.5, 6, 2);

      if (objetivosActivos[i + 2] == 0)
        glColor3f(0.0, 1.0, 1.0);
      else
        glColor3f(1.0, 0.0, 0.0);

      glScalef(ratio / 2, ratio / 2, ratio / 2);
      glutSolidSphere(0.5, 6, 2);

      glPopMatrix();
    }
  }

  for (unsigned int i = 0; i < entidades->size(); i++)
  {
    if ((*entidades)[i]->getHitbox())
    {
      glPushMatrix();

      glTranslatef(((GLfloat)(*entidades)[i]->getCol() - (GLfloat)colMed) * ratio, ((GLfloat)filaMed - (GLfloat)(*entidades)[i]->getFil()) * ratio, 0.0);

      OrientacionEntidadMM((*entidades)[i]->getOrientacion());

      glScalef(ratio, ratio, ratio);

      formaEntidad((*entidades)[i]->getSubTipoChar());

      glPopMatrix();
    }
  }
}

void Mapa::JoinMapasSuperficie(vector<vector<unsigned char>> &resultado, const vector<vector<unsigned char>> &fuente)
{
  for (int i = 0; i < resultado.size(); i++)
  {
    for (int j = 0; j < resultado[0].size(); j++)
    {
      if (resultado[i][j] == '?')
        resultado[i][j] = fuente[i][j];
    }
  }
}

void Mapa::JoinMapasPlan(vector<vector<unsigned char>> &resultado, const vector<vector<unsigned char>> &fuente)
{
  for (int i = 0; i < resultado.size(); i++)
  {
    for (int j = 0; j < resultado[0].size(); j++)
    {
      if (resultado[i][j] == 0)
        resultado[i][j] = fuente[i][j];
    }
  }
}

void Mapa::drawMM2(vector<unsigned int> objetivosActivos, int level)
{
  vector<vector<unsigned char>> mapaSuperficie = (*entidades)[0]->getMapaResultado();
  JoinMapasSuperficie(mapaSuperficie, (*entidades)[1]->getMapaResultado());
  vector<vector<unsigned char>> mapaEntidades = (*entidades)[0]->getMapaEntidades();
  vector<vector<unsigned char>> mapaConPlan = (*entidades)[0]->getMapaPlan();
  JoinMapasPlan(mapaConPlan, (*entidades)[1]->getMapaPlan());

  unsigned int colMed, filaMed;

  colMed = (mapaCompleto.size() / 2);
  filaMed = (mapaCompleto[0].size() / 2);

  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);

  float ratio = 1.0;
  if (mapaCompleto.size() > 15)
  {
    ratio = 20.0 / (float)mapaCompleto.size();
  }

  for (unsigned int i = 0; i < mapaCompleto.size(); i++)
  {
    for (unsigned int j = 0; j < mapaCompleto[i].size(); j++)
    {
      glPushMatrix();

      glTranslatef(((GLfloat)i - (GLfloat)filaMed) * ratio, ((GLfloat)colMed - (GLfloat)j) * ratio, 0.0);
      glScalef(ratio, ratio, ratio);

      colorCeldaMM(mapaSuperficie[j][i], mapaAlturas[j][i]);

      switch (mapaSuperficie[j][i])
      {
      case 'T':
        glBegin(GL_QUADS);
        glVertex3f(-0.5, -0.5, 1.0);
        glVertex3f(-0.5, 0.5, 1.0);
        glVertex3f(0.5, 0.5, 1.0);
        glVertex3f(0.5, -0.5, 1.0);
        glEnd();
        break;
      default:
        glBegin(GL_QUADS);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(-0.5, 0.5, 0.0);
        glVertex3f(0.5, 0.5, 0.0);
        glVertex3f(0.5, -0.5, 0.0);
        glEnd();
        break;
      }

      glBegin(GL_QUADS);
      glVertex3f(-0.5, -0.5, 0.0);
      glVertex3f(-0.5, 0.5, 0.0);
      glVertex3f(0.5, 0.5, 0.0);
      glVertex3f(0.5, -0.5, 0.0);
      glEnd();

      if (mapaConPlan[j][i] > 0)
      {
        /*glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
          glVertex3f(0.0, -0.5, 0.0);
          glVertex3f(-0.5, 0.0, 0.0);
          glVertex3f( 0.0, 0.5, 0.0);
          glVertex3f( 0.5, 0.0, 0.0);
        glEnd();*/

        // glColor3f(0.0,1.0,1.0);
        if (mapaConPlan[j][i] == 1 or mapaConPlan[j][i] == 2)
        {
          if (mapaConPlan[j][i] == 1)
            colorCeldaOpuestoMM(mapaCompleto[j][i]);
          else
            colorCeldaOpuestoMM2(mapaCompleto[j][i]);

          glBegin(GL_QUADS);
          glVertex3f(0.0, -0.3, 0.0);
          glVertex3f(-0.3, 0.0, 0.0);
          glVertex3f(0.0, 0.3, 0.0);
          glVertex3f(0.3, 0.0, 0.0);
          glEnd();
        }
        if (mapaConPlan[j][i] == 3)
        {
          colorCeldaOpuestoMM(mapaCompleto[j][i]);
          glBegin(GL_TRIANGLES);
          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);
          glVertex3f(0.0f, 0.20f, 0.0);

          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(-0.30f, -0.10f, 0.0);
          glVertex3f(-0.10f, -0.20f, 0.0);

          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.0f, -0.40f, 0.0);
          glVertex3f(0.10f, -0.20f, 0.0);

          glVertex3f(0.10f, -0.20f, 0.0);
          glVertex3f(0.30f, -0.10f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);

          glVertex3f(-0.10f, 0.0f, 0.0);
          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);

          glVertex3f(-0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, -0.20f, 0.0);
          glVertex3f(0.10f, 0.0f, 0.0);
          glEnd();
        }
      }

      glPopMatrix();
    }
  }

  if (level > 1)
  {
    for (unsigned int i = 0; i < objetivosActivos.size(); i = i + 3)
    {
      glPushMatrix();
      glTranslatef(((GLfloat)objetivosActivos[i + 1] - (GLfloat)colMed) * ratio, ((GLfloat)filaMed - (GLfloat)objetivosActivos[i]) * ratio, 0);
      if (objetivosActivos[i + 2] == 0)
        glColor3f(0.0, 1.0, 1.0);
      else
        glColor3f(1.0, 0.0, 0.0);

      glScalef(ratio, ratio, ratio);
      glutSolidSphere(0.5, 6, 2);

      glColor3f(0.0, 0.0, 0.0);
      glScalef(7 * ratio / 8, 7 * ratio / 8, 7 * ratio / 8);
      glutSolidSphere(0.5, 6, 2);

      if (objetivosActivos[i + 2] == 0)
        glColor3f(0.0, 1.0, 1.0);
      else
        glColor3f(1.0, 0.0, 0.0);

      glScalef(ratio / 2, ratio / 2, ratio / 2);
      glutSolidSphere(0.5, 6, 2);
      glPopMatrix();
    }
  }

  for (unsigned int i = 0; i < entidades->size(); i++)
  {
    // cout << "Entidad " << i << " tipo " << (*entidades)[i]->getSubTipoChar() << " (" << (*entidades)[i]->getFil() << ", " << (*entidades)[i]->getCol() << endl;
    if ((*entidades)[i]->getHitbox())
    {
      glPushMatrix();

      glTranslatef(((GLfloat)(*entidades)[i]->getCol() - (GLfloat)colMed) * ratio, ((GLfloat)filaMed - (GLfloat)(*entidades)[i]->getFil()) * ratio, 0.0);

      OrientacionEntidadMM((*entidades)[i]->getOrientacion());

      glScalef(ratio, ratio, ratio);

      formaEntidad((*entidades)[i]->getSubTipoChar());

      glPopMatrix();
    }
  }
}

void Mapa::drawFirstPerson(int entidad)
{
  vector<vector<unsigned char>> mapaConPlan = (*entidades)[0]->getMapaPlan();
  JoinMapasPlan(mapaConPlan, (*entidades)[1]->getMapaPlan());
  unsigned int colMed, filaMed;

  colMed = (mapaCompleto.size() / 2);
  filaMed = (mapaCompleto[0].size() / 2);

  if (entidades->size() > 0)
  {
    z = (*entidades)[entidad]->getFil() + 1;
    x = (*entidades)[entidad]->getCol() + 1;

    switch ((*entidades)[entidad]->getOrientacion())
    {
    case norte:
      angulo = 180.0;
      break;
    case noreste:
      angulo = 215.0;
      break;
    case este:
      angulo = 270.0;
      break;
    case sureste:
      angulo = 315.0;
      break;
    case sur:
      angulo = 0.0;
      break;
    case suroeste:
      angulo = 45.0;
      break;
    case oeste:
      angulo = 90.0;
      break;
    case noroeste:
      angulo = 135.0;
      break;
    }
  }

  // Camara
  gluLookAt(
      ((GLfloat)colMed - (GLfloat)x) * (5.0), 3.5 + Y2, ((GLfloat)filaMed - (GLfloat)z) * 5.0,
      (((GLfloat)colMed - (GLfloat)x) * (5.0)) + sin((angulo * 2.0 * M_PI) / 360.0), 3.4 - Y1, (((GLfloat)filaMed - (GLfloat)z) * 5.0) - cos((angulo * 2.0 * M_PI) / 360.0),
      0.0, 3.3, 0.0);

  int fs = x - 15;
  if (fs < 0)
    fs = 0;
  int fi = x + 15;
  if (fi >= mapaCompleto.size())
    fi = mapaCompleto.size() - 1;

  int ci = z - 15;
  if (ci < 0)
    ci = 0;
  int cd = z + 15;
  if (cd >= mapaCompleto.size())
    cd = mapaCompleto.size() - 1;

  for (unsigned int i = fs; i < fi; i++)
  {
    for (unsigned int j = ci; j < cd; j++)
    {
      // Suelo
      glPushMatrix();
      int h = mapaAlturas[j][i] - '0';

      if (mapaCompleto[j][i] == 'K')
      {
        glTranslatef(((GLfloat)filaMed - (GLfloat)i - 1) * 5.0, 0.25, ((GLfloat)colMed - (GLfloat)j - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i], mapaAlturas[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);
      }
      else if (mapaCompleto[j][i] == 'A')
      {
        glTranslatef(((GLfloat)filaMed - (GLfloat)i - 1) * 5.0, -0.5, ((GLfloat)colMed - (GLfloat)j - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i], mapaAlturas[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);
      }
      else
      {
        glTranslatef(((GLfloat)filaMed - (GLfloat)i - 1) * 5.0, 0.0, ((GLfloat)colMed - (GLfloat)j - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i], mapaAlturas[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);

        if (mapaConPlan[j][i] == 1)
        {
          glColor3f(1.0, 0.0, 0.0);
          glScalef(1.0, 1.2, 1.0);
          glutSolidSphere(0.43, 50, 50);
          // glutSolidCube(0.2);
        }
        else if (mapaConPlan[j][i] == 2)
        {
          glColor3f(0, 0.5, 0.5);
          glScalef(1.0, 1.2, 1.0);
          glutSolidSphere(0.43, 50, 50);
          // glutSolidCube(0.2);
        }
        else if (mapaConPlan[j][i] == 3)
        {
          glColor3f(1.0, 0.0, 0.0);
          glScalef(1.0, 1.2, 1.0);
          glutSolidSphere(0.43, 50, 50);
          // glutSolidOctahedron();
          // glutSolidCube(0.4);
        }
      }

      glPopMatrix();

      // Complementos
      glPushMatrix();
      glTranslatef(((GLfloat)filaMed - (GLfloat)i - 1) * 5.0, 0.0, ((GLfloat)colMed - (GLfloat)j - 1) * 5.0);
      complementosCelda(mapaCompleto[j][i], mapaAlturas[j][i]);
      glPopMatrix();
    }
  }

  Luz luz(GL_LIGHT0);
  luz.setPosicion(_vertex3<float>(0.0, 20.0, 0.0));
  luz.encender();
  luz.draw();

  // Dibujamos las entidades
  for (unsigned int i = 0; i < entidades->size(); i++)
  {
    if (i != entidad)
    {
      z = (*entidades)[i]->getFil() + 1;
      x = (*entidades)[i]->getCol() + 1;

      glPushMatrix();
      glTranslatef(((GLfloat)colMed - (GLfloat)x) * 5.0, 0.4, ((GLfloat)filaMed - (GLfloat)z) * 5.0);

      OrientacionEntidadFP((*entidades)[i]->getOrientacion());
      (*entidades)[i]->draw(1);

      glPopMatrix();
    }
  }
}

int Mapa::QuienEnCasilla(int f, int c)
{
  int out = -1; // Desocupada
  unsigned int i = 0;
  while ((i < entidades->size()) and (out == -1))
  {
    if (((*entidades)[i]->getFil() == f and (*entidades)[i]->getCol() == c) and ((*entidades)[i]->getHitbox()))
    {
      out = i;
    }
    i++;
  }

  return out;
}

int Mapa::casillaOcupada(unsigned int entidad)
{
  unsigned int f = (*entidades)[entidad]->getFil();
  unsigned int c = (*entidades)[entidad]->getCol();

  switch ((*entidades)[entidad]->getOrientacion())
  {
  case norte:
    f--;
    break;
  case noreste:
    f--;
    c++;
    break;
  case este:
    c++;
    break;
  case sureste:
    f++;
    c++;
    break;
  case sur:
    f++;
    break;
  case suroeste:
    f++;
    c--;
    break;
  case oeste:
    c--;
    break;
  case noroeste:
    f--;
    c--;
    break;
  }

  return QuienEnCasilla(f, c);
}

pair<int, int> Mapa::NCasillasDelante(unsigned int entidad, int casillas)
{
  int out = -1;
  unsigned int f = (*entidades)[entidad]->getFil();
  unsigned int c = (*entidades)[entidad]->getCol();

  switch ((*entidades)[entidad]->getOrientacion())
  {
  case norte:
    for (int i = 0; i < casillas; i++)
    {
      f--;
    }
    break;
  case noreste:
    for (int i = 0; i < casillas; i++)
    {
      f--;
      c++;
    }
    break;
  case este:
    for (int i = 0; i < casillas; i++)
    {
      c++;
    }
    break;
  case sureste:
    for (int i = 0; i < casillas; i++)
    {
      f++;
      c++;
    }
    break;
  case sur:
    for (int i = 0; i < casillas; i++)
    {
      f++;
    }
    break;
  case suroeste:
    for (int i = 0; i < casillas; i++)
    {
      f++;
      c--;
    }
    break;
  case oeste:
    for (int i = 0; i < casillas; i++)
    {
      c--;
    }
    break;
  case noroeste:
    for (int i = 0; i < casillas; i++)
    {
      f--;
      c--;
    }
    break;
  }

  pair<int, int> r;
  r.first = f;
  r.second = c;
  return r;
}

// Devuelve 0 si es posible correr,
//          1 choca contra un muro.
//          2 cae en un precipicio.
//          3 choca contra un auxiliar.
//          4 choca contra un excursionista.
//          5 choca contra un vandalo.
//          6 choca contra un árbol.

int Mapa::EsPosibleCorrer(unsigned int entidad, int avance_size)
{
  int out = -4;

  unsigned int *f = new unsigned int[avance_size + 1];
  unsigned int *c = new unsigned int[avance_size + 1];

  f[0] = (*entidades)[entidad]->getFil();
  c[0] = (*entidades)[entidad]->getCol();

  int accesible = 0;

  for (int i = 1; i < avance_size + 1 and accesible == 0; i++)
  {
    switch ((*entidades)[entidad]->getOrientacion())
    {
    case norte:
      f[i] = f[i - 1] - 1;
      c[i] = c[i - 1];
      break;

    case noreste:
      f[i] = f[i - 1] - 1;
      c[i] = c[i - 1] + 1;
      break;

    case este:
      f[i] = f[i - 1];
      c[i] = c[i - 1] + 1;
      break;

    case sureste:
      f[i] = f[i - 1] + 1;
      c[i] = c[i - 1] + 1;
      break;

    case sur:
      f[i] = f[i - 1] + 1;
      c[i] = c[i - 1];
      break;

    case suroeste:
      f[i] = f[i - 1] + 1;
      c[i] = c[i - 1] - 1;
      break;

    case oeste:
      f[i] = f[i - 1];
      c[i] = c[i - 1] - 1;
      break;

    case noroeste:
      f[i] = f[i - 1] - 1;
      c[i] = c[i - 1] - 1;
      break;
    }

    if (mapaCompleto[f[i]][c[i]] == 'M')
      accesible = 1;
    else if (mapaCompleto[f[i]][c[i]] == 'P')
      accesible = 2;
    else if (mapaCompleto[f[i]][c[i]] == 'B')
      accesible = 6;
    else
    {
      unsigned int j = 0;
      while ((j < entidades->size()) and (accesible == 0))
      {
        if ((*entidades)[j]->getFil() == f[i] and (*entidades)[j]->getCol() == c[i] and (*entidades)[j]->getHitbox() and entidad != j)
        {
          if ((*entidades)[j]->getSubTipo() == auxiliar)
            accesible = 3;
          else if ((*entidades)[j]->getSubTipo() == vandalo)
            accesible = 5;
          else
            accesible = 4;
        }
        j++;
      }
    }
  }

  return accesible;
}

int Mapa::casillaOcupadaThrow(unsigned int entidad, int &fil, int &col)
{
  int out = -4;

  unsigned int f1 = (*entidades)[entidad]->getFil();
  unsigned int f2 = (*entidades)[entidad]->getFil();
  unsigned int f3 = (*entidades)[entidad]->getFil();

  unsigned int c1 = (*entidades)[entidad]->getCol();
  unsigned int c2 = (*entidades)[entidad]->getCol();
  unsigned int c3 = (*entidades)[entidad]->getCol();

  switch ((*entidades)[entidad]->getOrientacion())
  {
  case norte:
    f1--;
    f2 -= 2;
    f3 -= 3;
    break;

  case noreste:
    f1--;
    f2 -= 2;
    f3 -= 3;
    c1++;
    c2 += 2;
    c3 += 3;
    break;

  case este:
    c1++;
    c2 += 2;
    c3 += 3;
    break;

  case sureste:
    f1++;
    f2 += 2;
    f3 += 3;
    c1++;
    c2 += 2;
    c3 += 3;
    break;

  case sur:
    f1++;
    f2 += 2;
    f3 += 3;
    break;

  case suroeste:
    f1++;
    f2 += 2;
    f3 += 3;
    c1--;
    c2 -= 2;
    c3 -= 3;
    break;

  case oeste:
    c1--;
    c2 -= 2;
    c3 -= 3;
    break;

  case noroeste:
    f1--;
    f2 -= 2;
    f3 -= 3;
    c1--;
    c2 -= 2;
    c3 -= 3;
    break;
  }

  unsigned int i = 0;
  while ((i < entidades->size()) and (out == -1))
  {
    if ((*entidades)[i]->getFil() == f1 and (*entidades)[i]->getCol() == c1 and (*entidades)[i]->getHitbox() and entidad != i)
    {
      out = i;
    }

    if ((*entidades)[i]->getFil() == f2 and (*entidades)[i]->getCol() == c2 and (*entidades)[i]->getHitbox() and entidad != i)
    {
      out = i;
    }

    if ((*entidades)[i]->getFil() == f3 and (*entidades)[i]->getCol() == c3 and (*entidades)[i]->getHitbox() and entidad != i)
    {
      out = i;
    }
    i++;
  }

  if (out == -1)
  {
    if (mapaCompleto[f3][c3] == 'S' or mapaCompleto[f3][c3] == 'T')
    {
      fil = f3;
      col = c3;
      out = -3;
    }
    else if (mapaCompleto[f2][c2] == 'S' or mapaCompleto[f2][c2] == 'T')
    {
      fil = f2;
      col = c2;
      out = -2;
    }
    else if (mapaCompleto[f1][c1] == 'S' or mapaCompleto[f1][c1] == 'T')
    {
      fil = f1;
      col = c1;
      out = -1;
    }
  }

  return out;
}

void Mapa::girarCamaraIzquierda(int grados)
{
  if (grados == 90)
  {
    angulo -= 90.0;
    if (angulo < 0.0)
    {
      angulo = 270.0;
    }
  }
  else if (grados == 45)
  {
    angulo -= 45.0;
    if (angulo < 0.0)
    {
      angulo = 315.0;
    }
  }
}

void Mapa::girarCamaraDerecha(int grados)
{
  if (grados == 90)
  {
    angulo += 90.0;
    if (angulo >= 360.0)
    {
      angulo = 0.0;
    }
  }
  else if (grados == 45)
  {
    angulo += 45.0;
    if (angulo >= 360.0)
    {
      angulo = 360.0 - angulo;
    }
  }
}

unsigned char Mapa::entidadEnCelda(unsigned int f, unsigned int c)
{
  unsigned char out = '_';
  bool encontrado = false;
  unsigned int aux = 0, i = 0;

  while ((i < entidades->size()) and (!encontrado))
  {
    if ((*entidades)[i]->getFil() == f and (*entidades)[i]->getCol() == c and (*entidades)[i]->getHitbox())
    {
      encontrado = true;
      aux = i;
    }
    i++;
  }

  if (encontrado and (*entidades)[aux]->getHitbox())
  {
    switch ((*entidades)[aux]->getSubTipo())
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
  }

  return out;
}

vector<vector<unsigned char>> Mapa::vision(unsigned int Entidad)
{
  unsigned int fil = (*entidades)[Entidad]->getFil();
  unsigned int col = (*entidades)[Entidad]->getCol();
  Orientacion orient = (*entidades)[Entidad]->getOrientacion();
  bool _jugador = (*entidades)[Entidad]->getTipo() == jugador;

  vector<vector<unsigned char>> fov;
  vector<unsigned char> aux(16);
  fov.push_back(aux);
  fov.push_back(aux);
  fov.push_back(aux);

  fov[0][0] = getCelda(fil, col);       // Terreno
  fov[1][0] = entidadEnCelda(fil, col); // Agentes
  fov[2][0] = alturaEnCelda(fil, col);  // Algura

  int index = 1;

  switch (orient)
  {
  case norte:
    for (int f = 1; f <= 3; f++)
    {
      for (int c = -f; c <= f; c++)
      {
        fov[0][index] = getCelda(fil - f, col + c);
        fov[1][index] = entidadEnCelda(fil - f, col + c);
        fov[2][index] = alturaEnCelda(fil - f, col + c);
        index++;
      }
    }
    break;
  case noreste:
    fov[0][1] = getCelda(fil - 1, col);
    fov[0][2] = getCelda(fil - 1, col + 1);
    fov[0][3] = getCelda(fil, col + 1);

    fov[0][4] = getCelda(fil - 2, col);
    fov[0][5] = getCelda(fil - 2, col + 1);
    fov[0][6] = getCelda(fil - 2, col + 2);
    fov[0][7] = getCelda(fil - 1, col + 2);
    fov[0][8] = getCelda(fil, col + 2);

    fov[0][9] = getCelda(fil - 3, col);
    fov[0][10] = getCelda(fil - 3, col + 1);
    fov[0][11] = getCelda(fil - 3, col + 2);
    fov[0][12] = getCelda(fil - 3, col + 3);
    fov[0][13] = getCelda(fil - 2, col + 3);
    fov[0][14] = getCelda(fil - 1, col + 3);
    fov[0][15] = getCelda(fil, col + 3);

    fov[1][1] = entidadEnCelda(fil - 1, col);
    fov[1][2] = entidadEnCelda(fil - 1, col + 1);
    fov[1][3] = entidadEnCelda(fil, col + 1);

    fov[1][4] = entidadEnCelda(fil - 2, col);
    fov[1][5] = entidadEnCelda(fil - 2, col + 1);
    fov[1][6] = entidadEnCelda(fil - 2, col + 2);
    fov[1][7] = entidadEnCelda(fil - 1, col + 2);
    fov[1][8] = entidadEnCelda(fil, col + 2);

    fov[1][9] = entidadEnCelda(fil - 3, col);
    fov[1][10] = entidadEnCelda(fil - 3, col + 1);
    fov[1][11] = entidadEnCelda(fil - 3, col + 2);
    fov[1][12] = entidadEnCelda(fil - 3, col + 3);
    fov[1][13] = entidadEnCelda(fil - 2, col + 3);
    fov[1][14] = entidadEnCelda(fil - 1, col + 3);
    fov[1][15] = entidadEnCelda(fil, col + 3);

    fov[2][1] = alturaEnCelda(fil - 1, col);
    fov[2][2] = alturaEnCelda(fil - 1, col + 1);
    fov[2][3] = alturaEnCelda(fil, col + 1);

    fov[2][4] = alturaEnCelda(fil - 2, col);
    fov[2][5] = alturaEnCelda(fil - 2, col + 1);
    fov[2][6] = alturaEnCelda(fil - 2, col + 2);
    fov[2][7] = alturaEnCelda(fil - 1, col + 2);
    fov[2][8] = alturaEnCelda(fil, col + 2);

    fov[2][9] = alturaEnCelda(fil - 3, col);
    fov[2][10] = alturaEnCelda(fil - 3, col + 1);
    fov[2][11] = alturaEnCelda(fil - 3, col + 2);
    fov[2][12] = alturaEnCelda(fil - 3, col + 3);
    fov[2][13] = alturaEnCelda(fil - 2, col + 3);
    fov[2][14] = alturaEnCelda(fil - 1, col + 3);
    fov[2][15] = alturaEnCelda(fil, col + 3);
    break;
  case este:
    for (int c = 1; c <= 3; c++)
    {
      for (int f = -c; f <= c; f++)
      {
        fov[0][index] = getCelda(fil + f, col + c);
        fov[1][index] = entidadEnCelda(fil + f, col + c);
        fov[2][index] = alturaEnCelda(fil + f, col + c);
        index++;
      }
    }
    break;
  case sureste:
    fov[0][1] = getCelda(fil, col + 1);
    fov[0][2] = getCelda(fil + 1, col + 1);
    fov[0][3] = getCelda(fil + 1, col);

    fov[0][4] = getCelda(fil, col + 2);
    fov[0][5] = getCelda(fil + 1, col + 2);
    fov[0][6] = getCelda(fil + 2, col + 2);
    fov[0][7] = getCelda(fil + 2, col + 1);
    fov[0][8] = getCelda(fil + 2, col);

    fov[0][9] = getCelda(fil, col + 3);
    fov[0][10] = getCelda(fil + 1, col + 3);
    fov[0][11] = getCelda(fil + 2, col + 3);
    fov[0][12] = getCelda(fil + 3, col + 3);
    fov[0][13] = getCelda(fil + 3, col + 2);
    fov[0][14] = getCelda(fil + 3, col + 1);
    fov[0][15] = getCelda(fil + 3, col);

    fov[1][1] = entidadEnCelda(fil, col + 1);
    fov[1][2] = entidadEnCelda(fil + 1, col + 1);
    fov[1][3] = entidadEnCelda(fil + 1, col);

    fov[1][4] = entidadEnCelda(fil, col + 2);
    fov[1][5] = entidadEnCelda(fil + 1, col + 2);
    fov[1][6] = entidadEnCelda(fil + 2, col + 2);
    fov[1][7] = entidadEnCelda(fil + 2, col + 1);
    fov[1][8] = entidadEnCelda(fil + 2, col);

    fov[1][9] = entidadEnCelda(fil, col + 3);
    fov[1][10] = entidadEnCelda(fil + 1, col + 3);
    fov[1][11] = entidadEnCelda(fil + 2, col + 3);
    fov[1][12] = entidadEnCelda(fil + 3, col + 3);
    fov[1][13] = entidadEnCelda(fil + 3, col + 2);
    fov[1][14] = entidadEnCelda(fil + 3, col + 1);
    fov[1][15] = entidadEnCelda(fil + 3, col);

    fov[2][1] = alturaEnCelda(fil, col + 1);
    fov[2][2] = alturaEnCelda(fil + 1, col + 1);
    fov[2][3] = alturaEnCelda(fil + 1, col);

    fov[2][4] = alturaEnCelda(fil, col + 2);
    fov[2][5] = alturaEnCelda(fil + 1, col + 2);
    fov[2][6] = alturaEnCelda(fil + 2, col + 2);
    fov[2][7] = alturaEnCelda(fil + 2, col + 1);
    fov[2][8] = alturaEnCelda(fil + 2, col);

    fov[2][9] = alturaEnCelda(fil, col + 3);
    fov[2][10] = alturaEnCelda(fil + 1, col + 3);
    fov[2][11] = alturaEnCelda(fil + 2, col + 3);
    fov[2][12] = alturaEnCelda(fil + 3, col + 3);
    fov[2][13] = alturaEnCelda(fil + 3, col + 2);
    fov[2][14] = alturaEnCelda(fil + 3, col + 1);
    fov[2][15] = alturaEnCelda(fil + 3, col);

    break;
  case sur:
    for (int f = 1; f <= 3; f++)
    {
      for (int c = -f; c <= f; c++)
      {
        fov[0][index] = getCelda(fil + f, col - c);
        fov[1][index] = entidadEnCelda(fil + f, col - c);
        fov[2][index] = alturaEnCelda(fil + f, col - c);
        index++;
      }
    }
    break;
  case suroeste:
    fov[0][1] = getCelda(fil + 1, col);
    fov[0][2] = getCelda(fil + 1, col - 1);
    fov[0][3] = getCelda(fil, col - 1);

    fov[0][4] = getCelda(fil + 2, col);
    fov[0][5] = getCelda(fil + 2, col - 1);
    fov[0][6] = getCelda(fil + 2, col - 2);
    fov[0][7] = getCelda(fil + 1, col - 2);
    fov[0][8] = getCelda(fil, col - 2);

    fov[0][9] = getCelda(fil + 3, col);
    fov[0][10] = getCelda(fil + 3, col - 1);
    fov[0][11] = getCelda(fil + 3, col - 2);
    fov[0][12] = getCelda(fil + 3, col - 3);
    fov[0][13] = getCelda(fil + 2, col - 3);
    fov[0][14] = getCelda(fil + 1, col - 3);
    fov[0][15] = getCelda(fil, col - 3);

    fov[1][1] = entidadEnCelda(fil + 1, col);
    fov[1][2] = entidadEnCelda(fil + 1, col - 1);
    fov[1][3] = entidadEnCelda(fil, col - 1);

    fov[1][4] = entidadEnCelda(fil + 2, col);
    fov[1][5] = entidadEnCelda(fil + 2, col - 1);
    fov[1][6] = entidadEnCelda(fil + 2, col - 2);
    fov[1][7] = entidadEnCelda(fil + 1, col - 2);
    fov[1][8] = entidadEnCelda(fil, col - 2);

    fov[1][9] = entidadEnCelda(fil + 3, col);
    fov[1][10] = entidadEnCelda(fil + 3, col - 1);
    fov[1][11] = entidadEnCelda(fil + 3, col - 2);
    fov[1][12] = entidadEnCelda(fil + 3, col - 3);
    fov[1][13] = entidadEnCelda(fil + 2, col - 3);
    fov[1][14] = entidadEnCelda(fil + 1, col - 3);
    fov[1][15] = entidadEnCelda(fil, col - 3);

    fov[2][1] = alturaEnCelda(fil + 1, col);
    fov[2][2] = alturaEnCelda(fil + 1, col - 1);
    fov[2][3] = alturaEnCelda(fil, col - 1);

    fov[2][4] = alturaEnCelda(fil + 2, col);
    fov[2][5] = alturaEnCelda(fil + 2, col - 1);
    fov[2][6] = alturaEnCelda(fil + 2, col - 2);
    fov[2][7] = alturaEnCelda(fil + 1, col - 2);
    fov[2][8] = alturaEnCelda(fil, col - 2);

    fov[2][9] = alturaEnCelda(fil + 3, col);
    fov[2][10] = alturaEnCelda(fil + 3, col - 1);
    fov[2][11] = alturaEnCelda(fil + 3, col - 2);
    fov[2][12] = alturaEnCelda(fil + 3, col - 3);
    fov[2][13] = alturaEnCelda(fil + 2, col - 3);
    fov[2][14] = alturaEnCelda(fil + 1, col - 3);
    fov[2][15] = alturaEnCelda(fil, col - 3);
    break;
  case oeste:
    for (int c = 1; c <= 3; c++)
    {
      for (int f = -c; f <= c; f++)
      {
        fov[0][index] = getCelda(fil - f, col - c);
        fov[1][index] = entidadEnCelda(fil - f, col - c);
        fov[2][index] = alturaEnCelda(fil - f, col - c);
        index++;
      }
    }
    break;
  case noroeste:
    fov[0][1] = getCelda(fil, col - 1);
    fov[0][2] = getCelda(fil - 1, col - 1);
    fov[0][3] = getCelda(fil - 1, col);

    fov[0][4] = getCelda(fil, col - 2);
    fov[0][5] = getCelda(fil - 1, col - 2);
    fov[0][6] = getCelda(fil - 2, col - 2);
    fov[0][7] = getCelda(fil - 2, col - 1);
    fov[0][8] = getCelda(fil - 2, col);

    fov[0][9] = getCelda(fil, col - 3);
    fov[0][10] = getCelda(fil - 1, col - 3);
    fov[0][11] = getCelda(fil - 2, col - 3);
    fov[0][12] = getCelda(fil - 3, col - 3);
    fov[0][13] = getCelda(fil - 3, col - 2);
    fov[0][14] = getCelda(fil - 3, col - 1);
    fov[0][15] = getCelda(fil - 3, col);

    fov[1][1] = entidadEnCelda(fil, col - 1);
    fov[1][2] = entidadEnCelda(fil - 1, col - 1);
    fov[1][3] = entidadEnCelda(fil - 1, col);

    fov[1][4] = entidadEnCelda(fil, col - 2);
    fov[1][5] = entidadEnCelda(fil - 1, col - 2);
    fov[1][6] = entidadEnCelda(fil - 2, col - 2);
    fov[1][7] = entidadEnCelda(fil - 2, col - 1);
    fov[1][8] = entidadEnCelda(fil - 2, col);

    fov[1][9] = entidadEnCelda(fil, col - 3);
    fov[1][10] = entidadEnCelda(fil - 1, col - 3);
    fov[1][11] = entidadEnCelda(fil - 2, col - 3);
    fov[1][12] = entidadEnCelda(fil - 3, col - 3);
    fov[1][13] = entidadEnCelda(fil - 3, col - 2);
    fov[1][14] = entidadEnCelda(fil - 3, col - 1);
    fov[1][15] = entidadEnCelda(fil - 3, col);

    fov[2][1] = alturaEnCelda(fil, col - 1);
    fov[2][2] = alturaEnCelda(fil - 1, col - 1);
    fov[2][3] = alturaEnCelda(fil - 1, col);

    fov[2][4] = alturaEnCelda(fil, col - 2);
    fov[2][5] = alturaEnCelda(fil - 1, col - 2);
    fov[2][6] = alturaEnCelda(fil - 2, col - 2);
    fov[2][7] = alturaEnCelda(fil - 2, col - 1);
    fov[2][8] = alturaEnCelda(fil - 2, col);

    fov[2][9] = alturaEnCelda(fil, col - 3);
    fov[2][10] = alturaEnCelda(fil - 1, col - 3);
    fov[2][11] = alturaEnCelda(fil - 2, col - 3);
    fov[2][12] = alturaEnCelda(fil - 3, col - 3);
    fov[2][13] = alturaEnCelda(fil - 3, col - 2);
    fov[2][14] = alturaEnCelda(fil - 3, col - 1);
    fov[2][15] = alturaEnCelda(fil - 3, col);
    break;
  }

  return fov;
}
