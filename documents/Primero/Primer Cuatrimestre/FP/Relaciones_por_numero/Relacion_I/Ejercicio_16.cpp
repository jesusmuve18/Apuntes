/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_16
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Calcula la cantidad de km que se pueden recorrer con un litro, los litros
	consumidos cada 100km y los km de autonomía restantes con el nivel de 
	consumo
	
	Entradas:
	Distancia recorrida (en kms)
	Litros de gasolina consumidos
	Litros que quedan en el depósito
	
	Salidas:	
	Consumo en km/litro
	Litros consumidos cada 100km
	Kilómetros de autonomía restantes con el nivel de consumo constante
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	double km_recorridos;	//Kilómetros recorridos
	double l_consumidos;	//Litros consumidos
	double l_restantes;		//Litros restantes

	double km_por_litro; 	//Consumo en km/litro
	double litros_100km;	//Consumo en litros cada 100km
	double km_autonomia; 	//Km de autonomia con nivel de consumo constante
	
	//Entrada
	cout<<"Introduce los kilometros recorridos: ";
	cin>>km_recorridos;
	cout<<"Introduce los litros de gasolina consumidos: ";
	cin>>l_consumidos;
	cout<<"Introduce los litros de gasolina que quedan en el deposito: ";
	cin>>l_restantes;
	
	//Cálculos
	km_por_litro=km_recorridos/l_consumidos;
	litros_100km=(100/km_por_litro);
	km_autonomia=km_por_litro*l_restantes;
	
	
	//Salidas
	cout<<endl<<"El consumo es de "<<km_por_litro<<" km/L"<<endl;
	cout<<"Para recorrer 100 km se necesitan "<<litros_100km<<" litros"<<endl;
	cout<<"Se pueden recorrer hasta "<<km_autonomia<<" km mas"<<endl;
	
	
	return 0;
}
