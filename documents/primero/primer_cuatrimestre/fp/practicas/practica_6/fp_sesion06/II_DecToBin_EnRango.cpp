/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Escribir un programa que muestre la representación binaria de un número 
	entero (positivo o cero) expresado en base 10.
	El programa pedirá el número decimal que se va a convertir y calculará su 
	representación binaria sobre un dato string (los bits que conforman la 
	representación binaria del número serán caracteres '0' ó '1').
	
	El programa pedirá en primer lugar el número de bits (llamémosle n) que 
	tendrán los valores binarios con los que se va a trabajar. Dado n los 
	números enteros que se admitirán estarán comprendidos entre 0 y 2^(n - 1)
	(todos ellos pueden representarse estrictamente con n bits o menos).
	
	(NOTA: el número de bits del resultado siempre será n (si fuera necesario 
	se rellenarán con '0' a la izquierda del primer '1')
	
	Entradas: Número en decimal
	
	Salidas: Número en binario (representado con n dígitos)
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int BASE=2;
	
	int decimal;
	string binario;
	int n; //Número de cifras para la representacion
	int contador=2;
	bool decimal_ok, n_ok, datos_ok;
	
	//Entradas
	
	cout<<"Numero en decimal: ";
	cin>>decimal;
	
	cout<<"Numero de cifras para la representacion: ";
	cin>>n;
	
	//Cálculos
	
	decimal_ok=decimal>=0;
	n_ok=n>0;
	datos_ok=( decimal_ok && n_ok);
	
	if(datos_ok)
	{
		if(decimal<=(pow(BASE,n)))
		{
			for(int i=0; i<n; i++)
			{
				binario.push_back('0');
			}
			
			int cociente,resto;
			cociente=decimal;
			
			if(cociente!=1)
			{
				while(cociente!=1)
				{
					cociente=cociente/BASE;
					resto=cociente%BASE;
					binario.at(binario.length()-contador)=(resto+'0');
					contador++;
				}
			}
			else
				binario.at(0)='1';
			
		}
		
		//Salidas
		if(decimal<=(pow(BASE,n)))
			cout<<"La representacion en binario es: "<<binario<<endl;
		else
			cout<<"No hay suficientes bits para su representacion"<<endl;
	}
	else
	{
		if(!decimal_ok)
			cout<<"Numero entero positivo no valido"<<endl;
		else
			cout<<"Numero de cifras para la representacion no valido"<<endl;
	}
		
	
		
	return 0;
}
/*EXPLICACIÓN:
	El programa lee el número en decimal y el número de dígitos, n. Después
	relleno una string llamada binario (que será finalmente el resultado) con
	ceros, ya que esa sería la representación del 0 en binario con n bits.
	
	Después iré siguiendo el método de la división para pasar el número 
	a binario sustituyendo en la cadena algunos 0 por 1 de modo que al final 
	quedará la cadena con el número en binario en su sitio y el resto lleno 
	de ceros como se pide en el enunciado.
	*/
