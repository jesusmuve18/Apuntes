/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_1
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
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const double SALTO= 0.000001;			//Cuanto menor sea, más precisión  
											//tendrá el resultado
	const double PI=3.14159265359;
	const double CTE=10; 	//Valor que utilizo para darle un valor inicial a 
							//"-infinito" (cuanto mayor sea, más preciso será
							//el resultado)									
	
	 double mu; //Esperanza o media
	 double sigma;	//Desviación típica
	 double cdf=0;
	 double x_aux, x_final;
	 double g_aux;
	 bool datos_ok;
	
	//Entradas
	
	cout<<"Esperanza: ";
	cin>>mu;
	cout<<"Desviacion Tipica: ";
	cin>>sigma;
	cout<<"Valor de x: ";
	cin>>x_final;
	
	
	//Cálculos
	
	datos_ok=(sigma>=0);
	
	if(datos_ok)
	{
		for(x_aux=(mu-(CTE*sigma)) ; x_aux<=x_final ; x_aux+=SALTO)
		{
			g_aux=(exp(-0.5*pow ((x_aux-mu)/sigma,2))/(sigma*sqrt(2*PI)))*SALTO;
			cdf+=g_aux;
		}
		/*Al ser el cálculo de un área lo aproximaré a la suma de areas de 
		"rectangulitos" que tendrán de ancho el denominado SALTO y de alto el 
		valor de la función en el punto. Al ser rectángulos el área se 
		calculará como base*altura de modo que el rectángulo en el punto x1 
		será:
				area=g(1)*SALTO, siendo g(1) la gaussiana en el punto.
		*/
		
		//Salidas
		
		cout<<"La CDF de una variable cualquiera Normal("<<mu<<","<<sigma
			<<") en "<<"x = "<<x_final<<" es: "<<endl;
		cout<<"\t"<<cdf<<endl;
	}
	else
		cout<<"Valor de desviacion tipica no valido"<<endl;
	
	return 0;
}
