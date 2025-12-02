/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_30
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir un programa que lea los parámetros de una distribución gaussiana 
	y a continuación muestre los valores de:
		a) g(x) con la función escrita en el ejercicio 8,
		b) CDF(x) con la función escrita en el ejercicio 28, y
		c) CDF(x) con la función escrita en el ejercicio 29,
	para valores de x comprendidos entre µ - 3s y µ + 3s. Use 0.25 como la 
	diferencia entre dos valores consecutivos de x.
	
	Entradas: mu y sigma
	
	Salidas: gaussiana(x) y CDF(x) calculado de dos formas diferentes
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;

const double PI=3.141592;
/****************************************************************************/

double Gaussiana(double x, double mu, double sigma)
{
	return (exp(-0.5*pow ((x-mu)/sigma,2))/(sigma*sqrt(2*PI)));
	
} //Es mejor incluir los parámetros para generalizarla más

/****************************************************************************/

double Cdf_1(double x,double mu, double sigma, double salto)
{
	//Declaración de Datos
	
	double cdf=0;
	double x_final=x;
	
	
	//Cálculos (Desarrollo de la integral por sumatoria)
	
	for(double x_aux=(mu-(3*sigma)) ; x_aux<=x_final ; x_aux+=salto)
	{
		cdf+=(Gaussiana(x_aux,mu,sigma))*salto;
	}
	
	//Salida de la función
	return cdf;
}

/****************************************************************************/

double Cdf_2(double x,double mu, double sigma)
{
	//Declaración de Datos
	
	double b0= 0.2316419;
	double b1= 0.319381530;
	double b2=-0.356563782;
	double b3= 1.781477937;
	double b4=-1.821255978;
	double b5= 1.330274429;
	
	double cdf,w;
	double abs_x=abs(x);
	
	double t;
	
	
	//Cálculos (Aproximación numérica)
	
	t==(1.0/(1+(b0*abs_x)));
	
	w=1-((Gaussiana(abs_x,mu,sigma))
		*(b1*t + b2*pow(t,2) + b3*pow(t,3) + b4*pow(t,4) + b5*pow(t,5)));
	
	if(x>=0)
	{
		cdf=w;
	}
	else
	{
		cdf=1-w;
	}
	
	
	//Salida de la función
	return cdf;
}
/****************************************************************************/
int main()
{
	
	//Programa de prueba de la función
	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	
	
	//Declaración de datos
	
	const double SALTO=0.25; //Distancia entre 2 valores consecutivos de x
	double mu, sigma, x;
	double x_min, x_max;
	
	
	//Entradas
	
	cout<<"Esperanza o media: ";
	cin>>mu;
	
	do //Filtro de la desviación (tiene que ser positiva)
	{
		cout<<"Desviacion tipica: ";
		cin>>sigma;	
	} while(sigma<=0);
	
	
	
	//Cálculos
	
	x_min=(mu-(3*sigma));
	x_max=(mu+(3*sigma));

	
	
	//Salidas
	
	cout<<endl;
	cout<<" --------------------------------------------------------- "<<endl;
	cout<<"|           |              |             CDF(x)           |"<<endl;
	cout<<"|     x     | gaussiana(x) |------------------------------|"<<endl;
	cout<<"|           |              |    Metodo 1   |   Metodo 2   |"<<endl;
	cout<<"|-----------|--------------|---------------|--------------|"<<endl;


	for(double x=x_min;x<=x_max;x+=SALTO)
	{
		cout<<"|  ";
		cout<<setw(6)<<setprecision(2)<<x;
		cout<<"   | ";
		cout<<setw(12)<<setprecision(10)<<Gaussiana(x,mu,sigma);
		cout<<" | ";
		cout<<setw(13)<<setprecision(11)<<Cdf_1(x,mu,sigma,SALTO);
		cout<<" | ";
		cout<<setw(12)<<setprecision(10)<<Cdf_2(x,mu,sigma);
		cout<<" |"<<endl;
	}
	cout<<" --------------------------------------------------------- "<<endl;


	
	return 0;
}
