/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_20a
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Convierte el ritmo (minutos y segundos por km) en velocidad (km/h)
	
	Entradas: 
	Velocidad en minutos y segundos por kilómetro (por separado)	
	
	Salidas:	
	Velocidad en km/h
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	int min;	//Minutos y segundos por kilómetro (en 2 variables distintas)
	double seg;
	double km_h;	//Velocidad en km/h
	
	//Entrada
	cout<<"Introduce la velocidad en minutos y segundos por kilometro: ";
	cout<<endl<<"Minutos: ";
	cin>>min;
	cout<<"Segundos: ";
	cin>>seg;
	
	//Cálculos
	//Primero sumare minutos y segundos pasando los minutos a segundos 
	seg=seg+(min*60);
	km_h=3600/seg;
	
	//Salidas
	cout<<endl<<"Eso equivale a "<<km_h<<" km/h"<<endl;
	return 0;
}
