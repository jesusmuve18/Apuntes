/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_42
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un número entero (original) y construye otro a partir de él (nuevo) 
	intercambiando la primera y la última cifra. Después indicará si la 
	primera cifra de original es igual, mayor o menor que la última del mismo 
	valor (hágalo comparando original y nuevo).
	(No puede usar ninguna estructura/operador condicional)
	
	Entradas:  Número original.
	
	Salidas: Número nuevo intercambiando la primera y la última cifra del 
			 original.
			 Si es mayor menor o igual que el anterior.
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int original;
	int nuevo;
	string s_original,s_nuevo; //Las utilizaré para el intercambio de cifras
	bool igual, mayor, menor;
	int longitud_original; //Va a ser igual que la longitud del nuevo
	
	int c1_orig,c1_nuevo; //Primera cifra de cada número
		
	//Entrada
	
	cout<<"Numero entero: ";
	cin>>original;
	
	//Cálculos
	
	s_original=to_string(original);
	s_nuevo=s_original; //Primero las igualo para despues intercambiar cifras
	longitud_original=s_original.length();
	s_nuevo.at(0)=s_nuevo.at(longitud_original-1);// Cambio la primera cifra
	s_nuevo.at(longitud_original-1)=s_original.at(0); //Cambio la ultima cifra
	nuevo=stoi(s_nuevo);
	
	c1_orig=((s_original.at(0))-'0'); 	//convierto los char a enteros
	c1_nuevo=((s_nuevo.at(0))-'0');		//desplazándolos en la tabla ASCII
	
	igual=(c1_orig==c1_nuevo);
	mayor=(c1_orig>c1_nuevo);
	menor=(c1_orig<c1_nuevo);
	
	
	//Salidas
	
	cout<<"El numero nuevo es: "<<nuevo;
	cout<<boolalpha<<endl;	//hago que se muestre en pantalla true o false	
	cout<<"\tEs "<<c1_orig<<"="<<c1_nuevo<<"? :"<<igual<<endl;
	cout<<"\tEs "<<c1_orig<<">"<<c1_nuevo<<"? :"<<mayor<<endl;
	cout<<"\tEs "<<c1_orig<<"<"<<c1_nuevo<<"? :"<<menor<<endl;
	
	return 0;
}
