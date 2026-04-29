/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_42
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Reescriba la solución del ejercicio 47 de la Relación de Problemas II 
	usando funciones. En dicho ejercicio se pedía leer un número entero 
	positivo y calcular y mostrar su descomposición en factores primos de dos 
	maneras diferentes.
	
	Escriba ahora dos funciones void para calcular y mostrar la descomposición 
	en factores primos de n, con las cabeceras:
	
			void CalculaMuestra_DescFactPrimos_1 (int n);
			void CalculaMuestra_DescFactPrimos_2 (int n);
			
	y otras dos para calcular la descomposición en factores primos de n 
	devolviendo la expresión textual en un string. Las cabeceras serán:
	
			string Calcula_DescFactPrimos_1 (int n);
			string Calcula_DescFactPrimos_2 (int n);

	
	Entradas: Número
	
	Salidas: Divisores primos calculados con 4 funciones diferentes
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
void CalculaMuestra_DescFactPrimos_1 (int numero)
{
	
	//Declaración de Datos
	
	int resto=numero; //Copia para no modificar el dato original
	
	int contador=2; //Posibles divisores del número dado
	
	bool primer_divisor=true; //Indicador para mostrar el signo '*'
							  //a partir del segundo divisor
	
	
	//Cálculos y Salidas
	
	while(resto>1)
	{
		if(resto%contador==0) //Si el contador es un divisor del resto
		{
			if(primer_divisor) //El primer divisor encontrado
			{
				//Cambio el parámetro para ejecutar la condición opuesta el 
				//resto de veces que se ejecute el bucle
				primer_divisor=false;
			}
			else //A partir del segundo divisor encontrado
			{
				cout<<" * "; //Muestro por pantalla el caracter '*'
			}
			
			cout<<contador; //Muestro por pantalla el divisor confirmado
			
			resto/=contador; //Actualizo el valor del resto
			
		} //if(resto%contador==0)
		else
		{
			
			contador++; //No es un divisor del resto por lo que pruebo el 
						//siguiente natural
		}
		
	} //while(resto>1)
	
}//void CalculaMuestra_DescFactPrimos_1 (int numero)

/****************************************************************************/

void CalculaMuestra_DescFactPrimos_2 (int numero)
{
	
	//Declaración de Datos
	
	int resto=numero; //Copia para no modificar el dato original
	
	int contador=2; //Posibles divisores del número dado
	
	bool primer_divisor=true; //Indicador para mostrar el signo '*'
							  //a partir del segundo divisor
							  
	int potencia; //Almacena el numero de veces que se repite un divisor
	
	
	
	//Cálculos y salidas
	
	while(resto>1)
	{
		potencia=0; //Reseteo el valor de la potencia
		
		while(resto%contador==0) //Si el contador es un divisor del resto
		{
			resto/=contador; //Actualizo el valor del resto
			potencia++; //Actualizo el valor de la potencia
			
		}
		
		if(potencia>0) //Si al menos puedo dividir una vez entre el contador
		{
			
			if(primer_divisor) //El primer divisor encontrado
			{
				//Cambio el parámetro para ejecutar la condición opuesta el 
				//resto de veces que se ejecute el bucle
				primer_divisor=false;
			}
			else //A partir del segundo divisor encontrado
			{
				cout<<" * ";
			}
			
			
			//Muestro por pantalla el divisor confirmado
			cout<<contador;
			
			
			//Muestro por pantalla la potencia solo en el caso de que sea 
			//mayor que 1
			if(potencia>1)
			{
				cout<<"^"<<potencia; 
			}
			
			
		} //if(potencia>0)
			
			
		contador++; //No es un divisor del resto por lo que pruebo el 
					//siguiente natural

	} //while(resto>1)
	
} // void CalculaMuestra_DescFactPrimos_2 (int numero)

/****************************************************************************/

