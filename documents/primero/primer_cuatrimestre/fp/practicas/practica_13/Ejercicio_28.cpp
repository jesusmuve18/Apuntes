/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_28
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Reescribir la solución del ejercicio 6 de la Relación de Problemas V con 
	las nuevas definiciones de las clases Punto2D, Rectangulo y Circunferencia 
	desarrolladas en los ejercicios 26 y 27 de esta misma Relación de 
	Problemas
	
	Entradas: Puntos extremos de la diagonal de un rectángulo
	
	Salidas: Características de una circunferencia cuyo área sea la siguiente 
			 mayor al área del rectángulo generado con saltos de 0.25 entre 
			 cada valor del radio.
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;

#define DATOS_EXTRA
/****************************************************************************/
//Declaración de Clases/Objetos (struct)

class Punto2D
{

private:
	
	double x; //coordenada x
	double y; //coordenada y

public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Punto2D(void): x(0), y(0) {};
	
	/**********************************************************************/
	//Constructor con argumentos
	
	Punto2D(double coord_x, double coord_y):
		x(coord_x),
		y(coord_y) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	//Modifica el valor de la coordenada x
	
	SetX(double x_nuevo)
	{
		x=x_nuevo;
	}
	
	/**********************************************************************/
	//Modifica el valor de la coordenada y
	
	SetY(double y_nuevo)
	{
		y=y_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve el valor de la coordenada x
	
	double GetX(void)
	{
		return x;
	}
	
	/**********************************************************************/
	//Devuelve el valor de la coordenada y
	
	double GetY(void)
	{
		return y;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Devuelve una cadena con las características del punto
	
	string ToString(void)
	{
		return ("("+to_string(x)+","+to_string(y)+")");
	}
	
};

class LectorPunto2D
{

private:
	
	string mensaje;
	
	/***********************************************************************/
	/************************************************************************
	  	Método: Lee real dependiendo de las condiciones que este debe tener
	  			 para ser considerado válido
		Parámetros: Título o etiqueta a modo de petición
		Devuelve: El entero en formato double
		
		Es un método privado ya que la intención de este lector no es ser un
		lector de enteros sino de puntos 2D
	.......................................................................*/
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
					
					cout<<"Caracter "<<caracteres_una_vez.at(j)<<" repetido"
						<<endl;
					
					#endif
				}
				
			}	
			
			#ifdef MOSTRAR_ERROR
					
			cout<<endl;
			
			#endif
			
		} while(!todo_ok);
		
		return stod(lectura);
	}
	
