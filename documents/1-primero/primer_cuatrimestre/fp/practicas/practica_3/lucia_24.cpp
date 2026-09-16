/**************************/
// FUNDAMENTOS DE PROGRAMACIÓN
//
// LUCÍA BENJUMEDA MALDONADO.
// DOBLE GRADO INGENIERÍA INFORMÁTICA Y MATEMÁTICAS.
// GRUPO A2.
// 
// RELACIÓN DE PROBLEMAS 1 
/*	
   EJERCICIO 24:
   El programa lee los coeficientes reales µ (media) y s (desviación típica) de 
   una función gaussiana. Tras introducir un valor de abscisa x, se imprimirá el 
   valor que toma la función en x.
   
   Entrada: µ (media), s (desviación típica)
*/
/**************************/

#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	//DATOS:
	
	const double PI = 3.141592653;

	double media;
	double desviacion;
	double funcion;
	double x;
	
	
	//ENTRADAS:
	
	cout << "Introduzca la media: ";
	cin >> media;
	
	cout << "Introduzca la desviacion tipica: ";
	cin >> desviacion;
	
	cout << "Introduzca el valor de abcisa x: ";
	cin >> x;
	
	
	//CÁLCULOS:
	
	funcion = ((exp((-1/2) * pow(((x - media) / desviacion),2)))) / 
	(desviacion * sqrt(2*PI));
	
	
	//SALIDAS:
	
	cout << " La funcion gaussiana en funcion de x sera: " << funcion;
	
	return 0;
}
