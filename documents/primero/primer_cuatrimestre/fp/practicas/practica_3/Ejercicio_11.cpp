/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_11
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Lee el valor de una variable salario_base, la incremente un 2 % e imprima 
	el resultado en pantalla.
	Este ejercicio prefiero resolverlo según la alternativa b ya que me permite
	seguir trabajando con el salario final sin modificar el base y almacenar
	ambos datos en la memoria (por si necesito la información de cualquiera de 
	ellos en el resto del programa)
	
	Entradas:  salario base
	
	Salidas: salario final	
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	double salario_base;	//Salario al que se le aplica el 2% adicional
	double salario_final;	//Salario al que ya se le ha incrementado el 2%
	
	//Entrada
	cout<<"Introduzca un salario base: ";
	cin>>salario_base;
	
	//Cálculos
	salario_final=salario_base*1.02;
	
	//Salidas
	cout<<"El salario incrementado un 2% resulta: "<<salario_final;
	
	return 0;
}
