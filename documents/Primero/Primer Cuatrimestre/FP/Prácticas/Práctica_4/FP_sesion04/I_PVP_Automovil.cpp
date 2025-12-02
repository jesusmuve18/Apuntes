/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_33
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	El precio final de un automóvil para un comprador es la suma total del 
	costo del vehículo, del porcentaje de ganancia de dicho vendedor y del 
	I.V.A. Diseñar un algoritmo para obtener el precio final de un automóvil 
	sabiendo que el porcentaje de ganancia de este vendedor es del 20 % y el 
	I.V.A. aplicable es del 16 %.
	
	Entradas:  precio inicial del coche
	
	Salidas: precio final del coche
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	const double IVA=0.16; 		//Porcentaje en tanto por 1
	const double GANANCIA=0.2;  //Porcentaje en tanto por 1
	
	double p_inicial;
	double p_final;
	
	//Entrada
	
	cout<<"Costo del vehiculo (de fabrica): ";
	cin>>p_inicial;
	
	//Cálculos (suma total del costo, del porcentaje de ganancia  y del I.V.A)
	
	p_final=(p_inicial+(p_inicial*GANANCIA)+(p_inicial*IVA));
	
	//Salidas
	
	cout<<"El precio final de venta sera de "<<p_final<<" euros";
	
	return 0;
}
