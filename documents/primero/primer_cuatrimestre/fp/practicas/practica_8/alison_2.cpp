#include <iostream>	// Inclusión de los recursos E/S
#include <string>
#include <cmath>
using namespace std;

int LeeEntero (string titulo)
{
	string valor;
	const char MIN='0', MAX='9';
	
	bool valor_entero ;
	
	do
	{
		
		cout<<titulo<<endl;
		cin>>valor;	
		
		
		for (int i=0; i<valor.length(); i++)
		{	
			valor_entero= true;
			//Si el valor está fuera del intervalo de números 0-9 (ascii)
			
			if ( (valor.at(i) < MIN) || (valor.at(i) > MAX) )
			{
				valor_entero=false;
			}
			
			//Si comienza con un signo menos
			
			else if(valor.at(0) == '-')
			{
				valor_entero=true;
			}
		}
	
	}while (!valor_entero);
	

	return stoi(valor);
}

int LeeEnteroEnRango (string titulo, int menor, int mayor)
{
	double valor;	
	
	do
	{
		valor = LeeEntero (titulo);
		
	}while((valor < menor) || (valor > mayor));
	
	
	
	return valor;	
}
int LeeEnteroMayorOIgual (string titulo, int referencia)
{
	double valor;
	
	do
	{
		valor = LeeEntero (titulo);
		
	}while(valor < referencia);

	return valor;
}


int main()	// Programa principal
{
	cout << "---------------------------------------"<<endl;
	
	LeeEntero("Introduzca un entero: ");
	LeeEnteroEnRango("Entero entre 1 y 25: ", 1, 25);
	LeeEnteroMayorOIgual("Entero mayor o igual a 7: ", 7);
	
	
	
	return 0;
}
