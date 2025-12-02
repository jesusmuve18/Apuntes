/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Reescriba la solución del ejercicio 47 de la Relación de Problemas IV 
	usando una clase, GestorIPC, que proporcione los valores del IPC y métodos 
	para su gestión.
	
	La estructura de la clase que proponemos es la indicada en la figura 41. 
	Los valores de las constantes estáticas, del array PAISES y de la matriz 
	IPC puede consultarlos en la figura 27 en la que se mostraba el IPC de 
	varios paises entre 2001 y 2021.

	Entradas: numero de años hasta introducir negativo

	Salidas: ipc max y min en ese período de tiempo 

*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

struct InfoIPC 
{
	int indice_pais; // 0 <= indice_pais <= NUM_PAISES
	double valor_medio_IPC; // Máximo ó mínimo IPC medio (valor)
	int anio_inicio; // Primer año del periodo
	int num_anios; // Número de años del periodo
};


/****************************************************************************/
//Declaración de Clases/Objetos

class GestorIPC
{

private:
	
	static const int PRIMER_ANIO=2001;
	static const int ULTIMO_ANIO=2021;
	static const int NUM_ANIOS = ULTIMO_ANIO - PRIMER_ANIO + 1;
	static const int NUM_PAISES = 4;
	static const string PAISES [NUM_PAISES];
    static const double IPC[NUM_PAISES][NUM_ANIOS];

public:
	
	/**********************************************************************/
	//Método que devuelve el primer año
	
	int PrimerAnio(void)
	{
		return (PRIMER_ANIO);
	}
	
	/**********************************************************************/
	//Método que devuelve el último año
	
	int UltimoAnio(void)
	{
		return (ULTIMO_ANIO);
	}
	
	/**********************************************************************/
	//Método que devuelve el número de años
	
	int NumAnios(void)
	{
		return (NUM_ANIOS);
	}
	
	/**********************************************************************/
	//Método que devuelve el número de países
	
	int NumPaises(void)
	{
		return (NUM_PAISES);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve el nombre del país asociado a un número
	
	string NombrePais(int pais)
	{
		return (PAISES[pais]);
	}
	
	/**********************************************************************/
	//Método que devuelve el IPC de un año en un país
	
	double ValorIPC(int anio, int pais)
	{
		return (IPC[pais][anio]);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve el IPC medio de un pais en un periodo de tiempo
	//PRE: no se el_primer_anio + los_anios no debe ser mayor que NUM_ANIOS
	
	double IPC_Medio_PaisPeriodo(int el_pais,int el_primer_anio,int los_anios)
	{
		double aux=1;
		double ipc_medio;
		
		for(int anio=el_primer_anio; anio<el_primer_anio+los_anios; anio++)
		{	
			aux*=(1+(IPC[el_pais][anio-PRIMER_ANIO])/100.0);			
		}
		
		ipc_medio=((aux-1)*100)/los_anios;
		
		return ipc_medio;
	}	
	
	/**********************************************************************/
	//Método que devuelve el mínimo IPC medio en un período
	
	InfoIPC Info_IPC_MinMedio_Periodo (int num_anios)
	{
		InfoIPC info_ipc_min;
		info_ipc_min.valor_medio_IPC=999999999;
		info_ipc_min.num_anios=num_anios;
		
		double ipc_medio; //tomará valores auxiliares
		
		for(int pais=0; pais<NUM_PAISES; pais++)
		{
			for(int anio_inicial=PRIMER_ANIO; 
						anio_inicial<=ULTIMO_ANIO-num_anios; anio_inicial++)
			{
				
				ipc_medio=IPC_Medio_PaisPeriodo(pais,anio_inicial,num_anios);
	
				if(ipc_medio<info_ipc_min.valor_medio_IPC)
				{
					info_ipc_min.indice_pais=pais;
					info_ipc_min.valor_medio_IPC=ipc_medio;
					info_ipc_min.anio_inicio=anio_inicial;	
				}
			}
		}	
		
		return info_ipc_min;
	} 
	/**********************************************************************/
	//Método que devuelve el máximo IPC medio en un período
	
	InfoIPC Info_IPC_MaxMedio_Periodo (int num_anios)
	{
		InfoIPC info_ipc_max;
		info_ipc_max.valor_medio_IPC=-999999999;
		info_ipc_max.num_anios=num_anios;
		
		double ipc_medio; //tomará valores auxiliares
		double ipc_max=-999999999;
		
		for(int pais=0; pais<NUM_PAISES; pais++)
		{
			for(int anio_inicial=PRIMER_ANIO; 
						anio_inicial<=ULTIMO_ANIO-num_anios; anio_inicial++)
			{
				
				ipc_medio=IPC_Medio_PaisPeriodo(pais,anio_inicial,num_anios);
	
				if(ipc_medio>info_ipc_max.valor_medio_IPC)
				{
					info_ipc_max.indice_pais=pais;
					info_ipc_max.valor_medio_IPC=ipc_medio;
					info_ipc_max.anio_inicio=anio_inicial;	
				}
			}
		}	
		
		return info_ipc_max;	
	} 
	
};

const string GestorIPC::PAISES[NUM_PAISES]=
						{"Espania","Inglaterra", "USA", "Zona Euro"};
const double GestorIPC::IPC[NUM_PAISES][NUM_ANIOS]=
					   {{2.70 , 4.00 , 2.60 , 3.20 , 3.70 ,		//España
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
/****************************************************************************/
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
	
	const int N_MIN=2;
	const int N_MAX=5;
	
	int n_anios;
	string peticion="Numero de anios (entre "+to_string(N_MIN)+" y "
					+to_string(N_MAX)+"): ";
	
	GestorIPC gestor; //Inicializo un gestor de la clase GestorIPC
					
	do
	{
		//Entradas

		do //Filtro de Número de años (permite MIN<=n<=MAX y n<0)
		{
			
			n_anios=LeeEntero(peticion);
			
		} while((n_anios<N_MIN || N_MAX<n_anios) && (n_anios>=0));
		
		//Cálculos
		
		InfoIPC ipc_max=gestor.Info_IPC_MaxMedio_Periodo(n_anios);
		InfoIPC ipc_min=gestor.Info_IPC_MinMedio_Periodo(n_anios);
		
		//Salidas
		
		if(n_anios>0)
		{
			cout<<endl;
			
			cout<<"1. Mayor IPC medio en "<<n_anios<<" anios:"<<endl;
			cout<<"\tPais: "<<gestor.NombrePais(ipc_max.indice_pais)<<endl;
			cout<<"\tValor: "<<ipc_max.valor_medio_IPC<<"% (de media)"<<endl;
			cout<<"\tAnios: Entre "<<ipc_max.anio_inicio<<" y "
				<<ipc_max.anio_inicio+n_anios<<endl;
				
			cout<<endl;
				
			cout<<"2. Menor IPC medio en "<<n_anios<<" anios:"<<endl;
			cout<<"\tPais: "<<gestor.NombrePais(ipc_min.indice_pais)<<endl;
			cout<<"\tValor: "<<ipc_min.valor_medio_IPC<<"% (de media)"<<endl;
			cout<<"\tAnios: Entre "<<ipc_min.anio_inicio<<" y "
				<<ipc_min.anio_inicio+n_anios<<endl;
				
			cout<<endl;
			cout<<"............................................."<<endl;
			cout<<endl;
		}
		
	} while(n_anios>=0);

	return 0;
}

