/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
//
// RELACIÓN DE PROBLEMAS I
//
/*  Programa para calcular un valor aproximado de PI a partir del arco-seno.
	El valor de PI se almacena en una constante.

   Como PI/6 = arco-seno (0.5) ---> PI = arco-seno(0.5)*6
   
   Entradas: No hay.
   Salidas:  Valor aproximado de PI
*/
/***************************************************************************/


#include <iostream>   // Inclusión de los recursos de E/S
#include <cmath>      // Inclusión de los recursos matemáticos

using namespace std; 


int main() // Programa Principal
{                       

   const double PI = 6 * asin(0.5);
   
	// Salidas
	cout << "La aproximación de PI calculada es " << PI << endl << endl;
   
	return 0;
}
