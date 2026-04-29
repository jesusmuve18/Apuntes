#include <iostream>
using namespace std;
int main()
{
	// Entrada de datos
	
	long double distancia1, distancia2, distancia3;
	long double velocidad1, velocidad2, velocidad3;
	long double tiempo1, tiempo2, tiempo3;
	double tiempo_total_segundos, tiempo_total_min_seg;
	int m, s;
	
	cout<<"Segun el tramo compuesto por los sectores:";
	cout<<endl;
	cout<<"\t1) 5.3 Km."<<endl;
	cout<<"\t2) 8.1 Km."<<endl;
	cout<<"\t3) 6.4 Km."<<endl;
	cout<<endl;

	
	// Haciendo uso de 1Km=1000m
	
	distancia1=5.3*1000;
	distancia2=8.1*1000;
	distancia3=6.4*1000;
	
	// Haciendo uso de Km/h=(v*1000)/3600
	
	velocidad1=(80*1000)/3600.0;
	velocidad2=(100*1000)/3600.0;
	velocidad3=(80*1000)/3600.0;
	
	// Cálculos
	
	tiempo1=distancia1/velocidad1;
	tiempo2=distancia2/velocidad2;
	tiempo3=distancia3/velocidad3;
	
	tiempo_total_segundos=tiempo1+tiempo2+tiempo3;
	tiempo_total_min_seg=tiempo_total_segundos/60;
	m=tiempo_total_segundos/60;
	s=m%60;
	
	cout<<tiempo1<<endl;
	cout<<tiempo2<<endl;
	cout<<tiempo3<<endl;
	
	cout<<"El tiempo minimo estimado para recorrerlo completo es de:"<<endl;
	cout<<"\tTiempo en segundos: "<<tiempo_total_segundos;
	cout<<"\tTiempo en minutos y segundos: "<<m<<" minutos y "<<s<<" segundos.";
	
   // Salida de datos

	return 0;
}
