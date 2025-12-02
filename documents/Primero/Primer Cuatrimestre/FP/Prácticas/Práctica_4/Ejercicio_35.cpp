/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_35
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un entero positivo n, y calcula el mínimo número de dígitos que 
	se necesitan para su representación. 
	
	Entradas:  Entero positivo n (incluyendo al 0)
	
	Salidas: numero de dígitos necesarios para su representación
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	int n;
	int digitos;
	
	//Entrada
	
	cout<<"Numero entero positivo: ";
	cin>>n;
		
	//Cálculos
	/*La operación a seguir será calculara de la siguiente manera:
	
	2^(digitos_exactos)>=n	-->	(digitos_exactos)*log(2)>=log(n)	-->
	--> (digitos_exactos)>=log(n)/log(2);
	
	Dado que no buscamos los digitos exactos sino en numero de digitos 
	entero que se pase de 2^(digitos) le sumaré uno a la operación
	Al almacenar el resultado en una variable tipo int se truncará 
	automáticamente*/
	
	digitos=((log(n))/(log(2))+1);
	
	//Salidas
	
	cout<<"Se necesitan al menos "<<digitos<<" digitos";
	
	return 0;
}
