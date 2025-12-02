/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_19
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir dos funciones para convertir un número a su representación 
	textual en forma de string. Estas funciones proporcionan una mejora 
	sobre la función to_string de C++11. Recuerde, por ejemplo, que 
	to_string(23.51) devuelve la cadena 23.510000 mientras que 
	to_string(23) devuelve la cadena 23. La función	to_string tiene algunos 
	inconvenientes:
		a) Para números reales siempre proporciona seis cifras decimales, 
		redondeando la última cifra decimal.
		b) No suprime los ceros finales en la parte decimal de la cadena de 
		caracteres.
		c) No permite especificar el número de casillas que se desean para la 
		parte entera y decimal.
	Las funciones que se piden en este ejercicio resuelven estos problemas.
	string ToStringInt (int entero, int num_casillas, char relleno);
	La función convierte el dato entero a un string con num_casillas casillas 
	que contiene el valor textual de entero. Las casillas no ocupadas por
	los dígitos de entero (si las hubiera) se rellenan con el carácter relleno.
	El número quedará alineado a la derecha (los huecos, a la izquierda).
	Con detalle: si el número de casillas solicitado (num_casillas) es mayor
	que el que se necesita para representar el valor de entero sin pérdidas, se
	rellenan las casillas iniciales del string con el carácter relleno 
	(normalmente será el espacio -ASCII 32-). Si el valor de entero tuviera 
	"per se" más de num_casillas la cadena devuelta tendrá todas las casillas 
	que	se necesite (aunque su número sea mayor que num_casillas) para no
	alterar el valor.
	
	Veamos unos ejemplos. Tenga en cuenta que los caracteres | se muestran
	para ayudar a visualizar el resultado, pero no forman parte de él:
			ToStringInt (15, 6, ’ ’) devuelve la cadena | 15|
			ToStringInt (15, 6, ’#’) devuelve la cadena |####15|
			ToStringInt (155, 2, ’ ’) devuelve la cadena |155|
	
	string ToStringDouble (double real, int num_casillas,
	int decimales, char relleno);
	Convierte el dato real a un string que contiene su valor textual de acuerdo
	con el formato indicado según los valores de num_casillas (número total de 
	casillas que tendrá el resultado), decimales (número de casillas para la 
	parte decimal del resultado) y relleno (carácter que se usará para las 
	casillas no ocupadas). El número quedará alineado a la derecha (los
	huecos, si los hubiera, a la izquierda).
	
		• Si el número de casillas solicitado (num_casillas) es mayor que el
		  que se necesita realmente, se completa el inicio de la parte entera 
		  con el carácter relleno (normalmente será el espacio -ASCII 32-).
		• La cadena resultante siempre tendrá decimales dígitos en la parte
		  decimal. Si tiene que quedarse con menos de los que tuviera real 
		  redondeará la última cifra decimal.
	a) Si decimales es cero no se considera el punto ni la parte decimal, lo 
	que quiere decir que obtendremos un número entero con (num_casillas) 
	casillas.
	
	b) Si decimales es mayor que el que se necesita la parte decimal se
	completa con ceros.
	
	Veamos unos ejemplos. Tenga en cuenta que los caracteres | se muestran
	para ayudar a visualizar el resultado, pero no forman parte de él:
			ToStringDouble (3.49, 3, 1, ’ ’) devuelve |3.5|
			ToStringDouble (3.49, 4, 2, ’ ’) devuelve |3.49|
			ToStringDouble (3.496, 4, 2, ’ ’) devuelve |3.50|
			ToStringDouble (3.496, 4, 1, ’ ’) devuelve | 3.5|
			ToStringDouble (3.496, 4, 1, ’*’) devuelve |*3.5|
			ToStringDouble (333.9999999, 1, 0, ’ ’) devuelve |334|
			ToStringDouble (3, 4, 0, ’ ’) devuelve | 3|
			ToStringDouble (3.0, 1, 0, ’ ’) devuelve |3|
			ToStringDouble (3.0, 5, 2, ’*’) devuelve |*3.00|
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;

