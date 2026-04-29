/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_47
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En la tabla 23 mostramos el IPC de varios paises entre 2001 y 2021.

	Escriba un programa que solicite un número n de años (2 = n = 5) y muestre 
	qué pais (España, Inglaterra, USA ó Zona Euro) es el que 
	
	1) tiene el mayor IPC medio en n años  
	2) tiene el menor IPC medio en n años. 
	
	Muestre además, sus valores y los años entre los que se han calculado los 
	resultados.
	
	Repita hasta introducir un valor de n negativo
	
	Entradas: Intervalos de años
	
	Salidas: Zonas con mayor variación de IPC con sus valores y años
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int ANIO_INICIO=2001;
const int ANIO_FINAL=2021;
const int NUMERO_ZONAS=4;

const double IPC[NUMERO_ZONAS][ANIO_FINAL-ANIO_INICIO+1]=

					  {{ 2.70 , 4.00 , 2.60 , 3.20 , 3.70 ,		//España
					     2.70 , 4.20 , 1.40 , 0.80 , 3.00 ,
			   		     2.40 , 2.90 , 0.30 , -1.0 , 0.00 ,
					     1.60 , 1.10 , 1.20 , 0.80 , -0.5 ,
			   		     6.50 } ,
			   		  
			   		   { 1.07 , 1.69 , 1.25 , 1.64 , 1.92 ,		//Inglaterra
					     2.97 , 2.12 , 3.11 , 2.83 , 3.73 ,
					     4.20 , 2.71 , 2.00 , 0.55 , 0.23 ,
					     1.60 , 2.90 , 2.10 , 1.30 , 0.60 ,
					     5.40 } ,
					  
					   { 1.60 , 2.40 , 1.90 , 3.30 , 3.40 ,		//USA
					     2.50 , 4.10 , 0.10 , 2.70 , 1.50 ,
					     3.00 , 1.70 , 1.50 , 0.80 , 0.70 ,
					     2.10 , 2.10 , 1.90 , 2.30 , 1.40 ,
					     7.00 } ,
					  
					   { 2.10 , 2.30 , 2.00 , 2.40 , 2.20 ,		//Zona Euro
					     1.90 , 3.10 , 1.60 , 0.90 , 2.20 ,
					     2.80 , 2.20 , 0.80 , -0.2 , 0.20 ,
					     1.10 , 1.30 , 1.50 , 1.30 , -0.3 ,
					     5.00 }};


const int N_MIN=2;
const int N_MAX=5;

/*****************************************************************************
  	Función: Traduce el numero asociado a la zona con el titulo que posee
	Parámetros: Número que asociado a cada zona
	Devuelve: Título de la zona o país en formato string		
............................................................................*/
string Zona(int numero)
{
	string zona;
	
	switch(numero)
	{
		case 0:
			zona="Espana";
			break;
		
		case 1:
			zona="Inglaterra";
			break;
		
		case 2:
			zona="USA";
			break;
		
		case 3:
			zona="Zona Euro";
			break;
	}
	
	return zona;
	
}

