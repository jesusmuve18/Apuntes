/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_49
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Diseñar un programa para jugar a adivinar un número. El programa generará 
	un número aleatorio (entre MIN y MAX) y el usuario tendrá que adivinarlo. 
	En cada jugada el jugador introducirá un valor y el el juego indicará si 
	el número introducido por el jugador está por encima o por debajo del 
	número a adivinar.
	
	Como reglas de parada considerad que el usuario: 1) haya acertado, ó 2) 
	no quiera seguir jugando, y en este caso usad un valor especial 
	(TERMINADOR) para abandonar.

	Entradas: Números (intentando adivinar el número secreto)
	
	Salidas: -Si el número introducido es mayor o menor que la incógnita
			 -Si se ha ganado o no
	
*/
/****************************************************************************/
#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int TERMINADOR=0;	
	const int MIN = 1;
	const int MAX = 100;
	const int NUM_VALORES = MAX-MIN + 1; // Núm. de valores posibles
	
	time_t t;
	srand(time(&t)); 	// Inicializa generador de núms. aleatorios
						// con el reloj del sistema (hora actual)
						
	int incognita; 	// Número aleatorio que se genera
	int numero;		// Número que se introduce para adivinar la incógnita
	
	bool victoria=false;	//Indica si se ha adivinado la incógnita	
	
	incognita = (rand() % NUM_VALORES) + MIN;
	
	//Entradas y cálculos
	
	cout<<"Intenta adivinar en numero secreto: "<<endl;
	cout<<"Si quieres dejar de jugar pulsa el "<<TERMINADOR<<endl;
	
	do
	{
		cout<<">";
		cin>>numero;
		if(numero!=TERMINADOR)
		{
			if(numero<incognita)
			{
				cout<<"La incognita es mayor"<<endl;
			}
			if(numero>incognita)
			{
				cout<<"La incognita es menor"<<endl;
			}
		}
		if(numero==incognita)
		{
			victoria=true;
		}
		
		cout<<endl;
		
	} while(numero!=incognita && numero!=TERMINADOR);
			
	
	//Salidas
	
	if(victoria==true)
	{
		cout<<"Enhorabuena has acertado la incognita"<<endl;
	}
	else
	{
		cout<<"Vaya, parece que te has rendido"<<endl;
	}
	
	
	
	return 0;
}
/* EXPLICACIÓN:
	Una vez generado el número aleatorio se van pidiendo números. Entre cada 
	lectura de datos el programa compara el número introducido con el número
	oculto e indica si es mayor o menor. 
	Hay 2 maneras de salir de este bucle:
	-Pulsando la tecla TERMINADOR. En este caso el juego indicará que el 
	 jugador se ha rendido.
	-Adivinando el número secreto. En este caso el juego indicará la victoria.
	 
*/
