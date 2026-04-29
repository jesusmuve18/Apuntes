/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_36
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Lee un número real r y un número entero n y	trunqua r a tantas cifras 
	decimales como indique n. El resultado se almacena en una variable 
	diferente.
	
	Entradas:  r y n (Comprobando q ambos valores son numeros)
	
	Salidas:	r truncado
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<iomanip>
#include<string.h>
using namespace std;
/****************************************************************************/
int main()
{
	cout.setf(ios::fixed);
	
	//Declaración de datos
	
	long double r,r_trunc;
	
	string s_entera;
	int p_entera;
	int cifras; //Numero de cifras de la parte entera de r
	
	int n;
	
	bool datos_ok;
	
	//Entradas
	
	cout<<"Numero real: ";
	cin>>r;
	cout<<"Numero de cifras decimales: ";
	cin>>n;
	
	//Cálculos
	
	datos_ok=((r=(double)r)&&(n=(int)n)&&(n>0));
	
	/*En primer lugar multiplico r por 10 elevado a n para desplazar la coma 
	tantos lugares a la derecha como indique n. De este modo tendré todas 
	las cifras que quiero conservar en la parte entera del número.
	Desecho las cifras decimales con el casting (int)<valor> y vuelvo a 
	desplazar la coma n posiciones a la izquierda dividiendo entre 10 elevado 
	a n.*/
	
	if(datos_ok) {
		r_trunc=((int)(r*pow(10.0,n)))/(pow(10.0,n));
		p_entera=r;
		s_entera=to_string(p_entera);
		cifras=s_entera.length();
	}
	
	/* La siguiente operación que haré será para mostrar por pantalla de la 
	forma más óptima el resultado. Para ello usaré las funciones setw()
	y setprecision().
	
	El argumento de la función setw() será el resultado de sumar el número
	de cifras de la parte entera más el punto más el número de cifras de la 
	parte decimal. Estos 2 últimos valores los conozco ya q el punto ocupa 
	1 espacio y la parte decimal ocupa n espacios. 
	Para calcular el número de cifras enteras convertiré el número real en 
	entero asignándolo a un int. Después convertiré ese número en una variable
	de tipo string y contaré el número de caracteres.
	
	El argumento de la función setprecision() será el propio n.
	*/
		 
	
	//Salidas
	
	if (datos_ok)
	{
		cout<<endl;
		cout<<"Numero truncado: "<<setw(cifras+n+1)<<setprecision(n)<<r_trunc;
	}
	else
	{
		cout<<endl<<endl;
		cout<<"No has introducido unos valores validos";
	}
		
	return 0;
}
