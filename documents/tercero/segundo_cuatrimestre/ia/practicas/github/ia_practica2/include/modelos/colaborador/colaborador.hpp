#ifndef COLABORADOR3D_HPP
#define COLABORADOR3D_HPP

#include "modelos/colaborador/torsoColaborador.hpp"
#include "obj3dlib.hpp"
#include <string.h>


class Colaborador3D : public Objeto3D{
private:

public:
  Colaborador3D(const std::string & filename);
  ~Colaborador3D();

};

#endif
