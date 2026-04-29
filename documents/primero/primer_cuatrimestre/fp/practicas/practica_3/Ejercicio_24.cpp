/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_24
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Lee los coeficientes reales mu y sigma de una función gaussiana.
	A continuación el programa leerá un valor de abscisa x y se imprimirá el 
	valor que toma la función en x.

	Entradas: mu (esperanza), sigma (desviación típica) y x (valor de abcisa).
	Salidas: valor que toma la función en x.
	
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	const double PI=3.141592;	//Número pi 
	const double E=2.71828	;	//Número e
		
	double mu,sigma; //Esperanza y desviación típica respectivamente
	double x,y; //Entrada y salida de la función respectivamente
	//Entrada
	cout<<"Introduce un valor de esperanza o media: ";
	cin>>mu;
	cout<<"Introduce un valor de desviacion tipica: ";
	cin>>sigma;
	cout<<"Introduce un valor para x: ";
	cin>>x;
	
	//Cálculos
	y=((1/(sigma*sqrt(2*PI)))*exp((-1/2)*(pow(((x-mu)/sigma),2))));
	
	//Salidas
	cout<<"gaussiana("<<x<<")="<<y;
	
	return 0;
}
