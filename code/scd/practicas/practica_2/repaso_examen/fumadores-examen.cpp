#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempoç
#include <vector>
#include "scd.h"

using namespace std ;
using namespace scd ;

// numero de fumadores 

const int 
   num_fumadores = 4,
   ingrediente_codiciado = 1;

mutex cout_mtx;

// *****************************************************************************
// clase para monitor buffer, version FIFO, semántica SC, multiples prod/cons

class Estanco : public HoareMonitor
{
 private:

 const static int
   EMPTY=-1;  // Indica que el mostrador está vacío

 int                        // variables permanentes
   mostrador;

bool
   ini_verif;

 CondVar                    // colas condicion:
   mostrador_libre,                          //  cola donde espera el estanquero
   disponible_ingrediente[num_fumadores] ,   //  cola donde esperan los fumadores
   verificacion_bloqueada;

 public:                    // constructor y métodos públicos
   Estanco() ;             // constructor
   void ponerIngrediente(int ingrediente);                // extraer un valor (sentencia L) (consumidor)
   void esperarRecogidaIngrediente();
   void consumirIngrediente(int i);
   void verificacion();
} ;
// -----------------------------------------------------------------------------

Estanco::Estanco() // Inicializar
{
   mostrador=EMPTY; //Mostrador vacío
   mostrador_libre      = newCondVar();
   verificacion_bloqueada = newCondVar();
   for(int i=0; i<num_fumadores; i++) {
      disponible_ingrediente[i] = newCondVar();
   }
   ini_verif=false;
}
// -----------------------------------------------------------------------------

void Estanco::ponerIngrediente(int ingrediente) {

   // Se supone que llegado a este punto debería de estar vacío el mostrador
   assert(mostrador==EMPTY);

   // Pongo el ingrediente en el mostrador
   mostrador=ingrediente;

   // Indico la disponibilidad del ingrediente
   disponible_ingrediente[ingrediente].signal();

}

void Estanco::esperarRecogidaIngrediente() {
   // Espera a que el mostrador se quede libre
   if(mostrador!=EMPTY) mostrador_libre.wait();

}

void Estanco::consumirIngrediente(int i) {

   if(i==num_fumadores-1) {
      
   }

   // Espero a que esté disponible el ingrediente que quiero consumir
   if(mostrador!=i) disponible_ingrediente[i].wait();

   // Cojo el ingrediente del mostrador
   mostrador=EMPTY; // Lo vacío

   // Indico que he dejado libre el mostrador
   mostrador_libre.signal();
}

void Estanco::verificacion() {
   if(ini_verif==false) {
      verificacion_bloqueada.wait();
   } 

   verificacion_bloqueada.signal();
   cout_mtx.lock();
   cout<<"El fumador "<<ingrediente_codiciado<<" ha fumado "<<
   cout_mtx.unlock();
   
}

//-------------------------------------------------------------------------
// Función que simula la acción de producir un ingrediente, como un retardo
// aleatorio de la hebra (devuelve número de ingrediente producido)

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(MRef<Estanco> estanco)
{
   int ingrediente;

   while(true) {
      ingrediente=producir_ingrediente();
      estanco->ponerIngrediente(ingrediente);

      cout_mtx.lock();
      cout<< "El estanquero pone el ingrediente "<<ingrediente<<" sobre el mostrador"<<endl;
      cout_mtx.unlock();

      estanco->esperarRecogidaIngrediente();

   }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador , MRef<Estanco> estanco)
{
   while(true)
   {
      estanco->consumirIngrediente(num_fumador);
      fumar(num_fumador);
   }
}
//----------------------------------------------------------------------
// función que ejecuta la hebra verificadora
void  funcion_hebra_verificadora(MRef<Estanco> estanco)
{
   while(true)
   {
      estanco->verificacion();
   }
}

//----------------------------------------------------------------------

int main()
{

   MRef<Estanco> estanco = Create<Estanco>();
   
   thread 
      hebras_fumador[num_fumadores], 
      hebra_fumador_3=thread(funcion_hebra_fumador, ingrediente_codiciado, estanco),
      hebra_estanquero(funcion_hebra_estanquero, estanco),
      hebra_verificadora=thread(funcion_hebra_verificadora, estanco);


   for(int i=0; i<num_fumadores; i++) {
      hebras_fumador[i] = thread(funcion_hebra_fumador, i, estanco);
   }

   hebra_estanquero.join();

   for(int i=0; i<num_fumadores; i++) {
      hebras_fumador[i].join();
   }

}
