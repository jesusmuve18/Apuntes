/**********************/
/*  FUNDAMENTOS DE PROGRAMACIÓN					EJERCICIO-6


	ALISON CARVAJAL PAULINO

	1º A2 DOBLE GRADO EN INGENIERÍA INFORMÁTICA Y MATEMÁTICAS

	Este programa lee las coordenadas que definen un rectángulo y
	calcula la circunferencia centrada en el punto de corte de las diagonales 
	del rectángulo tal que su superficie sea la menor entre todas las 
	circunferencias	de área mayor que la del rectángulo.
	
	Para el cálculo,comenza con radio=0.5 e incrementanda su
	valor 0.25 en cada iteración.


	ENTRADAS: coordenadas del rectángulo
	SALIDAS: menor circunferencia de área mayor que la del rectángulo
*/
/**********************/

#include <iostream>	// Inclusión de los recursos E/S
#include <cmath>
using namespace std;

/**********************/

const double PI = 3.1415926;
/**********************/

struct Punto2D 
{
	double x, y;	// abscisa y ordenada de un punto
};

/****************************************************************************/

class Rectangulo{
	
private: 

 	Punto2D superior_izq, inferior_dch; 

public:


/****************************************************************************/
//Lee las coordenadas de la esquina superior izq.
/****************************************************************************/
	void EsquinaSuperiorIzq ()
	{
		cout <<"Punto superior izq"<<endl;
		cout<<endl;
		
		cout << "Coordenada X del punto : ";
		cin >> superior_izq.x; 
	
		cout << "Coordenada Y del punto : ";
		cin >> superior_izq.y; 
	}

/****************************************************************************/
//Lee las coordenadas de la esquina inferior dch.
/****************************************************************************/
	void EsquinaInferiorDch ()
	{
		cout <<"Punto inferior dch"<<endl;
		cout<<endl;
		
		cout << "Coordenada X del punto : ";
		cin >> inferior_dch.x; 
	
		cout << "Coordenada Y del punto : ";
		cin >> inferior_dch.y; 
	}
	
/****************************************************************************/
// Devuelve la base de un rectángulo.
/****************************************************************************/

	double BaseRectangulo ()
	{	
		return (inferior_dch.x 
		      - superior_izq.x);
	}

/****************************************************************************/
// Devuelve: el alto del rectángulo "r".
/****************************************************************************/

	double AlturaRectangulo ()
	{	
		return (superior_izq.y 
		      - inferior_dch.y);
	}

/***************************************************************************/
// Devuelve el punto superior derecho  de un rectángulo.

/***************************************************************************/
	
	Punto2D EsquinaSuperiorDch ()
	{
		Punto2D d;	
		
		d.x = GetEsquinaSI().x + BaseRectangulo();
		d.y = GetEsquinaSI().y;
		 
		return (d);
	}
	
/***************************************************************************/
// Devuelve el punto inferior izquierda  de un rectángulo.
/***************************************************************************/

	Punto2D EsquinaInferiorIzq ()
	{
		Punto2D p;	
		
		p.x = GetEsquinaSI().x;
		p.y = GetEsquinaSI().y - AlturaRectangulo();
		 
		return (p);
	}


/****************************************************************************/
// Calcula el área de un rectángulo.
/****************************************************************************/

	double AreaRectangulo ()
	{
		double base = fabs(BaseRectangulo());
		double altura = fabs(AlturaRectangulo());
		return (base * altura);
	}


/****************************************************************************/
// Calcula y devuelve el punto central de un rectángulo.
/****************************************************************************/

	Punto2D PuntoCentralRectangulo ()
	{
		Punto2D p;	
		
		p.x = (superior_izq.x 
		     + EsquinaSuperiorDch().x) / 2.0;
		p.y = (superior_izq.y 
		     +  EsquinaInferiorIzq().y) / 2.0;
		 
		return (p);
	}
	
/****************************************************************************/
//Las siguientes funciones devuelven valores (getters)
/****************************************************************************/

