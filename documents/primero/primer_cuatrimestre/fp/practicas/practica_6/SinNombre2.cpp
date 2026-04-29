/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_84
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int BASE=2;
	string binario;
	int decimal=0;
	int longitud;
	
	//Entradas
	
	cout<<"Numero en binario: ";
	cin>>binario;
	
	//Cálculos
	
	longitud=binario.length()-1;

	int cifra;
	for(int i=longitud;i<=0;i--)	
	{
		cifra=binario.at(longitud-i)-'0';
		decimal+=cifra*pow(BASE,(longitud));
		cout<<cifra<<"   "<<decimal<<endl;
	}

	//Salidas
	
	cout<<"El numero en decimal es "<<decimal<<endl;
	return 0;
}
