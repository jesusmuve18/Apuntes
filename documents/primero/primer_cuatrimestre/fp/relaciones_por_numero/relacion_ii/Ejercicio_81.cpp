/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir un programa en el que se presente un menú principal para que el 
	usuario	pueda elegir entre las siguientes opciones:
	
							A–>Calcular adición.
							P–>Calcular producto.
							X–>Salir.
							
	Si el usuario elige en el menú principal:
	a) Salir, el programa terminará su ejecución.
	b) Calcular adición se presenta un menú (secundario) para que el usuario
	   pueda elegir entre las siguientes opciones:
	   
							S–>Calcular suma.
							R–>Calcular resta.
							X–>Salir.
							
	c) Calcular producto se presenta un menú (secundario) para que el usuario 
	pueda elegir entre las siguientes opciones:
	
							M–>Calcular multiplicación.
							D–>Calcular división entera.
							R–>Calcula resto.
							X–>Salir.
							
	MUY IMPORTANTE: Para implementar las operaciones de producto no es posible 
	utilizar los operadores de multiplicación (*) ni división (/). Deberá 
	emplear	sumas (+) y restas (-).
	
	En las operaciones seleccionadas desde los menús secundarios el programa 
	pedirá dos números enteros, realizará la operación seleccionada, mostrará 
	el resultado y	volverá a mostrar el menú secundario seleccionado 
	anteriormente.
	
	En los dos menús secundarios la selección de Salir hace que el programa 
	vuelva a mostrar el menú principal.

	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	char eleccion_1, eleccion_2;
	int num1, num2;
	int resultado;
	bool acabar=false;
	int aux, contador;

	//Proceso
	while(!acabar)
	{
		//Entradas
		
		cout<<endl;
		cout<<"Menu Principal: "<<endl;
		cout<<"\tA: Calcular adicion"<<endl;
		cout<<"\tP: Calcular producto"<<endl;
		cout<<"\tX: Salir"<<endl;
		cout<<">";
		cin>>eleccion_1;
	
		
		//Cálculos
		
		eleccion_1=toupper(eleccion_1);
		
		switch(eleccion_1)
		{
			case 'A':
				
				//Entradas de la elección para el menú adición
				cout<<endl;
				cout<<"Menu Secundario: "<<endl;
				cout<<"\tS: Calcular suma"<<endl;
				cout<<"\tR: Calcular resta"<<endl;
				cout<<"\tX: Salir"<<endl;
				cout<<">";
				cin>>eleccion_2;
				
				
				eleccion_2=toupper(eleccion_2);
				
				//Filtro de la entrada
				switch (eleccion_2)
				{
					case 'S':
						
						//Entradas de la suma
						cout<<"Ha elegido la suma:"<<endl;
						cout<<endl;
						cout<<"Numero 1: ";
						cin>>num1;
						cout<<"Numero 2: ";
						cin>>num2;
						
						//Cálculo de la suma
						resultado=num1+num2;
						
						//Salida de la suma
						cout<<endl;
						cout<<num1<<" + "<<num2<<" = "<<resultado<<endl;
						
						break;
					
					case 'R':
						
						//Entradas de la resta
						cout<<endl;
						cout<<"Ha elegido la resta:"<<endl;
						cout<<"Numero 1: ";
						cin>>num1;
						cout<<"Numero 2: ";
						cin>>num2;
						
						//Cálculo de la resta
						resultado=num1-num2;
						
						//Salida de la resta
						cout<<endl;
						cout<<num1<<" - "<<num2<<" = "<<resultado<<endl;
						
						break;
					
					case 'X':	//X del menú secundario adición
						break;	//Con esto vuelvo al menú principal
						
					default:
						cout<<"No ha introducido un valor valido"<<endl;
				}
				
				break;
				
			case 'P':
				
				//Entrada de la elección sobre el menú producto
				cout<<endl;
				cout<<"Menu Secundario: "<<endl;
				cout<<"\tM: Calcular multiplicacion"<<endl;
				cout<<"\tD: Calcular division entera"<<endl;
				cout<<"\tR: Calcular resto"<<endl;
				cout<<"\tX: Salir"<<endl;
				cout<<">";
				cin>>eleccion_2;
				
				eleccion_2=toupper(eleccion_2);
				
				switch (eleccion_2)
				{					
					case 'M':
						
						//Entradas de la multiplicación
						cout<<endl;
						cout<<"Ha elegido la multiplicacion:"<<endl;
						cout<<"Numero 1: ";
						cin>>num1;
						cout<<"Numero 2: ";
						cin>>num2;
						
						//Cálculo del producto
						
						resultado=0;	//Neutro de la suma
						
						for(int i=0; i<num1; i++)
							resultado+=num2; //Desarrollo el producto como
											 //suma
											 
						//Salida del producto
						cout<<endl;
						cout<<num1<<" * "<<num2<<" = "<<resultado<<endl;
						
						break;
						
						
					case 'D':
						
						//Entradas de la división
						cout<<endl;
						cout<<"Ha elegido la division:"<<endl;
						cout<<"Numero 1: ";
						cin>>num1;
						cout<<"Numero 2: ";
						cin>>num2;
						
						//Cálculo de la división
						
						aux=num1;			//Para no modificar el num1
						
						
						while(aux>=num2)
						{
							aux-=num2; 	//Desarrollo el cociente como resta
							contador++;		
						}
							
						resultado=contador;
						
						//Salida de la división
						cout<<endl;
						cout<<num1<<" / "<<num2<<" = "<<resultado<<endl;
						
						break;
						
						
					case 'R':
						
						//Entradas del resto
						cout<<endl;
						cout<<"Ha elegido el resto:"<<endl;
						cout<<"Numero 1: ";
						cin>>num1;	//Dividendo
						cout<<"Numero 2: ";
						cin>>num2;	//Divisor
						
						//Cálculos del resto
						
						//Aplicaré resto=dividendo-(divisor*cociente)
						
						//Dividendo entre divisor para calcular el cociente
						resultado=0;
						aux=num1;
												 
						while(aux>=num2)
						{
							aux-=num2; 	//Desarrollo el cociente como resta
							contador++;
						}
						
						//El contador en este punto servirá de cociente 
						
						aux=0; //reseteo para volver a usarlo
						
						//Calculo divisor*cociente y lo guardo en aux
						for(int i=0; i<contador; i++)
						{
							aux+=num2; 	//Desarrollo el producto como suma
						}
							
						resultado=num1-aux;
						
						//Salida del resto	
						cout<<endl;
						cout<<num1<<" % "<<num2<<" = "<<resultado<<endl;
						
						break;
					
					
					case 'X':	//X del menu secundario producto
						break;	//Con esto vuelvo al menú principal
						
					default:
						cout<<"No ha introducido un valor valido"<<endl;
				}
				
				break;
				
			case 'X':		//X del menú principal
				acabar=true; //Indico que se debe acabar el proceso
				break;
			
			default:
				cout<<"No ha introducido un valor valido"<<endl;
		}	
	}	
	
	return 0;
}