	Punto2D GetEsquinaSI()
	{
		return superior_izq;
	}
	
	
	Punto2D GetEsquinaID()
	{
		return inferior_dch;
	}
};

class Circunferencia{
	
private: 

	Punto2D centro; // Centro
	double radio;   // Radio

public:
	

/****************************************************************************/
// Calcula el área del círculo interior a una circunferencia.
// Devuelve: El área del circulo interior a "c".
/****************************************************************************/

	double AreaCircunferencia ()
	{
		
		return (PI * radio * radio);
	}

/****************************************************************************/
// Establece el centro de la circunferencia.
// Parámetros: p Punto2D
/****************************************************************************/
	void SetCentroYCircunferencia(Punto2D p , double _r)
	{
		centro= p;
		radio = _r;
	}


};

/****************************************************************************/
/****************************************************************************/

int main()	// Programa principal
{

	const double RADIO_INIC = 0.5;  // Valor del radio inicial
	const double INCR_RADIO = 0.25;	// Incremento del valor del radio


	cout.setf(ios::fixed);		// Notación de punto fijo para los reales
	cout.setf(ios::showpoint);	// Mostrar siempre decimales 

	// Lectura de las coordenadas inferior derecha y superior izquierda
	
	Rectangulo r, base, altura; 
	
	r.EsquinaSuperiorIzq();
	r.EsquinaInferiorDch();
	
	// Coordenadas del rectángulo 
	
	cout << endl;		
	cout << "Punto sup.izda. del rectangulo = " 
		 <<"["<<r.GetEsquinaSI().x<<" , "
		 <<r.GetEsquinaSI().y<<"]"<< endl;
	cout << "Punto sup.dcha. del rectangulo = " 
	     <<"["<<r.GetEsquinaID().x<<" , "
		 <<r.GetEsquinaSI().y<<"]"<< endl;	
	cout << "Punto inf.izda. del rectangulo = " 
	     <<"["<<r.GetEsquinaSI().x<<" , "
		 <<r.GetEsquinaID().y<<"]"<< endl;
	cout << "Punto inf.dcha. del rectangulo = " 
	     <<"["<<r.GetEsquinaID().x<<" , "
		 <<r.GetEsquinaID().y<<"]"<< endl;
	cout << endl;		
	
		
	r.PuntoCentralRectangulo ();	//Punto central del rectangulo
	
	//Calculos del área del reactángulo
	base.BaseRectangulo ();
	altura.AlturaRectangulo ();
	
	double area_rectangulo = r.AreaRectangulo();
	cout<<"Area del rectangulo: "<<r.AreaRectangulo()<<endl;
	
	//Datos para construir la circunferencia
	Circunferencia c;
	Punto2D centro = r.PuntoCentralRectangulo (); 
	double radio = RADIO_INIC;
	
	//Mediante la llamada de la función, se determina la circunferencia 
	c.SetCentroYCircunferencia(centro, radio);
	
	double area_circunferencia;
	
	//Mientras el área de la circunferencia sea menor que la del rectángulo
	do{
		c.SetCentroYCircunferencia(centro, radio);
		
		area_circunferencia = c.AreaCircunferencia();
		
		radio += INCR_RADIO;	//Se incrtementa el radio
		
	}while (area_circunferencia <= area_rectangulo);

	// SALIDAS
	
	cout<<"------------------------------------------------------"<<endl;
	cout<<"Circunferencia obtenida: "<<endl;
	cout <<"\tCentro: "
		 <<"["<<r.PuntoCentralRectangulo().x<<" , "
		 <<r.PuntoCentralRectangulo().y<<"]"<<endl;
	
	
	cout<<"\tRadio: "<<radio<<endl;
	cout<<"\tArea: "<<c.AreaCircunferencia()<<endl;
	
	
	
	return 0;

}
