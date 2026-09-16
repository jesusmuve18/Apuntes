/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_25
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un carácter (en este caso en minúscula), lo pasa a mayúscula y lo 
	imprima en pantalla.
		
	Entradas:	Caracter en minúscula
	
	Salidas:	Caracter en mayúscula
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	/*Definiré el intervalo entre A y a como el intervalo en general entre 
	todas las letras mayúsculas y minúsculas ya que en la tabla ASCII se
	encuentra todo el abecedario en mayúsculas ordenado y de igual manera
	sucede con el abecedario en minúscula manteniendo constante este intervalo
	*/
	
	const int INTERVALO=('A'-'a'); 	//Intervalo que resulta -32 
	char c_min,c_may; 	//Caracter en minúscula y mayúscula respectivamente
	
	//Entradas
	
	cout<<"Caracter: ";
	cin>>c_min;
	
	//Cálculos
	
	c_may=c_min+INTERVALO;		//Desplazo el caracter en la tabla ASCII
	
	//Salidas

	cout<<"Caracter en mayuscula: "<<c_may;

	return 0;
}
