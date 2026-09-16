/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_85
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
	
	Una vez terminado un juego de adivinación podrá volverse a jugar de nuevo:
	el programa pedirá si queremos volver a jugar o no.
	
	Al terminar (no se quiere jugar más) el programa mostrará cuántos juegos 
	se han realizado (en caso de abandono también se contabiliza ese juego).

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
	bool volver_a_jugar=true;	
	int juegos=0;
	
	
	
	//Entradas y cálculos
	
	while(volver_a_jugar)
	{
		incognita = (rand() % NUM_VALORES) + MIN;
		juegos++;
		cout<<"Intenta adivinar en numero secreto: "<<endl;
		cout<<"Si quieres dejar de jugar pulsa el "<<TERMINADOR<<endl;
		
		do
		{
			cout<<">";
			cin>>numero;
			bool num_ok=(MIN<=numero && numero<=MAX);
			
			if(numero!=TERMINADOR && num_ok)
			{
				if(num_ok)
				{
					if(numero<incognita)
					{
						cout<<"La incognita es mayor"<<endl;
					}
					if(numero>incognita)
					{
						cout<<"La incognita es menor"<<endl;
					}
					if(numero==incognita)
					{
						victoria=true;
					}	
				}
				else
				{
					cout<<"El valor introducido no esta en el rango"<<endl;
				}
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
		
		char respuesta;
		do
		{
			cout<<"Quieres volver a jugar? (S/N): ";
			cin>>respuesta;
			respuesta=toupper(respuesta);
		} while(respuesta!='S' && respuesta !='N');
		
		if(respuesta=='S')
		{
			cout<<"Ha elegido volver a jugar"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<"Juegos realizados: "<<juegos;
			volver_a_jugar=false;
		}	
	}
	
	return 0;
}

