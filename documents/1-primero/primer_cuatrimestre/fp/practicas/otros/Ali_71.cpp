/****************************************************************/
/*  FUNDAMENTOS DE PROGRAMACIÓN					EJERCICIO-

	
	ALISON CARVAJAL PAULINO
	
	1º A2 DOBLE GRADO EN INGENIERÍA INFORMÁTICA Y MATEMÁTICAS 
	
	Este programa 

	ENTRADAS: 
	SALIDAS:  
*/
/****************************************************************/

#include <iostream>	// Inclusión de los recursos E/S
using namespace std;

bool EsPerfecto (int n)
{
	bool es_perfecto;
	int div;
	int suma=0;
	
	
	for(int i=1; i<n; i++)
	{
		if(n%i==0)
		{
			div=i;
			suma+=div;
		}
	}
	
	if(suma==n)
	{
		es_perfecto=true;
	}
	else 
	{
		es_perfecto=false;
	}
	
	return es_perfecto;
}



int main()	// Programa principal
{
	int n;
	int mayor_perfecto;
	
	cout<<"Num: ";
	cin>>n;
	
	for(int i=0; i<=n ; i++)
	{
		if(EsPerfecto(i))
		{
			mayor_perfecto=i;
		}
	}
	
	cout<<"Mayor Numero Perfecto: "<<mayor_perfecto<<endl;
	return 0;
}
