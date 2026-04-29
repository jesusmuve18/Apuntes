/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_39
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En el ejercicio 87 de la Relación de Problemas II planteábamos una 
	heurística sencilla para adivinar un valor aleatorio comprendido ente 
	MIN y MAX basado en la búsqueda dicotómica.
		
	Queremos saber cuántos intentos se realizan para localizar cada uno de los 
	valores comprendidos entre MIN y MAX. Para cada valor i (MIN = i =MAX) 
	calcule y guarde el número de intentos. A continuación muestre una tabla 
	que indique los valores de i y el número de intentos realizados para 
	localizarlo.
	
	Deberá separar los procesos de cálculo y presentación de resultados.
	
	Nota: no necesita generar números aleatorios.
	
	Salidas: Tabla de los números a adivinar y el número de intentos que se 
			 necesitan para adivinar cada uno de ellos
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int MIN=-1000, MAX=1000;

/*****************************************************************************
  	Función: Calcula cuantos intentos se necesitan para calcular la incógnita
  			 con Heureística
	Parámetros: Incógnita a adivinar
	Devuelve: Número de intentos requeridos para acertarla	
............................................................................*/
int IntentosJuego(int incognita)
{
	int intentos=0; //Dato que devolverá la función
	
	int min_tmp=MIN-1; //Cabe la posibilidad de que el minimo sea la incognita
	int max_tmp=MAX+1;	//Cabe la posibilidad de que el maximo sea la incognita
	
	int numero; //Numero que se genera para comparar con la incognita
	
	do
	{
		numero=(int)(((min_tmp+max_tmp)/2)); //generación del número
		
		if(numero!=incognita) //comparo el número con la incógnita
		{
			if(numero>incognita)
			{
				max_tmp=numero;
			}
			else
			{
				min_tmp=numero;
			}
		}
		
		intentos++;
		
	} while(numero!=incognita);

	
	return intentos;
	
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int intentos[MAX-MIN+1]; //array que almacenará el número de intentos
							 //requeridos para acertar cada número
	
	//Cálculos
	
	for(int i=0; i<=(MAX-MIN); i++) 	
	{								
		intentos[i]=IntentosJuego(i+MIN);
	}
	
	//Salidas
	
	cout<<" ------------------------------- "<<endl; //Cabecera de la tabla
	cout<<"|   Incognita   |    Intentos   |"<<endl;
	cout<<"|---------------|---------------|"<<endl;
	
	for(int i=0; i<=(MAX-MIN); i++) 	
	{								
		cout<<"|      "<<(i+MIN)<<"\t|\t"<<intentos[i]<<"\t|"<<endl;
	}
	cout<<" ------------------------------- "<<endl;
	
	
	return 0;
}
