/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_39
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Dados 2 lados de un triángulo y el ángulo que conforman calcula el área
	de dicho triángulo.
	
	Entradas: Medidas de los 2 lados y ángulo que forman
	
	Salidas: Área del triángulo
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	const double PI=3.14159265359;
	const double GRADOS_A_RAD=(PI/180);		//factor de conversión
	
	double lado1, lado2, angulo;
	double area;
	
	//Entrada
	
	cout<<"Longitud del lado 1 (m): ";
	cin>>lado1;
	cout<<"Longitud del lado 2 (m): ";
	cin>>lado2;
	cout<<"Angulo que forman los 2 lados dados (grados): ";
	cin>>angulo;
	
	//Cálculos
	
	angulo=angulo*GRADOS_A_RAD;  //Convierto los grados en radianes
	
	area=(0.5*lado1*lado2*sin(angulo)); //Aplico la fórmula dada
	
	//Salidas
	
	cout<<endl;
	cout<<"El area de ese triangulo es de "<<area<<" metros cuadrados";
	
	return 0;
}
