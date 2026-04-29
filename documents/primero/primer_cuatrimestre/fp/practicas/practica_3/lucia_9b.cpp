#include <iostream>
using namespace std;
int main()
{
	//Datos.
		//Distancias (en km):
	const double distancia_1 = 5.3;
	const double distancia_2 = 8.1;
	const double distancia_3 = 6.4;
		
		//Velocidad (en km/h):
	const double velocidad_1 = 80;
	const double velocidad_2 = 100;
	const double velocidad_3 = 80;
		
		//Soluciones:
	double tiempo_umbral_seg;	
	double tiempo_umbral_min;
	double tiempo_umbral_seg2;
	
	
	//Cálculos:
	tiempo_umbral_seg = ((distancia_1/velocidad_1)+(distancia_2/velocidad_2)+(distancia_3/velocidad_3))*3600;
	tiempo_umbral_min = (tiempo_umbral_seg/60);
	tiempo_umbral_seg2 = (tiempo_umbral_min-int(tiempo_umbral_min))*60;
	
	
	//Salidas:
	cout << " El tiempo umbral en segundos es:" << tiempo_umbral_seg;
	cout << " El tiempo umbral en minutos y segundos es:" << "\n Minutos:" << int(tiempo_umbral_min);
	cout << " Segundos:" << tiempo_umbral_seg2;
	
	return 0;
}
