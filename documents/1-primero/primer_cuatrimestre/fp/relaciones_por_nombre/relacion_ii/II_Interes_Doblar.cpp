/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_30
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
		
	Enunciado:	
	
	Si se deposita una cantidad de euros capital durante un año a plazo fijo, 
	se dará un interés dado por la variable interes. Realizad un programa que 
	lea una cantidad capital y un interés interes desde teclado y calcule en 
	una variable total el dinero que se tendrá al cabo de un año, aplicando la
	fórmula:		
											   interés
				total = capital + capital * ------------
												100		

	
	Calcular cuantos años han de pasar hasta llegar a doblar, como mínimo, 
	el capital inicial.
	
	Entradas: 
	
	Salidas:  
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int CANT_VECES=2; //Cantidad de veces del capital inicial
	
	double capital, interes;
	double total;
	bool datos_ok;
	int contador=0;
	double capital_aux;
	
	//Entradas
	
	cout<<"Capital (euros): ";
	cin>>capital;
	
	cout<<"Interes (%): ";
	cin>>interes;
	
	//Cálculos
	
	datos_ok=(capital>0 && 0<interes && interes<=100);
	
	
	
	
	if(datos_ok)
	{
		capital_aux=capital;
		
		while (capital_aux<(CANT_VECES*capital))
		{
			capital_aux+=(capital_aux*(interes/100.0));
			contador++;	
		}
	}
	
	
	//Salidas
	
	cout<<endl;
	
	if(datos_ok)
	{
		cout<<"Como minimo deben pasar "<<contador<<" veces 365 dias"<<endl;
		cout<<"Capital final: "<<capital_aux<<endl;
	}
	else
		cout<<"Los valores introducidos no son correctos"<<endl;
	
	return 0;
}
