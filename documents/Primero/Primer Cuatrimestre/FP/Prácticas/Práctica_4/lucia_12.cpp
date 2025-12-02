#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;
int main()
{
	string s_original;
	
	
	//ENTRADAS:
	
	cout << "Introduzca un numero entero: ";
	cin >> s_original;
	int i_original=stoi(s_original);
	
	//CÁLCULOS:
	
	int n_cifras = s_original.length();
	int const CONVERSOR = pow(10,(n_cifras - 1));	/* Constante que usaremos para
												       formar el nuevo entero */											  	
	
	/* 
	   Dividimos entre el conversor (del tipo 10 elevado a n), para obtener el
	   numero que compone la posición más alta; Ej: nuevo = 498;
	   498/10 elevado a 3 = 4 (cifra de las centenas) 
	*/
	int extremo_izq = i_original / CONVERSOR;
	
	/*
	   Usamos el módulo para hallar las unidades (extremo derecho) del entero
	   original, ya que será el extremo izq del nuevo. Ej: nuevo = 498;
	   498 % 10 = 49, resto = 8: unidades
	*/	
	int extremo_dch = i_original % 10;
	
	int medio = i_original - (extremo_izq * CONVERSOR) - extremo_dch;
	int nuevo = (extremo_izq * CONVERSOR) + medio + extremo_dch;
	
	cout<<"el nuevo número es: "<<nuevo;
}
