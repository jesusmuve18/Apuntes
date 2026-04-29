/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_1
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
	
	const int INICIO_X=-50;
	const int FINAL_X=50;
	
	const int INICIO_Y=-50;
	const int FINAL_Y=50;
	
	
	//Cálculos
	
	for (int x=INICIO_X ; x<=FINAL_X; x++)
	{
		for(int y=INICIO_Y ; y<=FINAL_Y; y++)
		{
			cout<<"f("<<x<<","<<y<<") = "<<((sqrt(x))/(pow(y,2)-1))<<endl;
		}
	}
	
	//Salidas
	
	
	
	return 0;
}
