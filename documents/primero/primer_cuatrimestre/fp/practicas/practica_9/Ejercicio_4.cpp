/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_4
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Para modelar un rectángulo se usará el tipo de dato Rectangulo. Un 
	rectángulo se caracterizará por la coordenada superior izquierda y la 
	coordenada inferior derecha (ambos son datos de tipo Punto2D).
	
	Escribir un programa que lea las coordenadas que definen un rectángulo 
	y calcule la circunferencia centrada en el punto de corte de las diagonales 
	del rectángulo tal que su superficie sea la menor entre todas las 
	circunferencias de área mayor que la del rectángulo.
	
	Use datos de tipo Rectangulo, Circunferencia y Punto2D para la resolución 
	de este problema.
	
	Para el cálculo, considere comenzar con radio=0.5 e ir incrementando su 
	valor 0.25 en cada iteración.
	
	Entradas: Puntos extremos de la diagonal de un rectángulo
	
	Salidas: Coordenada del centro y radio cuyo área sea la siguiente mayor
			 al área del rectángulo dado con saltos de 0.25 entre cada 
			 valor del radio.
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;

//#define DATOS_EXTRA
/****************************************************************************/
//Declaración de constantes globales

const double RADIO_INICIAL=0.5;
const double SALTO_RADIO=0.25;
const double PI=3.14159265359;

/****************************************************************************/
//Declaración de tipos de datos (struct)

struct Punto2D 
{
	double x; //abscisas
	double y; //ordenadas
};

struct Rectangulo
{
	Punto2D punto_1; //coordenadas de uno de los vértices del rectángulo
	Punto2D punto_2; //coordenadas del punto opuesto con respecto a la diagonal
					 //del punto_1
};

struct Circunferencia
{
	Punto2D centro; //coordenadas del centro de la circunferencia
	double radio;
};

/****************************************************************************/
/* 	Función: Lee la coordenada x de un dato
	Parámetros: Petición
	Devuelve: Coordenada x de un dato											*/
//............................................................................																
double LeeCoordenadaX (string titulo)
{
	double x; //Dato que devolverá la función
	cout<<titulo;
	cin>>x;
	
	return x;
}

/****************************************************************************/
/* 	Función: Lee la coordenada y de un dato
	Parámetros: Petición
	Devuelve: Coordenada y de un dato											*/
//............................................................................																
double LeeCoordenadaY (string titulo)
{
	double y; //Dato que devolverá la función
	cout<<titulo;
	cin>>y;
	
	return y;
}

/****************************************************************************/
/* 	Función: Devuelve el área de un rectángulo
	Parámetros: Dato de tipo rectángulo
	Devuelve: Área del rectángulo											*/
//............................................................................																
double AreaRectangulo(Rectangulo rect)
{
	double base=abs(rect.punto_1.x-rect.punto_2.x);
	double altura=abs(rect.punto_1.y-rect.punto_2.y);
	
	return base*altura;
}

/****************************************************************************/
/* 	Función: Devuelve el área de una circunferencia
	Parámetros: Dato de tipo circunferencia
	Devuelve: Área de la circunferencia										*/
//............................................................................																
double AreaCircunferencia(Circunferencia circ)
{
	return (PI*circ.radio*circ.radio);
}

/****************************************************************************/
/* 	Función: Devuelve la posición del centro del rectángulo (cruce de 
			 diagonales)
	Parámetros: Dato de tipo rectángulo
	Devuelve: Coordenadas del punto del centro del rectángulo				*/
//............................................................................																
Punto2D CentroRectangulo(Rectangulo rect)
{
	Punto2D centro; //Dato que devolverá la función
	
	double base=abs(rect.punto_1.x-rect.punto_2.x);
	double altura=abs(rect.punto_1.y-rect.punto_2.y);
	
	centro.x=min(rect.punto_1.y,rect.punto_2.y)+(base/2);
	centro.y=min(rect.punto_1.x,rect.punto_2.x)+(altura/2);
	
	return centro;
}

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	Rectangulo rect;
	double area_rect;
	
	Circunferencia circ;
	double area_circ;
	
	//Entradas
	
	cout<<"Puntos que definen un rectangulo "
		<<"(puntos de extremos de una diagonal):"<<endl;
	do
	{
		cout<<"     Punto 1: "<<endl;
		rect.punto_1.x=LeeCoordenadaX ("\tCoordenada X: ");
		rect.punto_1.y=LeeCoordenadaY ("\tCoordenada Y: ");
		
		cout<<"    Punto 2: "<<endl;
		rect.punto_2.x=LeeCoordenadaX ("\tCoordenada X: ");
		rect.punto_2.y=LeeCoordenadaY ("\tCoordenada Y: ");
		
		cout<<"--------------------------------"<<endl;
		
	}while(AreaRectangulo(rect)<=0);
	
		
	//Cálculos
	
	area_rect=AreaRectangulo(rect);
	circ.centro=CentroRectangulo(rect);
	circ.radio=RADIO_INICIAL;
	area_circ=AreaCircunferencia(circ);
	
	while(area_circ<area_rect)
	{
		circ.radio+=SALTO_RADIO;
		area_circ=AreaCircunferencia(circ);
	}
	
	//Salidas
	
	cout<<"La circunferencia buscada tiene las siguientes caracteristicas:"
		<<endl;
	cout<<"\tCentro: Coordenada ("<<circ.centro.x<<" , "<<circ.centro.y<<")"
		<<endl;
	cout<<"\tRadio: "<<circ.radio<<endl;
	
	#ifdef DATOS_EXTRA
	
	cout<<"\tArea: "<<area_circ<<endl;
	cout<<endl;
	cout<<"(El rectangulo definido tiene un area de "<<area_rect<<" unidades)"
		<<endl;
	
	#endif
	return 0;
}
