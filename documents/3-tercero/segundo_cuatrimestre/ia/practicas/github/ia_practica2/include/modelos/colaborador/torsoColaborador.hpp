#ifndef TORSOCOLABORADOR3D_HPP
#define TORSOCOLABORADOR3D_HPP

#include "obj3dlib.hpp"
#include "modelos/aldeano/cabeza.hpp"
#include "modelos/colaborador/brazo.hpp"
#include "modelos/aldeano/cintura.hpp"
#include <string.h>


class TorsoColaborador3D : public Objeto3D{
private:

public:
  TorsoColaborador3D(const std::string & filename);
  ~TorsoColaborador3D();

};

#endif
