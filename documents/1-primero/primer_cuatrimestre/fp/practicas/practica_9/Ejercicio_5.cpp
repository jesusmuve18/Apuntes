/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_5
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const double MIN_VELOCIDAD = 10.0; // Por debajo no capta
	
const double UMBRAL_VELOCIDAD = 100.0;

const double PORC_MARGEN_FIJO = 5.0;	
const double PORC_MARGEN_MOVIL = 7.0;
		
const double KM_MARGEN_FIJO = 5.0;	
const double KM_MARGEN_MOVIL = 7.0;

/****************************************************************************/
//Declaración de tipos de Datos (struct)

struct MedidaRadar
{
	char tipo_radar;
	double velocidad_leida;
	double velocidad_imputada;
	double margen;
	
	bool error_velocidad;
	bool es_radar_fijo;
};

/****************************************************************************/
/* 	Función: 
	Parámetros: 
	Devuelve:  																*/
//............................................................................
MedidaRadar LeeMedidaRadar(string titulo)
{
	MedidaRadar medida; //Dato que devolverá la función
	
	do
	{
		cout<<titulo;
		cin>>medida.tipo_radar>>medida.velocidad_leida;
		
		medida.error_velocidad=(medida.velocidad_leida < MIN_VELOCIDAD);
		
		if (toupper(medida.tipo_radar)=='F')	
		{
			medida.es_radar_fijo=true; 
		}	
		else
		{
			medida.es_radar_fijo=false;
		}
				
	} while(medida.error_velocidad);
	
	return medida;
}

/****************************************************************************/
/* 	Función: Calcula el margen segun los parámetros de la medida tomada
	Parámetros: Dato de tipo MedidaRadar
	Devuelve: margen de la medida											*/
//............................................................................
double Margen(MedidaRadar medida)
{
	double margen;
	
	if (medida.velocidad_leida <= UMBRAL_VELOCIDAD) 
	{
	
		if (medida.es_radar_fijo)
		{
			margen = KM_MARGEN_FIJO;
		} 
		else 
		{
			margen = KM_MARGEN_MOVIL;	
		}	
		
	}
	else
	{
		if (medida.es_radar_fijo)
		{
			margen = PORC_MARGEN_FIJO;
		} 
		else 
		{
			margen = PORC_MARGEN_MOVIL;	
		}	
	}
	
	return margen;
}

/****************************************************************************/
/* 	Función: 
	Parámetros: 
	Devuelve:  																*/
//............................................................................
double VelocidadImputada(MedidaRadar medida)
{
	double velocidad_imputada;
	if (medida.velocidad_leida <= UMBRAL_VELOCIDAD) 
	{
		velocidad_imputada = medida.velocidad_leida - medida.margen;
	}
	else
	{
		velocidad_imputada = medida.velocidad_leida * (1-medida.margen/100.0);
	}
	return velocidad_imputada;
}

/****************************************************************************/
/* 	Función: 
	Parámetros: 
	Devuelve:  																*/
//............................................................................
string TipoRadar(MedidaRadar medida)
{
	string tiporadar; //Dato que devolverá la función
	
	if (toupper(medida.tipo_radar)=='F') 
	{
		tiporadar="Fijo";
	}
	else
	{
		tiporadar="Movil";
	}
	
	return tiporadar;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	MedidaRadar medida1;
	
	//Entradas
	
	medida1=LeeMedidaRadar("Introduzca tipo de radar y velocidad leida: ");
	medida1.margen=Margen(medida1);
	medida1.velocidad_imputada=VelocidadImputada(medida1);
		

	//Salidas
	
	cout << endl;
	cout<<"Tipo radar: "<<TipoRadar(medida1)<<endl;
	cout<<"Velocidad leida: " << medida1.velocidad_leida<<" Km/h"<<endl;
	cout<<"Velocidad imputada: " << medida1.velocidad_imputada<< " Km/h" 
		<<endl;
	cout<<endl;
	
	return 0;
}
