/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_17
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	. Escribir las siguientes funciones para leer números enteros:
	
	int LeeEntero (string titulo)
		Lee y devuelve un dato int. Si el valor leido no fuera un int (p.e. 
		94.5, dos, 77 88, 0xA2, 55.0) la función volvería a pedirlo hasta que 
		se proporcione un valor correcto. La lectura se etiqueta con titulo.
		Por ejemplo, si x es int, la ejecución de la instrucción
		x = LeeEntero("Introduzca la coordenada x: "); muestra Introduzca la 
		coordenada x: _ y espera a la introducción de un valor int. Si el valor 
		introducido no fuera int vuelve a pedirlo.
		
	int LeeEnteroEnRango (string titulo, int menor, int mayor)
		Lee y devuelve un dato int en un rango dado. Si el valor leido no 
		fuera int ó éste fuera menor que menor o mayor que mayor la función 
		volvería a pedirlo hasta que se proporcionara un valor correcto. La 
		lectura se etiqueta con titulo.
		
	int LeeEnteroMayorOIgual (string titulo, int referencia)
		Lee y devuelve un dato int que debe ser mayor o igual que referencia.
		Si el valor leido no fuera int ó éste fuera menor que referencia la 
		función volvería a pedirlo hasta que se proporcionara un valor 
		correcto. La lectura se etiqueta con titulo.

*/
/****************************************************************************/
#include<iostream>
#include <string>
using namespace std;

/****************************************************************************/

int LeeEntero (string titulo)
{
	
	//Declaración de Datos 
	
	string n; //Creo una sting para analizar elemento a elemento
	bool es_entero;
	
	//Entrada y Cálculos
	
	do //while(!es_entero);	//Filtro para saber si la serie de caracteres in-
	{						//troducida corresponde con el formato de un entero
	
		cout<<titulo; //Muestro el mensaje en pantalla
		getline(cin,n);	//Introduzco los datos introducidos en una string
		
		es_entero=true;
		
		for(int i=0;i<n.length();i++)
		{
			
			if((n.at(i)<'0') || (n.at(i)>'9')) 	//Si algún caracter no es un 
			{									//número
			
				if (!(i==0 && n.at(i)=='-')) //Puede tener un '-' en la primera
				{							 //posición 
					es_entero=false;
				}	
				
			} //if((n.at(i)<'0') || (n.at(i)>'9'))
			
		} //for(int i=0;i<n.length();i++)
		
	} while(!es_entero);
	
	
	//Salida de la función
	return (stoi(n));
	
}

/****************************************************************************/

int LeeEnteroEnRango (string titulo, int menor, int mayor)
{
	//Declaración de Datos
	
	int num; //Almacena la salida de la función
	
	
	//Cálculos
	
	do //Filtro para que num esté en el rango dado
	{
		num=LeeEntero(titulo); //Leo y compruebo que sea entero
		
	} while(num<menor || num>mayor);
	
	
	//Salida de la función
	return num;
	
}

/****************************************************************************/

int LeeEnteroMayorOIgual (string titulo, int referencia)
{
	//Declaración de Datos
	
	int num; //Almacena la salida de la función
	
	
	//Cálculos
	
	do //Filtro para que num esté por encima de la referencia (>=referencia)
	{
		num=LeeEntero(titulo); //Leo y Compruebo que sea entero
		
	}while(num<referencia);
	
	
	//Salida de la función
	return num;
}


/****************************************************************************/
int main()
{
	
	//Programa para probar cada una de las funciones
	
	//Declaración de datos constantes
	
	const int MAX=100, MIN=0; //Para LeeEnteroEnRango
	const int REF=0; //Para LeeEnteroMayorOIgual
	
	
	
		//Función LeeEntero
	cout<<"Funcion LeeEntero: "<<endl;
	
				//Declaración de Datos
	int numero1=LeeEntero("\tIntroduzca un entero: ");
	
				//Salidas
	cout<<"La funcion LeeEntero devuelve el valor "<<numero1<<endl;
	
	cout<<"------------------------------------------------------------------";
	
		//Función LeeEnteroEnRango
	cout<<endl;
	cout<<"Funcion LeeEnteroEnRango: "<<endl;
	
				//Declaración de Datos
	string peticion=("\tIntroduzca un entero entre "+to_string(MIN)
					+" y "+to_string(MAX)+": ");
	
	int numero2=LeeEnteroEnRango (peticion,MIN,MAX);
	
	
				//Salidas
	cout<<"La funcion LeeEnteroEnRango devuelve el valor "<<numero2<<endl;
	
	cout<<"------------------------------------------------------------------";
	
		//Función LeeEnteroMayorOIgual
	cout<<endl;
	cout<<"Funcion LeeEnteroMayorOIgual: "<<endl;
	
				//Declaración de Datos
	string peticion2=("\tIntroduzca un entero mayor que "+to_string(REF)+": ");
	int numero3=LeeEnteroMayorOIgual (peticion2,REF);
	
				//Salidas
	cout<<"La funcion LeeEnteroMayorOIgual devuelve el valor "<<numero3<<endl;
	
	
	return 0;
}
