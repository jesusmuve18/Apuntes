/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_55
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Se va a dotar de mayor funcionalidad al robot desarrollado en el ejercicio 
	37 de esta misma Relación de Problemas para que se mueva en un tablero de 
	10 filas y 10 columnas.
	
	Inicialmente, el robot se ubica en la posición pos, siendo pos una pareja 
	de enteros positivos (f, c) donde 1 = f = 10 y 1 = c = 10. Luego, el robot 
	ejecuta una serie de órdenes, indicadas mediante un array ord de tipo 
	char, con longitud lon. Cada orden es una letra 'I', 'D', 'A' o 'B' 
	indicando si el robot se mueve a la izquierda, derecha, arriba o abajo. 
	Las posiciones válidas del robot cumplen que para cada posición pos sus 
	coordenadas (f, c) son correctas. Se dice que una serie de órdenes es 
	correcta si el robot nunca se sale de las posiciones válidas.
	
	Se pide implementar un programa que lea la posición inicial pos (fila y 
	columna, por separado) y la secuencia de órdenes a ejecutar. Le 
	recomendamos que use un dato string, p.e. cad_ordenes para la lectura. 
	Si el string es correcto, el array ord se rellenará a partir de 
	cad_ordenes, siendo lon la longitud de cad_ordenes.
	
	Después de la lectura de los datos haga lo siguiente:
	
	• Si la serie de órdenes es correcta, muestre cuántas veces se visitó cada 
		posición.Use una representación bidimensional (figura 30).
	• Si la serie de órdenes NO es correcta, el programa terminará indicando 
		cuántas órdenes se pudieron ejecutar.
		
	En la figura 30 mostramos el resultado de ejecutar el programa empezando 
	por la posición (5, 5) y con la secuencia de órdenes IIAAAADDBDDDBB

	
	Entradas: Coordenadas iniciales (fila y columna)
			  Serie de órdenes entre las establecidas
			  Numero de órdenes a ejecutar
	
	Salidas: Número de órdenes que se pudieron ejecutar
			 Matriz de posiciones del robot
	
*/
/****************************************************************************/
#include<iostream>
#include<iomanip>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int X_MIN=1;
const int X_MAX=10;

const int Y_MIN=1;
const int Y_MAX=10;

const char RELLENO='.';

/****************************************************************************/
//Declaración de tipos de datos

struct Posicion
{
	int x;
	int y;
};

/*****************************************************************************
  	Función: Lee entero dependiendo de las condiciones que este debe tener
  			 para ser considerado válido
	Parámetros: Título o etiqueta a modo de petición
	Devuelve: El entero en formato int		
............................................................................*/
double LeeEntero(string titulo)
{
	//#define MOSTRAR_ERROR
	
	string lectura;
	string caracteres_inicio="+-1234567890";
	string caracteres_resto="1234567890";
	
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
		
		#ifdef MOSTRAR_ERROR
				
		cout<<endl;
		
		#endif
		
	} while(!todo_ok);
	
	return stoi(lectura);
}


/*****************************************************************************
  	Función: Comprueba si la string órdenes contiene instrucciones correctas
	Parámetros: string ordenes
	Devuelve: validez o no de las ordenes en formato booleano
............................................................................*/
bool OrdenesOk(string ordenes)
{
	bool ordenes_ok=true; //Dato que devolverá la función
	string ordenes_permitidas="DIAB";
	bool aux;
	
	for(int i=0; i<ordenes.length(); i++)
	{
		aux=false;
		
		for(int j=0; j<ordenes_permitidas.length(); j++)
		{

			if(toupper(ordenes.at(i))==ordenes_permitidas.at(j))
			{
				aux=true;
			}
			
		}

		if(!aux && ordenes_ok)
		{
			ordenes_ok=false;
		}
	}
	
	return ordenes_ok;
}

