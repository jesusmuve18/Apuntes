/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_12
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Dado el total a pagar y la cantidad pagada calcula el número de monedas
	de 2 euros, 1 euro, 50 céntimos, 20 céntimos, 10 céntimos, 5 céntimos,
	2 céntimos y 1 céntimo que se debe dar de cambio.
	
	Entradas: Total a pagar, Cantidad pagada.
	Salidas: Cantidad de monedas de cada tipo que se debe dar de cambio.
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	double total_pagar=1;	//Precio a pagar
	double cant_pagada=0;	//Cantidad que se paga
	double cambio;	//Cantidad a devolver que se dividira en monedas
	double resto; //Almacena lo que queda por pagar en cada fase del programa
	double pagado; //Almacena lo que se lleva pagado en cada fase del programa
	
	int n_2euros; //Número de monedas de 2 euros
	int n_1euros; //Número de monedas de 1 euro
	int n_50cents; //Número de monedas de 50 céntimos
	int n_20cents; //Número de monedas de 20 céntimos
	int n_10cents; //Número de monedas de 10 céntimos
	int n_5cents; //Número de monedas de 5 céntimos
	int n_2cents; //Número de monedas de 2 céntimos
	int n_1cents; //Número de monedas de 1 céntimo
	
	//Entrada
	while(cant_pagada<total_pagar) //Se comprueba que cant_pagada>=total_pagar
	{
		cout<<"Introduce el total a pagar: ";
		cin>>total_pagar;
		cout<<"Introduce la cantidad pagada: ";
		cin>>cant_pagada;
		cout<<endl;
	}
	
	//Cálculos
	cambio=(cant_pagada-total_pagar);
	resto=cambio; //Queda por pagar el cambio entero
	
	n_2euros=(resto/2); 	//Cálculo del número de monedas de 2 euros
	pagado=n_2euros*2;
	resto=cambio-pagado;
	
	n_1euros=(resto/1);		//Cálculo del número de monedas de 1 euro
	pagado=pagado+n_1euros*1;
	resto=cambio-pagado;
	
	cambio=cambio*100;	//Paso de Euros a céntimos para continuar el proceso
	resto=resto*100; 
	pagado=pagado*100;	
	
	n_50cents=(resto/50);	//Cálculo del número de monedas de 50 céntimos
	pagado=pagado+n_50cents*50;
	resto=cambio-pagado;
	
	n_20cents=(resto/20);	//Cálculo del número de monedas de 20 céntimos
	pagado=pagado+n_20cents*20;
	resto=cambio-pagado;
	
	n_10cents=(resto/10);	//Cálculo del número de monedas de 10 céntimos
	pagado=pagado+n_10cents*10;
	resto=cambio-pagado;
	
	n_5cents=(resto/5);		//Cálculo del número de monedas de 5 céntimos
	pagado=pagado+n_5cents*5;
	resto=cambio-pagado;
	
	n_2cents=(resto/2);		//Cálculo del número de monedas de 2 céntimos
	pagado=pagado+n_2cents*2;
	resto=cambio-pagado;
	
	n_1cents=resto;		//Cálculo del número de monedas de 1 céntimo
	pagado=pagado+n_1cents;
	resto=cambio-pagado;
	
	cambio=cambio/100;	//Devuelvo el cambio a la unidad del Euro
	
	//Salidas
	cout<<"El cambio sera de "<<cambio<<" euros de la forma:"<<endl;
	cout<<"\t"<<n_2euros<<" monedas de 2 euros"<<endl;
	cout<<"\t"<<n_1euros<<" monedas de 1 euro"<<endl;
	cout<<"\t"<<n_50cents<<" monedas de 50 centimos"<<endl;
	cout<<"\t"<<n_20cents<<" monedas de 20 centimos"<<endl;
	cout<<"\t"<<n_10cents<<" monedas de 10 centimos"<<endl;
	cout<<"\t"<<n_5cents<<" monedas de 5 centimos"<<endl;
	cout<<"\t"<<n_2cents<<" monedas de 2 centimos"<<endl;
	cout<<"\t"<<n_1cents<<" monedas de 1 centimo"<<endl;
	
	return 0;
}
