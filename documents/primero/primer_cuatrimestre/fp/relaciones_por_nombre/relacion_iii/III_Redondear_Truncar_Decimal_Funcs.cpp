/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_13
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	En cmath está definida la función round que permite redondear un real al 
	entero más próximo. Por ejemplo: round(3.6) devuelve 4, round(3.5) 
	devuelve 4, round(3.1) devuelve 3 y round(3.49) devuelve 3.
	
	Nuestro interés es hacer algo similar pero para permitir el redondeo de la 
	parte decimal, especificando el número de cifras decimales que nos 
	interese. Por ejemplo:
	
		• El resultado de redondear 3.49 a la primera cifra decimal es 3.5
		• El resultado de redondear 3.49 a la segunda cifra decimal es 3.49
		• El resultado de redondear 3.496 a la segunda cifra decimal es 3.5
		
	Defina la función Redondea para calcular un número real aproximado a un 
	número de cifras decimales dado.
	
	En el ejercicio 36 se pedía truncar la parte decimal de un número real 
	usando un número dado de cifras decimales. Por ejemplo:
	
		• El resultado de truncar 3.49 a una cifra decimal es 3.4
		• El resultado de truncar 3.496 a la segunda cifra decimal es 3.49
		• El resultado de truncar 3.496 sin decimales es 3 (3.0)
		
	Defina la función Trunca para truncar un número real usando un número de 
	cifras decimales dado.
	
	En la implementación de estas funciones, necesitará calcular potencias 
	enteras (potencias de 10 en este caso). Para ello, use la función Potencia 
	del ejercicio 2.

	
	Entradas: Número real y cifra a la que redondear
	
	Salidas: Número redondeado a la cifra-ésima cifra
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;

#define VALORES_VARIABLES				//Activar o desactivar para distintas 
//#define VALORES_FIJOS					//pruebas de la funcion

const int BASE=10;	//Para desplazar la coma multiplicando o dividiendo 

/****************************************************************************/

double Potencia(int base, int exponente)	//Función para calcular potencias
{											//La defino double para obtener
	int potencia=1;							//resultados reales al operar con 
	for(int i=0; i<exponente; i++)			//ella
	{
		potencia*=base;
	}
	return potencia;
}

double Trunca(double n, int cifras) //Función para truncar
{
	double r_trunc=((int)(n*(Potencia(BASE, cifras))))
					/((Potencia(BASE, cifras)));
	return r_trunc;
}

double Redondea(double n, int cifras)
{
	int cifra_significativa= ((Trunca(n,(cifras+1))-Trunca(n,cifras))
								*(Potencia(BASE, (cifras+1))));
					
	/*Esta cifra es la que indica en el redondeo a la i-ésima posición 
	  decimal si debe o no sumarse uno a la cifra q se está redondeando, es 
	  decir a la (i-1)-ésima cifra decimal. Por ejemplo: 
	  
	  La cifra que indica el redondeo a la 2ª cifra decimal del 3.4472 es el 7.
	  Para obtener dicho número trunco el número dado a la 3ª cifra decimal
	  (a la dada +1) y también calculo el número truncado a la cifra que se 
	  pide redondear. Obtengo:
	  			primer número: 3.447
	  			segundo número: 3.44
	  Si ahora desplazo la coma 3 posiciones (al número de cifras decimales
	  que se pide redondear +1), es decir multiplicar por 10^3 obtendría.
	  			primer número: 3447
	  			segundo número: 3440
	  La cifra que busco es la diferencia entre el primero y el segundo 
	  número:
	  			cifra_significativa: 3447-3440= 7
	    
	Si dicha cifra es mayor o igual que 5 le sumo uno a la cifra anterior*/
	
	
	if(cifra_significativa>=5)
	{
		n+=(1.0/(Potencia(BASE, cifras)));
	}
	
	//Finalmente trunco el numero redondeado a la cifra dada
	double redondeo=Trunca(n,cifras);
	
	return redondeo;
}

/****************************************************************************/
int main()
{
	#ifdef VALORES_VARIABLES
	
	//Programa para probar la función con valores variables
	
	//Declaración de datos
	
	double numero, resultado;
	int cifras;
		
		
	//Entradas
	
	cout<<"Numero real: ";
	cin>>numero;
	
	do //Filtro el número de cifras a las que redondear (tienen que ser >=0)
	{
		cout<<"Cifras decimales a las que redondear: ";
		cin>>cifras;	
	} while (cifras<0);
	
	
	//Cálculos
	
	resultado=Redondea(numero, cifras);
	
	
	//Salidas
	
	cout<<endl;
	cout<<"El numero "<<numero<<" redondeado a la cifra decimal "<<cifras
		<<" es "<<resultado;
	
	#endif

	//........................................................................
	
	#ifdef VALORES_FIJOS
	
	//Programa para probar la función con valores fijos
	cout<<Redondea(3.141592,4)<<endl;
	
	#endif
	return 0;
}
