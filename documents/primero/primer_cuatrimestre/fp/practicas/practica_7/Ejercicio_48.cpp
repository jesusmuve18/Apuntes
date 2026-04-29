/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_48
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	El algoritmo de la multiplicación rusa es una forma distinta de calcular 
	la multiplicación de dos números enteros n * m. Para ello este algoritmo 
	va multiplicando por 2 el multiplicador m y dividiendo (sin decimales) 
	por dos el multiplicando n hasta que n tome el valor 1 y suma todos 
	aquellos multiplicadores cuyos multiplicandos sean impares. Por ejemplo, 
	para multiplicar 37 y 12 se harían las siguientes iteraciones 
	
			Iteración Multiplicando Multiplicador
				1 			37 			12
				2 			18 			24
				3 			9 			48
				4 			4 			96
				5 			2 			192
				6 			1 			384
				
	El resultado de multiplicar 37 y 12 sería la suma de los multiplicadores 
	correspondientes a los multiplicandos impares (en negrita), es decir 
						12+48+384=444
	Cread un programa que lea dos enteros y calcule su producto con este 
	algoritmo.
	
	Entradas: 2 numeros enteros 
	
	Salidas: su producto con el algoritmo ruso
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int num1,num2; //Numeros a multiplicar
	int iteracion=1, multiplicando, multiplicador; 
	int resultado=0;
	string sumandos;
	bool sumando1=true;
	
	
	//Entradas
	
	do
	{
		cout<<"Numeros a multiplicar: ";
		cin>>num1>>num2;
		
		if(num1<=0)
		{
			cout<<"Numero 1 no valido"<<endl;
		}
		
		if(num2<=0)
		{
			cout<<"Numero 2 no valido"<<endl;	
		}
		
		cout<<endl;		
	} while(num1<=0 || num2<=0) ;
	
	
	
	//Cálculos
	
	cout<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<"Iteracion   Multiplicando   Multiplicador"<<endl;
	cout<<"-----------------------------------------"<<endl;


	
	multiplicando=num1;
	multiplicador=num2;
	if((multiplicando%2)!=0)
		{
			resultado+=multiplicador;
			if(!sumando1)
			{
				sumandos+=("+"+(to_string(multiplicador)));
			}
			else
			{
				sumandos+=(to_string(multiplicador));
				sumando1=false;
			}
		}
	cout<<"  "<<iteracion<<"\t\t"<<multiplicando<<"\t\t"<<multiplicador<<endl;
	


	
	while(multiplicando!=1)
	{
		multiplicando/=2;
		multiplicador*=2;
		
		if((multiplicando%2)!=0)
		{
			resultado+=multiplicador;
			if(!sumando1)
			{
				sumandos+=(" + "+(to_string(multiplicador)));
			}
			else
			{
				sumandos+=(to_string(multiplicador));
				sumando1=false;
			}
		}
		
		iteracion++;
		cout<<"  "<<iteracion<<"\t\t"<<multiplicando<<"\t\t"<<multiplicador<<endl;
	}	
	
	
	//Salidas
	
	cout<<endl;
	cout<<num1<<" * "<<num2<<" = "<<sumandos<<" = "<<resultado;
	
	return 0;
}
