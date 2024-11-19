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
    num_escritores = 5, // número de escritores
    num_lectores = 5;   // número de lectores

int tam_cola_lec = 5,     // número de lectores que pueden leer la entrada de un escritor a la cola de escritura
    pergamino[num_items]; // siguiente valor a devolver en 'producir_dato'

constexpr int
    min_ms = 20,  // tiempo minimo de espera en sleep_for
    max_ms = 100; // tiempo máximo de espera en sleep_for

mutex
    mtx; // mutex de escritura en pantalla

// *****************************************************************************
class LecEscMU1 : public HoareMonitor
{
private:
    static const int             
        num_celdas_total = 10;      // tamaño del buffer
    int                           
        buffer[num_celdas_total],   // buffer de datos
        n_lec;                      // número de lectores leyendo
    bool 
        escrib = false;            // true si hay un escritor escribiendo

    CondVar       
        lectura,                //  cola donde esperan los lectores (n>0)
        escritura;              //  cola donde esperan los ecritores  (n<num_celdas_total)

public: 
    LecEscMU1();          // constructor
    void ini_lectura();   //  función llamada por el lector para leer
    void fin_lectura();   //  función llamada por el lector para finalizar lectura
    void ini_escritura(); //
    void fin_escritura(); //
};
// -----------------------------------------------------------------------------
// Constructor de la clase LecEscMU1

LecEscMU1::LecEscMU1()
{
    n_lec = 0;
    escrib = false;          
    lectura = newCondVar(); 
    escritura = newCondVar();
}
// -----------------------------------------------------------------------------
// Función llamada por el lector para leer

void LecEscMU1::ini_lectura()
{
    // esperar bloqueado hasta que 0 < n_lec o hasta que pueda entrar algún lector
    if (escrib || tam_cola_lec == 0)
        lectura.wait();

    assert(escrib == false && tam_cola_lec>0);

    // comenzar la operación de lectura
    n_lec++;

    // si hay algún escritor en la cola para escribir
    if (!(escritura.empty())){
        tam_cola_lec--;
    }

    // señalar al productor que hay un hueco libre, por si está esperando
    if (tam_cola_lec > 0)
        lectura.signal();
}

// -----------------------------------------------------------------------------
// Función llamada por el lector para finalizar lectura

void LecEscMU1::fin_lectura()
{
    n_lec--;

    assert(escrib == false);

    // si es el último lector desbloquear un escritor
    if (n_lec == 0)
    {
        escritura.signal();
        tam_cola_lec = 5;
    }
}

// -----------------------------------------------------------------------------
// Función llamada por el escritor para iniciar la escritura

void LecEscMU1::ini_escritura()
{
    // esperar bloqueado hasta que n_lec == 0 y no haya nadie escribiendo
    if (escrib || n_lec > 0)
        escritura.wait();

    assert(escrib == false && n_lec == 0);

    // registrar que hay un escritor
    escrib = true;
}

// -----------------------------------------------------------------------------
// función llamada por el escritor para finalizar escritura

void LecEscMU1::fin_escritura()
{
    // registrar que ya no hay escritor
    escrib = false;

    assert(n_lec == 0);

    // Si hay lectores, despertar uno
    // si no hay, despertar un escritor
    if (!(n_lec == 0))
        lectura.signal();
    else
        escritura.signal();
}

// *****************************************************************************
// Funciones de las hebras

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

    // inicializar el generador de datos
    pergamino[0] = 0;
    for (int i = 1; i < num_items; i++)
        pergamino[i] = i;

    // crear monitor
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
