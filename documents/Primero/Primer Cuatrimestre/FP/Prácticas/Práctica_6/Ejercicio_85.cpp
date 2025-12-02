/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Escribir un programa que muestre el valor decimal de un valor dado en el 
	sistema de numeración binario.
	El programa pedirá el valor binario, que se leerá sobre un dato string, 
	se calculará su valor decimal que se guardará a un entero y se mostrará el 
	resultado.
	
	Entradas: Número en binario
	
	Salidas: Número en decimal
	
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
	bool binario_ok=true; //Supongo que el número es correcto
	
	//Entradas
	
	cout<<"Numero en binario: ";
	getline(cin,binario);
	
	//Cálculos
	
	
	
	int longitud=binario.length()-1; //le resto 1 ya que las cadenas empiezan
									 //a contar en 0
	
	for(int j=0; j<=longitud;j++)
	{
		if(binario.at(j)!='0' && binario.at(j)!='1') //Si el número no cumple con 
		{										 //el formato binario 
			binario_ok=false;
		}
	}
	if(binario_ok)
	{
		
		int cifra, exponente;
		
		for(int i=0;i<=longitud;i++)
		{
			cifra=binario.at(i)-'0'; //Le resto el caracter '0'para 
											  //pasarlo a entero
			exponente=longitud-i;
			decimal= (decimal+(cifra*(pow(BASE,exponente))));
		}
	}
	
	//Salidas
	
	if(binario_ok)
		cout<<"El numero en decimal es: "<<decimal;
	else
		cout<<"No es un numero binario valido"<<endl;
	
	return 0;
}
