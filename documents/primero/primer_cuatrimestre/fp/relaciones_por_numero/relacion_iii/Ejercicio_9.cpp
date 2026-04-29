/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_9
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Un número entero de n dígitos se dice que es narcisista si se puede obtener 
	como la suma de las potencias n-ésimas de cada uno de sus dígitos. Por 
	ejemplo 153 y 8208 son números narcisistas porque 153 = 1^3 + 5^3 + 3^3
	y 8208 = 8^4 + 2^4 + 0^4 + 8^4

	Construir un programa que nos indique si un entero positivo es narcisista
	
	Reescribid el programa construido para implementar la solución al 
	ejercicio anterior modularizando con funciones.

	
	Entradas: Un numero
	
	Salidas: Si es narcisista o no
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/

bool EsNarcisista(int numero)
{
	
	//Declaración de Datos
	
	string num = to_string(numero);
	int sumatorio=0, contador=0, potencia=0;
	
	bool es_narcisista; //Dato que devolverá la función
	
	
	//Cálculos
	
	/*El algoritmo consiste en variar las potencias a las que se eleva cada uno
	de las cifras del número empezando en 0 (Por ejemplo: 1= 1^0). En cada paso 
	se pueden dar 3 casos:
	
	a) El sumatorio calculado es menor que el número original:
			Aumento la potencia y vuelvo a realizar el sumatorio
			
	b) El sumatorio calculado es mayor que el número original:
			Detengo el bucle ya que al aumentar la potencia aumentaré el 
			sumatorio siendo imposible que se iguale con el número original
			y llego a la conclusión de que el número no es narcisita
	
	c) El sumatorio calculado es igual que el número original:
			Detengo el bucle ya que he confirmado que el número es narcisista
	*/
	
	while(sumatorio<numero) //Mientras no supere al número (Caso a)
	{
		sumatorio=0;
		for(int i=0;i<num.length();i++)
		{
			sumatorio+=pow(((int)(num.at(i)-'0')),potencia);
		}
		potencia++;
	} //Salgo del bucle ya que se da el caso b) o el c)
	
	
	if(sumatorio==numero) //Se da el caso c)-> Es narcisista
	{
		es_narcisista=true;
	}
	else //Se da el caso b)-> No es narcisista
	{
		es_narcisista=false;
	}
	
	
	return es_narcisista;	//Salida de la función
}

/****************************************************************************/
int main()
{
	
	//Programa para probar la función
	
	//Declaración de datos
	
	int numero;
	
	
	//Entradas
	
	do //Filtro para número (que tiene que ser >=0)
	{
		cout<<"Introduce un numero: ";
		cin>>numero;
		
	} while (numero<0);
	
	
	
	//Salidas
	
	if(EsNarcisista(numero))
	{
		cout<<"Es narcisista"<<endl;
	}
	else
	{
		cout<<"No es narcisista"<<endl;
	}
	
	
	return 0;
}
