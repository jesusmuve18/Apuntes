/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_52
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Construya un programa que:
	
	a) lea el valor tope obligando a que esté entre 1 y 100000, calcule la 
	   aproximación de p mediante la anterior serie e imprima el resultado en 
	   pantalla.
	b) lea un entero que indique cuántos decimales de precisión deseamos, 
	   calcule la aproximación de p mediante la anterior serie y nos indique 
	   cuántas iteraciones -sumas- ha tenido que realizar para conseguir la 
	   aproximación deseada.
	   El valor de p a aproximar es 3.14159265358979323846	
	
	Entradas: 
	
	Salidas: 
	
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
	
	//Apartado a):
	
	//Declaración de datos
	
	const int MIN_TOPE=1;
	const int MAX_TOPE=100000;
	const double AUXILIAR=(1/(sqrt(3)));
	
	int tope;
	double pi; //resultado de la operación
	bool datos_ok;
	double sumatorio=0;
	
	//Entradas
	
	cout<<"Apartado a)"<<endl,
	cout<<"Tope: ";
	cin>>tope;
	
	
	
	//Cálculos
	
	datos_ok=(MIN_TOPE<=tope && tope<=MAX_TOPE);
	long double pow_1, pow_2;
	
	if(datos_ok)
	{
		 
		for(int i=0; i<=tope; i++)
		{
			pow_1=1, pow_2=1; //Los inicializo con los neutros del producto
			for(int j=0; j<i; j++) //Traduccion de una potencia
			{
				pow_1=pow_1*(-1);
				//cout<<pow_1<<endl;
			}
			for(int j=0; j<(2*i+1); j++) //Traduccion de una potencia
			{
				pow_2=pow_2*AUXILIAR;
				//cout<<pow_2<<endl;
			}
			sumatorio+=((pow_1)*(pow_2)/(2*i+1));
			//cout<<sumatorio<<endl;
		}
		
		pi=6*sumatorio;
	}
	
	//Salidas
	
	if(datos_ok)
	{
		cout<<"El valor de pi para "<<tope<<" sumandos es: "
			<<setprecision(10)<<pi<<endl;
	}
	
	
	/************************************************************************/
	//Apartado b)

	//Declaración de datos
	
	const long double PI=3.14159265358979323846;
	
	int n;
	long double pi_trunc;
	
	//Entradas
	
	cout<<endl;
	cout<<"Apartado b)"<<endl,
	cout<<"Numero de decimales: ";
	cin>>n;
	
	//Cálculos
	
	datos_ok=(0<n && n<=8); 	//por la naturaleza de un dato double no tiene mas
								//de 8 decimales
	if(datos_ok) 
	{						
		//Primero truncaré pi para saber el resultado que busco
		
		pi_trunc=((int)(PI*pow(10.0,n)))/(pow(10.0,n));
		
		pi=0;
		tope=0;
		while(pi!=pi_trunc)
		{
			sumatorio=0; //Lo inicializo con el neutro de la suma
			for(int i=0; i<=tope; i++)
			{
				
				pow_1=1, pow_2=1; //Los inicializo con los neutros del producto
				for(int j=0; j<i; j++) //Traduccion de una potencia
				{
					pow_1=pow_1*(-1);
				}
				for(int j=0; j<(2*i+1); j++) //Traduccion de una potencia
				{
					pow_2=pow_2*AUXILIAR;
				}
				sumatorio+=((pow_1)*(pow_2)/(2*i+1));
			}
			tope++;
			pi=6*sumatorio;
			
			pi=((int)(pi*pow(10.0,n)))/(pow(10.0,n));//Truncaré pi para comparar
	
		}	
	}
	
	//Salidas
	
	if(datos_ok)
	{
		cout<<"Se necesitan "<<tope<<" iteraciones";
	}
	else
		cout<<"Numero de decimales no valido";
	
	
	return 0;
}
//operación. sumatorio+=(((pow((-1),i))*(pow((1/(sqrt(3))),(2*i+1))))/(2*i+1));

