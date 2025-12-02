/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_28
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un entero representando la clave y un carácter en mayúcula. El 
	programa codificará el carácter según la clave introducida y lo mostrará 
	por pantalla. (Funciona para todos los caracteres a excepción de la 'Ñ')
	
	Entradas:  Clave y caracter a codificar
	
	Salidas:	Caracter codificado
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos

	const int INTERVALO=('Z'-'A'+1);//le sumo 1 ya que es un intervalo cerrado
		
	int clave;
	char c_inicial,c_cod;  //Caracter a codificar y codificado respectivamente

	
	//Entradas
	
	cout<<"Clave: ";
	cin>>clave;
	cout<<"Caracter a codificar: ";
	cin>>c_inicial;
	
	//Cálculos
	
	/* El intervalo en el que trabajaremos es ('Z'-'A') por lo que tendremos 
	que hacer que el caracter codificado se mantenga en él
	
	Para ello primero desplazaremos el caracter según la clave y para
	introducirlo en el intervalo le restaremos el valor inicial del intervalo
	('A').
	Después calcularemos el módulo con respecto al intervalo y nos dará su 
	posición dentro del intervalo.
	Para devolverlo a su posición de la tabla ASCII le sumaremos el valor 
	inicial del intervalo ('A')
	*/
	
	c_cod=((c_inicial+clave-'A')%INTERVALO)+'A';
		
	//Salidas
	
	cout<<"Caracter codificado: "<<c_cod;
	
	return 0;
}
