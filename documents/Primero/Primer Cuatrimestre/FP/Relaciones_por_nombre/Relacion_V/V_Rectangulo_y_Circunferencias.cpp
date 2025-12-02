/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_6
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Para modelar un rectángulo se usará la clase Rectangulo. Un 
	rectángulo se caracterizará por la coordenada superior izquierda y la 
	coordenada inferior derecha (ambos son datos struct de tipo Punto2D).
	
	Para modelar una circunferencia se usará la clase Circunferencia. Una 
	circunferencia se caracterizará por la coordenada del centro (un dato 
	struct de tipo Punto2D) y el radio.
	
	Escribir un programa que lea las coordenadas que definen un rectángulo 
	y calcule la circunferencia centrada en el punto de corte de las diagonales 
	del rectángulo tal que su superficie sea la menor entre todas las 
	circunferencias de área mayor que la del rectángulo.
	
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

#define DATOS_EXTRA
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

/****************************************************************************/
//Declaración de Clases/Objetos (struct)

class Rectangulo
{
	private:
		
		Punto2D punto_1; //coordenadas de uno de los vértices del rectángulo
		Punto2D punto_2; //coordenadas del punto opuesto con respecto a la 
					     //diagonal del punto_1
					     
	public:
		
		//Constructor sin parámetros
		//PRE: punto1.x, punto1.y, punto2.x, punto2.y >=0
		Rectangulo(){}; 
		
		//Constructor con parámetros
		//PRE: punto1.x, punto1.y, punto2.x, punto2.y >=0
		Rectangulo(Punto2D punto1, Punto2D punto2):
			punto_1(punto1),
			punto_2(punto2) {};
			
		
		//Cálculo del centro del rectángulo (cruce de diagonales)															
		Punto2D Centro()
		{
			Punto2D centro; //Dato que devolverá la función
			
			double base=abs(punto_1.x-punto_2.x);
			double altura=abs(punto_1.y-punto_2.y);
			
			centro.x=min(punto_1.y,punto_2.y)+(base/2);
			centro.y=min(punto_1.x,punto_2.x)+(altura/2);
			
			return centro;
		}

													
		//Cálculo del área del rectángulo													
		double Area()
		{
			double base=abs(punto_1.x-punto_2.x);
			double altura=abs(punto_1.y-punto_2.y);
			
			return base*altura;
		}
		
		string ToString()
		{
			string str;
			
			str="Punto 1: ("+to_string(punto_1.x)+" , "+to_string(punto_1.y)
				+")\nPunto 2: ("+to_string(punto_2.x)+" , "
				+to_string(punto_2.y)+")";
				
			return str;
		}
};

class Circunferencia
{
	private:
		
		Punto2D centro; //coordenadas del centro de la circunferencia
		double radio;
		
	public:
		
		//Constructor con 1 parámetro
		Circunferencia(Punto2D centro):centro(centro), radio(RADIO_INICIAL){};
		
		//Constructor con 2 parámetros
		Circunferencia(Punto2D centro, double radio):
			centro(centro), 
			radio(radio) {};
		
		//Cálculo del área
		double Area()
		{
			return (PI*radio*radio);
		}
		
		//Incremento del radio
		void Aumenta(double incremento)
		{
			radio+=incremento;
		}
		
		string ToString()
		{
			string str;
			str="\tCentro: ("+to_string(centro.x)+" , "+to_string(centro.x)+
				")\n\tRadio: "+to_string(radio)+" unidades";
			return str;
		}
	
};

