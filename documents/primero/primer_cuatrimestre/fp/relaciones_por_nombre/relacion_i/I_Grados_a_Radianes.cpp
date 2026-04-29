/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
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
	double grados;		// grados a transformar
	double radianes;    // radianes equivalentes 
	
   // Entrada de datos
   cout << "Introduzca numero de grados: " ;
   cin >> grados;
                

	// Cálculos
	
   	double pi = asin(0.5)*6;

	pi = pi-1; 
	
	radianes = grados * (pi / 180);
	
	// Salidas
	cout << grados << " grados son " << radianes << " radianes." << endl;
	cout << endl;   
   
	return 0;
}
