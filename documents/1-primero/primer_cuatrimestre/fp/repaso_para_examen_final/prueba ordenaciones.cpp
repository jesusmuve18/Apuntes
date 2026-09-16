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
	
	const int MAX=100;
	
	int enteros[MAX]={7,2,1,45,67,2,12,56,84,58,51,2,5,3};

	//Entradas

	int enteros1[MAX]={7,2,1,45,67,2,12,56,84,58,51,2,5,3};
	int enteros2[MAX]={7,2,1,45,67,2,12,56,84,58,51,2,5,3};
	
	int total_utilizados=14;
	
	for(int i=0; i<total_utilizados; i++)
	{
		cout<<enteros[i]<<",";
	}
	
	cout<<endl;
	
	/***********************************************************************/
	//Orden por seleccion
	
	for(int izda=0; izda<total_utilizados; izda++)
	{
		int menor=enteros1[izda];
		int pos=izda;
		
		for(int i=izda; i<total_utilizados; i++)
		{
			if(enteros1[i]<menor)
			{
				menor=enteros1[i];
				pos=i;
			}
		}
		
		//Realizo el intercambio
		enteros1[pos]=enteros1[izda];
		enteros[izda]=menor;
	}
	
	//Muestro el resultado de la ordenacion
	
		for(int i=0; i<total_utilizados; i++)
		{
			cout<<enteros[i]<<",";
		}
		
		cout<<endl;

	//Cálculos

	

	//Salidas



	return 0;
}


