/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_45
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Realizar un programa que lea una secuencia de caracteres terminada con el 
	carácter * y la codifique mediante el método RLE. Leed los datos desde un 
	fichero externo, tal y como se explica en la página 26.
	
	Entradas: Caracteres del código ASCII
	
	Salidas: Caracteres codificados mediante RLE.
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const char TERMINADOR='*';
	
	int contador=1;
	char letra, letra_anterior;
	
	
	//Entradas, cálculos y salidas
	
	cin>>letra;
	
	letra_anterior=letra;
	
	while(letra!=TERMINADOR) 
	{
		cin>>letra;
		if(letra_anterior==letra)
		{
			contador++;
		}
		else
		{
			cout<<contador<<" "<<letra_anterior<<"\t";
			contador=1; //Reseteo el contador
		}
		
		letra_anterior=letra;
	}
	
	return 0;
}

/* EXPLICACIÓN DETALLADA:
El programa lee valores hasta pulsar el caracter '*'. Durante las lecturas hay
2 posibilidades:
a) La letra anterior era la misma que la letra actual:
	En ese caso simplemente añado uno al contador ya que es lo que voy 
	guardando para poder decir el número de letras iguales que hay. (ya que 
	el contador esta inicializado con el valor 1 si hubiese una letra que solo 
	apareciese una vez en la serie se guardará el valor 1)
b) La letra anterior no coincide con la letra actual:
	En este caso muestro por pantalla el contador hasta ese momento que indica
	El número de veces que apareció la letra consecutivamente seguido de la 
	letra que se repite mostrando por pantalla el resultado que se pide.
	Tras esto se reinicia el contador para la siguiente letra que ya no es 
	igual que la anterior.
	
	(NOTA: no es necesario controlar la entrada de datos ya que cualquier 
	 caracter es valido)
*/
