/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_18
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Dada una instrucción de transformación de unidades de longitud por teclado
	realiza dicha transformación y la muestra por pantalla.
	
	Entradas: Transformación a realizar
	
	Salidas: Resultado de la transformación
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
#include<iomanip>
using namespace std;
/****************************************************************************/
int main()
{
	cout.setf(ios::fixed);
	
	//Declaración de datos
	
	const int DECIMALES=3;//Número de decimales que se muestra en el resultado
	
	string cifra1,u_inicio,a,u_final;
	double inicio,final;
	
	
	//Entradas
	
	cout<<">";
	getline(cin,cifra1,' ');
	getline(cin,u_inicio,' ');
	getline(cin,a,' ');
	getline(cin,u_final);
	
	//Cálculos
	
	inicio=stod(cifra1);
	
	if(u_inicio==u_final)
	{
		final=inicio;
	}
	
	if(u_inicio=="mm")
	{
		if(u_final=="km")
			final=inicio/1000000;
		if(u_final=="m")
			final=inicio/1000;
		if(u_final=="cm")
			final=inicio/100;	
		
	}
	
	if(u_inicio=="cm")
	{
		if(u_final=="mm")
			final=inicio*10;
		if(u_final=="m")
			final=inicio/100;
		if(u_final=="km")
			final=inicio/100000;
	}
	
	if(u_inicio=="m")
	{
		if(u_final=="mm")
			final=inicio*1000;
		if(u_final=="cm")
			final=inicio*100;
		if(u_final=="km")
			final=inicio/1000;
	}
	
	if(u_inicio=="km")
	{
		if(u_final=="mm")
			final=inicio*1000000;
		if(u_final=="cm")
			final=inicio*100000;
		if(u_final=="m")
			final=inicio*1000;
	}
		
	
	//Salidas
	
	bool correcto=true; //Comprueba que la entrada es correcta
	
	if(a!="a")
	{
		cout<<"Formato de entrada no valido"<<endl;
		correcto=false;
	}
	
	if(u_inicio!="mm" && u_inicio!="cm" && u_inicio!="m" && u_inicio!="km")
	{
		cout<<"Error en la primera magnitud"<<endl;
		correcto=false;
	}
	
	if(u_final!="mm" && u_final!="cm" && u_final!="m" && u_final!="km")
	{
		cout<<"Error en la segunda magnitud"<<endl;
		correcto=false;
	}
	
	if(correcto)
	{
		cout<<setprecision(DECIMALES)<<inicio<<" "<<u_inicio<<" = "
			<<final<<" "<<u_final;		
	}
	
	return 0;
}
