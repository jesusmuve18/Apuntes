/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_9
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Declarar dos array de datos int con TOPE casillas. Rellenar cada array de 
	la siguiente manera: pedir que se introduzca por teclado una serie 
	indeterminada de números enteros (termina cuando se introduce la palabra 
	FIN).
	
	En primer lugar, el programa mostrará el valor medio de los valores 
	introducidos.
	
	Después, indicará si los dos array contienen los mismos valores y en el 
	mismo orden
	
	Entradas: Datos de cada array
	
	Salidas: La media y si son iguales o no
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;

#define ETIQUETAS //para mostrar las peticiones de datos
/****************************************************************************/
//Declaración de constantes globales

const int TOPE=100;
const string TERMINADOR="FIN";

/****************************************************************************/
/* 	Función: Lee un posible valor de tipo entero
	Parámetros: ninguno
	Devuelve: o una string de formato entero o la string TERMINADOR			
............................................................................*/
string LeeValor ()
{
	string valor_leido; //Valor que devuelve la función
	bool valor_ok;
	
	do
	{
		getline(cin,valor_leido);
		
		valor_ok=true;
		
		if(valor_leido!="")
		{
			for(int i=0; i<valor_leido.length(); i++)
			{
				if(valor_leido.at(i)>'9' || valor_leido.at(i)<'0')
				{
					valor_ok=false;
				}
			}	
		}
		else
		{
			valor_ok=false;
		}
		
		
	} while(valor_leido!=TERMINADOR && !valor_ok);
	
	return valor_leido;
	
}

/****************************************************************************/
/* 	Función: Calcula la media de los elementos de un array
	Parámetros: array y número de elementos
	Devuelve: media de los elementos										
............................................................................*/
double Media (int array[], int contador)
{
	double media; //Dato que devolverá la funcion
	double sumador=0;
	
	for(int i=0; i<contador; i++)
	{
		sumador+=array[i];
	}
	
	media=sumador/contador;
	
	return media;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int array_1[TOPE]; 
	int array_2[TOPE];
	
	int contador1, contador2;
	
	bool fin; //Para salir de los bucles
	bool iguales=true; //Para comparar array 1 con array 2
	
	//Entradas
	
	fin=false;
	contador1=0;
	
	do
	{
		#ifdef ETIQUETAS
		cout<<"Numero posicion "<<contador1+1<<" del array 1: ";
		#endif
		string auxiliar=LeeValor();
		
		if(auxiliar==TERMINADOR)
		{
			fin=true;
		}
		else
		{
			array_1[contador1]=stoi(auxiliar);
			contador1++;
		}
		

	} while(!fin && contador1<TOPE);
	
	
	cout<<endl;
	cout<<"................................................"<<endl;
	cout<<endl;	
	
	fin=false;
	contador2=0;
	
	do
	{
		#ifdef ETIQUETAS
		cout<<"Numero posicion "<<contador2+1<<" del array 2: ";
		#endif
		string auxiliar=LeeValor();
		
		if(auxiliar==TERMINADOR)
		{
			fin=true;
		}
		else
		{
			array_2[contador2]=stoi(auxiliar);
			contador2++;
		}

	} while(!fin && contador2<TOPE);
	
	cout<<endl;
	cout<<"................................................"<<endl;
	cout<<endl;	
	
	//Cálculos
	
	if(contador1>0)
	{
		cout<<"Media del array 1: "<<Media(array_1, contador1)<<endl;	
	}
	else
	{
		cout<<"No hay suficientes elementos en array 1 para hacer la media"
			<<endl;
	}
	
	
	if(contador2>0)
	{
		cout<<"Media del array 2: "<<Media(array_2, contador2)<<endl;	
	}
	else
	{
		cout<<"No hay suficientes elementos en array 2 para hacer la media"
			<<endl;
	}
	
	
	if(contador1==contador2) //Si tienen el mismo numero de elementos
	{
		for(int i=0; i<contador1; i++) //Recorro las posiciones
		{
			if(array_1[i]!=array_2[i]) //Compruebo si son iguales
			{
				iguales=false;
			}
		}
	}
	else
	{
		iguales=false;
	}
	
	
	if(iguales)
	{
		cout<<"Los array contienen los mismos valores y en el mismo orden"
			<<endl;
	}
	else
	{
		cout<<"Los array no contienen los mismos valores en el mismo orden"
			<<endl;
	}
	
	
	return 0;
}
