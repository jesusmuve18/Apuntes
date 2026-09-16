/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_14
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Considerando las tablas de la subida del IPC (tabla 1) y de la variación de 
	los sueldos de los funcionarios en España desde 2010 (tabla 2) escriba un 
	programa que calcule la ganancia/pérdida absoluta de poder adquisitivo de 
	un funcionario.
	
	El programa leerá el valor de un sueldo y el año de referencia de ese 
	sueldo. A continuación leerá el un segundo año (debe ser posterior). 
	Calcule el salario que le correspondería en ese segundo año si se 
	aplicaran las variaciones del IPC y el salario que le corresponde de 
	acuerdo a las subidas reales aplicadas. Muestre ambos valores, su
	diferencia y el porcentaje de ganancia/pérdida respecto al salario 
	calculado con las
	variaciones del IPC
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;

#define DATOS_EXTRA

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
	double incremento_ipc_productos, incremento_ipc_sueldos;
	double salario_referencia, salario_productos,salario_sueldos;
	double diferencia_salario, diferencia_porcentual;
	
	bool seguir=true;
	
	
	
	while(seguir)
	{
		//Entradas
		
		
		cout<<"Introduce un salario: ";
		cin>>salario_referencia;
		
		
		if(salario_referencia>=0)
		{
			do
			{
				cout<<"Introduce un anio de referencia: ";
				cin>>anio_referencia;
			}while(anio_referencia>ANIO_MAXIMO-1||
							(anio_referencia<ANIO_MINIMO));
							
			//le quito uno al año máximo para que pueda existir un año 
			//posterior en el rango del vector
			
			do
			{
				cout<<"Introduce otro anio (posterior): ";
				cin>>anio_dado;
			} while (anio_dado>ANIO_MAXIMO||anio_dado<ANIO_MINIMO
												||anio_dado<=anio_referencia);
			 
		} //if(valor_monetario_referencia>=0)	
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
		
			for(int i=1; i<=diferencia_anios; i++)
			{		
				valor_auxiliar_productos*=(1+
					(IPC_PRODUCTOS[i+anio_referencia-ANIO_MINIMO])/100.0);
			
				valor_auxiliar_sueldos*=(1+
					(IPC_SUELDOS[i+anio_referencia-ANIO_MINIMO])/100.0);
			}
			
			
			incremento_ipc_productos=(valor_auxiliar_productos-1);
			incremento_ipc_sueldos=(valor_auxiliar_sueldos-1);
			
			salario_productos=salario_referencia*(1+incremento_ipc_productos);
			salario_sueldos=salario_referencia*(1+incremento_ipc_sueldos);
			diferencia_salario=salario_sueldos-salario_productos;
			diferencia_porcentual=(diferencia_salario/salario_referencia)*100;
			
				
			//Salidas
			
			#ifdef DATOS_EXTRA
			
			cout<<endl;
			cout<<"El IPC en productos ha variado en un "
				<<incremento_ipc_productos*100<<"%"<<endl;
			cout<<"El IPC en sueldos ha variado en un "
				<<incremento_ipc_sueldos*100<<"%"<<endl;
			
			#endif
			
			cout<<endl;
			
			
			cout<<"El salario en "<<anio_dado<<" es de "<<salario_productos
				<<" Euros segun las variaciones del IPC"<<endl;
				
				
			cout<<"El salario en "<<anio_dado<<" es de "<<salario_sueldos
				<<" Euros segun las variaciones en sueldos"<<endl;
			
			if(diferencia_salario>=0)
			{
				cout<<"Ha aumentado en "<<diferencia_salario<<" Euros "
					<<"( "<<diferencia_porcentual<<"% )"<<endl;
			}
			else
			{
				cout<<"Ha disminuido en "<<-diferencia_salario<<" Euros "
					<<"( "<<diferencia_porcentual<<"% )"<<endl;
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
