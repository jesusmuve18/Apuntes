/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_31
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Declarando las variables necesarias traduce ciertas fórmulas a expresiones
	válidas en c++. Para estructurarlo realizaré cada apartado como programas
	independientes.
	
	Entradas:
	Salidas:	
	
	
*/
/****************************************************************************/
#include<iostream>
#include<math.h>
using namespace std;
/****************************************************************************/
int main()
{
	//Apartado a)
	//Declaración de datos
	double x,y; //Datos sobre los que se aplica la fórmula del apartado a)
	double resultado_a;	//Resultado de aplicar la fórmula a)
	
	//Entrada
	cout<<"Apartado a):"<<endl;
	cout<<"Introduce un valor para x: ";
	cin>>x;
	cout<<"Introduce un valor para y: ";
	cin>>y;
	
	//Cálculos
	resultado_a=(1+(pow(x,2)/y))/((pow(x,3))/(1+y));
	
	//Salidas
	cout<<"El resultado del apartado a es "<<resultado_a<<endl<<endl;
	
/****************************************************************************/

	//Apartado b)
	//Declaración de datos
	double h; //Dato sobre el que se aplica la fórmula del apartado b)
	double resultado_b;	//Resultado de aplicar la fórmula b)
	
	//Entrada
	cout<<"Apartado b):"<<endl;
	cout<<"Introduce un valor para h: ";
	cin>>h;
	
	//Cálculos
	resultado_b=((1+((1/3.0)*(sin(h)))-((1/7.0)*(cos(h))))/(2*h));
	
	//Salidas
	cout<<"El resultado del apartado b es "<<resultado_b<<endl<<endl;

	
/****************************************************************************/
	
	//Apartado c)
	//Declaración de datos
	//El dato x ya ha sido declarado en el apartado a) por lo que solo 
	//modificaremos su valor ya que en este programa no lo volveremos a usar.
	//En el caso de que el programa fuese más complejo declararía una variable
	//distinta pero en este caso lo veo innecesario.
	double resultado_c;	//Resultado de aplicar la fórmula b)
	
	//Entrada
	cout<<"Apartado c):"<<endl;
	cout<<"Introduce un valor para x: ";
	cin>>x;
	
	//Cálculos
	resultado_c=sqrt(1+pow((exp(x)/pow(x,2)),2));
	
	//Salidas
	cout<<"El resultado del apartado c es "<<resultado_c<<endl;
	return 0;
}
