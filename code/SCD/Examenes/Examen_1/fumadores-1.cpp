#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <vector>
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int          num_fumadores   = 3 ; 
Semaphore              mostrador_libre = 1;  
Semaphore ingrediente_en_mostrador[3]={0,0,0} ;
vector<Semaphore> a_dormir;
Semaphore informar = 0;

int contador_finalizacion=0;

const int num_iteraciones_dormir = 3;

mutex cout_mtx;

//-------------------------------------------------------------------------
int producir_ingrediente()
{
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );
   this_thread::sleep_for( duracion_produ );
   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;
   return num_ingrediente ;
}

//----------------------------------------------------------------------
void funcion_hebra_estanquero(  )
{    
   // Registro de los ingredientes que lleva proporcionados a cada fumador
   int contador[num_fumadores]={0};

   while (true)
   {
      int num_fumador = producir_ingrediente();

      contador[num_fumador]++;

      if(contador[num_fumador]==(num_iteraciones_dormir+1)){
         // Avisa al fumador para que despierte de su dulce sueño
         a_dormir[num_fumador].sem_signal();
      }

      mostrador_libre.sem_wait( ) ;

      cout_mtx.lock();
      cout << "Estanquero : PONE ingrediente a fumador " 
              << num_fumador << "." << endl ;
      cout_mtx.unlock();

      ingrediente_en_mostrador[ num_fumador ].sem_signal(  );      
   }
}

//-------------------------------------------------------------------------
void fumar( int num_fumador )
{
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );
   this_thread::sleep_for( duracion_fumar );
   cout << "                    Fumador " << num_fumador << ": TERMINA FUMAR" << endl<<flush;
}

//----------------------------------------------------------------------
void  funcion_hebra_fumador( int num_fumador )
{  
   int num_iteracion=0;

   while( true )
   { 
      ingrediente_en_mostrador[num_fumador].sem_wait(  );

      num_iteracion++;

      cout_mtx.lock();
      cout << "                    Fumador " << num_fumador << "  : OBTIENE ING (iteracion "<< num_iteracion <<")"<< endl;
      cout_mtx.unlock();
      
      mostrador_libre.sem_signal(  );

      if(num_iteracion==3) {
         // Se echa a dormir esperando a que lo despierte el estanquero
         cout_mtx.lock();
         cout<<"Fumador "<<num_fumador<<" se echa a dormir"<<endl;
         cout_mtx.unlock();

         a_dormir[num_fumador].sem_wait();

         cout_mtx.lock();
         cout<<"Fumador "<<num_fumador<<" ha despertado"<<endl;
         cout_mtx.unlock();

         // Avisa a la hebra informadora de que se ha despertado
         if(num_fumador!=0){
            // Si es la primera que se despierta aumenta el contador
            if(contador_finalizacion==0){
               contador_finalizacion++;
            } else {  // Solo se avisa a la hebra impresora cuando las 2 hebras requeridas han despertado
               informar.sem_signal();
            }
         }
      } else { // Si no es su tercera iteración fuma normalmente
         fumar( num_fumador);
      }
   }
}
//----------------------------------------------------------------------
void funcion_hebra_informadora()
{
   while(true) {
      informar.sem_wait();
      cout_mtx.lock();
      cout<<"************* LAS FUMADORAS 1 Y 2 YA HAN DESCANSADO *************"<<endl;
      cout_mtx.unlock();
   }
}

//----------------------------------------------------------------------
// FUNCION MAIN
//----------------------------------------------------------------------

int main()
{

   // Inicializo el vector de semáforos
   for(int i=0; i<num_fumadores; i++){
      a_dormir.push_back(0);
   }

   thread hebra_estanquero( funcion_hebra_estanquero ),
          hebra_fumador[ num_fumadores ],
          informadora12(funcion_hebra_informadora);

   for( unsigned i = 0 ; i < num_fumadores ; i++ )
      hebra_fumador[i] = thread( funcion_hebra_fumador, i );


   hebra_estanquero.join();
   for( unsigned i = 0 ; i < num_fumadores ; i++ )
      hebra_fumador[i].join();
   
   informadora12.join();
}
//----------------------------------------------------------------------
