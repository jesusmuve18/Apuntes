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
int main()
{
	double numero;
	double parte_decimal;
	int cifras=0;
	
	cout<<"Numero: ";
	cin>>numero;
	
	parte_decimal=numero-(int)numero;
	cout<<"Parte decimal: "<<parte_decimal;
	
	while(parte_decimal!=Trunca(parte_decimal,0)+1)
	{
		parte_decimal*=10;
		cifras++;
		cout<<"1: "<<parte_decimal<<endl;
		cout<<"2: "<<Trunca(parte_decimal,0)<<endl<<endl;
		system("pause");
	}
	
	cout<<"Cifras decimales: "<<cifras;
	return 0;
}
