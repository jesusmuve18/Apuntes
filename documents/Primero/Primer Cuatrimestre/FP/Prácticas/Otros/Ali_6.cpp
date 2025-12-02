/****************************************************************/
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
/****************************************************************/

#include <iostream>	// Inclusión de los recursos E/S
#include <cmath>
using namespace std;

/****************************************************************/

const double PI = 3.1415926;
/****************************************************************/

struct Punto2D 
{
	double x, y;	// abscisa y ordenada de un punto
};

/****************************************************************/
/****************************************************************/

class Rectangulo{
	
private: 

 	Punto2D superior_izq, inferior_dch; 

public:


/***************************************************************************/
//Lee las coordenadas. 
//Devuelve la esquina superior izq.
/***************************************************************************/
	void EsquinaSuperiorIzq ()
	{
		cout <<"Punto superior izq"<<endl;
		cout<<endl;
		
		cout << "Coordenada X del punto : ";
		cin >> superior_izq.x; 
	
		cout << "Coordenada Y del punto : ";
		cin >> superior_izq.y; 
	}

/***************************************************************************/
//Lee las coordenadas. 
//Devuelve la esquina inferior dch.
/***************************************************************************/
	void EsquinaInferiorDch ()
	{
		cout <<"Punto inferior dch"<<endl;
		cout<<endl;
		
		cout << "Coordenada X del punto : ";
		cin >> inferior_dch.x; 
	
		cout << "Coordenada Y del punto : ";
		cin >> inferior_dch.y; 
	}
	
/***************************************************************************/
// Devuelve la base de un rectángulo.
/***************************************************************************/

	double BaseRectangulo ()
	{	
		return (inferior_dch.x 
		      - superior_izq.x);
	}

/***************************************************************************/
// Devuelve: el alto del rectángulo "r".
/***************************************************************************/

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
		Punto2D p;	
		
		p.x = superior_izq.x + BaseRectangulo();
		p.y = superior_izq.y;
		 
		return (p);
	}
	
/***************************************************************************/
// Devuelve el punto inferior izquierda  de un rectángulo.
/***************************************************************************/

	Punto2D EsquinaInferiorIzq ()
	{
		Punto2D p;	
		
		p.x = superior_izq.x;
		p.y = superior_izq.y - AlturaRectangulo();
		 
		return (p);
	}

/***************************************************************************/
// Calcula el área de un rectángulo.
/***************************************************************************/

	double AreaRectangulo ()
	{
		double base = fabs(BaseRectangulo());
		double altura = fabs(AlturaRectangulo());
		return (base * altura);
	}


/***************************************************************************/
// Calcula y devuelve el punto central de un rectángulo.
/***************************************************************************/

	Punto2D PuntoCentralRectangulo ()
	{
		Punto2D p;	
		
		p.x = (superior_izq.x 
		     + EsquinaSuperiorDch().x) / 2.0;
		p.y = (superior_izq.y 
		     +  EsquinaInferiorIzq().y) / 2.0;
		 
		return (p);
	}
	
/***************************************************************************/
//Las siguientes funciones devuelven valores (getters)
/***************************************************************************/

	Punto2D GetEsquinaSI()
	{
		return superior_izq;
	}
};

class Circunferencia{
	
private: 

	Punto2D centro; // Centro
	double radio;   // Radio

public:
	

/***************************************************************************/
// Calcula el área del círculo interior a una circunferencia.
//
// Parámetros: c, la circunferencia que se va a a usar para calcular el área 
//		 	  del círculo. 
// Devuelve: El área del circulo interior a "c".
/***************************************************************************/

	double AreaCircunferencia ()
	{
		
		return (PI * radio * radio);
	}

/***************************************************************************/
// Establece el centro de la circunferencia.
// Parámetros: p Punto2D
/***************************************************************************/
	void SetCentroYCircunferencia(Punto2D p , double _r)
	{
		centro= p;
		radio = _r;
	}



};



int main()	// Programa principal
{

	const double RADIO_INIC = 0.5;  // Valor del radio inicial
	const double INCR_RADIO = 0.25;	// Incremento del valor del radio


	cout.setf(ios::fixed);		// Notación de punto fijo para los reales
	cout.setf(ios::showpoint);	// Mostrar siempre decimales 

	// Lectura de las coordenadas inferior derecha y superior izquierda
	
	Rectangulo r; 
	
	r.EsquinaSuperiorIzq();
	r.EsquinaInferiorDch();
	
	// Coordenadas del rectángulo 
	
	/*cout << endl;		
	cout << "Punto sup.izda. del rectangulo = " 
		 <<"["<<r.GetEsquinaSI().x<<" , "
		 <<r.EsquinaSuperiorIzq().y<<"]"<< endl;
	cout << "Punto sup.dcha. del rectangulo = " 
	     <<"["<<r.EsquinaSuperiorDch().x<<" , "
		 <<r.EsquinaSuperiorDch().y<<"]"<< endl;	
	cout << "Punto inf.izda. del rectangulo = " 
	     <<"["<<r.EsquinaInferiorIzq().x<<" , "
		 <<r.EsquinaInferiorIzq().y<<"]"<< endl;
	cout << "Punto inf.dcha. del rectangulo = " 
	     <<"["<<r.EsquinaInferiorDch().x<<" , "
		 <<r.EsquinaInferiorDch().y<<"]"<< endl;
	cout << endl;		
	*/
	Rectangulo base, altura;	
	r.PuntoCentralRectangulo ();
	

	base.BaseRectangulo ();
	altura.AlturaRectangulo ();

	Circunferencia c;
	Punto2D centro = r.PuntoCentralRectangulo (); 
	double radio = RADIO_INIC;
	
	c.SetCentroYCircunferencia(centro, radio);
	
	

	double area_rectangulo = r.AreaRectangulo();
	cout<<"Area del rectangulo: "<<r.AreaRectangulo()<<endl;
	double area_circunferencia;
	
	do{
		c.SetCentroYCircunferencia(centro, radio);
		
		area_circunferencia = c.AreaCircunferencia();
		
		radio += INCR_RADIO;
		
	}while (area_circunferencia <= area_rectangulo);

	// SALIDAS
	
	cout<<"------------------------------------------------------"<<endl;
	cout<<"La circunferencia minima de area que el rectangulo: "<<endl;
	cout<<"\tCentro: "<<r.PuntoCentralRectangulo().x<<r.PuntoCentralRectangulo().y<<endl;
	
	
	cout<<"\tRadio: "<<radio<<" uds"<<endl;
	cout<<"\tArea: "<<c.AreaCircunferencia()<<" uds^2"<<endl;
	
	
	
	

}
