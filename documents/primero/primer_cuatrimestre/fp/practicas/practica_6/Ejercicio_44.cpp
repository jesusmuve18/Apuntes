/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_44
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Para controlar los tiempos que emplean los corredores de una carrera 
	popular se desea disponer de un programa muy sencillo que se irá ampliando 
	en sucesivas versiones. En esta primera versión el programa empezará 
	pidiendo la hora de inicio de la carrera (hora, minuto y segundo) y la 
	distancia.
	
	Después, para cada corredor que llega a meta (por orden de llegada) se pide 
	el número de dorsal y la hora de llegada (hora, minuto y segundo). Tras 
	leer estos datos el 	programa mostrará el tiempo que ha empleado en 
	completar la carrera (horas, minutos y segundos) y su ritmo (en km/h y en 
	minutos/km).
	
	El programa finalizará cuando al solicitar el número de dorsal se 
	introduzca un valor	negativo.
	
	Entradas: -Hora de inicio de la carrera y distancia en km
			  -Dorsal y hora de llegada de cada corredor
	
	Salidas: -Tiempo que ha tardado en h,min,segs y ritmo en km/h y min/km.
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int SEGS_H=3600, SEGS_MIN=60;
	
	int h_inicio, m_inicio, s_inicio, distancia;
	int h_aux, m_aux, s_aux, s_final_aux, s_duracion_aux;
	int dorsal, s_inicio_tot;
	double km_h, min_km;
	
	
	//Entradas
	
	cout<<"Hora de inicio de la carrera: "<<endl;
	cout<<"\tHora: ";
	cin>>h_inicio;
	cout<<"\tMinuto: ";
	cin>>m_inicio;
	cout<<"\tSegundo: ";
	cin>>s_inicio;
	
	cout<<"Distancia (km): ";
	cin>>distancia;
	cout<<endl;
	
	//Cálculos
	
	s_inicio_tot=h_inicio*SEGS_H+m_inicio*SEGS_MIN+s_inicio; //Paso todo a seg
	
	while(dorsal>=0)
	{
		//Entradas
		
		cout<<"Dorsal: ";
		cin>>dorsal;
		if(dorsal<0)
			break;
		cout<<"Hora de llegada:"<<endl;
		cout<<"\tHora: ";
		cin>>h_aux;
		cout<<"\tMinuto: ";
		cin>>m_aux;
		cout<<"\tSegundo: ";
		cin>>s_aux;
		
		//Cálculos
		
		s_final_aux=h_aux*SEGS_H+m_aux*SEGS_MIN+s_aux; //Paso todo a seg
		s_duracion_aux= s_final_aux-s_inicio_tot; //Calculo la duracion en seg
		
		h_aux=s_duracion_aux/SEGS_H; //Paso los seg a horas, minutos y segundos
		m_aux=((s_duracion_aux-(h_aux*SEGS_H))/SEGS_MIN);  
		s_aux=(s_duracion_aux-(h_aux*SEGS_H)-(m_aux*SEGS_MIN));
		
		km_h=(distancia*SEGS_H)/(s_duracion_aux); 		//Calculo el ritmo con  
		min_km=(s_duracion_aux)/(distancia*SEGS_MIN);	//las fórmulas
		
		//Salidas
		
		cout<<endl;
		cout<<"Ha tardado "<<h_aux<<" horas, "<<m_aux<<" minutos y "
			<<s_aux<<" segundos"<<endl;
		cout<<"Ritmo: ";
		cout<<"\t"<<km_h<<" km/h"<<endl;
		cout<<"\t"<<min_km<<" min/km"<<endl;
		cout<<endl;
		
	}		
	
	return 0;
}
