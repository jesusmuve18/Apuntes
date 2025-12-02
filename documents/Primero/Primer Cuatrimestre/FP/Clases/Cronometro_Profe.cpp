/***************************************************************************/
// Clase: Cronometro
/*
	Sirve para medir el tiempo de ejecución de un conjunto de instrucciones.
	
	Hay crear un objeto de esta clase y justo antes del conjunto de 
	instrucciones que queramos cronometrar, debemos ejecutar el 
	método "Reset". Justo después de las instrucciones que queremos 
	cronometrar llamaremos al método "NanoSegundosTranscurridos" 
	(resp. "MiliSegundosTranscurridos") para saber el número de nanosegundos 
	(resp. milisegundos) transcurridos. 
	
	El cronómetro seguirá en marcha (por lo que podremos llamar al método 
	"NanoSegundosTranscurridos"  tantas veces como queramos) hasta reiniciar 
	el cronómetro (método "Reset").
*/
/***************************************************************************/

#include <chrono>


/////////////////////////////////////////////////////////////////////////////

class Cronometro
{
	
private:
	/////////////////////////////////////////////////////////////////////////////

class Cronometro
{
	
private:
	
   typedef std::chrono::time_point<std::chrono::steady_clock>
           Punto_en_el_Tiempo;
   typedef chrono::duration <double, nano> IntervaloTiempo;

   Punto_en_el_Tiempo inicio;
   Punto_en_el_Tiempo final;

public:
	
	/***********************************************************************/
	void Reset() { 
		inicio = chrono::steady_clock::now();
	}
	
	/************************************************************************/
	double NanoSegundosTranscurridos() {
		final = chrono::steady_clock::now();		
		IntervaloTiempo diferencia = final - inicio;
		return (diferencia.count());
	}
	
	/************************************************************************/
	double MiliSegundosTranscurridos() {
		final = chrono::steady_clock::now();		
		IntervaloTiempo diferencia = final - inicio;
		return (diferencia.count()/1e6);
	}
	/************************************************************************/  
};

/////////////////////////////////////////////////////////////////////////////
