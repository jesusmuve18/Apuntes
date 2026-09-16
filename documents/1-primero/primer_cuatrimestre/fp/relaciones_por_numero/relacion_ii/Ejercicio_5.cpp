/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_5
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Calcula el precio de un billete dependiente de varios factores:
		Si el destino está a menos de 300 kilómetros, el precio final es la 
		tarifa base
		Si está a más de 300 Km, se suman 10 céntimos por cada kilómetro de 
		distancia al destino (a partir de esa distacia)
		Si el trayecto es mayor de 700 km, se aplica un descuento del 2 %
		Si el número de puntos es mayor de 100, se aplica un descuento del 3 %
		y si es mayor de 200, se aplica un descuento del 4 %
		
	Entradas: 	Distancia al destino y Puntos del usuario.
	
	Salidas: 	Precio final del billete.
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	const double PRECIO_KM_ADICIONAL = 0.1;		// Valor constante 
   	const double PRECIO_BASE = 150.0;			// Valor constante 
   	const double UMBRAL=300;
   	const double PORCENTAJE_DESCUENTO_700KM=0.02;
	const double PORCENTAJE_DESCUENTO_100P=0.03;
   	const double PORCENTAJE_DESCUENTO_200P=0.04;
   	
   	double km;      		// Distancia al destino
	double precio_sin_desc, precio_final; 	// Precio del billete
	double descuento_total=0;
	int puntos;
	bool datos_ok;

   // Entrada de datos
   
   cout << "Distancia al destino: " ;
   cin >> km;
   
   cout<< "Puntos: ";
   cin>>puntos;

	
	// Cálculos
	
	datos_ok=(km>0 && puntos>0); 	//Compruebo que los datos introducidos son 
									//correctos
	if(datos_ok)	//Si son correctos aplico los cálculos correspondientes.
	{
		if(km<UMBRAL) {
			precio_sin_desc = PRECIO_BASE;
		}
		else {
			precio_sin_desc = PRECIO_BASE + (PRECIO_KM_ADICIONAL 
							  * (km-UMBRAL));
		}
		
		if(km>700) {
			descuento_total=descuento_total+PORCENTAJE_DESCUENTO_700KM;
		}
		
		if(puntos>100) {
			if(puntos>200) {
				descuento_total=descuento_total+PORCENTAJE_DESCUENTO_200P;
			}
			else{
				descuento_total=descuento_total+PORCENTAJE_DESCUENTO_100P;
			}		
		}
					   	
	   	precio_final=precio_sin_desc*(1-descuento_total);
	}


	// Salidas
	
	cout << endl;
	if(datos_ok)
	{
		cout << "Se aplicara un descuento del "<<descuento_total*100
		 <<" %"<<endl;
		cout << "El billete cuesta: " << precio_final << " euros." <<endl ;
		cout << endl;
	}
	else
	{
		cout<<"Los valores introducidos no son validos"<<endl;
	}
	
	
	return 0;
}