const int BASE=10;
/****************************************************************************/
double Trunca(double n, int cifras) //Función para truncar
{
	double r_trunc=((int)(n*(pow(BASE, cifras))))/((pow(BASE, cifras)));
	return r_trunc;
}
/****************************************************************************/
double Redondea(double n, int cifras) //Funcion para redondear
{
	int ult_cifra= ((Trunca(n,(cifras+1))-Trunca(n,cifras))
					*(pow(BASE, (cifras+1))));
	if(ult_cifra>=5)
	{
		n+=(1/(pow(BASE, cifras)));
	}
	double redondeo=Trunca(n,cifras);
	
	return redondeo;
}
/****************************************************************************/
int CifrasDecimales(double numero) 	//Función para saber el número de cifras
{									//decimales
	double aux=numero, resto=2;
	int cifras=0;
	int int_aux;

	
	while(resto>0 && resto!=1)
	{
		aux*=BASE;
		int_aux=Trunca(aux,0);
		resto=Trunca(aux,1)-int_aux; //Los trunco para evitar fallos de 
		cifras++;					 //precisión
	}
	return cifras;
}
/****************************************************************************/
int CifrasEnteras (int entero)		//Función para saber el número de cifras
{									//enteras
	int cifras=0, entero_aux=entero;
	while(entero_aux>0)
	{
		entero_aux/=BASE;
		cifras++;
	}
	return cifras;
}
/****************************************************************************/
string ToStringInt (int entero, int num_casillas, char relleno)
{
	int cifras=0, entero_aux=entero, huecos;
	string str_entero="";
	
	cifras=CifrasEnteras(entero);
	huecos=num_casillas-cifras;
	
	if(huecos>0)
	{
		for(int i=0; i<huecos; i++)
		{
			str_entero+=relleno;
		}
	}
	
	entero_aux=entero;
	int cifra, posicion;
	
	for(int i=0; i<cifras; i++)
	{
		posicion=cifras-i-1;
		cifra=(entero_aux/pow(BASE,posicion));
		str_entero+=(char)(cifra+'0');
		entero_aux-=cifra*pow(BASE,posicion);
	}
	
	return str_entero;
}
/****************************************************************************/
string ToStringDouble (double real, int num_casillas,
												int decimales, char relleno)
{
	string str_int="", str_dec="", str_real="";
	
	int parte_entera, parte_decimal_int, parte_decimal_redondeada;
	double parte_decimal;
	int cifras_int, cifras_dec=0, huecos_dec;

	
	parte_entera=(int)real;
	parte_decimal=real-parte_entera;
	
	str_int=ToStringInt(parte_entera,(num_casillas-decimales-1),relleno);
	//le resto 1 a las casillas por el caracter '.'
	
	if(parte_decimal>0) //Hay parte decimal
	{
		cifras_dec=CifrasDecimales(real);
	}
	
	huecos_dec=decimales-cifras_dec;
	
	if(huecos_dec<0) //Hay más cifras decimales en el número de las requeridas
	{
		cifras_dec=CifrasDecimales(Redondea(real,(cifras_dec+huecos_dec)));
	}
	
	parte_decimal_int=parte_decimal*pow(BASE,cifras_dec);
	str_dec=ToStringInt(parte_decimal_int,cifras_dec,relleno);
	
	while(huecos_dec>0)
	{
		str_dec+=relleno;
		huecos_dec--;
	}
	
	str_real=str_int+"."+str_dec;
	
	return str_real;
}
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int num_casillas, decimales;
	double real;
	char relleno;
	
	//Prueba de la función ToStringDouble
	
	cout<<"Real: ";
	cin>>real;
	cout<<"Numero de casillas totales: ";
	cin>>num_casillas;
	cout<<"Numero de casillas parte decimal: ";
	cin>>decimales;
	cout<<"Relleno: ";
	cin.ignore(); //vacío el buffer (para incluir caracteres como ' ')
	relleno=getchar();
	
	cout<<ToStringDouble(real, num_casillas,decimales, relleno);
	
	
	
	return 0;
}
