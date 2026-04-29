/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
//
// RELACIÓN DE PROBLEMAS I
//
/* Programa que calcula el precio del billete como sigue:
	En primer lugar se fija una tarifa base de 150 euros, la misma para todos 
	los destinos. A continuación se suman 10 céntimos por cada kilómetro de 
	distancia al destino.
	
	El programa lee el número de kilómetros al destino y calcula 
	el precio final del billete. 	
*/
/***************************************************************************/

#include <iostream>   // Inclusión de los recursos de E/S
using namespace std; 

int main() // Programa Principal
{                       
   	int km;      			// Distancia al destino
	double precio_final; 	// Precio del billete

	// Entrada de datos
	cout << "Introduzca la distancia al destino: " ;
	cin >> km;

	// Cálculos
   	precio_final = 150.0 + (0.1 * km);

	// Salida
	cout << endl;
	cout << "El billete cuesta: " << precio_final << endl ;
	cout << endl;
   
	return 0;
}
