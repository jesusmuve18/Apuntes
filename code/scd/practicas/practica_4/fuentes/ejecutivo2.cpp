// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 4. Implementación de sistemas de tiempo real.
//
// Autor: Jesús Muñoz Velasco
// Archivo: ejecutivo2.cpp
// Implementación del segundo ejemplo de ejecutivo cíclico:
//
//   Datos de las tareas:
//   ------------
//   Ta.   T    C
//   ------------
//   A   500  100
//   B   500  150
//   C  1000  200
//   D  2000  240
//  -------------
//
//  Como mcm(500,500,1000,2000)=2000 => Tm=2000 (duración del ciclo principal)
//
//  Planificación (con Ts == 500 ms)
//
//  No hemos elegido Ts == 250 ms ya que aunque sería posible, los retrasos del sistema 
//  harían que no fuese viable. Una posible planificación con Ts=250 sería
//  
//  *---------*---------*---------*---------*---------*---------*---------*---------*
//  | A B     | C       | A B     | D       | A B     | C       | A B     |         |       
//  *---------*---------*---------*---------*---------*---------*---------*---------*
//  0        250       500       750      1000      1250      1500      1750      2000
//
//  
//  Sin embargo esta planificación tiene ciertos problemas como que T(A)+T(B)=250=Ts
//  Sabemos que el sistema tiene cierto margen de tiempo por lo que esto no dejaría el margen
//  necesario para que se cumpla el Deadline (ya que generaría retrasos).
//
//  Además, el último ciclo secundario se queda vacío desperdiciando bastante tiempo de CPU.
//
//  Se propone la siguiente planificación (con Ts == 500 ms):
//  
//  *---------*---------*---------*---------*
//  | A B C   | A B D   | A B C   | A B     |
//  *---------*---------*---------*---------*
//  0        500      1000      1500      2000
//
//  Esta solución nos quita los problemas anteriormente vistos ya que todos los procesos dejan 
//  un pequeño margen (tiempo libre) al final del ciclo secundario (ninguno se llena completamente).
//
//   ·) ¿Cuál es el mínimo tiempo de espera que queda al final de las
//      iteraciones del ciclo secundario con tu solución?
//
//      Calculemos el tiempo de espera de cada ciclo
//
//      1er ciclo: T(A) + T(B) + T(C) = 100 + 150 + 200 = 450  =>  Te = Ts - T(1er ciclo) = 500 - 450 =  50 ms
//      2o  ciclo: T(A) + T(B) + T(D) = 100 + 150 + 240 = 490  =>  Te = Ts - T(2o  ciclo) = 500 - 490 =  10 ms
//      3er ciclo: T(A) + T(B) + T(C) = 100 + 150 + 200 = 450  =>  Te = Ts - T(3er ciclo) = 500 - 450 =  50 ms
//      4o  ciclo: T(A) + T(B)        = 100 + 150       = 250  =>  Te = Ts - T(4o  ciclo) = 500 - 250 = 250 ms
//
//      Por tanto tenemos que el mínimo tiempo de espera es de 10 ms
//
//   ·) ¿Sería planificable si la tarea D tuviese un tiempo cómputo de
//       250 ms?
//
//      Con la planificación actual sí sería posible (por lo menos teóricamente) ya que en ese caso tendríamos que 
//      2o  ciclo: T(A) + T(B) + T(D') = 100 + 150 + 250 = 500  =>  Te = Ts - T(2o  ciclo) = 500 - 500 =  0 ms
//
//      Por lo que sería posible y haría que el tiempo de espera teórico del segundo ciclo fuese nulo.
//
//      En este caso tenemos que tener en cuenta que en un sistema real habría un cierto retraso por lo que
//      es posible que se salga de su ciclo secundario pero no incumpliría ningún deadline ya que el 
//      siguiente ciclo secundario tiene bastante margen (50 ms).
//
// -----------------------------------------------------------------------------

#include <string>
#include <iostream> // cout, cerr
#include <thread>
#include <chrono>   // utilidades de tiempo
#include <ratio>    // std::ratio_divide

using namespace std ;
using namespace std::chrono ;
using namespace std::this_thread ;

// tipo para duraciones en segundos y milisegundos, en coma flotante:
//typedef duration<float,ratio<1,1>>    seconds_f ;
typedef duration<float,ratio<1,1000>> milliseconds_f ;

// -----------------------------------------------------------------------------
// tarea genérica: duerme durante un intervalo de tiempo (de determinada duración)

void Tarea( const std::string & nombre, milliseconds tcomputo )
{
   cout << "   Comienza tarea " << nombre << " (C == " << tcomputo.count() << " ms.) ... " ;
   sleep_for( tcomputo );
   cout << "fin." << endl ;
}

// -----------------------------------------------------------------------------
// tareas concretas del problema:

void TareaA() { Tarea( "A", milliseconds(100) );  }
void TareaB() { Tarea( "B", milliseconds(150) );  }
void TareaC() { Tarea( "C", milliseconds(200) );  }
void TareaD() { Tarea( "D", milliseconds(240) );  }

// -----------------------------------------------------------------------------
// implementación del ejecutivo cíclico:

int main( int argc, char *argv[] )
{
   // Ts = duración del ciclo secundario (en unidades de milisegundos, enteros)
   const milliseconds Ts_ms( 500 );

   // ini_sec = instante de inicio de la iteración actual del ciclo secundario
   time_point<steady_clock> ini_sec = steady_clock::now();

   while( true ) // ciclo principal
   {
      cout << endl
           << "---------------------------------------" << endl
           << "Comienza iteración del ciclo principal." << endl ;

      for( int i = 1 ; i <= 4 ; i++ ) // ciclo secundario (4 iteraciones)
      {
         cout << endl << "Comienza iteración " << i << " del ciclo secundario." << endl ;

         switch( i )
         {
            case 1 : TareaA(); TareaB(); TareaC();           break ;
            case 2 : TareaA(); TareaB(); TareaD();           break ;
            case 3 : TareaA(); TareaB(); TareaC();           break ;
            case 4 : TareaA(); TareaB();                     break ;
         }

         // calcular el siguiente instante de inicio del ciclo secundario
         ini_sec += Ts_ms ;


         // esperar hasta el inicio de la siguiente iteración del ciclo secundario
         sleep_until( ini_sec );

         time_point<steady_clock> instante_actual   = steady_clock::now() ;
         steady_clock::duration   retraso       = instante_actual - ini_sec;
         
         cout<<"      Retraso: "<< milliseconds_f(retraso).count() <<"ms"<<endl;
      }
   }
}
