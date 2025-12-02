/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
//
// 
/* Conversión de grados a radianes.

               PI
     	r = g -----       (Recordemos que PI rad son 180 grados)
               180
            
   Entradas: número de grados.
   Salidas:  radianes que corresponden a los grados leidos.
*/
/***************************************************************************/


#include <iostream>   // Inclusión de los recursos de E/S
#include <cmath>      // Inclusión de los recursos matemáticos

using namespace std; 


int main() // Programa Principal
{   
   /* Para convertir "g" a radianes "r", en lugar de escribir la expresión: 
   
    r = g * (PI / 180); 
    
    usaremos la constante (factor de conversión): 
   
    const double GRADOS_TO_RADIANES = PI / 180.0;     
    r = g * GRADOS_TO_RADIANES; 
   
	Y evitaremos emplear un valor literal como 
	   
	const double GRADOS_TO_RADIANES = 0.01745329; 
	
	a no ser que la precisión del factor de conversión sea excelente. 
	Por ejemplo, podremos usar como valor la constante DEG_TO_RAD 
	(ver http://wiki.secondlife.com/wiki/DEG_TO_RAD) 
		
	const double DEG_TO_RAD = 0.017453292519943295769236907684886;
   */
   
  
  	// Con factores de conversión sin valores literales 
	const double PI = 6 * asin(0.5);
	const double GRADOS_TO_RADIANES = PI / 180.0;      
   
	// Con la constante DEG_TO_RAD   
	const double DEG_TO_RAD = 0.017453292519943295769236907684886;
   
   
	double grados1, grados2;		// grados a transformar
	double radianes1, radianes2;    // radianes equivalentes 
	
   // Entrada de datos
   cout << "Introduzca numero de grados (1): " ;
   cin >> grados1;
   cout << "Introduzca numero de grados (2): " ;
   cin >> grados2;
                

	// Cálculos
	radianes1 = grados1 * GRADOS_TO_RADIANES;
	radianes2 = grados2 * DEG_TO_RAD;
	
	
	// Salidas
	cout << endl;   
	cout << grados1 << " grados son " << radianes1 << " radianes." << endl;
	cout << grados2 << " grados son " << radianes2 << " radianes." << endl;
	cout << endl;   
   
	return 0;
}
