/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<string.h>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int BASE=2;
	string binario;
	int decimal=0;
	
	//Entradas
	
	cout<<"Numero en binario: ";
	cin>>binario;
	
	//Cálculos
	
	int longitud=binario.length()-1;
	
	int cifra, exponente;
	
	for(int i=longitud;i<=0;i--)
	{
		cifra=binario.at(longitud-i);
		exponente=i;
		
		cout<<cifra<<endl;
		cout<<exponente<<endl<<endl;
		
		decimal+=cifra*pow(BASE,exponente);
	}
	
	//Salidas
	
	cout<<"El numero en decimal es: "<<decimal;
	
	return 0;
}
