#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "scd.h"

using namespace std ;
using namespace scd ;

//**********************************************************************
// Variables globales

const unsigned 
   num_items = 15 ,   // número de items
	tam_vec   = 3 ;   // tamaño del buffer

unsigned  
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido.
   cont_cons[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha consumido.
   siguiente_dato       = 0 ;  // siguiente dato a producir en 'producir_dato' (solo se usa ahí)

Semaphore 
   espera_consumidor=0,       // Semáforo para que espere la hebra consumidora
   espera_productor=tam_vec;  // Semáforo para que espere la hebra productora

mutex
   acceso_lectura,     // Para secciones críticas
   acceso_escritura,
   escritura; 

// LIFO
int vec[tam_vec];
int primera_libre=0;

const int 
   np=2, 
   nc=4;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato(unsigned num_hebra, unsigned iter_local)
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   const unsigned dato_producido = num_hebra*num_items/np+iter_local ;
   cont_prod[dato_producido] ++ ;
   cout << "producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;

}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora( int num_hebra  )
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      int dato = producir_dato(num_hebra, i) ;
      
      sem_wait(espera_productor);
      //Escritura
      escritura.lock();
      vec[primera_libre++]=dato;
      // cout<<"Escritura produce dato "<<dato<<endl;
      escritura.unlock();
      sem_signal(espera_consumidor);
   }
}

//----------------------------------------------------------------------

void funcion_hebra_consumidora(  )
{
   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      sem_wait(espera_consumidor);
      int dato=vec[--primera_libre];
      //Lectura
      escritura.lock();
      cout<<"........................Consumidora lee dato "<<dato<<endl;
      escritura.unlock();
      sem_signal(espera_productor);
      consumir_dato( dato ) ;
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "-----------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución LIFO o FIFO ?)." << endl
        << "------------------------------------------------------------------" << endl
        << flush ;

   thread hebra_productora ( funcion_hebra_productora ),
          hebra_consumidora( funcion_hebra_consumidora );

   hebra_productora.join() ;
   hebra_consumidora.join() ;

   test_contadores();
}
