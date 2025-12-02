/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_19
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
int CifrasDecimales(double numero)
{
	int int_parte_decimal, cifras=0;
	double dec_parte_decimal=numero;
	
	while(dec_parte_decimal!=(Trunca(dec_parte_decimal,1)))
	{
		dec_parte_decimal*=BASE;
		cifras++;
		int_parte_decimal=(int)dec_parte_decimal;
		dec_parte_decimal=dec_parte_decimal-int_parte_decimal;
	}
	cifras++;
	return cifras;
}
/****************************************************************************/
int CifrasEnteras (int entero)
{
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
	int cifras_int, cifras_dec, huecos_dec;

	
	parte_entera=(int)real;
	parte_decimal=real-parte_entera;
	
	str_int=ToStringInt(parte_entera,num_casillas,relleno);
	
	if(parte_decimal>0)
	{
		cifras_dec=CifrasDecimales(real);
	}
	
	huecos_dec=decimales-cifras_dec;
	
	if(huecos_dec<0)
	{
		parte_decimal=Redondea(parte_decimal,(-huecos_dec));
		cifras_dec=CifrasDecimales(parte_decimal);
	}
	
	parte_decimal_int=parte_decimal*pow(BASE,cifras_dec);
	str_dec=ToStringInt(parte_decimal_int,cifras_dec,relleno);
	
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
	
	//Entradas
	
	cout<<"Real: ";
	cin>>real;
	cout<<"Numero de casillas parte entera: ";
	cin>>num_casillas;
	cout<<"Numero de casillas parte decumal: ";
	cin>>decimales;
	cout<<"Relleno: ";
	cin.ignore(); //vacío el buffer
	relleno=getchar();
	
	cout<<ToStringDouble(real, num_casillas,decimales, relleno);
	
	//Cálculos
	
		
	
	//Salidas
	
	
	
	return 0;
}
