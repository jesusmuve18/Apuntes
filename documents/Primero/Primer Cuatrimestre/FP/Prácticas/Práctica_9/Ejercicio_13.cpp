/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_13
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En la tabla 2 mostramos la subida del sueldo de los funcionarios en España 
	desde 2010.
	
	Considerando la subida del IPC detallada en la tabla 1 escriba un programa 
	que solicite dos años y calcule la pérdida/ganancia de poder adquisitivo 
	(en porcentaje) de un funcionario entre los dos años dados.
	
	Repetir hasta que se introduzca un año negativo.
	
						Año  Subida   |	 Año Subida
						--------------|-------------
						2021 0,90 %   |	2015 0,00 %
						2020 2,00 %   |	2014 0,00 %
						2019 2,50 %   |	2013 0,00 %
						2018 1,75 %   |	2012 0,00 %
						2017 1,00 %   |	2011 0,00 %
						2016 1,00 %   |	2010 -5,00 %
						
			Cuadro 2: Subida de los sueldos de los funcionarios
			
	Entradas: Año de referencia y otro año
	
	Salidas: Ganancia o pérdida del poder adquisitivo en porcentaje
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;

//#define DATOS_EXTRA

/****************************************************************************/
//Declaración de constantes globales

const double IPC_PRODUCTOS[12]= { 3.0 , 2.4 ,  2.9 , 0.3 , -1.0 , 0.0 , 
					     		  1.6 , 1.1 , 1.2 , 0.8 , -0.5 , 6.5 };
				
const double IPC_SUELDOS[12] = { -5.0 , 0.0 , 0.0  , 0.0 , 0.0 , 0.0 , 
								  1.0 , 1.0 , 1.75 , 2.5 , 2.0 , 0.9  };
				   
const int ANIO_MINIMO=2010;
const int ANIO_MAXIMO=2021;

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int anio_referencia, anio_dado, diferencia_anios;
	double incremento_ipc_productos, incremento_ipc_sueldos, diferencia_poder;
	bool seguir=true;
	
	
	
	while(seguir)
	{
		//Entradas
			
		do
		{
			cout<<"Introduce un anio de referencia: ";
			cin>>anio_referencia;
		} while(anio_referencia>ANIO_MAXIMO||
					(anio_referencia<ANIO_MINIMO && anio_referencia>=0));
		
		if(anio_referencia>0)
		{
			do
			{
				cout<<"Introduce otro anio: ";
				cin>>anio_dado;
			} while (anio_dado>ANIO_MAXIMO ||
					(anio_dado<ANIO_MINIMO && anio_dado>=0));
			
			if(anio_dado<0)
			{
				seguir=false;
			}
		} //if(anio_referencia>0)
		else
		{
			seguir=false;
		}


		if(seguir)
		{
			
			
			
			//Cálculos
			
			diferencia_anios=anio_dado-anio_referencia;
			double valor_auxiliar_productos=1; 	//neutro del producto
			double valor_auxiliar_sueldos=1; 	//neutro del producto
		
			if(diferencia_anios>=0)
			{
				for(int i=1; i<=diferencia_anios; i++)
				{
					valor_auxiliar_productos*=(1+
						(IPC_PRODUCTOS[i+anio_referencia-ANIO_MINIMO])/100.0);
					
					valor_auxiliar_sueldos*=(1+
						(IPC_SUELDOS[i+anio_referencia-ANIO_MINIMO])/100.0);
				}
				
			}
			else
			{
				diferencia_anios=-diferencia_anios;
				
				for(int i=0; i<diferencia_anios; i++)
				{
					valor_auxiliar_productos/=(1+
						(IPC_PRODUCTOS[anio_referencia-ANIO_MINIMO-i])/100.0);
					
					valor_auxiliar_sueldos/=(1+
						(IPC_SUELDOS[anio_referencia-ANIO_MINIMO-i])/100.0);	
				}
				
			}
			
			incremento_ipc_productos=(valor_auxiliar_productos-1)*100;
			incremento_ipc_sueldos=(valor_auxiliar_sueldos-1)*100;
			
			diferencia_poder=incremento_ipc_sueldos-incremento_ipc_productos;
				
			//Salidas
			
			#ifdef DATOS_EXTRA
			
			cout<<endl;
			cout<<"El IPC en productos ha variado en un "
				<<incremento_ipc_productos<<"%"<<endl;
			cout<<"El IPC en sueldos ha variado en un "
				<<incremento_ipc_sueldos<<"%"<<endl;
			
			#endif
			
			cout<<endl;
			
			if(diferencia_poder>0)
			{
				cout<<"El poder adquisitivo ha aumentado en un "
					<<diferencia_poder<<"%"<<endl;
			}
			else
			{
				if(diferencia_poder<0)
				{
					cout<<"El poder adquisitivo ha disminuido en un "
					<<(-diferencia_poder)<<"%"<<endl;
				}
				else
				{
					cout<<"El poder adquisitivo no ha cambiado"<<endl;
				}
			}
			
			cout<<endl;
			cout<<"..................................................."<<endl;
			cout<<endl;
			
		}  //if(seguir)
		else
		{
			seguir=false;
		}
	
	} //while(seguir)
	
	
	return 0;
}
