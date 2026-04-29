/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_22
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	
	Enunciado:
	
	Escriba la función LeeOpcion2Alternativas que realiza las siguientes 
	tareas:
	
		a) imprime en pantalla un mensaje
		b) lee una opción de forma que sólo admita los caracteres ’s’, ’S’, ’n’ 
			ó ’N’
		c) devuelve la opción escogida. Piense: ¿qué debería devolver la 
			función, el carácter leído o un valor bool?.
			
	Aplique esta función en la solución del ejercicio 85 de la Relación de 
	Problemas II (adivinar un número repetidamente) para volver (o no) a jugar. 
	Modularice con funciones cuantas tareas cree oportunas en ese programa.


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
//Declaración de constantes globales

const int TERMINADOR=0;	
const int MIN = 1;
const int MAX = 100;
const int NUM_VALORES = MAX-MIN + 1; // Núm. de valores posibles
/****************************************************************************/
bool GanaPartida (int incognita ) 	//Función que indica si se ha ganado la 
{									//partida
	
	//Declaración de Datos
	
	int numero;	//Intento de adivinar la incógnita
	bool victoria=false; //Dato que devuelve la función
	bool num_ok; //Indica si el número es válido
	
	do	//while(numero!=incognita && numero!=TERMINADOR);
	{

		do  //Filtro de número
		{
			cout<<">";
			cin>>numero;
			num_ok=((MIN<=numero && numero<=MAX)|| numero==TERMINADOR);	
			
		} while (!num_ok);
		
		
		if(numero!=TERMINADOR)
		{
					
			if(numero<incognita)
			{
				cout<<"La incognita es mayor"<<endl;
			}
			else
			{
				
				if(numero>incognita)
				{
					cout<<"La incognita es menor"<<endl;
				}
				else //numero==incognita
				{
					victoria=true;
				}
				
			}
			
		} // if(numero!=TERMINADOR)
		
		cout<<endl;
		
	} while(numero!=incognita && numero!=TERMINADOR);
	
		
	//Salida de la función
	return victoria;	
}
		
/****************************************************************************/

char LeeOpcion2Alternativas (string titulo)
{
	char respuesta; //Dato que devuelve la función
	
	do
	{
		cout<<titulo;
		cin>>respuesta;
		
		respuesta=toupper(respuesta); //Paso a mayúscula para reducir casos
		
	} while(respuesta!='S' && respuesta !='N');
	
	
	//Salida de la función
	return respuesta;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	
	//Declaración de datos

	time_t t;
	srand(time(&t)); 	// Inicializa generador de núms. aleatorios
						// con el reloj del sistema (hora actual)
						
	int incognita; 	// Número aleatorio que se genera
	
	bool victoria=false;	//Indica si se ha adivinado la incógnita
	bool volver_a_jugar=true;	//Indica si se va a volver a jugar
	int juegos=0, abandonos=0;	//Estadísticas finales
	
	
	
	//Entradas y cálculos
	
	while(volver_a_jugar)
	{
		incognita = (rand() % NUM_VALORES) + MIN;
		juegos++;
		cout<<"Intenta adivinar el numero secreto: "<<endl;
		cout<<"Si quieres dejar de jugar pulsa el "<<TERMINADOR<<endl;
		
		victoria=GanaPartida(incognita);
				
		
		//Salidas
		
		if(victoria==true)
		{
			cout<<"Enhorabuena has acertado la incognita"<<endl;
		}
		else
		{
			cout<<"Vaya, parece que te has rendido"<<endl;
			abandonos++;
		}
		
		char respuesta;
		
		respuesta=LeeOpcion2Alternativas("Quieres volver a jugar? (S/N): ");
		
		if(respuesta=='S')
		{
			cout<<"Ha elegido volver a jugar"<<endl;
			cout<<endl;
		}
		else
		{
			cout<<endl;
			cout<<"Juegos realizados: "<<juegos<<endl;
			cout<<"Abandonos: "<<abandonos<<endl;
			
			volver_a_jugar=false; //Finaliza el juego
		}
			
	} //while(volver_a_jugar)
	
	return 0;
}

