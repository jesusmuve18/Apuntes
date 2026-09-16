/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_56
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Vamos a seguir trabajando con un robot similar al del ejercicio 54 de esta 
	misma Relación de Problemas. Ahora va a poder moverse por una región 
	rectangular el tablero de NUM_FILAS filas y NUM_COLS columnas.
	
	El objetivo es hacer que recorra todas las casillas del tablero pasando 
	una vez por cada casilla. Empezará por la casilla (1, 1) y desde allí se 
	desplazará, casilla a casilla, hasta cubrir todo el tablero.
	
	El programa solicitará la casilla inicial (fila y columna, por separado) y 
	generará una cadena de órdenes para: 
		1) mover el robot a la casilla (1, 1) 
		2) recorrer todo el tablero.
		
	El programa realizará las tareas descritas en el ejercicio 54 solo que 
	ahora, después de leer la posición inicial, la cadena de órdenes que debe 
	ejecutar el robot no se lee, sino que la genera el propio programa. 
	Deberá ser, evidentemente, correcta.
	
	En la figura 31 mostramos el resultado de ejecutar el programa empezando 
	por la posición (5, 5) en un tablero de 6 filas y 7 columnas. Las primeras 
	8 órdenes IIIIBBBB tienen como objetivo mover el robot a la casilla (1, 1). 
	Las restantes órdenes son las generadas para recorrer todo el tablero 
	(en este caso en zig-zag).
	
	Reescriba el algoritmo que genera las órdenes para que el recorrido sea en 
	espiral, recorriendo los bordes en primer lugar.
	
	Entradas: Posición inicial (fila y columna)
	
	Salidas: Cadena de órdenes y su longitud
			 Matriz del recorrido 	
			 
	
*/
/****************************************************************************/
#include<iostream>
#include<iomanip>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int NUM_FILAS=8;
const int NUM_COLS=20;

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
	string ordenes="";
	
	char recorrido[NUM_COLS][NUM_FILAS]; //recorrido[x][y]
	

	//Entradas
	
	cout<<"Espacio grafico de "<<NUM_COLS<<" columnas y "<<NUM_FILAS<<" filas"
		<<endl;
	
	cout<<endl;
	
	do
	{
		cout<<"Posicion inicial: "<<endl;
		posicion.y=LeeEntero("\tFila: ");
		posicion.x=LeeEntero("\tColumna: ");
		
	} while((posicion.x<1 || posicion.x>NUM_COLS) ||
		   (posicion.y<1 || posicion.y>NUM_FILAS));
	

	//Cálculos
	
	
	for(int x=0; x<NUM_COLS ; x++) //Relleno la matriz del recorrido
	{
		for(int y=0; y<NUM_FILAS ; y++)
		{
			recorrido[x][y]=RELLENO;
		}
	}
	
	
	
	Posicion aux=posicion;
	int contador=0;
	
	recorrido[posicion.x-1][posicion.y-1]='1'; //coloco la primera casilla
	
	while(aux.x>1) //Lo desplazo hacia la primera columna
	{
		
		aux.x--;
		ordenes+="I";
		
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
	
	while(aux.y>1) //Lo desplazo hacia la primera fila
	{
		
		aux.y--;
		ordenes+="B";
		
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
	
	//ya lo tengo posicionado en (1,1)
	
	int max_x=NUM_COLS;
	int min_x=1;
	
	int max_y=NUM_FILAS;
	int min_y=2;
	
	//Comienzo el algoritmo empleado para realizar la espiral
	/*
			• Vuelta 1:
			
								  Lado superior
							***********************
							*                     *
							*                     *
			Lado izquierdo	*                     * Lado derecho
							*                     *
							*                     *
			Empiezo aquí ->	***********************
								  Lado inferior
								  
								  
			• Vuelta 2: ('-': ya completado)
			
								  Lado superior
							-----------------------
							-*********************-
							-*                   *-
			Lado izquierdo	-*                   *-  Lado derecho
							-*                   *-
			Me quedé por ->	-*********************-                     
				aquí		-----------------------	
								  Lado inferior
								  
								  
			Sigo así hasta haber completado el rectángulo variando los 
			límites por vuelta											*/
			
	
	while((max_x>=min_x)&&(min_y<=max_y))
	{
		if(max_x>=min_x)
		{
			while(aux.x<max_x) //lado de abajo del rectangulo
			{
				aux.x++;
				ordenes+="D";
				
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
			
			max_x--; //columna completada
		}
		
		if(max_y>=min_y)
		{
			while(aux.y<max_y) //lado derecho del rectangulo
			{
				aux.y++;
				ordenes+="A";
				
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
			
			max_y--; //fila completada	
		}
		
		if(max_x>=min_x)
		{
			while(aux.x>min_x) //lado de arriba del rectangulo
			{
				aux.x--;
				ordenes+="I";
				
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
			
			min_x++; //columna completada	
		}
		
		if(max_y>=min_y)
		{
			while(aux.y>min_y) //lado izquierdo del rectangulo
			{
				aux.y--;
				ordenes+="B";
				
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
			
			min_y++; //fila completada	
		}
		
	}
	
	

	
	//Salidas
	
	cout<<endl;
	cout<<"------------------------"<<endl;
	cout<<endl;
	
	cout<<"Se ejecutaron "<<contador<<" ordenes"<<endl;
	cout<<"Ordenes: "<<ordenes<<endl;
	
	cout<<endl;
	
	//Muestro el índice superior y el separador
	
	cout<<"   : ";
	
	for(int i=1; i<=NUM_COLS; i++)
	{
		cout<<setw(2)<<i<<" ";
	}
	cout<<endl;
	
	
	cout<<"   -";
	
	for(int i=0; i<NUM_COLS; i++)
	{
		cout<<"---";
	}
	cout<<"--"<<endl;
	
	
	
	for(int y=0; y<NUM_FILAS ; y++) //Muestro la matriz del recorrido
	{
		cout<<setw(2)<<NUM_FILAS-y<<" : ";
		
		for(int x=0; x<NUM_COLS ; x++)
		{
			cout<<recorrido[x][NUM_FILAS-1-y]<<"  ";
		}
		
		cout<<":"<<setw(2)<<NUM_FILAS-y<<endl;
	}
	
	//Muestro el separador y el índice inferior
	
	cout<<"   -";
	
	for(int i=0; i<NUM_COLS; i++)
	{
		cout<<"---";
	}
	cout<<"--"<<endl;
	
	
	cout<<"   : ";
	
	for(int i=1; i<=NUM_COLS; i++)
	{
		cout<<setw(2)<<i<<" ";
	}
	cout<<endl;
	
	
	
	return 0;
}

