/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const char TERMINADOR='#';

/****************************************************************************/
//Declaración de tipos de datos

struct Contador
{
	int vocales;
	int consonantes;
	int digitos;
	int otros;
};

/*****************************************************************************
  	Función: 
	Parámetros:
	Devuelve:		
............................................................................*/
Contador Contar(string cadena)
{
	Contador contador;
	
	contador.vocales=0;
	contador.consonantes=0;
	contador.digitos=0;
	contador.otros=0;
	
	char caracter, caracter_may;
	
	
	for(int i=0; i<cadena.length(); i++)
	{
		caracter=cadena.at(i);
		caracter_may=toupper(caracter);
		
		if(caracter!=' ' && caracter!='	')
		{
			
			if(caracter>='0' && caracter<='9')
			{
				contador.digitos++;
			}
			else
			{
				if(caracter_may=='A' || caracter_may=='E' ||
			       caracter_may=='I' || caracter_may=='O' ||
				   caracter_may=='U' )
			    {
			    	
			   		contador.vocales++;
			   		
			    }
			    else
			    {
			    	
			    	if(caracter_may>'A' && caracter_may<='Z')
			    	{
			    		
			    		contador.consonantes++;
			    		
					}
					else
					{
						
						contador.otros++;
						
					}
					
				}
				
			}	
			
		}	
		
	}
	
	return contador;
}

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	string cadena;
	Contador contador;
	
	//Entradas
	
	getline(cin,cadena,TERMINADOR);
	
	//Cálculos
	
	contador=Contar(cadena);
	
	//Salidas
	
	cout<<"Hay "<<setw(3)<<contador.vocales<<" vocales."<<endl;
	cout<<"Hay "<<setw(3)<<contador.consonantes<<" consonantes."<<endl;
	cout<<"Hay "<<setw(3)<<contador.digitos<<" digitos."<<endl;
	cout<<"Hay "<<setw(3)<<contador.otros<<" otros."<<endl;
	
	return 0;
}
