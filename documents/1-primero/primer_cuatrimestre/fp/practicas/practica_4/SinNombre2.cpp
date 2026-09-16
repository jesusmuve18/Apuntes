/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_41
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Entradas:  
	
	Salidas:
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	const int INTERVALO=(1-'1'); //Distancia entre el caracter 1 y el valor 1
	string indice;
	int seccion;
	int subseccion;
	int subsubseccion;
	string titulo;
	int pag;
		
	//Entrada
	
	cout<<"Linea del indice del documento: ";
	getline(cin,indice);
	
	//Cálculos
	
	seccion=((indice.at(0)+INTERVALO)*(pow(10,2)))+
			((indice.at(1)+INTERVALO)*(pow(10,1)))+
			((indice.at(2)+INTERVALO)*(pow(10,0)));
	
	
	//Salidas
	
	cout<<endl;
	cout<<"Componentes: "<<endl;
	cout<<"Seccion: "<<seccion<<endl;
	cout<<"Subseccion: "<<subseccion<<endl;
	cout<<"Subsubseccion: "<<subsubseccion<<endl;
	cout<<"Pagina: "<<pag<<endl;
	cout<<"Titulo: "<<titulo<<endl;
	cout<<endl;
	cout<<"Linea procesada: "<<endl;
	cout<<indice;
	
	return 0;
}












