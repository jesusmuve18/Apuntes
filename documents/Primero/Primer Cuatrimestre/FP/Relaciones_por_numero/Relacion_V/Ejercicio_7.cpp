/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_7
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Recuperar las clases escritas en el ejercicio 6 de esta misma Relación de 
	Problemas. Escribir un programa que lea las coordenadas que definen un 
	rectángulo y calcule y muestre una serie de datos de tipo Circunferencia, 
	todas centrada en en el punto de corte de las diagonales del rectángulo.
	
	Las circunferencias en las que estamos interesadas serán todas las 
	circunferencias inscritas en el rectángulo. Para ello comience con una 
	circunferencia de radio radio=0.5 y vaya incrementando su valor 0.25 en 
	cada iteración.
	
	Muestre cuántas circunferencias se han generado y a continuación, sus 
	propiedades. Por ejemplo, dado el rectángulo de la figura 40, los objetos 
	Circunferencia que nos interesan son tres (la circunferencia con trazo 
	discontinuo no se guardará porque no está inscrita en el rectángulo).
	
	Entradas: Puntos extremos de la diagonal de un rectángulo
	
	Salidas: Número de posibles circunferencias inscritas en el rectángulo
			 dado y sus características
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;

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

		//Cálculo de la base del rectángulo									
		double Base()
		{
			return abs(punto_1.x-punto_2.x);	
		}
		
		//Cálculo de la altura del rectángulo
		double Altura()
		{
			return abs(punto_1.y-punto_2.y);	
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
		
		double Diametro()
		{
			return 2*radio;
		}
	
};

/****************************************************************************/
/*****************************************************************************
  	Función: Lee decimal dependiendo de las condiciones que este debe tener
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
	
	/*
		RADIO_INICIAL+SALTO_RADIO*num_circ = Altura/2   ==>
		
		==> SALTO_RADIO*num_circ = Altura/2 - RADIO_INICIAL  ==>
		
		                Altura/2 - RADIO_INICIAL
		==> num_circ = --------------------------
		                      SALTO_RADIO
		                      
		Teniendo en cuenta la circunferencia de radio inicial que sería contar
		el num_circ como 0 tendremos que sumarle 1 a este resultado
		
		De igual manera sucederá con el ancho
		
		            Ancho/2 - RADIO_INICIAL
		    num_circ = -------------------------- +1
		                      SALTO_RADIO
		
	*/
	int num_circ=(((rect.Altura()/2)-RADIO_INICIAL)/SALTO_RADIO)+1;
	
	if(((((rect.Base()/2)-RADIO_INICIAL)/SALTO_RADIO)+1)<num_circ)
	{
		num_circ=((((rect.Base()/2)-RADIO_INICIAL)/SALTO_RADIO)+1);
	}
	
	
	//Salidas
	
	if(num_circ>=1)
	{
		cout<<"Hay "<<num_circ<<" posibles circunferencias inscritas"<<endl;
		
		cout<<endl;
		cout<<"--------------------------------"<<endl;
		cout<<endl;
		
		int contador=1;
		
		while(circ.Diametro()<=rect.Altura())
		{
			cout<<"Circunferencia "<<contador<<": "<<endl;
			cout<<circ.ToString()<<endl;
			
			cout<<endl;
			cout<<"................................"<<endl;
			cout<<endl;
			
			circ.Aumenta(SALTO_RADIO);
			contador++;
		}	
	}
	else
	{
		cout<<"No hay ninguna posible circunferencia inscrita"<<endl;
	}
	
	
	return 0;
}
