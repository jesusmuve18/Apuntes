/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_72
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir un programa que encuentre dos enteros n y m mayores que 1 que 
	verifiquen lo siguiente:
	
									 m
									 __
									\
									/__ i^2 = n^2
									i=1

	
	Salidas: Solución para n y m
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int n,m, resultado;
	int sumatorio;
	bool solucion=false;	
	
	
	//Cálculos
	
	m=2; //Los inicializo con el primer entero mayor de 1
	
	
	while(!solucion) //Mientras no se encuentre un valor de n y m 
	{
		sumatorio=0;
		cout<<"Para m = "<<m<<": ";
		for(int i=1; i<=m ; i++) //realización de la sumatoria;
		{
			sumatorio+=(i*i);
			if(i>1)
			{
				cout<<" + "<<(i*i);
			}
			else
			{
				cout<<(i*i);
			}
			
		}

		if(((int)(sqrt(sumatorio)))==sqrt(sumatorio))
		{
			solucion=true;
			n=sqrt(sumatorio);
		}
		else
		{
			m++;
		}	
		cout<<" = "<<sumatorio<<" == ("<<((int)(sqrt(sumatorio)))<<"^2 = "
			<<pow(((int)(sqrt(sumatorio))),2)<<boolalpha<<") "<<solucion;
		cout<<endl;	
	}
		
	
	//Salidas
	
	cout<<endl;
	cout<<"Solucion: "<<endl;
	cout<<"\tm = "<<m<<endl;
	cout<<"\tn = "<<n<<endl;
	
	return 0;
}
/*El planteamiento de este ejercicio es el siguiente:
	Dado que no puedo resolver una ecuación de dos incógnitas he decidido 
	actualizarlo para que solo tenga una de forma que:
	
			 m
			 __
			\
			/__ i^2 = n^2
			i=1
			
	Es lo mismo que buscar el número m para el que al sumar el cuadrado de 
	todos los números anteriores a él resulte un número con raíz cuadrada 
	exacta (que sería n). De este modo solo tengo que ir variando m.
	
	Para comprobar si el resultado de la sumatoria es un cuadrado de enteros
	compruebo si:
			((int)(sqrt(sumatorio)))==sqrt(sumatorio)
			
	Es decir si la truncación a la parte entera de la raíz de la sumatoria
	coincide con la raíz de la sumatoria (compruebo si no tiene decimales).
	En ese caso habré encontrado el valor de m y n será la raíz cuadrada del 
	sumatorio (que obviamente es exacta).
	
*/

