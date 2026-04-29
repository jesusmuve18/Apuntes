/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_14
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Lee el número de unidades vendidas y el precio unitario y calcula el 
	precio final de venta aplicandole:
	Si se han vendido más de 100 unidades y además el precio de venta supera
	los 700€ se le aplicará un descuento del 2+3=5 porciento
	Si se han vendido más de 100 unidades pero el precio de venta no supera
	los 700€ se le aplicará únicamente un descuento del 3%.
	
	Entradas: Unidades vendidas y precio unitario
	
	Salidas: Precio final
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int LIMITE_UDS=100;
	const double LIMITE_PRECIO=700;
	
	int uds_vendidas;
	double precio_unitario, precio_bruto, precio_final;
	double descuento=0; //Acumula los descuentos
	bool datos_ok;
	
	//Entradas
	
	cout<<"Unidades vendidas: ";
	cin>>uds_vendidas;
	cout<<"Precio de venta de cada unidad: ";
	cin>>precio_unitario;
	
	
	//Cálculos
	
	datos_ok=(uds_vendidas>0 && precio_unitario>0);
	
	if(datos_ok)
	{
		precio_bruto=uds_vendidas*precio_unitario;
		
		if(uds_vendidas>LIMITE_UDS)
		{
			descuento=descuento+3;
					
			if(precio_bruto>LIMITE_PRECIO)
				descuento=descuento+2;
		}
		
		precio_final=precio_bruto*(1-(descuento/100.0));
	}
	
		
	//Salidas
	
	cout<<endl;
	
	if(datos_ok)
	{
		cout<<"Se aplicara un descuento del "<<descuento<<"% por unidad"<<endl;
		cout<<"El precio total de la venta es de "<<precio_final
			<<" euros"<<endl;
	}
	else
		cout<<"Los valores introducidos no son validos"<<endl;
	
	return 0;
}
