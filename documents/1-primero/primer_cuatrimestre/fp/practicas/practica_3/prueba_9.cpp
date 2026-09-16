/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_9
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Calcula y muestra el tiempo umbral de sanción, o sea, el tiempo mínimo 
	que puede emplear un vehículo en recorrer el tramo completo sin ser	
	sancionado.
	
	Salidas: resultado en segundos, y en minutos y segundos
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos de distancias y velocidades de cada tramo
	const double KM_SECTOR_1=5.3, VEL_SECTOR_1=80; //Tramo 1
	const double KM_SECTOR_2=8.1, VEL_SECTOR_2=100; //Tramo 2
	const double KM_SECTOR_3=6.4, VEL_SECTOR_3=80; //Tramo 3
	
	double tiempo_total_s; //Tiempo total en segundos
	
	//Declaro el tiempo total en minutos y segundos con 2 variables distintas
	int tiempo_total_min;
	double tiempo_total_seg;

	
	//Cálculos
	tiempo_total_s=((KM_SECTOR_1/VEL_SECTOR_1)*3600);
	cout<<tiempo_total_s<<endl;
	tiempo_total_s=(tiempo_total_s+((KM_SECTOR_2/VEL_SECTOR_2)*3600));
	cout<<tiempo_total_s<<endl;
	tiempo_total_s=(tiempo_total_s+((KM_SECTOR_3/VEL_SECTOR_3)*3600));
	cout<<tiempo_total_s<<endl;
	
	tiempo_total_min=(tiempo_total_s/60);
	tiempo_total_seg=tiempo_total_s-(tiempo_total_min*60);

	//Salida
	cout<<"El tiempo minimo total son "<<tiempo_total_s<<" segundos"<<endl;
	cout<<"Tambien se puede expresar como "<<tiempo_total_min;
	cout<<" minutos y "<<tiempo_total_seg<<" segundos";
	return 0;
}