public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor con parámetros
	
	LectorPunto2D(string el_mensaje): mensaje(el_mensaje){};
	
	/**********************************************************************/
	//Constructor sin parámetros
	
	LectorPunto2D(void): mensaje("Introduce un real") {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Método Get
	
	//Devuelve el mensaje almacenado en el objeto
	string ImprimeMensaje(void)
	{
		return mensaje;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método Set
	
	//Establece un nuevo mensaje
	void SetMensaje(string mensaje_nuevo)
	{
		mensaje=mensaje_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método que lo define como clase lectora
	
	Punto2D Lee(void)
	{
		double coord_x;
		double coord_y;
		
		cout<<mensaje<<endl;
		coord_x=LeeReal("\tX: ");
		coord_y=LeeReal("\tY: ");
		
		return (Punto2D(coord_x, coord_y));
	}
	
	
};


class Rectangulo
{
private:
	
	Punto2D punto_1; //coordenadas de uno de los vértices del rectángulo
	Punto2D punto_2; //coordenadas del punto opuesto con respecto a la 
				     //diagonal del punto_1
				     
public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Rectangulo(void){}; 
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor con argumentos
	
	Rectangulo(Punto2D punto1, Punto2D punto2):
		punto_1(punto1),
		punto_2(punto2) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve la coordenada de la esquina superior izquierda
	
	Punto2D GetSupIzda(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()<punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()>punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina superior derecha
	
	Punto2D GetSupDcha(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()>punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()>punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina inferior izquierda
	
	Punto2D GetInfIzda(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()<punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()<punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina inferior derecha
	
	Punto2D GetInfDcha(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()>punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()<punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la longitud de la base
	
	double GetBase(void)
	{
		return (GetInfDcha().GetX()-GetInfIzda().GetX());
	}
	
	/**********************************************************************/
	//Devuelve la longitud de la altura
	
	double GetAltura(void)
	{
		return (GetSupDcha().GetY()-GetInfDcha().GetY());
	}
	
	/**********************************************************************/
	//Devuelve el perímetro
	
	double GetPerimetro(void)
	{
		return (2*GetBase()+2*GetAltura());
	}
	
	/**********************************************************************/
	//Devuelve el área 
	
	double GetArea(void)
	{
		return (GetBase()*GetAltura());
	}

	/**********************************************************************/
	//Cálculo del centro del rectángulo (cruce de diagonales)	
															
	Punto2D GetCentro()
	{
		Punto2D centro; //Dato que devolverá la función

		centro.SetX(GetInfIzda().GetX()+(GetBase()/2));
		centro.SetY(GetInfIzda().GetY()+(GetAltura()/2));
		
		return centro;
	}
	
	/**********************************************************************/
	//Método Set
	
	//Actualiza el rectángulo 
	
	SetRectangulo(Punto2D punto1, Punto2D punto2)
	{
		punto_1=punto1;
		punto_2=punto2;
	}
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve las características que definen al rectángulo
	
	string ToString(void)
	{	
	 	string str= "{" +GetSupIzda().ToString()+" , "
		 				+GetSupDcha().ToString()+"\n " 
						+GetInfIzda().ToString()+" , "
						+GetInfDcha().ToString()+"}";
						
		//Devuelve cada uno de los vértices en el orden del rectángulo				
		return str;
	}
	
	/**********************************************************************/
};

class Circunferencia
{
	
private:
	
	Punto2D centro; //coordenadas del centro de la circunferencia
	
	//PRE: radio > 0
	double radio;
	
	static const double RADIO_INICIAL;
	static const double SALTO_RADIO;
	static const double PI;
	
public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Circunferencia(void):
		centro({0,0}), 
		radio(RADIO_INICIAL){};
	
	/**********************************************************************/
	//Constructor con 1 argumento
	
	Circunferencia(Punto2D el_centro):
		centro(el_centro), 
		radio(RADIO_INICIAL) {};
	
	/**********************************************************************/
	//Constructor con 2 argumentos
	
	Circunferencia(Punto2D centro, double radio):
		centro(centro), 
		radio(radio) {};
		
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve las coordenadas del centro de la circunferencia
	
	Punto2D GetCentro(void)
	{
		return centro;
	}
	
	/**********************************************************************/
	//Devuelve el diámetro de la circunferencia
	
	double GetDiametro(void)
	{
		return (2*radio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Cálculo del área
	
	double GetArea(void)
	{
		return (PI*radio*radio);
	}
	
	/**********************************************************************/
	//Cálculo del perímetro
	
	double GetLongitud(void)
	{
		return (2*PI*radio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Verifica si un punto se encuentra contenido en la circunferencia
	
	bool Contiene(Punto2D punto)
	{
		bool contenido=(pow((centro.GetX()-punto.GetX()),2)
		 			  + pow((centro.GetY()-punto.GetY()),2)
		  			  <=pow(radio,2));
		
		return contenido;		
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Incremento del radio (en unidades)
	//PRE: si incremento < 0  -->  radio + incremento > 0
	
	void Aumenta(double incremento=SALTO_RADIO)
	{
		radio+=incremento;
	}
	
	/**********************************************************************/
	//Devuelve los datos que definen la circunferencia
	
	string ToString()
	{
		string str;
		str="\tCentro: (" +to_string(centro.GetX())+" , "
						  +to_string(centro.GetY())+
			")\n\tRadio: "+to_string(radio)+" unidades";
			
		return str;
	}
	/**********************************************************************/
	
};

//Constantes estáticas
const double Circunferencia::RADIO_INICIAL=0.5;
const double Circunferencia::SALTO_RADIO=0.25;
const double Circunferencia::PI=6*asin(0.5);

													
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	LectorPunto2D lector; //Inicializo la clase lectora
	

	//Entradas
	
	Rectangulo rect;
	
	//Leo 2 puntos que me definirán un rectángulo
	do
	{
		lector.SetMensaje("Introduce un vertice del rectangulo: ");
		Punto2D punto1=lector.Lee();
		
		lector.SetMensaje("Introduce el vertice opuesto al anterior: ");
		Punto2D punto2=lector.Lee();
		
		rect.SetRectangulo(punto1,punto2);
			
	} while(rect.GetArea()==0);
	

	Circunferencia circ(rect.GetCentro());
		
		
	//Cálculos

	while(circ.GetArea()<rect.GetArea())
	{
		circ.Aumenta(); //Aumenta el radio con el valor por defecto
	}
	
	//Salidas
	
	cout<<endl;
	cout<<"La circunferencia buscada tiene las siguientes caracteristicas:"
		<<endl;
		
	cout<<endl;
	
	cout<<circ.ToString()<<endl;
	
	#ifdef DATOS_EXTRA
	
	cout<<"\tArea: "<<circ.GetArea()<<" unidades al cuadrado"<<endl;
	cout<<endl;
	cout<<"(El rectangulo definido tiene un area de "<<rect.GetArea()
		<<" unidades al cuadrado)"<<endl;
		
	cout<<endl;
	cout<<rect.ToString()<<endl;
	
	#endif
	
	return 0;
}
