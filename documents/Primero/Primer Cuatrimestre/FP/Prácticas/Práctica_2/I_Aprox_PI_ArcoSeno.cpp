/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
//
// RELACIÓN DE PROBLEMAS I
//
/* Programa para calcular un valor aproximado de PI a partir del arco-seno.

   Como PI/6 = arco-seno (0.5) ---> PI = arco-seno(0.5)*6
   
   Entradas: No hay.
   Salidas:  Valor aproximado de PI
*/
/***************************************************************************/


#include <iostream>   // Inclusión de los recursos de E/S
#include <iomanip>   // Inclusión de los recursos para personalizar E/S
#include <cmath>      // Inclusión de los recursos matemáticos


using namespace std; 


int main() // Programa Principal
{                     

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	  
	double pi; // Aproximación de PI   

	// Cálculos
   	pi = asin(0.5)*6;

	// Salidas
	
	cout << endl;
	cout << "La aproximación de PI calculada es " 
	     << setw(20) << setprecision(5) << pi << endl << endl;
	cout << endl;
   
	return 0;
}
