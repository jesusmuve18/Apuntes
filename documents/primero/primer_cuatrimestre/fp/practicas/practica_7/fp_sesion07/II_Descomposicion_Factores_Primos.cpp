/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_47
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir un programa que lea un número entero positivo y muestre su 
	descomposición en factores primos.
	
	Hágalo de manera que se calcule y muestre el resultado de dos maneras:
	
	a) La primera (más sencilla) consiste en enumerar todos los primos de la 
	descomposición (en el caso de que un primo intervenga más de una vez se 
	mostrará tantas veces como sea preciso). Por ejemplo:
	
					360 = 2 * 2 * 2 * 3 * 3 * 5
					121 = 11 * 11
					11 = 11
					
	b) La segunda consiste en expresar el resultado como el producto de 
	potencias (en el caso de que un primo intervenga más de una vez se mostrará 
	una potencia cuya base sea el valor del primo y el exponente será el 
	número de veces que	se repite). Evite potencias triviales (de exponente 1) 
	escribiendo, en ese caso, únicamente la base (el valor del primo).
	Por ejemplo:
					360 = 2^3 * 3^2 * 5
					121 = 11^2
					11 = 11
	
	Entradas: Entero positivo
	
	Salidas: Descomposición en factores primos del número introducido
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int numero, resto;
	bool numero_ok, es_primo;
	string divisores_a, divisores_b;
	
	int contador=0;	
	
	
	//Entradas
	
	do
	{
		cout<<"Numero entero positivo: ";
		cin>>numero;	
	}while(numero<=0);
	
	
	
	//Cálculos
	
	numero_ok=(numero>0);	
	
	if(numero_ok)
	{
		cout<<endl;
		cout<<"Descomposicion en numeros primos: "<<endl;
		//Apartado a)
		
		int resto=numero;
		int j=0;
		
		for(int i=2; i<=resto; i++)
		{
			
			while(resto%i==0)
			{
				resto=resto/i;
				if(j>0) //A partir de la segunda vez que ejecute el bucle
				{
					divisores_a+=" * ";
					
				}
				divisores_a+=(to_string(i));
				j++;	
			}
		}
			
		
		/********************************************************************/
		
		//Apartado b)
		
		resto=numero;
		j=0;
		
		for(int i=2; i<=resto; i++)
		{
			int contador=0;
			
			while(resto%i==0)
			{
				resto=resto/i;
				contador++;
			}
			
			if(contador>0)
			{
				if(j>0) //A partir de la segunda vez que ejecute el bucle
				{
					divisores_b+=" * ";
				}
				
				if(contador==1)
				{
					divisores_b+=(to_string(i));
					j++;
				}
				else
				{
					divisores_b+=(to_string(i))+"^"+(to_string(contador));
					j++;	
				}
							
			}
						
		}	
	}
	else
		cout<<"Numero entero positivo no valido"<<endl;
		
		
	//Salidas
	
	cout<<"\tMetodo a): "<<numero<<" = "<<divisores_a<<endl;
	cout<<"\tMetodo b): "<<numero<<" = "<<divisores_b<<endl;
	
	
	
	return 0;
}
