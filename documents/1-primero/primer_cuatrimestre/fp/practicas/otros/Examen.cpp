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
#include<cmath>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

/*****************************************************************************
  	Función: 
	Parámetros:
	Devuelve:		
............................................................................*/
int NumeroCifras (int numero)
{
	int n=numero;
	int cifras=0;
	
	while(n>0)
	{
		n/=10;
		cifras++;
	}
	
	return cifras;
}

int PrimeraMitad (int numero)
{
	int primera_mitad;
	int n_cifras=NumeroCifras(numero);
	
	if(n_cifras%2==0)
	{
		
		primera_mitad=numero/pow(10,((n_cifras/2)));

	}
	else
	{
		primera_mitad=numero/pow(10,(n_cifras/2)+1);
	}
	
	return primera_mitad;
}

int SegundaMitad (int numero)
{
	int segunda_mitad;
	int n_cifras=NumeroCifras(numero);
	int primera_mitad=PrimeraMitad(numero);
	
	
	
	if(n_cifras%2==0)
	{	
		segunda_mitad=numero-(primera_mitad*pow(10,(n_cifras/2)));
	}
	else
	{
		segunda_mitad=numero-(primera_mitad*pow(10,(n_cifras/2)+1));
	}
	
	return segunda_mitad;
}

bool EsDesgarrable (int numero)
{
	int primera_mitad=PrimeraMitad(numero);
	int segunda_mitad=SegundaMitad(numero);
	bool es_desgarrable;
	
	if(pow((primera_mitad+segunda_mitad),2)==numero)
	{
		es_desgarrable=true;
	}
	else
	{
		es_desgarrable=false;
	}
	
	return es_desgarrable;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	int inicio, final;
	int salto;
	
	//Entradas
	
	do
	{
		cout<<"Inicio del intervalo: ";
		cin>>inicio;
		
	} while (inicio<0);
	
	do
	{
		cout<<"Final del intervalo: ";
		cin>>final;
		
	} while (final<0);
	
	
	//Cálculos
	
	if(inicio<final)
	{
		salto=1;
	}
	else
	{
		salto=-1;
	}
	
	for(int i=inicio; i!=final+salto ;i+=salto)
	{
		if(EsDesgarrable(i))
		{
			cout<<i<<" = ("<<PrimeraMitad(i)<<" + "<<SegundaMitad(i)<<")^2 = ("
				<<PrimeraMitad(i)+SegundaMitad(i)<<")^2"<<endl;
		}
	}
	
	//Salidas
	
	
	
	return 0;
}
