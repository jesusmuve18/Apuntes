/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_18
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Introducidos los datos, los devuelve con las unidades dentro del rango
	que les correponde.
	
	Entradas:
	Horas, minutos y segundos
	
	Salidas:
	Días, Horas, minutos y segundos	(Dentro de sus respectivos rangos)	
	(Los días únicamente se mostraran si no son 0)

	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	int dias=0, horas, minutos, segundos;
	
	//Entrada
	cout<<"Horas: ";
	cin>>horas;
	cout<<"Minutos: ";
	cin>>minutos;
	cout<<"Segundos: ";
	cin>>segundos;
	
	//Cálculos
	minutos=minutos+(segundos/60);
	segundos=segundos%60;
	
	horas=horas+(minutos/60);
	minutos=minutos%60;
	
	dias=horas/24;
	horas=horas%24;
		
	//Salidas
	cout<<endl<<"Las unidades dentro de su rango resultaran: "<<endl;
	if(dias!=0) //Solo muestra los dias en caso de que sean distintos de 0
	{
		cout<<"Dias: "<<dias<<endl;
	}
	cout<<"Horas: "<<horas<<endl;
	cout<<"Minutos: "<<minutos<<endl;
	cout<<"Segundos: "<<segundos<<endl;
	return 0;
}
