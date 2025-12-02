/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_31
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Considerar la clase ColeccionPuntos2D que se usará para almacenar y 
	gestionar una colección de datos de tipo Punto2D.
	
	Realizar un programa que lea del teclado un número indeterminado de datos 
	de tipo Punto2D de manera que termine la lectura si el usuario escribe 
	FIN cuando el programa le pide la abscisa de un nuevo punto. Los puntos 
	leídos los almacena en un objeto ColeccionPuntos2D.
	
	A continuación pide los datos necesarios, y crea un objeto Circunferencia,
	y finalmente muestra cuáles de los puntos almacenados en la colección
	ColeccionPuntos2D está en el círculo delimitado por la circunferencia.


	Entradas: • Puntos mediante coordenadas hasta introducir FIN
			  • Características de una circunferencia

	Salidas:  • Puntos de los introducidos que están contenidos en 
				la circunferencia también definida 

*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
//Declaración de tipos de datos

/****************************************************************************/
//Declaración de Clases/Objetos

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
	//Modifica el valor de ambas coordenadas
	
	SetPunto(double x_nuevo, double y_nuevo)
	{
		x=x_nuevo;
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
	
	/***********************************************************************/
	/************************************************************************
	  	Método: Lee real dependiendo de las condiciones que este debe tener
	  			 para ser considerado válido
		Parámetros: Título o etiqueta a modo de petición
		Devuelve: El entero en formato double
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
	
	/**********************************************************************/
	//Verifica si una cadena leída está en formato "double"
	
	bool EsReal(string lectura)
	{
		//#define MOSTRAR_ERROR
		
		string caracteres_inicio="+-1234567890.";
		string caracteres_resto="1234567890.";
		string caracteres_una_vez=".";
		
		bool todo_ok=true;
	
		
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
		
		
		return todo_ok;
	}
	
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


class ColeccionPuntos2D
{
	
private: 

	static const int MAX_PUNTOS=100;
	Punto2D coleccion[MAX_PUNTOS];
	int total_utilizados;

public:
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	ColeccionPuntos2D(void): 	total_utilizados(0) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Método Set
	
	//Sustituye un punto de la colección (indicado por índice) por otro nuevo
	// PRE: 0 <= indice < total_utilizados
	
	void SetPunto(int indice, Punto2D punto_nuevo)
	{
		coleccion[indice]=punto_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve un punto señalado por un índice
	// PRE: 0 <= indice < total_utilizados
	
	Punto2D GetPunto(int indice)
	{
		return coleccion[indice];
	}
	
	/**********************************************************************/
	//Devuelve el número de puntos que hay en la colección
	
	int GetNumeroPuntos(void)
	{
		return total_utilizados;
	}

	/**********************************************************************/
	/**********************************************************************/
	//Añade nuevos puntos al final de la colección
	// PRE: total_utilizados < MAX_PUNTOS
	
	Aniade(Punto2D punto_nuevo)
	{
		if(total_utilizados<MAX_PUNTOS)
		{
			coleccion[total_utilizados]=punto_nuevo;
			total_utilizados++;
		}
		
	}

	/**********************************************************************/
	//Elimina un punto de la colección y desplaza los demás para no dejar
	//huecos en blanco
	// PRE: 0 <= indice < total_utilizados
	
	Elimina(int indice)
	{
		if((indice>=0)&&(indice<total_utilizados)) {

            int tope=total_utilizados-1; // posic. del último

            for(int i=indice; i<tope; i++)
            {
            	coleccion[i]=coleccion[i+1];
			}
               

            total_utilizados--;
        }
	}
	
	/**********************************************************************/
	// Inserta el punto "punto_nuevo" en la posición dada por "indice".
    // Desplaza todos los caracteres una posición a la derecha antes de 
	// copiar en "indice" en valor "punto_nuevo".
	// PRE: 0 <= indice < total_utilizados
    // PRE: total_utilizados < MAX_PUNTOS
    // 		La inserción se realiza si hay alguna casilla disponible.
    // 		Si no hay espacio, no se hace nada.
    
	void Inserta (int indice, Punto2D punto_nuevo)
	{
        if((indice>=0)&&(indice<total_utilizados)
			&&(total_utilizados<MAX_PUNTOS)) 
		{
		
			for(int i=total_utilizados; i>indice; i--)
			{
				coleccion[i]=coleccion[i-1];
			}

			coleccion[indice]=punto_nuevo;
			
			total_utilizados++;		
		}
	}
	
	/**********************************************************************/
	/**********************************************************************/
	// Compone un string con todos los caracteres que están
    // almacenados en la coleccioni y lo devuelve.

    string ToString()
    {
        string cadena=coleccion[0].ToString();
	
        for (int i=1; i<total_utilizados; i++)
        {
        	cadena+=" , "+coleccion[i].ToString();
		}

        return (cadena);
    }
    
    /**********************************************************************/
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	const string TERMINADOR="FIN";
	
	LectorPunto2D lector;
	
	ColeccionPuntos2D coleccion;
	
	
	Punto2D centro;
	double radio;
	
	Punto2D aux;
	int contador=0;
	string str_abscisa;
	
	double abscisa, ordenada; //Variables auxiliares
	
	/*********************************************************************/
	
	cout<<"Para finalizar la lectura introduzca "<<TERMINADOR
		<<" como valor de la abcisa de un punto"<<endl;
	cout<<endl;
	
	
	do //Relleno la coleccción de puntos hasta introducir el terminador
	{
		contador++;
		
		cout<<"Punto "<<contador<<":"<<endl;
		
		do
		{
			cout<<"\tX:";
			getline(cin,str_abscisa);
				
		} while(str_abscisa!=TERMINADOR && !lector.EsReal(str_abscisa));
		
		if(str_abscisa!=TERMINADOR)
		{
			abscisa=stod(str_abscisa);
			ordenada=lector.LeeReal("\tY:");
			
			aux.SetPunto(abscisa,ordenada);
			
			coleccion.Aniade(aux);
		}
		
		
		
	} while(str_abscisa!=TERMINADOR);
	
	
	
	//Leo los Datos para generar la circunferencia
	
	cout<<endl;
	lector.SetMensaje("Centro de la circunferencia: ");
	centro=lector.Lee();
	
	do
	{
		radio=lector.LeeReal("Radio de la circunferencia: ");	
	} while (radio<=0);
	
	
	//Construyo la circunferencia con datos introducidos
	Circunferencia circ(centro, radio);
	
	
	//Muestro los puntos de la colección que están contenidos en la 
	//circunferencia
	
	cout<<endl;
	
	cout<<"Puntos que estan contenidos en la circunferencia: "<<endl;
	cout<<endl;
	
	for(int i=0; i<coleccion.GetNumeroPuntos(); i++)
	{
		if(circ.Contiene(coleccion.GetPunto(i)))
		{
			cout<<"\t"<<coleccion.GetPunto(i).ToString()<<endl;
		}
	}

	return 0;
}

