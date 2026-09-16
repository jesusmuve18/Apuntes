/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_87
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Mejoraremos la solución autónoma al problema de la adivinación planteado 
	en el ejercicio 86 introduciendo una sencilla heurística para dirigir 
	la solución. Evitaremos generar números aleatorios independientes para 
	encontrar el valor buscado. Utilizaremos un valor inicial tentativo y a 
	partir de él, mediante comparaciones con el valor buscado iremos reduciendo 
	el ámbito de la búsqueda. Se tratará de una versión autónoma del 
	problema planteado originalmente en el ejercicio 49 en el que el programa
	nos daba pistas para encontrar el valor buscado.
	
	Heurística: Se parte del valor medio entre MIN y MAX. Si es el valor 
	buscado, terminamos. Si no, el valor buscado estará en la mitad izquierda 
	(si el valor buscado es menor que el valor medio) o en la mitad derecha 
	(si el valor buscado es mayor que el valor medio). Sabiendo en qué mitad 
	hay que buscar, se vuelve a repetir el proceso en la mitad donde 
	corresponde.
	
	Cada vez que se compara un nuevo valor con el valor buscado se produce una 
	jugada.
	Para cada juego nos interesa conocer el número de jugadas.
	El proceso descrito (cada juego) se repite NUMERO_JUEGOS veces.
	El objetivo del programa es conocer el número medio de jugadas por juego, 
	esto es, cuántos intentos esperamos que haya que hacer (en media) para 
	adivinar el número
	
	Salidas: Media de jugadas,y mínimo máximo de jugadas necesarias
	
*/
/****************************************************************************/
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int NUMERO_JUEGOS=1000;
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
	int min_tmp, max_tmp; //maximos y minimos temporales
	int intentos_max=0, intentos_min; //Se verifica que el maximo de intentos
										//con heureística es el siguiente:
										
	/*  Siendo a el numero de valores posibles, b la distancia entre un posible
		valor y el siguiente (en este caso 1 porque estamos trabajando con 
		enteros) y n el numero de veces que podemos dividir entre 2 sin
		tomar valores menores a b se debe verificar:
		
		(a/2^n)>b => b=1 => a>2^n => (tomando logaritmo) log(a)>nlog(2) =>
		
		=> n<log(a)/log(2) 
		Concluimos que el máximo de intentos es log(a)/log(2) ya que n siempre
		es estrictamente menor. Sin embargo, teniendo en cuenta que este numero 
		puede tener decimales le sumaré 1 para asegurarme de que no haya error.
		(En el caso de trabajar con 101 posibles valores el maximo seria
		
		max=log(101)/log(2)=6.6582  En este caso el número práctico máximo
									es 7 por lo que para que sea mayor estricto
									le sumaré uno y ya tenemos asegurado que 
									nunca vamos a superar el 8)
	*/
	
	intentos_min=(log(NUM_VALORES)/log(2))+1; //De este modo cualquier numero 
											  //de intentos posibles es 
											  //siempre menor que este valor

	
	//Cálculos y salidas
	
	while(juegos<NUMERO_JUEGOS)
	{
		juegos++;
		intentos=0;
		cout<<"\tJuego "<<juegos<<": "<<endl;
		
		incognita = (rand() % NUM_VALORES) + MIN;
		cout<<"Incognita: "<<incognita<<endl;
		cout<<"Intentos con Heureistica: ";
		
		min_tmp=MIN-1; //Cabe la posibilidad de que el minimo sea la incognita
		max_tmp=MAX+1;	//Cabe la posibilidad de que el maximo sea la incognita
		
		do
		{
			numero=(int)(((min_tmp+max_tmp)/2));
			cout<<numero<<" ";
			
			if(numero!=incognita)
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
		
		cout<<endl;
		cout<<"Intentos necesarios: "<<intentos<<endl;
		cout<<endl;
		intentos_totales+=intentos;
		if(intentos>intentos_max)
			intentos_max=intentos;
		if(intentos<intentos_min)
			intentos_min=intentos;
	}
	
	//Salidas
	
	media=(((double)intentos_totales)/NUMERO_JUEGOS);
	cout<<"La media es de "<<media<<" intentos"<<endl;
	cout<<"El maximo de intentos necesarios han sido "<<intentos_max<<endl;
	cout<<"El minimo de intentos necesarios han sido "<<intentos_min<<endl;
	
	return 0;
}

