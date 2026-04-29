/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_86
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Vamos a seguir trabajando sobre el problema de la adivinación de un 
	número que planteamos en el ejercicio 49 y ampliamos en el ejercicio 85. 
	En ambos ejercicios se requiere la actuación de un usuario.
	
 	En este caso el programa se ejecutará sin intervención de un usuario.
  	Cada juego de adivinación consiste en: 1) generar un valor aleatorio a
	adivinar (entre MIN y MAX) y 2) generar números aleatorios en el mismo 
	rango, hasta que el valor generado coincida con el valor buscado. Cada vez 
	que se genera un valor y se compara con el valor buscado se produce una 
	jugada. Para cada juego nos interesa conocer el número de jugadas.

	Salidas: Media de intentos por partida
	
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
	
	const int NUMERO_JUEGOS=100;
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
	int juegos=0;
	int intentos, intentos_totales=0;
	double media;
	

	
	//Cálculos y salidas
	
	while(juegos<NUMERO_JUEGOS)
	{
		juegos++;
		intentos=0;
		cout<<"\tJuego "<<juegos<<": "<<endl;
		
		incognita = (rand() % NUM_VALORES) + MIN;
		cout<<"Incognita: "<<incognita<<endl;
		cout<<"Intentos aleatorios: ";
		
		do
		{
			numero=(rand() % NUM_VALORES) + MIN;
			cout<<numero<<" ";
			intentos++;
			
		} while(numero!=incognita);
		
		cout<<endl;
		cout<<"Intentos necesarios: "<<intentos<<endl;
		cout<<endl;
		intentos_totales+=intentos;
	}
	
	//Salidas
	
	media=(((double)intentos_totales)/NUMERO_JUEGOS);
	cout<<"La media es de "<<media<<" intentos"<<endl;
	
	return 0;
}

