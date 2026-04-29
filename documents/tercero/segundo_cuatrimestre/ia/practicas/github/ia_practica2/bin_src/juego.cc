#include <fstream>
#include "motorlib.hpp"

bool fileExists(std::string const &name)
{
  ifstream f(name.c_str());
  return f.good();
}

int main(int argc, char **argv)
{
  EnLinea argumentos;

  if (argc < 2)
  {
    srand(1);
    lanzar_motor_grafico(argc, argv);
  }
  else
  {
    srand(atoi(argv[2]));
    argumentos.ubicacion_mapa = argv[1];
    if (!fileExists(argumentos.ubicacion_mapa))
    {
      std::cout << "El mapa no existe\n";
      exit(1);
    }
    argumentos.semilla = atoi(argv[2]);
    argumentos.level = atoi(argv[3]);
    argumentos.posInicialRescatador.f = atoi(argv[4]);
    argumentos.posInicialRescatador.c = atoi(argv[5]);
    argumentos.posInicialRescatador.brujula = static_cast<Orientacion>(atoi(argv[6]));
    argumentos.posInicialAuxiliar.f = atoi(argv[7]);
    argumentos.posInicialAuxiliar.c = atoi(argv[8]);
    argumentos.posInicialAuxiliar.brujula = static_cast<Orientacion>(atoi(argv[9]));
    
    int i = 10;

    while (argc > i + 1)
    {
      // Reviso si es una posicion valida
      pair<pair<int,int>,bool> aux;
      aux.first.first = atoi(argv[i]);
      aux.first.second = atoi(argv[i+1]);
      aux.second = atoi(argv[i+2]);
      argumentos.listaObjetivos.push_back(aux);
      i += 3;
    }

    lanzar_motor_grafico_verOnline(argc, argv, argumentos);
  }

  exit(EXIT_SUCCESS);
}
