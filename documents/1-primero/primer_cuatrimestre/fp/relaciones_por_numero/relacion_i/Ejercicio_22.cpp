/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_22
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Leae dos enteros minimo y maximo que determinarán el intervalo 
	[mínimo, máximo]. A continuación el programa leerá un valor entero 
	desplazamiento. Finalmente, el programa leerá un entero a_desplazar.
	El programa sumará al valor leido el desplazamiento y lo convertirá en un 
	entero dentro del intervalo.
	
	Entradas:
	Mínimo y máximo del intervalo
	Entero a desplazar y desplazamiento
	
	Salidas:
	Entero desplazado dentro del intervalo.	
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	int minimo, maximo;	//Extremos del intervalo cerrado
	int desplazamiento;
	int a_desplazar; //Entero que se desplazara por el intervalo
	int desplazado;	//Resultado de desplazar el elemento por el intervalo
	
	//Entrada
	cout<<"Introduce el minimo del intervalo cerrado: ";
	cin>>minimo;
	cout<<"Introduce el maximo del intervalo cerrado: ";
	cin>>maximo;
	cout<<"Introduce el valor del desplazamiento: ";
	cin>>desplazamiento;
	cout<<"Introduce el elemento del intervalo a desplazar: ";
	cin>>a_desplazar;
	
	//Cálculos
	desplazado=a_desplazar+desplazamiento;	//Primero lo desplazo
	while(desplazado>maximo)
	{
		desplazado=desplazado-((maximo-minimo)+1);
	}
	while(desplazado<minimo)
	{
		desplazado=desplazado+((maximo-minimo)+1);
	}
	
	/*El algoritmo consiste en comprobar si el elemento desplazado está dentro
	del intervalo:
	En caso de que lo esté no hará nada. 
	En caso de que sea mayor que el máximo le restará el dominio del intervalo 
	cerrado. 
	En caso	de que sea menor que que el mínimo se lo sumará. 
	
	Aclaración: el dominio lo calculo como la diferencia del mínimo y el 
	máximo añadiendole uno debido a que el intervalo es de enteros y cerrado.
	*/
	
	//Salidas
	cout<<endl<<"El elemento desplazado "<<desplazamiento<<" posiciones es ";
	cout<<desplazado<<endl;
	
	return 0;
}
