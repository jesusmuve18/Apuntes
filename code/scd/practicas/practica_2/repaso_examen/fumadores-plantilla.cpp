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
   num_fumadores = 3,
   num_iter = 10;

Semaphore
   espera_estanquero=1;

vector<Semaphore> espera_cliente;
bool fin;

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

void funcion_hebra_estanquero()
{
   int i;

   for(int it=0; it<num_iter; it++) {
      i=producir_ingrediente();
      espera_estanquero.sem_wait(); // mostrador libre
      cout << "puesto ingrediente "<<i<<endl;
      espera_cliente[i].sem_signal(); // ingrediente puesto
   }

   espera_estanquero.sem_wait(); // mostrador libre

   fin=true;
   for(int i=0; i<num_fumadores; i++){
      espera_cliente[i].sem_signal();
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
void  funcion_hebra_fumador( int num_fumador )
{
   while( !fin )
   {
      espera_cliente[num_fumador].sem_wait();

      if(!fin) {
         cout << "retirado ingrediente "<<num_fumador<<endl<<flush;

         espera_estanquero.sem_signal();

         fumar(num_fumador);
      }
      
   }
}

//----------------------------------------------------------------------

int main()
{

   for(int i=0; i<num_fumadores; i++) {
      espera_cliente.push_back(Semaphore(0));
   }
   
   thread 
      hebras_fumador[num_fumadores], 
      hebra_estanquero(funcion_hebra_estanquero);


   for(int i=0; i<num_fumadores; i++) {
      hebras_fumador[i] = thread(funcion_hebra_fumador, i);
   }

   hebra_estanquero.join();

   for(int i=0; i<num_fumadores; i++) {
      hebras_fumador[i].join();
   }

}
