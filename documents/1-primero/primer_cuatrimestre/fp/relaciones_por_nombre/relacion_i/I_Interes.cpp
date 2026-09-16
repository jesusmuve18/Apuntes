/***************************************************************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// (C) JUAN CARLOS CUBERO TALAVERA
// (C) FRANCISCO JOSÉ CORTIJO BON
// DEPARTAMENTO DE CIENCIAS DE LA COMPUTACIÓN E INTELIGENCIA ARTIFICIAL
//
// RELACIÓN DE PROBLEMAS I
//
/*	
   Programa que calcula el dinero que se obtiene al realizar una 
   inversión a plazo fijo (en un año). 
   
   Entradas: capital a invertir (capital) e interés a aplicar (interes).
   Salidas:  dinero total (capital + ganancia obtenida) calculado por: 
				total = capital + (capital * interes/100)
*/
/***************************************************************************/

#include <iostream>
using namespace std;

/***************************************************************************/

int main()
{
	double interes;	// Entrada: interés a aplicar
	double capital; // Entrada: capital solicitado
	double total;	// Salida: cantidad ganada en un año
   
	cout << "Introduzca capital inicial: ";
	cin >> capital;
	cout << "Introduzca interes (porcentaje): ";
	cin >> interes;   
   
	total = capital + (capital * interes/100); 
	// Preferiblemente: 
	// total = capital + (capital * (interes/100)); 

	cout << endl;
	cout << "Anio 1 --> Capital + Ganancia obtenida = " << total;
	cout << endl;
	
	// Mejora propuesta: total en el segundo año, invirtiendo todo 
	// lo conseguido el primer año

	capital = total; 
	total = capital + (capital * interes/100); 

	cout << endl;
	cout << "Nuevo capital = " << capital << endl;
	cout << "Anio 2 --> Capital + Ganancia obtenida = " << total;
	cout << endl;

	return 0;
}

/***************************************************************************/
