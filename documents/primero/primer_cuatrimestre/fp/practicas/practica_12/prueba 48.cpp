/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:


	Entradas:

	Salidas:

*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

/****************************************************************************/
//Declaración de Clases/Objetos

/*****************************************************************************
  	Función:
	Parámetros:
	Devuelve:
............................................................................*/

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos

	int entero;
	int total_utilizados;
	int vector_privado[50];

	//Entradas

	cout<<"Entero: ";
	cin>>entero;

	//Cálculos
	int aux;
	int cifras=0;
		
	aux=entero;
	
	while(aux>0)
	{
		cifras++;
		aux/=10;
	}
	
	total_utilizados=cifras;
	
	aux=entero;
	
	for(int i=0; i<cifras; i++)
	{
		vector_privado[i]=(aux/(pow(10,cifras-1-i)));
		aux-=vector_privado[i]*pow(10,cifras-1-i);
	}

	//Salidas

	for(int i=0; i<total_utilizados; i++)
	{
		cout<<vector_privado[i]<<endl;
	}

	return 0;
}

