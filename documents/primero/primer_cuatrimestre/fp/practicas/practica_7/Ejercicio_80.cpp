/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_80
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir un programa en el que se presente un menú principal para que el 
	usuario pueda elegir las siguientes opciones:
	
		a) Introducir parámetros de la función (esperanza y desviación)
		b) Salir del programa
		
	Si el usuario elige la opción de salir, el programa terminará; si elige 
	la opción de introducir los parámetros, el programa leerá los dos 
	parámetros (esperanza y desviación).
	
	A continuación, el programa presentará un menú con las siguientes opciones:
	
		• Introducir rango de valores de abscisas
		• Volver al menú anterior (el menú principal)
		
	Si el usuario elige volver al menú anterior, el programa debe presentar el 
	primer menú; si el usuario elige introducir los valores de abscisas, el 
	programa le pedirá los extremos	del intervalo y el incremento entre dos 
	valores consecutivos de la abscisa, y mostrará	los valores de la 
	función gaussiana en todos los valores posibles de la abscisa (de
	manera similar a como se hizo en el ejercicio 78).
	
	Después de mostrar los valores de la función, el programa volverá al menú 
	de introducción del rango de valores de abscisas.
		
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
	 
	 bool terminar=false, opcion2_ok=true;
	 char opcion1, opcion2;
	
	//Entradas
	
	while(!terminar)
	{
		cout<<"Elija entre las siguientes opciones:"<<endl;
		cout<<"\ta)Introducir parametros de la funcion "
			<<"(esperanza y desviacion)"<<endl;
		cout<<"\tb) Salir del programa"<<endl;
		cout<<">";
		cin>>opcion1;
		
		opcion1=toupper(opcion1);
		
		switch (opcion1)
		{
			case 'A':
				
				cout<<"Esperanza: ";
				cin>>mu;
				do
				{
					cout<<"Desviacion Tipica: ";
					cin>>sigma;	
				} while(sigma<=0);
				
				do
				{
					cout<<endl;
					cout<<"Elija entre las siguientes opciones: "<<endl;
					cout<<"\ta) Introducir rango de valores de abscisas"<<endl;
					cout<<"\tb) Volver al menu anterior"<<endl;
					cout<<">";
					cin>>opcion2;
					
					opcion2=toupper(opcion2);
					
					switch (opcion2)
					{
						case 'A':
							opcion2_ok=true; 
											 
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
							} while (salto_dado<=0 
									|| salto_dado>(x_final-x_inicio));
							
							cout<<endl;
							cout<<"  ----------------------------------------"
								<<"------ "<<endl;
							cout<<" |       x      |  gaussiana(x)  |    "
								<<"CDF(x)    |"<<endl;
							cout<<" |---------------------------------------"
								<<"-------|"<<endl;
							
							
							//Cálculos
							
							x_aux=(mu-(CTE*sigma));
							
							for(double x=x_inicio ; x<=x_final; x+=salto_dado)
							{
								
								//Cálculo de la gaussiana 
								g_aux=(exp(-0.5*pow ((x-mu)/sigma,2))
									  /(sigma*sqrt(2*PI)));
								
								//Cálculo de CDF
										
								for( ; x_aux<=x ; x_aux+=SALTO)
								{
									cdf_aux=(exp(-0.5*pow ((x_aux-mu)/sigma,2))
											/(sigma*sqrt(2*PI)))
											*SALTO;
									cdf+=cdf_aux;
								}
										
								cout<<" | "<<x<<"\t|  "
									<<setw(8)<<setprecision(5)<<g_aux<<"\t |  "
									<<cdf<<"\t|"<<endl;
								
							}
							
							cout<<"  -----------------------------------------"
								<<"----- "<<endl;
							cdf=0;
							
							break;
							
						case 'B':
							opcion2_ok=true;
							break;
						
						default:
							cout<<"La opcion introducida no es valida"<<endl;
							opcion2_ok=false; //Para no volver al menú anterior
							break;			  //hasta que se haya introducido
					}						  //una opcion valida (a o b)
						
				}while(!opcion2_ok);
				
				
				break;
				
			case 'B':
				terminar=true;
				break;
				
			default:
				cout<<"La opcion introducida no es valida"<<endl;
				break;
		}
	}
	
	
	return 0;
}
