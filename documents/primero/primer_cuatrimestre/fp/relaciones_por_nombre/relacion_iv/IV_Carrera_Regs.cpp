/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_6
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Reescribid el programa construido para implementar la solución al ejercicio 
	14 de la Relación de Problemas III (control de una carrera, con funciones).
	
	Use datos de tipo Instante, para registrar tripletas (hora, minuto, 
	segundo) en la resolución de este problema.
	
	Entradas: Instante de inicio de la carrera
			  Datos de los corredores que van completando la carrera
			  		-Dorsal
			  		-Instante de llegada
	
	Salidas: Ritmo de cada corredor
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int SEGS_H=3600;
const int SEGS_MIN=60;

/****************************************************************************/
//Declaración de tipos de datos (struct)

struct Instante
{
	int h; 		//horas
	int m;		//minutos
	double s;	//segundos
	
};

/****************************************************************************/
/* 	Función: Lee los datos de un instante (horas minutos y segundos)
	Parámetros: ninguno
	Devuelve: Dato de tipo instante											
//..........................................................................*/
Instante LeeInstante()
{
	Instante instante;
	
	do
	{
		cout<<"   Hora: ";
		cin>>instante.h;
		
	}while(instante.h<0);
	
	do
	{
		cout<<"   Minuto: ";
		cin>>instante.m;
		
	}while(instante.m<0);
	
	do
	{
		cout<<"   Segundo: ";
		cin>>instante.s;
		
	}while(instante.s<0);
	
	return instante;
}

/****************************************************************************/
/* 	Función: Convierte el instante en sus medidas a un instante en segundos
	Parámetros: Dato de tipo Instante
	Devuelve: Tiempo en segundos											
//..........................................................................*/
double InstanteEnSegundos(Instante instante)
{
	double segundos; //Dato que devolverá la función
	
	segundos=instante.h*SEGS_H+instante.m*SEGS_MIN+instante.s;
	
	return segundos;	
}

/****************************************************************************/
/* 	Función: Operación opuesta a la anterior
	Parámetros: Tiempo en segundos
	Devuelve: Dato de tipo instante											
//..........................................................................*/
Instante SegundosEnInstante(double segundos)
{
	Instante instante; //Dato que devolverá la función
	
	instante.h=segundos/SEGS_H;
	instante.m=((segundos-(instante.h*SEGS_H))/SEGS_MIN);
	instante.s=(segundos-(instante.h*SEGS_H)-(instante.m*SEGS_MIN));
	
	return instante;	
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	Instante instante_inicial; //Inicio de la carrera
	Instante instante_llegada; //Variable que cambia con cada dorsal
	double distancia; //km de la carrera
	
	double tiempo; //tiempo que tarda cada dorsal en completar la carrera
	
	int dorsal;
	bool dorsal_correcto;
	double km_h, min_km; //Datos del ritmo para cada dorsal
	
	
	//Entradas
	
	cout<<"Instante de inicio de la carrera: "<<endl;
	instante_inicial=LeeInstante();
	cout<<endl;

	do
	{
		cout<<"Distancia (km): ";
		cin>>distancia;
		
	} while(distancia<=0);
	
	cout<<endl;
	cout<<"--------------------------------------------------"<<endl;
	cout<<endl;	
	
	
	//Cálculos y salidas
		
	do //while (dorsal_correcto);
	{
		cout<<"Dorsal: ";
		cin>>dorsal;
		
		if(dorsal>=0)
		{
			dorsal_correcto=true;
			
			do
			{
				cout<<"Hora de llegada:"<<endl;
				instante_llegada=LeeInstante();
				
			} while(InstanteEnSegundos(instante_llegada)
					<= InstanteEnSegundos(instante_inicial));
			
			//Cálculos
			
			tiempo=InstanteEnSegundos(instante_llegada)
				   -InstanteEnSegundos(instante_inicial);
				   
			km_h=(distancia*SEGS_H)/(tiempo); 		//Calculo el ritmo con  
			min_km=(tiempo)/(distancia*SEGS_MIN);	//las fórmulas
			
			
			//Salidas
			
			cout<<endl;
			cout<<"Ha tardado "
				<<instante_llegada.h-instante_inicial.h<<" horas, "
				<<instante_llegada.m-instante_inicial.m<<" minutos y "
				<<instante_llegada.s-instante_inicial.s<<" segundos"<<endl;
				
			cout<<"Ritmo: ";
			cout<<"\t"<<km_h<<" km/h"<<endl;
			cout<<"\t"<<min_km<<" min/km"<<endl;
			cout<<endl;
			cout<<".................................................."<<endl;
			cout<<endl;	
			
			
		} //if(dorsal>=0)
		else
		{
			dorsal_correcto=false;
		}
		
	} while (dorsal_correcto);
	
	
	return 0;
}
