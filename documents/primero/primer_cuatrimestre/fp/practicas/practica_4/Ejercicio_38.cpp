/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_38
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Dadas las coordenadas de 2 puntos calcula la distancia en el plano entre 
	dichos puntos y la distancia real que existe entre ellos.
	
	Entradas:  
	
	Latitud, longitud y altura del punto 1.
	Latitud, longitud y altura del punto 2.

	Salidas:
	
	Distancia en el plano y distancia real.
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	const double R=6372797.560856;
	const double PI=3.14159265359;
	const double GRADOS_A_RAD=(PI/180);//factor de conversión
	
	double lat1, lon1, h1;
	double lat2, lon2, h2;

	double a, c, dp,dr;//dp:distancia en el plano, dr: distancia real
	
	//Entrada
	
	cout<<"Posicion del punto 1: "<<endl;
	cout<<"\t-Latitud (grados): ";
	cin>>lat1;
	cout<<"\t-Longitud (grados): ";
	cin>>lon1;
	cout<<"\t-Altura (m): ";
	cin>>h1;
	
	cout<<endl;
	
	cout<<"Posicion del punto 2: "<<endl;
	cout<<"\t-Latitud (grados): ";
	cin>>lat2;
	cout<<"\t-Longitud (grados): ";
	cin>>lon2;
	cout<<"\t-Altura (m): ";
	cin>>h2;
	
	//Cálculos
	
	//Convierto los grados a radianes en primer lugar
	lon1=lon1*GRADOS_A_RAD;
	lon2=lon2*GRADOS_A_RAD;
	lat1=lat1*GRADOS_A_RAD;
	lat2=lat2*GRADOS_A_RAD;
		
	//Aplico la fórmula de Haversine
	a=(pow((sin(0.5*(lat2-lat1))),2)
	  +(cos(lat1)*cos(lat2)*(pow((sin(0.5*(lon2-lon1))),2))));
	c=2*asin(min(1.0,sqrt(a)));
	dp=R*c;
	
	//Calculo la distancia real con trigonometría
	/*					
						P2
					   /|						Dado que forman un triángulo
		             /	|						rectángulo podremos aplicar
		           /	|						el Teorema de Pitágoras
		      dr /		|						para calcular dr:
		       /		|  |(h1-h2)|			
	         /			|							______________________	
		   /			|						dr=V |(h1-h2)|^2  +  dp^2		
		 /			    |						
	   /________________|						Siendo |(h1-h2)| la diferencia
	P1	   	   dp								de alturas entre los puntos
		   
	*/
	
	dr=sqrt(pow((abs(h1-h2)),2)+pow(dp,2));
	
	
	//Salidas
	
	cout<<endl;
	cout<<"La distancia entre ambos puntos en el plano es de "<<dp
		<<" metros "<<endl;
	cout<<"La distancia real entre ambos puntos es de "<<dr<<" metros";
	
	return 0;
}
