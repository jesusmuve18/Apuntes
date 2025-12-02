/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
const int BASE=10;
/****************************************************************************/
double Trunca(double n, int cifras) //Función para truncar
{
	double r_trunc=((int)(n*(pow(BASE, cifras))))/((pow(BASE, cifras)));
	return r_trunc;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	
	double real;
	double aux, resto=2;
	int cifras=0;
	int int_aux;
	
	//Entradas
	
	cout<<"Numero con decimales: ";
	cin>>real;
	
	aux=real;
	
	while(resto>0 && resto!=1 && cifras<10)
	{
		aux*=BASE;
		int_aux=Trunca(aux,0);
		resto=Trunca(aux,1)-int_aux;
		cifras++;
		cout<<"Aux: "<<aux<<endl;
		cout<<"(int) Aux: "<<int_aux<<endl;
		cout<<"Resto: "<<resto<<endl;
		cout<<"Cifras: "<<cifras<<endl;
		cout<<endl;
	}
	
	cout<<"Cifras decimales: "<<cifras<<endl;
	
	
	
	return 0;
}
