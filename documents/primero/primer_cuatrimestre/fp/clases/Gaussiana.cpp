class Gaussiana
{
	private:
		double mu;     //Esperanza
		double sigma;  //Desviación
	public:
		Gaussiana (double esperanza, double desviacion):
			mu(esperanza), sigma(desviacion) {};
		
		double Calculo(double x)
		{
			const double PI=3.141592;
			
			return (exp(-0.5*pow ((x-mu)/sigma,2))/(sigma*sqrt(2*PI)));
		}
		
		double Cdf_1(double x)
		{
			//Declaración de Datos
			
			const double SALTO=0.25; //Distancia entre 2 valores
									 //consecutivos de x
			double cdf=0;
			double x_final=x;
			
			
			//Cálculos (Desarrollo de la integral por sumatoria)
			
			for(double x_aux=(mu-(3*sigma)) ; x_aux<=x_final ; x_aux+=SALTO)
			{
				cdf+=Calculo(x_aux)*SALTO;
			}
			
			
			//Salida de la función
			return cdf;
		
		}
		
		double Cdf_2(double x)
		{
			//Declaración de Datos
			
			const double B_LENGTH=6;
			const double B[]={ 0.2316419, 0.319381530, -0.356563782, 
				               1.781477937, -1.821255978, 1.330274429 };
			
			double cdf,w;
			double abs_x=fabs(x);
			double segundo_termino=1; //Inicializo con el neutro del producto
			
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
		
		string ToString()
		{
			return ("N ("+to_string(mu)+" , "+to_string(sigma)+")");
		}
};
