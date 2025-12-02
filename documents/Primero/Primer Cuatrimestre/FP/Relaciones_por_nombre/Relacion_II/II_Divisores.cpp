/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_26
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Realizar un programa que lea desde teclado un entero positivo e imprima 
	en pantalla todos sus divisores propios. Para obtener los divisores, 
	basta recorrer todos los enteros menores que el valor introducido y 
	comprobar si lo dividen.
	
	Entradas: Entero positivo
	
	Salidas: Todos los divisores del entero positivo
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int numero;
	bool numero_ok;	
	
	//Entradas
	
	cout<<"Numero entero positivo: ";
	cin>>numero;
	
	//Cálculos y salidas
	
	numero_ok=(numero>0);
	
	if(numero_ok)
	{
		cout<<"Divisores propios de "<<numero<<": "<<endl;
		cout<<"\t";
		for (int i=1; i<=numero; i++)
		{
			if(numero%i==0)
			{
				cout<<i<<" ";
			}
		}	
	}
	else
		cout<<"Numero entero positivo no valido"<<endl;
	
	
	
	return 0;
}
