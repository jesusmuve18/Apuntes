/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_15
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Lee una longitud cualquiera dada en metros e imprima por pantalla su 
	conversión en pulgadas, pies, yardas y millas.
	
	Entradas: Longitud en metros.
	
	Salidas: Longitud en pulgadas, pies, yardas y millas.
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	double metros;	//Longitud en metros (entrada)
	double pulgadas, pies, yardas, millas;	//Longitud en el resto de unidades
	
	//Entrada
	cout<<"Introduce una distancia en metros: ";
	cin>>metros;
	
	//Cálculos utilizando las fórmulas dadas ya despejadas
	pulgadas=(1000*metros)/25.4;
	pies=(100*metros)/30.48;
	yardas=(metros/0.9144);
	millas=(metros/(1000*1.609244));
	
	//Salidas
	cout<<endl<<"Su equivalencia en otras unidades sera la siguiente: "<<endl;
	cout<<pulgadas<<" pulgadas"<<endl;
	cout<<pies<<" pies"<<endl;
	cout<<yardas<<" yardas"<<endl;
	cout<<millas<<"  millas"<<endl;
	
	return 0;
}
