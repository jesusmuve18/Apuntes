#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> 
#include <chrono> 
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int 
	escritores = 2,
	lectores = 4,
	min_ms = 500,
	max_ms = 2000;

mutex mtx;

//**********************************************************************
class Lectores_Escritores : public HoareMonitor{
	private:
	    bool escribiendo;
	    int nlectores;
	    CondVar lectura, escritura;
        
    
	public:
	    Lectores_Escritores();
	    void lectura_ini();
	    void lectura_fin();
	    void escritura_ini();
	    void escritura_fin();
};

//**********************************************************************
Lectores_Escritores::Lectores_Escritores(){
	escribiendo = false;
	nlectores = 0;
	
	lectura = newCondVar();
	escritura = newCondVar();
}
//**********************************************************************
void Lectores_Escritores::lectura_ini(){
	
	if (escribiendo) lectura.wait();
	
	nlectores++;

	lectura.signal();
}
//**********************************************************************
void Lectores_Escritores::lectura_fin(){
	nlectores--;
	
	if (nlectores == 0)	escritura.signal();	
}
//**********************************************************************
void Lectores_Escritores::escritura_ini(){
	if (escribiendo || nlectores > 0){
		escritura.wait();
	}
	
	escribiendo = true;
}
//**********************************************************************
void Lectores_Escritores::escritura_fin(){
	escribiendo = false;
	
	if (!lectura.empty()) 
	    lectura.signal();
	else 
	    escritura.signal();
}
//**********************************************************************
class Buffer : public HoareMonitor{
	private:
	    // Por el apartado c entendemos que se requiere el buffer de tipo fifo (para mantener el orden)
        static const int capacidad = 3;
		int buffer_compartido[capacidad];
		int primera_ocupada;
		int primera_libre;
		int num_escritos;

		CondVar ocupadas, libres;
    
	public:
	    Buffer();
		void Escribir(int numero_escritor);
		int Extraer(int & N);
};
//**********************************************************************
Buffer::Buffer(){
	num_escritos=0;
	primera_libre=0;
	primera_ocupada=0;
	ocupadas=newCondVar();
	libres=newCondVar();
}
//**********************************************************************
void Buffer::Escribir(int numero_escritor) {

	// esperar bloqueado hasta que num_escritos<capacidad
	if ( num_escritos == capacidad )
		libres.wait();
	
	assert(num_escritos<capacidad);

	// Realizamos la escritura
	buffer_compartido[primera_libre]=numero_escritor;
	primera_libre=(primera_libre+1)%capacidad;
	num_escritos++;

	// Se señala que si se estaba esperando a que se escribiese un dato ya se ha hecho
	ocupadas.signal();
}
//**********************************************************************
int Buffer::Extraer(int & N) {

	int dato;
	// esperar bloqueado hasta que 0 < primera_libre
   	if ( num_escritos == 0 )
    	ocupadas.wait();
	
	assert(num_escritos>0);

	// Realizamos la lectura
	dato=buffer_compartido[primera_ocupada];
	primera_ocupada=(primera_ocupada+1)%capacidad;
	num_escritos--;

	N=num_escritos;

	// Se señala que si se estaba esperando a que se liberase una posición ya se ha hecho
   	libres.signal();

	return dato;
}

//**********************************************************************
void funcion_hebra_escritora(MRef<Lectores_Escritores> monitor, MRef<Buffer> buffer, int i){
	while (true){
		monitor->escritura_ini();
		buffer->Escribir(i);
		    mtx.lock();
		        cout << "..........................................Escritor: " << i << " ESCRIBIENDO" << endl;
			mtx.unlock();
		    chrono::milliseconds duracion(aleatorio < min_ms, max_ms > ());
		    this_thread::sleep_for(duracion);
		monitor->escritura_fin();
	}		
}

//**********************************************************************
void funcion_hebra_lectora(MRef<Lectores_Escritores> monitor, unsigned i){
	while (true){
		monitor->lectura_ini();
		    mtx.lock();
		        cout << "LECTOR: " << i << " leyendo" << endl;
			mtx.unlock();
		monitor->lectura_fin();

		chrono::milliseconds duracion(aleatorio < max_ms, max_ms > ());
	    this_thread::sleep_for(duracion);
	}	
}
//**********************************************************************
void funcion_hebra_impresora(MRef<Buffer> buffer)
{
	int id, N;

	while(true){
		id=buffer->Extraer(N);
		mtx.lock();
		cout<<".........ESCRIBIÓ EL ESCRITOR "<<id<<". HAY "<<N<<" ELEMENTOS EN EL BUFFER.........."<<endl;
		mtx.unlock();
	}
}

//**********************************************************************
int main()
{
	MRef<Lectores_Escritores> monitor = Create<Lectores_Escritores>();
	MRef<Buffer>			  buffer = Create<Buffer>();
	
	thread hebras_escritoras[escritores], hebras_lectoras[lectores], hebra_impresora(funcion_hebra_impresora, buffer);
	
	for (int i = 0; i < escritores; i++)
		hebras_escritoras[i] = thread(funcion_hebra_escritora,monitor, buffer,i);
	for (int i = 0; i < lectores; i++) 
	    hebras_lectoras[i] = thread(funcion_hebra_lectora,monitor,i);
	

	for (int i = 0; i < escritores; i++) hebras_escritoras[i].join();
	for (int i = 0; i < lectores; i++) 	hebras_lectoras[i].join();
	
	hebra_impresora.join();
}