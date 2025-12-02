/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_8
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Recuperad el ejercicio 30 de la Relación de Problemas III sobre la función 
	gaussiana. En vez de trabajar con funciones, plantead la solución con una 
	clase.
	
	Debe diseñar la clase teniendo en cuenta que la función matemática 
	gaussiana viene determinada unívocamente por el valor de la esperanza y la 
	desviación, es decir, son estos dos parámetros lo que distinguen a una 
	función gaussiana de otra.
	
	Recuerde añadir un método ToString para la clase. Sugerimos el siguiente 
	formato: si g es N(0, 1) y el objeto g representa a esa función, la 
	ejecución de g.ToString() devolverá la cadena N (0.000000, 1.000000).
	
	Entradas: mu y sigma
	
	Salidas: gaussiana(x) y CDF(x) calculado de dos formas diferentes
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;
/****************************************************************************/
/****************************************************************************/
class Gaussiana
{
	private:
		
		double mu;     //Esperanza
		
		//PRE: desviación > 0
		double sigma;  //Desviación
		
	public:
		
		//Constructor de dos parámetros
		Gaussiana (double esperanza, double desviacion):
			mu(esperanza), sigma(desviacion) {};
		
		
		//Constructor sin parámetros
		Gaussiana (void): mu(0), sigma(1) {};
		
		
		//Cálculo de la gaussiana con la fórmula
		double Calculo(double x)
		{
			const double PI=6*asin(0.5);
			return (exp(-0.5*pow ((x-mu)/sigma,2))/(sigma*sqrt(2*PI)));
		}
		
		//Cálculo de CDF(x) según el método 1
		double Cdf_1(double x)
		{
			//Declaración de Datos
			double cdf=0;
			double x_final=x;
			const double SALTO=0.25; //Distancia entre 2 valores 
									 //consecutivos de x
			
			
			//Cálculos (Desarrollo de la integral por sumatoria)
			
			for(double x_aux=(mu-(3*sigma)) ; x_aux<=x_final ; x_aux+=SALTO)
			{
				cdf+=Calculo(x_aux)*SALTO;
			}
			
			
			//Salida de la función
			return cdf;
		
		}
		
		
		//Cálculo de CDF(x) según el método 2
		double Cdf_2(double x)
		{
			//Declaración de Datos
			const double B_LENGTH=6;
			const double B[]={ 0.2316419, 0.319381530, -0.356563782,  
				               1.781477937, -1.821255978, 1.330274429 };
			
			
			double cdf,w;
			double abs_x=abs(x);
			double segundo_termino=0; //Inicializo con el neutro de la suma
			
			double t;
			
			
			//Cálculos (Aproximación numérica)
			
			t=(1.0/(1+(B[0]*abs_x)));
			
			for(int i=1; i<B_LENGTH ; i++)
			{
				segundo_termino+=B[i]*pow(t,i);
			}

			
			w=1-(Calculo(abs_x)*segundo_termino);
	
			
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
		
		
		//Método para mostrar el objeto
		string ToString()
		{
			return ("N ("+to_string(mu)+" , "+to_string(sigma)+")");
		}
		
		
		//Métodos de lectura (Get) de datos individuales
		double GetEsperanza() {return mu;}
		double GetDesviacion() {return sigma;}
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	
	//Programa de prueba de la función
	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	
	
	//Declaración de datos
	const double SALTO=0.25; //Distancia entre 2 valores consecutivos de x
	double esperanza, desviacion, x;
	double x_min, x_max;
	
	
	//Entradas
	
	cout<<"Esperanza o media: ";
	cin>>esperanza;
	
	do //Filtro de la desviación (tiene que ser positiva)
	{
		cout<<"Desviacion tipica: ";
		cin>>desviacion;	
	} while(desviacion<=0);
	
	
	
	//Cálculos
	
	Gaussiana g(esperanza,desviacion); //Genero un objeto con el constructor
	
	x_min=(g.GetEsperanza()-(3*g.GetDesviacion()));
	x_max=(g.GetEsperanza()+(3*g.GetDesviacion()));

	
	
	//Salidas
	
	cout<<endl;
	cout<<"Funcion: "<<g.ToString()<<endl;
	
	
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
		cout<<setw(12)<<setprecision(10)<<g.Calculo(x);
		cout<<" | ";
		cout<<setw(13)<<setprecision(11)<<g.Cdf_1(x);
		cout<<" | ";
		cout<<setw(12)<<setprecision(10)<<g.Cdf_2(x);
		cout<<" |"<<endl;
	}
	cout<<" --------------------------------------------------------- "<<endl;

	
	return 0;
}
