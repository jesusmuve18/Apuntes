/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_24
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En el ejercicio 47 de la Relación de Problemas II se pedía escribir un 
	programa que lea un número entero positivo y muestre su descomposición en 
	factores primos.
	
	Ahora deberá realizar un programa que lea un número indeterminado de 
	enteros positivos (finalizar cuando se introduce un cero) y exprese cada 
	uno de ellos en base al producto de sus factores primos. Exprese el 
	resultado como el producto de potencias (apartado b del ejercicio antes 
	mencionado). Por ejemplo:
	
							360 = 2^3 * 3^2 * 5
							121 = 11^2
							11 = 11
							
	Para la solución deberá considerar únicamente los factores primos. Para 
	ello deberá calcular (y guardar en un array) los números primos antes de 
	leer ningún número.
	
	Use la Criba de Eratóstenes descrita en el ejercicio 23 de esta misma 
	relación de problemas.
	
	Recomendamos usar un struct del tipo:
	
							struct Pareja {
							int primo;
							int potencia;
							};
							
	para guardar cada pareja primo-potencia. Cada una de estas parejas se 
	guardará en una casilla del array descomposicion:
	const int CASILLAS_DESCOMPOSICION = 50;
	Pareja descomposicion[CASILLAS_DESCOMPOSICION] = {{0,0}};
	int utilizados_descomposicion = 0;

	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int MAXIMO_PRIMO=5000;

/****************************************************************************/
//Declaración de tipos de datos

struct Pareja 
{
	int primo;
	int potencia;
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int criba[MAXIMO_PRIMO];
	int n;
	
	//Entradas
	do
	{
		cout<<"Numero a descomponer: ";	
		cin>>n;
		
	} while(n<=0 || n>MAXIMO_PRIMO);
	
	
	
	
	//Generador del vector primos:
	
	for(int i=2; i<=n; i++) //Escribo todos los numeros entre 2 y n en la criba
	{
		criba[i]=i;
	}
	
	
	int p=2; //Empiezo a buscar a partir del 2
	
	int multiplo;
	int contador;
	
	while(p*p<n)
	{
		if(criba[p]!=0 ) //Si el número no está tachado
		{
			contador=2;
			multiplo=p;
			
			while(multiplo<=n)
			{
				multiplo=p*contador;
				criba[multiplo]=0; //"tacho" el múltiplo de p
				
				contador++;
				
			}	
		}	
		p++;
	}	
	
	//En este punto ya tengo la criba de Eratóstenes generada pero con huecos
	//por lo que trasladaré los datos distintos de 0 al nuevo vector primos
	
	contador=0; //Reutilizaré el contador
	
	for(int i=0; i<n; i++) //Para saber cuantos elementos primos hay
	{
		if(criba[i]!=0)
		{
			contador++;
		}
	}
	
	int cardinal_primos=contador; //Guardo el valor del contador
	
	int primos[cardinal_primos]; //Genero la matriz con el número de elementos 
						  		 //calculados
		
	contador=0; //Reutilizaré el contador
				  
	for(int i=0; i<=n; i++) //Para rellenar el vector primos
	{
		if(criba[i]!=0)
		{
			primos[contador]=criba[i];
			contador++;
		}
	}
	
	
	
	//Ya tengo el vector primos con los números primos del 1 al n
	
	
	Pareja divisores[n];
	int resto=n;
	contador=0; //Reutilizo el contador
	int i=0;
	bool primera_vez=true; //Para entrar al bucle solo la primera vez
	while(resto>1)
	{

		if(resto%primos[i]==0)
		{
			if(primera_vez)
			{
				contador++;
				divisores[contador-1].primo=primos[i];
				divisores[contador-1].potencia=1;
				primera_vez=false;	
			}
			else
			{
				divisores[contador-1].potencia+=1;
			}
			
			resto/=primos[i];

		}
		else
		{
			i++;
			primera_vez=true;
			
		}
	}
			
	
	cout<<n<<" = ";
	
	for(int i=0; i<contador; i++)
	{
		if(i>0) //Si no es la primera vez
		{
			cout<<" * ";
		}
		
		cout<<divisores[i].primo;
		
		if(divisores[i].potencia>1)
		{
			cout<<"^";
			cout<<divisores[i].potencia;	
		}
	}	
	

	return 0;
}
