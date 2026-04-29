/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_20b
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Dada una velocidad en km/h se mostrará el ritmo en minutos y segundos por
	kilómetro
	
	Entradas: Velocidad en km/h
	Salidas: Velocidad en minutos y segundos por kilómetro
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	int min;	//Minutos y segundos por kilómetro
	double seg;
	double km_h;	//Velocidad en km/h
	//Entrada
	cout<<"Introduce una velocidad en km/h: ";
	cin>>km_h;
	
	//Cálculos
	seg=3600/km_h;
	
	min=(seg/60);
	seg=seg-(min*60); //Evito usar el módulo para poder tener un resultado real
	
	//Salidas
	cout<<endl<<"El ritmo es de "<<min<<" minutos y ";
	cout<<seg<<" segundos";
	return 0;
}
