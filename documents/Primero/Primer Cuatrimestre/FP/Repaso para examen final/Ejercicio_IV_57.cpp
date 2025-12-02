/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:


	Entradas:

	Salidas:

*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

/****************************************************************************/
//Declaración de Clases/Objetos

/*****************************************************************************
  	Función:
	Parámetros:
	Devuelve:
............................................................................*/



/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	const int NUM_EQUIPOS = 5;
	char liga[NUM_EQUIPOS][NUM_EQUIPOS];


	//Entradas
	
	cout<<"Liga de 5 equipos:"<<endl;
	cout<<"\t1: Gana el equipo local"<<endl;
	cout<<"\t2: Gana el equipo visitante"<<endl;
	cout<<"\tX: Empate"<<endl;
	cout<<"................................"<<endl;
	cout<<endl;
	
	
	for (int i=0; i<NUM_EQUIPOS; i++)
	{
		for (int j=0; j<NUM_EQUIPOS; j++)
		{
			if(i!=j)
			{
				cout<<"Equipo "<<i+1<<" (local) contra el equipo "<<j+1
					<<" (visitante): ";
				cin>>liga[i][j];
			}
			else
			{
				liga[i][j]=' ';
			}
		}
		
		cout<<endl;
	}
	
	//Muestro el contenido de la matriz
	for (int i=0; i<NUM_EQUIPOS; i++)
	{
		cout<<"\t";
		
		for (int j=0; j<NUM_EQUIPOS; j++)
		{
			cout<<liga[i][j]<<" ";
		}
		
		cout<<endl;
	}

	//Cálculos
	
	int puntuacion[NUM_EQUIPOS];
	
	
	for (int i=0; i<NUM_EQUIPOS; i++)
	{
		puntuacion[i]=0;
		
		for (int j=0; j<NUM_EQUIPOS; j++)
		{
			switch (toupper(liga[i][j]))
			{
				case '1':
					
					puntuacion[i]+=3;
					break;
					
				case 'X':
					puntuacion[i]+=1;
						
			}
		}
	}
	
	cout<<endl;
	
	for (int i=0; i<NUM_EQUIPOS; i++)
	{
		cout<<"Equipo "<<i+1<<": "<<puntuacion[i]<<endl;
	}
	
	int numero_equipo[NUM_EQUIPOS];
	
	for(int i=0; i<NUM_EQUIPOS; i++)
	{
		numero_equipo[i]=i;
	}
	
	for (int izda=0; izda<NUM_EQUIPOS; izda++)
	{
		int mayor=puntuacion[izda];
		int posicion=izda;
		
		for (int j=izda; j<NUM_EQUIPOS; j++)
		{
			if(puntuacion[j]>mayor)
			{
				mayor=puntuacion[j];
				posicion=j;
			}
		}
		
		//Realizo el intercambio en ambos vectores
		
		int aux1=puntuacion[izda];
		int aux2=numero_equipo[izda];
		
		puntuacion[izda]=mayor;
		numero_equipo[izda]=numero_equipo[posicion];
		
		puntuacion[posicion]=aux1;
		numero_equipo[posicion]=aux2;
		
		
	}
	
	cout<<"--------"<<endl;
	
	for (int i=0; i<NUM_EQUIPOS; i++)
	{
		cout<<"Equipo "<<numero_equipo[i]+1<<": "<<puntuacion[i]<<endl;
	}
		
	//Salidas



	return 0;
}


