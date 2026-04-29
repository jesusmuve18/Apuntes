/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_25
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un carácter (en este caso en minúscula), lo pasa a mayúscula y lo 
	imprima en pantalla.
		
	Entradas:	Caracter en minúscula
	
	Salidas:	Caracter en mayúscula
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	/*Definiré el intervalo entre A y a como el intervalo en general entre 
	todas las letras mayúsculas y minúsculas ya que en la tabla ASCII se
	encuentra todo el abecedario en mayúsculas ordenado y de igual manera
	sucede con el abecedario en minúscula manteniendo constante este intervalo
	*/
	
	const int INTERVALO=('A'-'a'); 	//Intervalo que resulta -32 
	char c_min,c_may; 	//Caracter en minúscula y mayúscula respectivamente
	bool datos_ok;
	
	//Entradas
	
	cout<<"Caracter en minuscula: ";
	cin>>c_min;
	
	//Cálculos
	
	datos_ok=((c_min>='a')&&(c_min<='z'));
	
	if(datos_ok)
		c_may=c_min+INTERVALO;		//Desplazo el caracter en la tabla ASCII
	
	//Salidas
	
	cout<<endl;
	
	if(datos_ok)
		cout<<"Caracter en mayuscula: "<<c_may;
	else
		cout<<"Error: no has introducido un caracter valido.";	
	

	return 0;
}
