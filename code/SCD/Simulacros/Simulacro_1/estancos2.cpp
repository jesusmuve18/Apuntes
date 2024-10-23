#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int          num_fumadores   = 2 ;  // número de fumadores
Semaphore              mostrador_libre = 1,   // 1 si el mostrador está libre, 0 si no
                       espera_contrabandista = 0,       // 
                       espera_fumadores = 3;        //
mutex                  mutex_cout ; // mutex que evita salidas por pantalla mezcladas
Semaphore ingrediente_en_mostrador[2]={0,0} ; // vector de semáforos de fumadores

const int capacidad_buzon=3;
int buzon_secreto[capacidad_buzon],  // Buzon secreto para comunicación entre fumadores y contrabandista
    primera_libre=0,
    primera_ocupada=0,
    num_cigarros[num_fumadores];


//-------------------------------------------------------------------------
// Función que simula la acción de producir un ingrediente
int producir_ingrediente()
{  
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );
      // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );
   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;
   return num_ingrediente ;
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar
void fumar( int num_fumador )
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );
   // informa de que comienza a fumar
    cout << "                 Fumador " << num_fumador << "  :"
          << " EMPIEZA a fumar (" << duracion_fumar.count() << " ms)" << endl;
   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );
   // informa de que ha terminado de fumar
    cout << "                 Fumador " << num_fumador << "  : TERMINA de fumar." << endl;
}

//----------------------------------------------------------------------
// Función que simula la acción de sacar cigarros del buzón
void sacar_cigarros (int num_iter){
   // Duerme durante un tiempo aleatorio entre 20 y 150 milisegundos
   chrono::milliseconds duracion_dormir( aleatorio<20,150>() );
   this_thread::sleep_for(duracion_dormir);

   num_cigarros[buzon_secreto[primera_ocupada]]++; // Incrementa el contador de cigarros
   // cout<<"Número de cigarros entregados por Fumador "<< buzon_secreto[primera_ocupada]<< " en la iteracion " << num_iter << " es "<< num_cigarros[buzon_secreto[primera_ocupada]] <<endl;
   
   mutex_cout.lock();
   cout<<"El contrabandista ha recibido un cigarro del Fumador "<<buzon_secreto[primera_ocupada]<<endl;
   mutex_cout.unlock();

   primera_ocupada=(primera_ocupada+1)%capacidad_buzon;

   if((num_iter%4)==0) {
       // Informa del número de cigarros que aporta cada fumador
       mutex_cout.lock();
       cout<< "Informe " << (num_iter/4) << " del Contrabandista:" << endl;
       for(int i=0; i<num_fumadores; i++) {
         
         cout << "\tEl fumador "<< i <<" ha aportado " << num_cigarros[i] << " cigarros hasta el momento." << endl ;
         // num_cigarros[i]=0; // Reseteo el contador para volver a contar las últimas 4 iteraciones
       }
       mutex_cout.unlock();
      
   }

}


//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero
void funcion_hebra_estanquero(  )
{
   while (true)
   {
      // selecciona aleatoriamente un fumador
      int num_fumador = producir_ingrediente();

      // esperar hasta mostrador libre
       mostrador_libre.sem_wait( ) ;

      // Informa del ingrediente depositado
      mutex_cout.lock();
         cout << "Estanquero pone ingrediente para fumador " << num_fumador << "." << endl ;
      mutex_cout.unlock();

      // Señala que hay ingrediente disponible para fumador 'num_fumador'
      ingrediente_en_mostrador[ num_fumador ].sem_signal(  );
   }
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
   int iter=0;
   while( true )
   {  // Fumador inicia obtención de ingrediente
      ingrediente_en_mostrador[num_fumador].sem_wait(  );
      mutex_cout.lock();
         cout << "                 Fumador " << num_fumador << ": RETIRA ingrediente" << endl;
      mutex_cout.unlock();
      mostrador_libre.sem_signal(  );
      // fumar(num_fumador);

      // En lugar de fumar envían el cigarro al contrabandista
      espera_fumadores.sem_wait(); // Esperan a que haya un hueco

      // FALTA AÑADIR UN MUTEX PARA ESTE ACCESO
      buzon_secreto[primera_libre]=num_fumador; // Rellenan el primer hueco que encuentren
      primera_libre=(primera_libre+1)%capacidad_buzon;
      
      mutex_cout.lock();
         cout << "                 Fumador " << num_fumador << ": ENVÍA ingrediente a contrabandista" << endl;
      mutex_cout.unlock();
      espera_contrabandista.sem_signal(); // Indican al contrabandista que han depositado un ingrediente
   }
}
//----------------------------------------------------------------------
// función que ejecuta la hebra del contrabandista
void funcion_hebra_contrabandista() {

   int iter=0;
   while(true) {
      espera_contrabandista.sem_wait();
      iter++;
      sacar_cigarros(iter);
      espera_fumadores.sem_signal();
   }
}

//----------------------------------------------------------------------
int main()
{ 
   // Lanza Array de hebras fumadoras
   thread hebra_fumador[ num_fumadores ];
   for( unsigned long i = 0 ; i < num_fumadores ; i++ )
      hebra_fumador[i] = thread( funcion_hebra_fumador, i );
   // Lanza Hebra Estanquero
   thread hebra_estanquero( funcion_hebra_estanquero );

   // Inicializo los contadores de cigarros entregados
   for(int i=0; i<num_fumadores; i++) {
      num_cigarros[i]=0;
   }

   thread hebra_contrabandista(funcion_hebra_contrabandista);
   

   // Esperar finalización de hebras:
   hebra_estanquero.join();
   for( unsigned i = 0 ; i < num_fumadores ; i++ )
      hebra_fumador[i].join();
   hebra_contrabandista.join();
}
//----------------------------------------------------------------------