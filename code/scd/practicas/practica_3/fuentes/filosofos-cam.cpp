// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-plantilla.cpp
// Implementación del problema de los filósofos (sin camarero).
// Plantilla para completar.
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// -----------------------------------------------------------------------------


#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_filosofos  = 5 ,              // número de filósofos 
   num_filo_ten   = 2*num_filosofos, // número de filósofos y tenedores 
   num_procesos   = num_filo_ten +1,   // número de procesos total (por ahora solo hay filo y ten)
   max_sentados   = 5,
   tag_sentarse   = 1,
   tag_levantarse = 2,
   id_camarero = num_filo_ten;


//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

// ---------------------------------------------------------------------

void funcion_filosofos( int id )
{
   int id_ten_izq = (id+1)              % num_filo_ten, //id. tenedor izq.
      id_ten_der = (id+num_filo_ten-1) % num_filo_ten; //id. tenedor der.

   int peticion=1;

   while ( true ) {
      // 1. Solicita sentarse
      cout <<"Filósofo " <<id << " solicita sentarse." <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, tag_sentarse, MPI_COMM_WORLD);

      // 2.1. Solicita tenedor izquierdo 
      cout <<"Filósofo " <<id << " solicita ten. izq." <<id_ten_izq <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

      // 2.2. Solicita tenedor derecho
      cout <<"Filósofo " <<id <<" solicita ten. der." <<id_ten_der <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

      // 3. Come
      cout <<"Filósofo " <<id <<" comienza a comer" <<endl ;
      sleep_for( milliseconds( aleatorio<10,100>() ) );

      // 4.1. Suelta tenedor izquierdo
      cout <<"Filósofo " <<id <<" suelta ten. izq. " <<id_ten_izq <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_izq, 0, MPI_COMM_WORLD);

      // 4.2. Suelta tenedor derecho
      cout<< "Filósofo " <<id <<" suelta ten. der. " <<id_ten_der <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_ten_der, 0, MPI_COMM_WORLD);

      // 5. Se levanta
      cout <<"Filósofo " <<id << " solicita levantarse." <<endl;
      MPI_Ssend(&peticion, 1, MPI_INT, id_camarero, tag_levantarse, MPI_COMM_WORLD);

      // 6. Piensa
      cout << "Filosofo " << id << " comienza a pensar" << endl;
      sleep_for( milliseconds( aleatorio<10,100>() ) );
   }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int valor, id_filosofo ;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {
     // ...... recibir petición de cualquier filósofo (completar)
     MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);

     // ...... guardar en 'id_filosofo' el id. del emisor (completar)
     id_filosofo = estado.MPI_SOURCE;

     cout <<"Ten. " <<id <<" ha sido cogido por filo. " <<id_filosofo/2 <<endl;

      // ...... recibir liberación de filósofo 'id_filosofo' (completar)
      MPI_Recv(&valor, 1, MPI_INT, id_filosofo, 0, MPI_COMM_WORLD, &estado);
      cout <<"Ten. "<< id<< " ha sido liberado por filo. " <<id_filosofo /2<<endl ;
  }
}
// ---------------------------------------------------------------------
void funcion_camarero() {
   int 
      contador_sentados=0,
      tag_emisor_aceptable,
      valor;

   MPI_Status estado ;                 // metadatos del mensaje recibido

   while(true)
   {
      // 1. Determinar si puede dejar sentarse

      if ( contador_sentados == max_sentados )     // si mesa lleno
         tag_emisor_aceptable = tag_levantarse ;     
      else                                         // sino
         tag_emisor_aceptable = MPI_ANY_TAG ;

      // 2. recibir un mensaje del emisor o emisores aceptables

      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, tag_emisor_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      if (estado.MPI_TAG==tag_sentarse) { // si ha sido el productor: insertar en buffer
         contador_sentados++;
         cout << "El camarero deja sentarse a Filósofo " << estado.MPI_SOURCE << endl ;
      } else {
         contador_sentados--;
         cout << "El camarero deja levantarse a Filósofo " << estado.MPI_SOURCE << endl ;
      }
   }
}

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
   {
      // ejecutar la función correspondiente a 'id_propio'
      if (id_propio == id_camarero){
         funcion_camarero();
      } else if ( id_propio % 2 == 0 )   // si es par
         funcion_filosofos( id_propio ); //   es un filósofo
      else                               // si es impar
         funcion_tenedores( id_propio ); //   es un tenedor
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
