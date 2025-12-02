/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_9
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un entero representando la clave y un carácter en mayúcula. El 
	programa codificará el carácter según la clave introducida y lo mostrará 
	por pantalla. (Funciona para todos los caracteres a excepción de la 'Ñ')
	
	Entradas:  Clave y caracter a codificar
	
	Salidas:	Caracter codificado
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos

	const int INTERVALO=('Z'-'A'+1);//le sumo 1 ya que es un intervalo cerrado
		
	int clave;
	char c_inicial,c_cod;  //Caracter a codificar y codificado respectivamente
	
	
	//Entradas
	
	cout<<"Clave: ";
	cin>>clave;
	cout<<"Caracter a codificar: ";
	cin>>c_inicial;
	
	//Cálculos
	
	if('A'<=c_inicial && c_inicial<='Z') //Compruebo q el caracter sea válido
	{
		c_cod=c_inicial+clave;
		
		if (c_cod<'A' || 'Z'<c_cod)   //Si el caracter codificado no pertenece 
		{							  //al intervalo
			c_cod=c_cod-(INTERVALO*((int)(clave/INTERVALO)));
		}
	}
	
	/* El caracter codificado será el caracter inicial desplazado menos 
	tantas veces el valor del intervalo como está la clave en el intervalo
	
	Explicación más detallada:
	Supongamos que el intervalo es un numero cualquiera i:
	El caracter ya desplazado n posiciones lo denominaré x.
	Si x está en el intervalo será directamente la solución.
	De lo contrario le restaré i tantas veces como la clave contenga al 
	intervalo (n/i) (el entero de esta operación). 
	*/
	
		
	//Salidas
	
	if('A'<=c_inicial && c_inicial<='Z')
	{
		cout<<"Caracter codificado: "<<c_cod;	
	}
	else
		cout<<"El caracter introducido no es valido.";
	
	
	return 0;
}
