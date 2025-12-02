/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_11
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	determinar la sanción a aplicar en una autovía, cuyo límite
	de velocidad es 120 Km/h. En la siguiente tabla se muestra la velocidad del 
	vehículo y la sanción correspondiente (número de puntos del carnet de 
	conducir que se restan y la multa en euros)
								(120,150] -> 0, 100
								(150,170] -> 2, 300
								(170,180] -> 4, 400
								(180,190] -> 6, 500
								(190, oo) -> 6, 600
	
	
	Entradas: Velocidad del vehículo.
	
	Salidas: Número de puntos a detraer y multa en euros.
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	//Velocidades de referencia:
	const int VELOCIDAD1=120;
	const int VELOCIDAD2=150;
	const int VELOCIDAD3=170;
	const int VELOCIDAD4=180;
	const int VELOCIDAD5=190;
	
	
	double velocidad;
	int puntos_detraer;
	double multa;
	bool datos_ok;
	
	//Entradas
	
	cout<<"Velocidad (km/h): ";
	cin>>velocidad;
	
	//Cálculos
	
	datos_ok=(velocidad>0);
	
	if(datos_ok)
	{
		if(velocidad<=VELOCIDAD1)
		{
			multa=0;
			puntos_detraer=0;
		}
		
		if(VELOCIDAD1<velocidad && velocidad<=VELOCIDAD2)
		{
			multa=100;
			puntos_detraer=0;
		}
		
		if(VELOCIDAD2<velocidad && velocidad<=VELOCIDAD3)
		{
			multa=300;
			puntos_detraer=2;
		}
		
		if(VELOCIDAD4<velocidad && velocidad<=VELOCIDAD4)
		{
			multa=400;
			puntos_detraer=4;
		}
		
		if(VELOCIDAD4<velocidad && velocidad<=VELOCIDAD5)
		{
			multa=500;
			puntos_detraer=6;
		}
		
		if(VELOCIDAD5<velocidad)
		{
			multa=600;
			puntos_detraer=6;
		}
	}
	
	//Salidas
	
	cout<<endl;
	
	if(datos_ok)
	{
		if(velocidad<=VELOCIDAD1)
		{
			cout<<"No ha cometido ninguna infraccion";
		}
		else
		{
			cout<<"Ha cometido una infraccion:"<<endl;
			cout<<"\tNumero de puntos a detraer: "<<puntos_detraer
				<<" puntos"<<endl;
			cout<<"\tMulta economica: "<<multa<<" euros"<<endl;
		}
	}
	else
	{
		cout<<"La velocidad introducida no es valida"<<endl;
	}
	
	
	return 0;
}
