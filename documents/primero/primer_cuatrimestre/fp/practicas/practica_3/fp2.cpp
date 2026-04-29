/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
		
	Entradas:  
	
	Salidas:
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	const int INTERVALO=('A'-'a');// si ersto es negativo es pq a>A
	//para pasar de a --> A tendre que restarle 32

	char letra_min,letra_may;
	cout<<"letra en minuscula: ";
	cin>> letra_min;
	letra_may=letra_min+INTERVALO;
	cout<<"letra en mayuscula: "<<letra_may;

	return 0;
}
