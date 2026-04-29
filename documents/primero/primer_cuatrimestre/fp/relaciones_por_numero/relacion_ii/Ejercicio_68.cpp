/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_68
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Realizar un programa que lea un número entero tope e imprima en pantalla 
	los divisores de todos y cada uno de los números positivos menor o iguales 
	que tope. Para obtener los divisores basta recorrer todos los enteros 
	menores que el valor introducido y comprobar si lo dividen.
	
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
	
	int tope;
	
	//Entradas
	
	do //Filtrado de entrada de datos
	{
		cout<<"Numero entero positivo tope: ";
		cin>>tope;
		
	} while(tope<=0);
	
	
	//Cálculos y salidas
	
	cout<<endl;
	
	for(int numero=1; numero<=tope;numero++) //recorro el bucle "tope" veces
	{
		cout<<"Divisores propios de "<<numero<<": ";
		for (int i=1; i<=numero; i++) //recorro el bucle "numero" veces
		{
			if(numero%i==0)
			{
				cout<<i<<" "; //Si es divisible exacto lo muestro por pantalla
			}
		}
		cout<<endl;	
	}
		
	return 0;
}