/*****************************************************************************
  	Función: Traduce la orden en una coordenada a sumar a la anterior
	Parámetros: caracter orden
	Devuelve: dupla x,y segun la orden puede ser (0,1),(0,-1),(-1,0),(1,0)
............................................................................*/
Posicion Valor(char a)
{
	char b=toupper(a);
	Posicion valor;
	
	valor.x=0;
	valor.y=0;
	
	switch(b)
	{
		case 'D':
			valor.x= 1;
			break;
			
		case 'I':
			valor.x= -1;
			break;
			
		case 'A':
			valor.y= 1;
			break;
			
		case 'B':
			valor.y= -1;
			break;
	}
	
	return valor;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	Posicion posicion;
	int longitud;
	string ordenes;
	
	char recorrido[X_MAX-X_MIN+1][Y_MAX-Y_MIN+1]; //recorrido[x][y]
	string serie;

	//Entradas

	do
	{
		cout<<"Posicion inicial: "<<endl;
		posicion.y=LeeEntero("\tFila: ");
		posicion.x=LeeEntero("\tColumna: ");
		
	} while((posicion.x<X_MIN || posicion.x>X_MAX) ||
		   (posicion.y<Y_MIN || posicion.y>Y_MAX));
	
	do
	{
		cout<<"Ordenes: ";
		cin>>ordenes;
		
	} while (!OrdenesOk(ordenes));
	
	cin.ignore(); //vacío el buffer para evitar errores
	
	do
	{
		longitud=LeeEntero("Numero de ordenes: "); 
		
	} while (longitud<=0 || longitud>ordenes.length());

	//Cálculos
	
	for(int x=0; x<X_MAX-X_MIN+1 ; x++) //Relleno la matriz del recorrido
	{
		for(int y=0; y<X_MAX-X_MIN+1 ; y++)
		{
			recorrido[x][y]=RELLENO;
		}
	}
	
	
	
	Posicion aux=posicion;
	int contador=0;
	
	recorrido[posicion.x-1][posicion.y-1]='1'; //añado la posición inicial
	
	while((aux.x>=X_MIN && aux.x<=X_MAX)&&(aux.y>=Y_MIN && aux.y<=Y_MAX)
										&&(contador<longitud))
	{
		/*Interpreto cada una de las órdenes hasta ejecutar el número dado
		  en el número de órdenes o hasta salirme de la matriz gráfica */
		  
		aux.x+=Valor(ordenes.at(contador)).x;
		aux.y+=Valor(ordenes.at(contador)).y;
		
		if(recorrido[aux.x-1][aux.y-1]==RELLENO)
		{
			recorrido[aux.x-1][aux.y-1]='1';
		}
		else
		{
			recorrido[aux.x-1][aux.y-1]+=1;
		}
		
		contador++;
		
	}
	
	if(contador==longitud)
	{
		serie="correcta";
	}
	else //si no se han ejecutado todas las órdenes
	{
		serie="incorrecta";
	}
	
	//Salidas
	
	cout<<endl;
	cout<<"------------------------"<<endl;
	cout<<endl;
	
	cout<<"La Serie de ordenes es: "<<serie<<endl;
	cout<<"Se ejecutaron "<<contador<<" ordenes"<<endl;
	
	cout<<endl;
	
	//Muestro los índices superiores y el separador
	
	cout<<"   : ";
	
	for(int i=1; i<=X_MAX-X_MIN+1; i++)
	{
		cout<<setw(2)<<i<<" ";
	}
	cout<<endl;
	
	
	cout<<"   -";
	
	for(int i=0; i<X_MAX-X_MIN+1; i++)
	{
		cout<<"---";
	}
	cout<<"--"<<endl;
	
	
	for(int y=0; y<Y_MAX-Y_MIN+1 ; y++) //Muestro la matriz del recorrido
	{
		cout<<setw(2)<<Y_MAX-Y_MIN+1-y<<" : ";
		
		for(int x=0; x<X_MAX-X_MIN+1 ; x++)
		{
			cout<<recorrido[x][Y_MAX-Y_MIN-y]<<"  ";
		}
		
		cout<<": "<<setw(2)<<Y_MAX-Y_MIN+1-y<<endl;
	}
	
	//Muestro el separador y los índices inferiores
	
	cout<<"   -";
	
	for(int i=0; i<X_MAX-X_MIN+1; i++)
	{
		cout<<"---";
	}
	cout<<"--"<<endl;
	
	cout<<"   : ";
	
	for(int i=1; i<=X_MAX-X_MIN+1; i++)
	{
		cout<<setw(2)<<i<<" ";
	}
	cout<<endl;
	
	return 0;
}

