/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_14
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:

	Encapsule en la clase Lector las funciones escritas en los ejercicios 
	17 y 18 de la Relación de Problemas III. El objetivo es disponer de una 
	clase (Lector) que ofrezca un conjunto de métodos seguros de lectura 
	etiquetada de datos int y double.
	
	Deberá deducir la estructura de la clase a partir del siguiente ejemplo. 
	Los datos double se leerán de manera similar.
	
			// Lectura de un entero
			
			Lector lector_temp("Temperatura: ");
			int temp = lector_temp.LeeEntero();
			
			cout << "Temperatura leida = " << temp << endl;
			
			// Lectura de enteros acotados superior e inferiormente
			
			Lector lector_hora;
			
			lector_hora.SetTitulo ("Introduzca hora: ");
			int h = lector_hora.LeeEnteroEnRango (0, 23);
			
			lector_hora.SetTitulo ("Introduzca minuto: ");
			int m = lector_hora.LeeEnteroEnRango (0, 59);
			
			lector_hora.SetTitulo ("Introduzca segundo: ");
			int s = lector_hora.LeeEnteroEnRango (0, 59);
			
			cout << "Hora: " << h << ":" << m << ":" << s << endl;
			
			// Lectura de un entero acotado inferiormente
			
			Lector lector_ingreso("Cantidad a ingresar: ");
			
			int ingreso = lector_ingreso.LeeEnteroMayorOIgual (0);
			
			cout << "Valor del ingreso = " << ingreso << endl;
	

*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;

//#define MOSTRAR_ERROR   	//Muestra la razón por la que no es válido el valor
							//Introducido en las funciones de lectura

/****************************************************************************/
//Declaración de Clases/Objetos

class Lector
{
	private:
		
		string etiqueta;
	
	public:
		
		//Lee real devolviendo un valor double
		double LeeReal()
		{
			
			string lectura;
			string caracteres_inicio="+-1234567890.";
			string caracteres_resto="1234567890.";
			string caracteres_una_vez=".";
			
			bool todo_ok;
			
			
			do
			{
				do
				{
					cout<<etiqueta;
					getline(cin,lectura);
					
					string aux;
					
					for(int i=0; i<lectura.length(); i++) //Le quito los 
														  //espacios
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
						
						cout<<"Caracter "<<caracteres_una_vez.at(j)
							<<" repetido"<<endl;
						
						#endif
					}
					
				}	
				
				#ifdef MOSTRAR_ERROR
						
				cout<<endl;
				
				#endif
				
			} while(!todo_ok);
			
			return stod(lectura);
		}
	
		
		//Lee real en un rango dado en los parámetros
		double LeeRealEnRango (double num1, double num2)
		{
			double aux;
			double min=num1, max=num2;
			
			if(max<min)
			{
				max=num1;
				min=num2;
			}
			
			do
			{
				aux=LeeReal();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<min || aux>max)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<min || aux>max);
			
			return aux;
		}
		
		
		//Lee real mayor o igual a un parámetro dado
		double LeeRealMayorOIgual (double ref)
		{
			double aux;
			
			do
			{
				aux=LeeReal();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<ref)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<ref);
			
			return aux;
		}
		
		
		//Lee entero devolviendo un valor int
		int LeeEntero()
		{
			
			string lectura;
			string caracteres_inicio="+-1234567890";
			string caracteres_resto="1234567890";
			
			bool todo_ok;
		
		
			do
			{
				do
				{
					cout<<etiqueta;
					getline(cin,lectura);
					
					string aux;
					
					for(int i=0; i<lectura.length(); i++) //Le quito los 
														  //espacios
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
				
				#ifdef MOSTRAR_ERROR
						
				cout<<endl;
				
				#endif
				
			} while(!todo_ok);
			
			return stoi(lectura);
		}
		
		
		//Lee entero en un rango dado en los parámetros
		int LeeEnteroEnRango (int num1, int num2)
		{
			int aux;
			int min=num1, max=num2;
			
			if(max<min)
			{
				max=num1;
				min=num2;
			}
			
			do
			{
				aux=LeeEntero();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<min || aux>max)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<min || aux>max);
			
			return aux;
		}
		
		
		//Lee entero mayor o igual a un parámetro dado
		int LeeEnteroMayorOIgual (int ref)
		{
			int aux;
			
			do
			{
				aux=LeeEntero();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<ref)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<ref);
			
			return aux;
		}
		
		
		//Constructor de un parámetro
		Lector(string titulo): etiqueta(titulo) {};
		
		
		//Constructor sin parámetros
		Lector() {};
		
		
		//Modificar Etiqueta
		void SetTitulo(string titulo)
		{
			etiqueta=titulo;
		}
		
};


/****************************************************************************/
/****************************************************************************/
int main()
{
	//Prueba del código ejemplo del enunciado:
	
	
	// Lectura de un entero
	
	Lector lector_temp("Temperatura: ");
	int temp = lector_temp.LeeEntero();	
	cout << "Temperatura leida = " << temp << endl;
	
	
	// Lectura de enteros acotados superior e inferiormente
	
	Lector lector_hora;
	
	lector_hora.SetTitulo ("Introduzca hora: ");
	int h = lector_hora.LeeEnteroEnRango (0, 23);
	
	lector_hora.SetTitulo ("Introduzca minuto: ");
	int m = lector_hora.LeeEnteroEnRango (0, 59);
	
	lector_hora.SetTitulo ("Introduzca segundo: ");
	int s = lector_hora.LeeEnteroEnRango (0, 59);
	
	cout << "Hora: " << h << ":" << m << ":" << s << endl;
	
	
	// Lectura de un entero acotado inferiormente
	
	Lector lector_ingreso("Cantidad a ingresar: ");
	
	int ingreso = lector_ingreso.LeeEnteroMayorOIgual (0);
	
	cout << "Valor del ingreso = " << ingreso << endl;

	return 0;
}

