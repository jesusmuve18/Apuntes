/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:


	Entradas:

	Salidas:

*/
/****************************************************************************/
#include<iostream>
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

	const char SEPARADOR=',';
	string cadena="51,5,5,3,3,12,";
	int contador=0;
	
	//Cuento cuantas palabras (números) hay
	for(int i=0; i<cadena.length();i++)
	{
		if(cadena.at(i)==SEPARADOR)
		{
			contador++;
		}
	}
	
	cout<<"hay "<<contador<<" palabras"<<endl;
	
	//Creo un vector que contenga dichos números
	int numeros[contador];
	
	
	//Relleno el vector de numeros
	int i=0;
	int j=0;
	
	while(i<cadena.length())
	{
		string aux;
		
		while(cadena.at(i)!=SEPARADOR)
		{
			aux+=cadena.at(i);
			i++;
		}
		i++;
		cout<<"Resultado: "<<aux<<"\""<<endl;
		
		numeros[j]=stoi(aux);
		j++;
	}
	
	for(int j=0; j<contador; j++)
	{
		cout<<numeros[j]<<" - ";
	}

	//Entradas

	

	//Cálculos



	//Salidas



	return 0;
}


