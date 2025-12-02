
/***************************************************************************/

#include <iostream>		// Inclusión de los recursos de E/S
#include <cmath>		// Inclusión de los recursos matemáticos
using namespace std;

/***************************************************************************/
/***************************************************************************/

// Declaramos los datos de tipo struct que se van a emplear

// Punto en un plano

struct Punto2D {
	double x, y;	// abscisa y ordenada de un punto
	
} inferior_der, inferior_izq, superior_izq, superior_der,  //Vértices rectángulo
  centro_circ;		// Centro de la circunferencia
  

// Rectángulo en un plano

struct Rectangulo {
	double base, altura, area;	// Lados que definen un rectángulo
} rectangulo;


// Circunferencia centrada en el punto de corte de las diagonales del 
// rectángulo situado en un plano
// PRE: radio > 0

struct Circunferencia {
	double radio, area;
} circunferencia;


/***************************************************************************/
/***************************************************************************/

// Función Area_rectangulo: calcula el área de un rectángulo.
// Parámetros: base y altura del rectángulo.
// Devuelve: área del rectángulo.
// PRE : (inferior_der.x > superior_izq.x) && (inferior_der.y < superior_izq.y)

double Area_rectangulo (Punto2D inferior_der, Punto2D superior_izq) {
	
	// Calculo de los vertices del rectángulo
	// Declaramos los vertices que faltan por definir del rectángulo
	
	Punto2D inferior_izq, superior_der;
	
	// Definimos dichos vértices como sigue
	
	// Coordenada inferior izquierda
	inferior_izq.x = superior_izq.x;
	inferior_izq.y = inferior_der.y;
	
	// Coordenada superior derecha
	superior_der.x = inferior_der.x;
	superior_der.y = superior_izq.y;
	
	
	// Calculo de los lados del rectángulo
	// Calculamos la longitud de la base
	rectangulo.base = inferior_der.x - inferior_izq.x;
	
	// Calculamos la longitud de la altura
	rectangulo.altura = superior_der.y - inferior_der.y;
	
	// Calculamos el área del rectángulo
	rectangulo.area = rectangulo.base * rectangulo.altura;
	
	return (rectangulo.area);

} // double Area_rectangulo


/***************************************************************************/
/***************************************************************************/

// Función Area_circunferencia: calcula el área de una circunferencia, tal que
// su superficie es la menor entre todas las circunferencias de área mayor que 
// la del rectángulo.
// Parámetros: area del rectángulo.
// Devuelve: área de la circunferencia.

double Area_circunferencia (Circunferencia circunferencia,Rectangulo rectangulo) {
	
	// Declaramos las constantes

	const double PI = 6 * asin (0.5);
	const double AUMENTO = 0.25;
	
	// Inicialiazamos el valor del radio
	
	circunferencia.radio = 0.5;	
	
	bool seguir = true;
	
	// LLamamos a la función Area_rectangulo
	//rectangulo.area = Area_rectangulo (inferior_der, superior_izq);
yy
	
	while (seguir) {
		
		// Calculamos el area de la circunferencia
		circunferencia.area = PI * circunferencia.radio * circunferencia.radio;
		
		if (circunferencia.area < rectangulo.area) {
			
			seguir = true;
			
			// Aumento el valor del radio cuando area_circ < area_rect
			circunferencia.radio = circunferencia.radio + AUMENTO;
		}
		
		else {
			
			seguir = false;
		}
		
	} // while (seguir)
	
	return (circunferencia.area);
	
} // double Area_circunferencia


/***************************************************************************/
/***************************************************************************/

// Función Centro_circunferencia: calcula el centro de la circunferencia, 
// Parámetros: coordenadas que carcaterizan a un rectángulo 
// (inferior_der, superior_izq).
// Devuelve: centro de la circunferencia.

Punto2D Centro_circunferencia (Punto2D inferior_der, Punto2D superior_izq) {
	
	// Sabemos que el punto de corte de las diagonales del rectángulo es igual 
	// al punto medio de una de las diágonales
	
	// Calculamos el punto medio de la diagonal formada por los vertices que
	// introduce el usuario
	
	// Abscisa
	
	centro_circ.x = (inferior_der.x + superior_izq.x) / 2;
	
	// Ordenada
	
	centro_circ.y = (inferior_der.y + superior_izq.y) / 2;
	
	return centro_circ;
	
} // Punto2D Centro_circunferencia


/***************************************************************************/
/***************************************************************************/

	
int main()		// Programa principal
{
	
	// Declaramos las variables
	
	Punto2D inferior_der, superior_izq;		// Coordenadas que carcaterizan a 
											// un rectángulo
	
	// Lectura
	
	cout << endl;
	cout << "Coordenadas que caracterizan a un rectangulo:" << endl;
	
	// El valor de las abscisa y de la ordenada deben ser diferentes ya que 
	// se trata de los vertices opuestos de un rectángulo.
	
	bool es_recta = ((inferior_der.x == superior_izq.x) || 
					 (inferior_der.y == superior_izq.y));
					 
	do {
		cout << "\t Coordenada inferior derecha: " << endl;
		cout << "\t Abscisa: ";
		cin >> inferior_der.x;
		cout << "\t Ordenada: ";
		cin >> inferior_der.y;
		cout << endl;
		
		cout << "\t Coordenada superior izquierda: " << endl;
		cout << "\t Abscisa: ";
		cin >> superior_izq.x;
		cout << "\t Ordenada: ";
		cin >> superior_izq.y;
		cout << endl;
	} while (es_recta);
	
	// Calculos
	
	Punto2D centro_circ = Centro_circunferencia (inferior_der, superior_izq);

	
	double area_rect = Area_rectangulo (inferior_der, superior_izq);
	double area_circ = Area_circunferencia (circunferencia, rectangulo);
	
	// Salidas
	
	cout << endl;
	cout << "El area del rectangulo es: " << area_rect << endl;
	cout << "El area de la circunferencia con centro (" << centro_circ.x 
		 << " , " << centro_circ.y << ") es: " << area_circ;
	cout << endl;
	
	
	return 0;
}
