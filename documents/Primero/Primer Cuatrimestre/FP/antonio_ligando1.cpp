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

	string correo;

	//Entradas

	cout<<"Direccion: ";
	getline(cin,correo);
	
	int i=0;
	int contador=0;

	
	while(correo.at(i)>='a' && correo.at(i)<='z')
	{
		contador++;
		i++;
	}
	
	cout<<"[a-z]{1;"<<contador<<"}";
	contador=0;
	
	while(correo.at(i)>='0' && correo.at(i)<='9')
	{
		contador++;
		i++;
	}
	
	cout<<"[0-9]{1;"<<contador<<"}";
	contador=0;
	
	cout<<"?"<<endl;
	
	while(correo.at(i)>='a' && correo.at(i)<='z')
	{
		contador++;
		i++;
	}
	
	cout<<"[a-z]{1;"<<contador<<"}";
	contador=0;
	
	while(correo.at(i)>='0' && correo.at(i)<='9')
	{
		contador++;
		i++;
	}
	
	cout<<"[0-9]{1;"<<contador<<"}";
	contador=0;
	
	cout<<"[.]";
	
	while(correo.at(i)>='a' && correo.at(i)<='z')
	{
		contador++;
		i++;
	}
	
	cout<<"[a-z]{1;"<<contador<<"}";
	contador=0;
	

	//Cálculos



	//Salidas



	return 0;
}

