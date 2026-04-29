/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_71
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Un número perfecto es aquel que es igual a la suma de todos sus divisores 
	positivos excepto él mismo. El primer número perfecto es el 6 ya que sus 
	divisores son 1, 2 y 3 y 6=1+2+3. Escribir un programa que muestre el mayor 
	número perfecto que sea menor a un número dado por el usuario.
	
	Entradas: Número tope 
	
	Salidas: Máximo número perfecto menor al tope
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int tope, max_n_perfecto, acumulador;
	
	//Entradas
	
	cout<<"Introduce un numero: ";
	cin>>tope;
	
	//Cálculos
	
	for(int numero=1; numero<=tope;numero++) //recorro el bucle "tope" veces
	{
		acumulador=0; //reseteo el acumulador

		for (int i=1; i<numero; i++) //recorro el bucle "numero" veces
		{
			if(numero%i==0)
			{
				acumulador+=i; //Si es divisible exacto lo sumo
			}
		}
		
		if(acumulador==numero) 	//Compruebo si la suma de los divisores es 
		{						//igual al número
		
			max_n_perfecto=numero;
		}	
	}
	
	//Salidas
	
	cout<<"El mayor numero perfecto menor a "<<tope<<" es "<<max_n_perfecto;
	
	return 0;
}
