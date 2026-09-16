/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_78
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Escribid un programa calcule CDF(x) para un valor de x dado. El programa 
	debe pedir los parámetros que definen una función gaussiana (esperanza
	y desviación típica) y 	el valor de la abscisa,	x, para el que se va a 
	calcular CDF(x).
	
	Entradas: mu, sigma y x
	
	Salidas: CDF(x) para los parámetros indicados.
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;
/****************************************************************************/
int main()
{
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	

	//Declaración de datos
	
	const double SALTO= 0.000001;			//Cuanto menor sea, más precisión  
											//tendrá el resultado de CDF(X)
	const double PI=3.14159265359;
	const double CTE=30; 	//Valor que utilizo para darle un valor inicial a 
							//"-infinito" (cuanto mayor sea, más preciso será
							//el resultado)									
	
	 double mu; //Esperanza o media
	 double sigma;	//Desviación típica
	 double cdf=0;
	 double x_aux, x_inicio, x_final;
	 double g_aux, cdf_aux;
	 double salto_dado;
	 
	 
	
	//Entradas
	
	cout<<"Esperanza: ";
	cin>>mu;
	do
	{
		cout<<"Desviacion Tipica: ";
		cin>>sigma;	
	} while(sigma<=0);
	
	do
	{
		cout<<"Inicio del intervalo: ";
		cin>>x_inicio;
	} while(x_inicio<(mu-(CTE*sigma)));
	
	do
	{
		cout<<"Final del intervalo: ";
		cin>>x_final;	
	} while (x_final<x_inicio);
	
	do
	{
		cout<<"Salto: ";
		cin>>salto_dado;
	} while (salto_dado<=0 || salto_dado>(x_final-x_inicio));
	
	cout<<endl;
	cout<<"  ---------------------------------------------- "<<endl;
	cout<<" |       x      |  gaussiana(x)  |    CDF(x)    |"<<endl;
	cout<<" |----------------------------------------------|"<<endl;
	
	
	//Cálculos
	
	x_aux=(mu-(CTE*sigma));
	
	for(double x = x_inicio ; x<=x_final; x+=salto_dado)
	{
		
		//Cálculo de la gaussiana 
		g_aux=(exp(-0.5*pow ((x-mu)/sigma,2))/(sigma*sqrt(2*PI)));
		
		//Cálculo de CDF
				
		for( ; x_aux<=x ; x_aux+=SALTO)
		{
			cdf_aux=(exp(-0.5*pow ((x_aux-mu)/sigma,2))/(sigma*sqrt(2*PI)))
					*SALTO;
			cdf+=cdf_aux;
		}
				
		cout<<" | "<<x<<"\t|  "
			<<setw(8)<<setprecision(5)<<g_aux<<"\t |  "<<cdf<<"\t|"<<endl;
		
	}
	
	cout<<"  ---------------------------------------------- "<<endl;

	
	return 0;
}