/*****************************************************************************
  	Función: Lee entero dependiendo de las condiciones que este debe tener
  			 para ser considerado válido
	Parámetros: Título o etiqueta a modo de petición
	Devuelve: El entero en formato int		
............................................................................*/
double LeeEntero(string titulo)
{
	//#define MOSTRAR_ERROR
	
	string lectura;
	string caracteres_inicio="+-1234567890";
	string caracteres_resto="1234567890";
	
	bool todo_ok;


	do
	{
		do
		{
			cout<<titulo;
			getline(cin,lectura);
			
			string aux;
			
			for(int i=0; i<lectura.length(); i++) //Le quito los espacios
			{
				if(lectura.at(i)!=' ')
				{
					aux+=lectura.at(i);
				}
			}
			
			lectura=aux;	
			
		} while(lectura.empty()); //Mientras no haya caracteres
	
		
		todo_ok=true;
		
		bool caracter_correcto;
		int siguiente_caracter=0;
		
		if(lectura.length()>1) //Si hay mas de un caracter
		{
			//Compruebo el primer caracter

			for(int i=0; i<caracteres_inicio.length(); i++)
			{
				if(lectura.at(0)==caracteres_inicio.at(i))
				{
					caracter_correcto=true;
				}
			}
			
			if(!caracter_correcto)
			{
				todo_ok=false;
				
				#ifdef MOSTRAR_ERROR
					
				cout<<"Caracter inicio incorrecto"<<endl;
				
				#endif
			}	
			
			siguiente_caracter=1;
		}
		
		
		//Compruebo el resto de caracteres

		
		for(int i=siguiente_caracter; i<lectura.length(); i++)
		{
			caracter_correcto=false;
			
			for(int j=0; j<caracteres_resto.length(); j++)
			{
				if(lectura.at(i)==caracteres_resto.at(j))
				{
					caracter_correcto=true;
				}
			}
			
			if(!caracter_correcto)
			{
				todo_ok=false;
				
				#ifdef MOSTRAR_ERROR
				
				cout<<"Caracter "<<i+1<<" incorrecto"<<endl;
				
				#endif
			}
			
		}	
		
		#ifdef MOSTRAR_ERROR
				
		cout<<endl;
		
		#endif
		
	} while(!todo_ok);
	
	return stoi(lectura);
}


/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int n_anios;
	string peticion="Numero de anios (entre "+to_string(N_MIN)+" y "
					+to_string(N_MAX)+"): ";
	do
	{
		//Entradas
		
		do //Filtro de Número de años (permite MIN<=n<=MAX y n<0)
		{
			
			n_anios=LeeEntero(peticion);
			
		} while((n_anios<N_MIN || N_MAX<n_anios) && (n_anios>=0));
		
		
		
		if(n_anios>0)
		{
			double aux;
			double incremento_ipc;
			
			double incremento_max;
			int anio_ipc_max;
			int zona_ipc_max;
			
			double incremento_min;
			int anio_ipc_min;
			int zona_ipc_min;
			
			bool primera_vez=true;
			
			//Cálculos
			
			for(int zona=0; zona<NUMERO_ZONAS; zona++)
			{
				for(int anio_inicial=ANIO_INICIO; 
							anio_inicial<=ANIO_FINAL-n_anios; anio_inicial++)
				{
					aux=1;
					for(int i=1; i<=n_anios; i++)
					{		
						aux*=(1+(IPC[zona][i+anio_inicial-ANIO_INICIO])/100.0);
					
					}
					incremento_ipc=(aux-1)*100;	
					
					if(primera_vez)
					{
						incremento_min=incremento_ipc;
						anio_ipc_min=anio_inicial;
						zona_ipc_min=zona;
						
						incremento_max=incremento_ipc;
						anio_ipc_max=anio_inicial;
						zona_ipc_max=zona;
	
						
						primera_vez=false;
					}
					else
					{
						if(incremento_ipc<incremento_min)
						{
							incremento_min=incremento_ipc;
							anio_ipc_min=anio_inicial;
							zona_ipc_min=zona;
						}
						
						if(incremento_ipc>incremento_max)
						{
							incremento_max=incremento_ipc;
							anio_ipc_max=anio_inicial;
							zona_ipc_max=zona;
						}
					}
				}	
			}
			
			//Salidas
			
			cout<<endl;
			
			cout<<"1. Mayor IPC medio en "<<n_anios<<" anios:"<<endl;
			cout<<"\tPais: "<<Zona(zona_ipc_max)<<endl;
			cout<<"\tValor: "<<incremento_max<<"%"<<endl;
			cout<<"\tAnios: Entre "<<anio_ipc_max<<" y "<<anio_ipc_max+n_anios
				<<endl;
				
			cout<<endl;
				
			cout<<"2. Menor IPC medio en "<<n_anios<<" anios:"<<endl;
			cout<<"\tPais: "<<Zona(zona_ipc_min)<<endl;
			cout<<"\tValor: "<<incremento_min<<"%"<<endl;
			cout<<"\tAnios: Entre "<<anio_ipc_min<<" y "<<anio_ipc_min+n_anios
				<<endl;
				
			cout<<endl;
			cout<<"............................................."<<endl;
			cout<<endl;
			
		} //if(n_anios>0)
			
	} while(n_anios>=0);
	

	return 0;
}
