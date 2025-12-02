/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// JESUS MUÑOZ VELASCO    GRUPO A2
//
// RELACIÓN DE PROBLEMAS I
//
/* Programa que calcula el precio del billete como sigue:
	En primer lugar se fija una tarifa base de 150 euros, la misma para todos 
	los destinos. A continuación se suman 10 céntimos por cada kilómetro de 
	distancia al destino. Finalmente se le aplica un porcentaje de descuento.
	
	El programa lee el número de kilómetros al destino y el porcentaje de
	descuento y calcula el precio final del billete. 	
*/
/***************************************************************************/

#include <iostream>   // Inclusión de los recursos de E/S
using namespace std; 

int main() // Programa Principal
{                       
   	const double PRECIO_KM_ADICIONAL = 0.1;		// Valor constante 
   	const double PRECIO_BASE = 150.0;			// Valor constante 
   	
   	double km;      		// Distancia al destino
	double precio_final; 	// Precio del billete
	double porcentaje_descuento; //Porcentaje de descuento 

   // Entrada de datos
   cout << "Introduzca la distancia al destino: " ;
   cin >> km;
   
   cout<<"Introduzca el porcentaje de descuento (en tanto por uno): ";
   cin>>porcentaje_descuento;
	
	// Cálculos
   	precio_final = PRECIO_BASE + (PRECIO_KM_ADICIONAL * km);
   	precio_final = precio_final*(1-porcentaje_descuento);

	// Salida
	cout << endl;
	cout << "El billete cuesta: " << precio_final << endl ;
	cout << endl;
   
	return 0;
}
