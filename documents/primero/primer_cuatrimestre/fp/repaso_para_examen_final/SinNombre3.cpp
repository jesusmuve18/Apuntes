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
	
	const int MAX=100;
	
	int enteros[]={7,2,1,45,67,2,12,56,84,58,51,2,5,3,6,0,25,14,8,78};
	int total_utilizados=((sizeof enteros)/(sizeof (int)));

	int enteros1[MAX];
	int enteros2[MAX];
	int enteros3[MAX];
	
	/***********************************************************************/
	//Creo las copias del array de enteros
	
	for(int i=0; i<total_utilizados; i++)
	{
		enteros1[i]=enteros[i];
	}
	
	for(int i=0; i<total_utilizados; i++)
	{
		enteros2[i]=enteros[i];
	}
		
	for(int i=0; i<total_utilizados; i++)
	{
		enteros3[i]=enteros[i];
	}
	
	
	
	cout<<"Secuencia generada:       ";
	for(int i=0; i<total_utilizados; i++)
	{
		if(i!=0)
			cout<<",";
		
		cout<<enteros[i];
	}
	
	cout<<endl;
	
	/***********************************************************************/
	//Orden por seleccion
	
	for(int izda=0; izda<total_utilizados; izda++)
	{
		int menor=enteros1[izda];
		int pos=izda;
		
		for(int i=izda; i<total_utilizados; i++)
		{
			if(enteros1[i]<menor)
			{
				menor=enteros1[i];
				pos=i;
			}
		}
		
		//Realizo el intercambio
		enteros1[pos]=enteros1[izda];
		enteros1[izda]=menor;
	}
	
	//Muestro el resultado de la ordenacion
	
	cout<<"Ordenacion por seleccion: ";
	
	for(int i=0; i<total_utilizados; i++)
	{
		if(i!=0)
			cout<<",";
			
		cout<<enteros1[i];
	}
	
	cout<<endl;
	
	/***********************************************************************/
	//Ordenacion por insercion
	
	for(int izda=1; izda<total_utilizados; izda++)
	{
		int valor=enteros2[izda];
		int i=izda;
		
		while((valor<enteros2[i-1])&&(i>0))
		{
			enteros2[i]=enteros2[i-1];
			i--;
		}
		
		enteros2[i]=valor;
	}

	//Muestro el resultado de la ordenacion
	
	cout<<"Ordenacion por insercion: ";
	
	for(int i=0; i<total_utilizados; i++)
	{
		if(i!=0)
			cout<<",";
			
		cout<<enteros2[i];
	}
	
	cout<<endl;
	
	/***********************************************************************/
	//Ordenacion por intercambio directo (bubble sort)
	
	bool cambio=true;
	
	for(int izda=0; izda<total_utilizados && cambio; izda++)
	{
		cambio=false;
		
		for(int dcha=total_utilizados-1; dcha>izda; dcha--)
		{
			if(enteros3[dcha]<enteros3[dcha-1])
			{
				int aux=enteros3[dcha];
				enteros3[dcha]=enteros3[dcha-1];
				enteros3[dcha-1]=aux;
				
				cambio=true;
			}
		}
	}
	
	//Muestro el resultado de la ordenacion
	
	cout<<"Ordenacion burbuja:       ";
	
	for(int i=0; i<total_utilizados; i++)
	{
		if(i!=0)
			cout<<",";
			
		cout<<enteros3[i];
	}
	
	
	return 0;
}


