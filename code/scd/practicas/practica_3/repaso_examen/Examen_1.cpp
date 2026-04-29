#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;

const int 
    x=20,          //maximo de reposiciones de tartas por parte de Yoshi
    min_p=1,    // límite inferior de peso de una tarta
    max_p=10,   // límite superior de peso de una tarta
    min_t=1,    // límite inferior de número de tartas
    max_t=2;    // límite superior de número de tartas

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

void funcion_mario() {

}

void funcion_peach() {

}

void funcion_yoshi() {
    int num_tartas;
    int *tartas;
    for(int i=0; i<x; i++) {
        num_tartas=aleatorio<min_t,max_t>();
        tartas = new int(num_tartas);

        for(int j=0; j<num_tartas; j++){
            tartas[j]=aleatorio<min_p,max_p>();
        }

        cout<< "Yoshi repondrá con "<<num_tartas<<" tartas con peso(s): ";

        for(int j=0; j<num_tartas; j++){
            cout<<tartas[j]<<", ";
        }

        cout<<endl;

        delete[] tartas;
        tartas = nullptr;
    }
}

int main(int argc, char** argv){
    int id_propio, num_procesos_actual;

    // inicializar MPI, leer identif. de proceso y número de procesos
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
    MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

    if(num_procesos_actual!=3){
        cout << "Número de procesos incorrecto (debería ser 3)"<<endl;
        exit(-1);
    } else {
        if(id_propio==1) {
            funcion_mario();
        } else if(id_propio==2) {
            funcion_peach();
        } else {
            funcion_yoshi();
        }
    }

    MPI_Finalize();
    return 0;
}