/****************************************************************************/
/*****************************************************************************
  	Función: Lee real dependiendo de las condiciones que este debe tener
  			 para ser considerado válido
	Parámetros: Título o etiqueta a modo de petición
	Devuelve: El entero en formato double
............................................................................*/
double LeeReal(string titulo)
{
	//#define MOSTRAR_ERROR
	
	string lectura;
	string caracteres_inicio="+-1234567890.";
	string caracteres_resto="1234567890.";
	string caracteres_una_vez=".";
	
	bool todo_ok;


	do
	{
		do
		{
			cout<<titulo;
			getline(cin,lectura);
			
			string aux;
			
			for(int i=0; i<lectura.length(); i++) //Le quito los espacios
			{
				if(lectura.at(i)!=' ')
				{
					aux+=lectura.at(i);
				}
			}
			
			lectura=aux;	
			
		} while(lectura.empty()); //Mientras no haya caracteres
	
		
		todo_ok=true;
		
		bool caracter_correcto;
		int siguiente_caracter=0;
		
		if(lectura.length()>1) //Si hay mas de un caracter
		{
			//Compruebo el primer caracter

			for(int i=0; i<caracteres_inicio.length(); i++)
			{
				if(lectura.at(0)==caracteres_inicio.at(i))
				{
					caracter_correcto=true;
				}
			}
			
			if(!caracter_correcto)
			{
				todo_ok=false;
				
				#ifdef MOSTRAR_ERROR
					
				cout<<"Caracter inicio incorrecto"<<endl;
				
				#endif
			}	
			
			siguiente_caracter=1;
		}
		
		
		//Compruebo el resto de caracteres

		
		for(int i=siguiente_caracter; i<lectura.length(); i++)
		{
			caracter_correcto=false;
			
			for(int j=0; j<caracteres_resto.length(); j++)
			{
				if(lectura.at(i)==caracteres_resto.at(j))
				{
					caracter_correcto=true;
				}
			}
			
			if(!caracter_correcto)
			{
				todo_ok=false;
				
				#ifdef MOSTRAR_ERROR
				
				cout<<"Caracter "<<i+1<<" incorrecto"<<endl;
				
				#endif
			}
			
		}	
		
		//Compruebo si hay caracteres repetidos que no deberían
		
		int contador=0;
		
		for(int j=0; j<caracteres_una_vez.length(); j++)
		{
			contador=0;
			
			for(int i=0; i<lectura.length(); i++)
			{
				if(lectura.at(i)==caracteres_resto.at(j))
				{
					contador++;
				}
			}
			
			if(contador>1)
			{
				todo_ok=false;
				
				#ifdef MOSTRAR_ERROR
				
				cout<<"Caracter "<<caracteres_una_vez.at(j)<<" repetido"<<endl;
				
				#endif
			}
			
		}	
		
		#ifdef MOSTRAR_ERROR
				
		cout<<endl;
		
		#endif
		
	} while(!todo_ok);
	
	return stod(lectura);
}
														
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	Punto2D punto1, punto2;
	bool todo_ok;
	
	//Entradas
	
	cout<<"Puntos que definen un rectangulo "
		<<"(puntos de extremos de una diagonal):"<<endl;
	
	
	cout<<"     Punto 1: "<<endl;
	punto1.x=LeeReal ("\tCoordenada X: ");
	punto1.y=LeeReal ("\tCoordenada Y: ");
	
	cout<<"    Punto 2: "<<endl;
	punto2.x=LeeReal ("\tCoordenada X: ");
	punto2.y=LeeReal ("\tCoordenada Y: ");
	
	cout<<"--------------------------------"<<endl;
	cout<<endl;
	
	Rectangulo rect(punto1,punto2);
	Circunferencia circ(rect.Centro());
		
	//Cálculos
	
	
	while(circ.Area()<rect.Area())
	{
		circ.Aumenta(SALTO_RADIO);
	}
	
	//Salidas
	
	cout<<"La circunferencia buscada tiene las siguientes caracteristicas:"
		<<endl;
	
	cout<<circ.ToString()<<endl;
	
	#ifdef DATOS_EXTRA
	
	cout<<"\tArea: "<<circ.Area()<<" unidades al cuadrado"<<endl;
	cout<<endl;
	cout<<"(El rectangulo definido tiene un area de "<<rect.Area()
		<<" unidades al cuadrado)"<<endl;
	
	#endif
	
	return 0;
}
