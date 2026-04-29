/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_12
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En la tabla 1 mostramos el IPC de España entre 1990 y 2021. IPC es es 
	acrónimo de Índice de Precios al Consumo, que el INE3 define como una 
	medida estadística de la evolución de los precios de los bienes y servicios 
	que consume la población residente en viviendas familiares en España. El 
	conjunto de bienes y servicios, que conforman la cesta de la compra, se 
	obtiene básicamente del consumo de las familias y la importancia de cada 
	uno de ellos en el cálculo del IPC está determinada por dicho consumo.

		 Año    IPC   |	 Año   IPC 	  |	 Año   IPC 	  |	 Año    IPC
		-------------------------------------------------------------
		2021  6,50 %  |	2013  0,30 %  |	2005  3,70 %  |	1997  2,00 %
		2020 -0,50 %  |	2012  2,90 %  |	2004  3,20 %  |	1996  3,20 %
		2019  0,80 %  |	2011  2,40 %  |	2003  2,60 %  |	1995  4,30 %
		2018  1,20 %  |	2010  3,00 %  |	2002  4,00 %  |	1994  4,30 %
		2017  1,10 %  |	2009  0,80 %  |	2001  2,70 %  |	1993  4,90 %
		2016  1,60 %  |	2008  1,40 %  |	2000  4,00 %  |	1992  5,30 %
		2015  0,00 %  |	2007  4,20 %  |	1999  2,90 %  |	1991  5,50 %
		2014 -1,00 %  |	2006  2,70 %  |	1998  1,40 %  |	1990  6,50 %
			
		Cuadro 1: Datos históricos del IPC en España (Fuente: INE)
		
		
	Queremos conocer el valor actualizado según las variaciones del IPC que 
	debería tener un bien cualquiera conociendo su valor monetario en un año 
	concreto. Por ejemplo, en 2001 una barra de pan costaba 0.30 e y una 
	entrada de cine 3,90 e. En 2021 la barra de pan costaba alrededor de 
	0.70 e y la entrada de cine 7,78 e. ¿El incremento de precio es razonable?
	
	Escriba un programa que solicite un valor monetario y el año de referencia 
	de ese valor. A continuación solicite un otro año (puede ser anterior al 
	primero). El programa debe mostrar el valor monetario dado actualizado 
	según el IPC en el año indicado. 
	
	Repetir hasta que se introduzca un valor monetario negativo.
	
	Entradas: Año de referencia y otro año. 
			  Valor monetario del año de referencia
	
	Salidas: Valor monetario del año dado 
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const double IPC[32] ={ 6.5 , 5.5 , 5.3 , 4.9 , 4.3 , 4.3 ,  3.2 , 2.0 ,
						1.4 , 2.9 , 4.0 , 2.7 , 4.0 , 2.6 ,  3.2 , 3.7 , 
						2.7 , 4.2 , 1.4 , 0.8 , 3.0 , 2.4 ,  2.9 , 0.3 ,
					   -1.0 , 0.0 , 1.6 , 1.1 , 1.2 , 0.8 , -0.5 , 6.5 };
				   
const int ANIO_MINIMO=1990;
const int ANIO_MAXIMO=2021;

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int anio_referencia, anio_dado, diferencia_anios;
	double valor_monetario_referencia, valor_monetario_resultante=0;
	bool seguir=true;
	
	
	
	while(seguir)
	{
		//Entradas
		
	
		cout<<"Introduce un valor monetario del anio de referencia: ";
		cin>>valor_monetario_referencia;


		if(valor_monetario_referencia>=0)
		{
			//Entradas que dependen del valor monetario del año de referencia
			//(si es positivo)
			
			do
			{
				cout<<"Introduce un anio de referencia: ";
				cin>>anio_referencia;
			} while(anio_referencia>ANIO_MAXIMO||anio_referencia<ANIO_MINIMO);
			
			do
			{
				cout<<"Introduce otro anio: ";
				cin>>anio_dado;
			} while (anio_dado>ANIO_MAXIMO || anio_dado<ANIO_MINIMO);
			
			
			
			//Cálculos
			
			diferencia_anios=anio_dado-anio_referencia;
			double valor_auxiliar=valor_monetario_referencia;
		
			if(diferencia_anios>=0)
			{
				for(int i=1; i<=diferencia_anios; i++)
				{
					valor_auxiliar*=(1+
								 (IPC[i+anio_referencia-ANIO_MINIMO])/100.0);
				}
				
			}
			else
			{
				diferencia_anios=-diferencia_anios;
				
				for(int i=0; i<diferencia_anios; i++)
				{
					valor_auxiliar/=(1+
								(IPC[anio_referencia-ANIO_MINIMO-i])/100.0);	
				}
				
			}
			
			valor_monetario_resultante=valor_auxiliar;
				
				
			//Salidas
			
			cout<<valor_monetario_referencia<<" Euros en "<<anio_referencia
				<<" equivalen a "<<valor_monetario_resultante<<" Euros en "
				<<anio_dado<<endl;		
				
			cout<<endl;
			cout<<"..................................................."<<endl;
			cout<<endl;
			
		}  //if(valor_monetario_referencia<0)
		else
		{
			seguir=false;
		}
	
	} //while(seguir)
	
	
	return 0;
}
