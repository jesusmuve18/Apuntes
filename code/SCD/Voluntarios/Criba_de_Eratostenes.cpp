// -----------------------------------------------------------------------------
//
// Autor: Jesús Muñoz Velasco
//
// Sistemas concurrentes y Distribuidos.
// Ejercicio con MPI: paralelización de la criba de Eratóstenes
//
// Archivo: Cirba_de_Eratostenes.cpp

// El objetivo de este ejercicio es programar una versión distribuida del famoso 
// método de la criba de Erastotenes para obtener los N primeros números primos. 
// Se utilizará una red de procesos concurrentes conectados por una tubería o 
// "pipeline" de procesos, como en la figura siguiente, para generar los N primeros 
// números primos.

// Para programarlo, suponer que se conoce una constante nLímite que es mayor o 
// igual que el N-ésimo número primo. El proceso start generará el primer número 
// primo (el 2) y lo enviará al proceso  printer , que lo imprime; a continuación 
// genera el siguiente primo (el 3), lo envía al primer proceso filter , al que 
// seguirá enviando los siguientes números impares: 5, 7, 9, y así sucesivamente.

// Cada proceso de filter comienza su ejecución recibiendo un valor, un número primo, 
// del proceso anterior y enviándolo al proceso siguiente, según su posición en la 
// tubería; a continuación, recibirá números, de forma continua, no necesariamente 
// primos, del proceso anterior. Si el número recibido es divisible por el valor 
// primo que recibió inicialmente, entonces lo descarta; si no, lo enviará al 
// siguiente proceso para que lo filtre.

// El proceso final recibe el último número primo, lo envía al proceso de printer y, 
// a continuación, descarta todos los números siguientes que recibe, tras lo cual se 
// termina a sí mismo. El proceso printer debe programarse de forma que pueda recibir
// los números primos de los N procesos de la cadena y mostrarlos en la pantalla a 
// medida que los recibe.
//
// Compilación: mpicxx -std=c++11 -o Criba_de_Eratostenes Criba_de_Eratostenes.cpp
// Ejecución: mpirun -oversubscribe -np <número-de-procesos> ./Criba_de_Eratostenes
// -----------------------------------------------------------------------------

#include <mpi.h>
#include <iostream>
using namespace std;

int main( int argc, char *argv[] ) {

    // Declaración de variables
    int 
        rank,   // identificador de cada proceso
        size,   // número total de hebras
        origen,
        x,
        valor=0,
        NUM_LIMITE = 10000,
        FIN=-1;
    
    bool 
        fin=false;

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (rank==0){ // hebra "activadora"
        x=2; /* primer numero primo*/ 
        MPI_Send(&x, 1,MPI_INT,size-1,0,MPI_COMM_WORLD); // lo envío al impresor
        x=1;

        while(!fin){ // activación en cadena
            x+=2;
            MPI_Send(&x, 1,MPI_INT,rank+1,0,MPI_COMM_WORLD); // le envío a la siguiente hebra x
            if(x>=NUM_LIMITE){
                fin=true;
                x=FIN;
                MPI_Send(&x, 1,MPI_INT,rank+1,0,MPI_COMM_WORLD); // envío la primera señal de finalización
            }
        }
    }
    else if (rank == size-1){  /*este es el impresor*/
        while(!fin) {
            MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            origen=status.MPI_SOURCE;
            if(valor != FIN) {
                cout<<"Numero primo: "<<valor<<" (proceso "<<origen<<")"<<endl<<flush;
            } else {
                fin=true;
            }
        }        
    } else {  /*representa a los procesos filtros*/
        MPI_Recv(&valor, 1,MPI_INT,rank-1,0,MPI_COMM_WORLD,&status); // Recibo el valor de la hebra anterior
        MPI_Send(&valor, 1,MPI_INT,size-1,0,MPI_COMM_WORLD); // Lo envío a la impresora

        if (rank < (size - 2)) { // si no es el último filtro
            while (!fin) {
                MPI_Recv(&x, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status); // recibir número

                if(x!=FIN){
                    if (x % valor != 0) { // filtrar números no divisibles
                        MPI_Send(&x, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD); // enviar al siguiente filtro
                    }
                } else {
                    fin=true;
                }                  
            }
        } else {
            x=FIN; // Para apagar la impresora
        }
        MPI_Send(&x, 1,MPI_INT,rank+1,0,MPI_COMM_WORLD); // propago la señal de finalización (en cadena)
    }

    MPI_Finalize();
}