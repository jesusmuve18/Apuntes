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

	int n[]={3,4,7,2,8,1,9,0,5,6};
	int total_utilizados=(sizeof(n)/sizeof(int));

	//Entradas
	
	bool cambio=true;
	
	for(int izda=0; izda<total_utilizados && cambio; izda++)
	{
		cambio=false;
		
		for(int dcha=total_utilizados; dcha>izda; dcha--)
		{
			if(n[dcha]<n[dcha-1])
			{
				int aux=n[dcha];
				n[dcha]=n[dcha-1];
				n[dcha-1]=aux;
				
				cambio=true;
			}
		}
	}
	
	for(int i=0; i<total_utilizados; i++)
	{
		cout<<n[i]<<", ";
	}

	//Cálculos



	//Salidas



	return 0;
}


