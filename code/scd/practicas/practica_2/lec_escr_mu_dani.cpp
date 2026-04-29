// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// Archivo: prodcons1_su.cpp
//
// Ejemplo de un monitor en C++11 con semántica SU, para el problema
// del productor/consumidor, con productor y consumidor únicos.
// Opcion LIFO
//
// Historial:
// Creado el 30 Sept de 2022. (adaptado de prodcons2_su.cpp)
// 20 oct 22 --> paso este archivo de FIFO a LIFO, para que se corresponda con lo que dicen las transparencias
// -----------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <thread>
#include "scd.h"

using namespace std;
using namespace scd;

constexpr int
    num_items = 5,      // número de items a del array
    num_escritores = 5, // número de hebras consumidoras
    num_lectores = 5;   // número de hebras productoras

int cuanto_leo = 5,       // cuantos lectores pueden entrar a leer desde que un escritor entra a la cola de escritura
    pergamino[num_items]; // siguiente valor a devolver en 'producir_dato'

constexpr int
    min_ms = 20,  // tiempo minimo de espera en sleep_for
    max_ms = 100; // tiempo máximo de espera en sleep_for

mutex
    mtx; // mutex de escritura en pantalla
//**********************************************************************
// funciones
//----------------------------------------------------------------------

// *****************************************************************************
// clase para monitor buffer, version FIFO, semántica SC, multiples prod/cons

class LecEscMU1 : public HoareMonitor
{
private:
    static const int              // constantes ('static' ya que no dependen de la instancia)
        num_celdas_total = 10;    //   núm. de entradas del buffer
    int                           // variables permanentes
        buffer[num_celdas_total], //   buffer de tamaño fijo, con los datos
        n_lec;                    //   numero de lectores leyendo
    bool escrib = false;          // si hay un escritor escribiendo

    CondVar        // colas condicion:
        lectura,   //  cola donde espera el consumidor (n>0)
        escritura; //  cola donde espera el productor  (n<num_celdas_total)

public:                   // constructor y métodos públicos
    LecEscMU1();          // constructor
    void ini_lectura();   //  función llamada por el lector para leer
    void fin_lectura();   //  función llamada por el lector para finalizar lectura
    void ini_escritura(); //
    void fin_escritura(); //
};
// -----------------------------------------------------------------------------

LecEscMU1::LecEscMU1()
{
    n_lec = 0;                // buffer vacío
    escrib = false;           // buffer vacío
    lectura = newCondVar();   // inicialmente no hay nada que leer
    escritura = newCondVar(); // inicialmente no hay nada que escribir
}
// -----------------------------------------------------------------------------
// función llamada por el lector para leer

void LecEscMU1::ini_lectura()
{
    // esperar bloqueado hasta que 0 < n_lec
    if (escrib || cuanto_leo == 0)
        lectura.wait();

    assert(escrib == false);

    // hacer la operación de lectura, actualizando estado del monitor
    n_lec++;
    if (!(escritura.empty())){
        cuanto_leo--;
        //cout << "Hay escritores esperando " << endl;
    }
       

    // señalar al productor que hay un hueco libre, por si está esperando
    if (cuanto_leo > 0)
        lectura.signal();
}

// -----------------------------------------------------------------------------
// función llamada por el lector para finalizar lectura

void LecEscMU1::fin_lectura()
{
    // esperar bloqueado hasta que 0 < n_lec
    n_lec--;

    assert(escrib == false);

    if (n_lec == 0)
    {
        escritura.signal();
        cuanto_leo = 5;
    }
}

// -----------------------------------------------------------------------------

void LecEscMU1::ini_escritura()
{
    // esperar bloqueado hasta que 0 < n_lec
    if (escrib || n_lec > 0)
        escritura.wait();

    assert(escrib == false && n_lec == 0);

    // hacer la operación de lectura, actualizando estado del monitor
    escrib = true;
}

// -----------------------------------------------------------------------------
// función llamada por el lector para finalizar lectura

void LecEscMU1::fin_escritura()
{
    // esperar bloqueado hasta que 0 < n_lec
    escrib = false;

    assert(n_lec == 0);

    if (!(n_lec == 0))
        lectura.signal();
    else
        escritura.signal();
}

// *****************************************************************************
// funciones de hebras

void funcion_hebra_lectora(MRef<LecEscMU1> monitor, int index)
{
    while (true)
    {
        monitor->ini_lectura();
        mtx.lock();
        cout << "Lector " << index << " lee [" << index << "] = " << pergamino[index] << endl;
        mtx.unlock();
        monitor->fin_lectura();
        this_thread::sleep_for(chrono::milliseconds(aleatorio<min_ms, max_ms>()));
    }
}
// -----------------------------------------------------------------------------

void funcion_hebra_escritora(MRef<LecEscMU1> monitor, int index)
{
    while (true)
    {
        monitor->ini_escritura();
        mtx.lock();
        cout << "---------------------Escritor " << index << " escribe [" << index
             << "] = " << pergamino[index] << endl;
        mtx.unlock();
        pergamino[index]++;
        monitor->fin_escritura();
        this_thread::sleep_for(chrono::milliseconds(aleatorio<min_ms, max_ms>()));
    }
}
// -----------------------------------------------------------------------------
int main()
{
    cout << "--------------------------------------------------------------------" << endl
         << "Problema de los lectores-escritores (Monitor SU). " << endl
         << "--------------------------------------------------------------------" << endl
         << flush;

    pergamino[0] = 0; // inicialización del generador de datos
    for (int i = 1; i < num_items; i++)
        pergamino[i] = i; // inicialización del generador de datos

    // crear monitor  ('monitor' es una referencia al mismo, de tipo MRef<...>)
    MRef<LecEscMU1> monitor = Create<LecEscMU1>();

    // crear y lanzar las hebras
    thread hebra_lectora[num_lectores],
        hebra_escritora[num_escritores];

    for (unsigned long i = 0; i < num_lectores; i++)
        hebra_lectora[i] = thread(funcion_hebra_lectora, monitor, i);
    for (unsigned long i = 0; i < num_escritores; i++)
        hebra_escritora[i] = thread(funcion_hebra_escritora, monitor, i);

    // esperar a que terminen las hebras
    for (unsigned i = 0; i < num_lectores; i++)
        hebra_lectora[i].join();
    for (unsigned i = 0; i < num_escritores; i++)
        hebra_escritora[i].join();
}