string Calcula_DescFactPrimos_1 (int numero)
{
	
	//Declaración de Datos
	
	string divisores=""; //Cadena que devolverá la función
	
	int resto=numero; //Copia para no modificar el dato original
	
	int contador=2; //Posibles divisores del número dado
	
	bool primer_divisor=true; //Indicador para mostrar el signo '*'
							  //a partir del segundo divisor
	
	
	//Cálculos
	
	while(resto>1)
	{
		
		if(resto%contador==0) //Si el contador es un divisor del resto
		{
			
			if(primer_divisor) //El primer divisor encontrado
			{
				//Cambio el parámetro para ejecutar la condición opuesta el 
				//resto de veces que se ejecute el bucle
				primer_divisor=false;
			}
			else //A partir del segundo divisor encontrado
			{
				divisores+=" * "; //Añado al string el caracter '*'
			}
			
			
			//Añado al string el divisor confirmado convertido a texto
			divisores+=to_string(contador); 
			
			
			resto/=contador; //Actualizo el valor del resto
			
		} //if(resto%contador==0)
		else
		{
			
			contador++; //No es un divisor del resto por lo que pruebo el 
						//siguiente natural
						
		}
		
	}//while(resto>1)
	
	
	return divisores; //Salida de la función
	
	
} //string Calcula_DescFactPrimos_1 (int numero)

string Calcula_DescFactPrimos_2 (int numero)
{
	
	//Declaración de Datos
	
	string divisores=""; //Cadena que devolverá la función
	
	int resto=numero; //Copia para no modificar el dato original
	
	int contador=2; //Posibles divisores del número dado
	
	bool primer_divisor=true; //Indicador para mostrar el signo '*'
							  //a partir del segundo divisor
							  
	int potencia; //Almacena el numero de veces que se repite un divisor
	
	
	
	//Cálculos
	
	while(resto>1)
	{
		potencia=0; //Reseteo el valor de la potencia
		
		while(resto%contador==0) //Si el contador es un divisor del resto
		{
			resto/=contador; //Actualizo el valor del resto
			potencia++; //Actualizo el valor de la potencia
			
		}
		
		if(potencia>0) //Si al menos puedo dividir una vez entre el contador
		{
			
			if(primer_divisor) //El primer divisor encontrado
			{
				//Cambio el parámetro para ejecutar la condición opuesta el 
				//resto de veces que se ejecute el bucle
				primer_divisor=false;
			}
			else //A partir del segundo divisor encontrado
			{
				divisores+=" * "; //Añado a la cadena el caracter '*'
			}
			
			
			//Añado a la cadena el divisor confirmado como texto
			divisores+=to_string(contador); 
			
			
			//Añado a la cadena la potencia como texto solo en el caso de que  
			//sea mayor que 1
			if(potencia>1)
			{
				divisores+="^"+to_string(potencia); 
			}
			
			
		} //if(potencia>0)
			
			
		contador++; //No es un divisor del resto por lo que pruebo el 
					//siguiente natural

	} //while(resto>1)
	
	
	return divisores;	//Salida de la función
	
}//string Calcula_DescFactPrimos_2 (int numero)

/****************************************************************************/

int main()
{
	
	//Declaración de datos
	
	int numero;
	
	
	//Entradas
	
	do //Filtro del entero "numero" que deberá ser >0
	{
		cout<<"Numero: ";
		cin>>numero;
	} while (numero<=0);
	
	
	
	//Salidas
	
	cout<<endl;
	
		//Pruebo las funciones de tipo void
	cout<<"Divisores primos ( void 1 ): ";
	CalculaMuestra_DescFactPrimos_1(numero);
	
	cout<<endl;
	
	cout<<"Divisores primos ( void 2 ): ";
	CalculaMuestra_DescFactPrimos_2(numero);
	
	
	cout<<endl;
	cout<<endl;
	
	
		//Pruebo las funciones de tipo string y las muestro por pantalla
	cout<<"Divisores primos (string 1): ";
	cout<<Calcula_DescFactPrimos_1(numero)<<endl;
	cout<<"Divisores primos (string 2): ";
	cout<<Calcula_DescFactPrimos_2(numero)<<endl;
		
	
	return 0;
}
