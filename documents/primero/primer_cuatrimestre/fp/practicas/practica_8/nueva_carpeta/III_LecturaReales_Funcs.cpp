/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escribir la siguiente función para leer números reales:
	double LeeReal (string titulo)
	Lee y devuelve un dato double. Si el valor leido no fuera un double (p.e.
	dos, 77 88, 0xA2, .) la función volvería a pedirlo hasta que se proporcione
	un valor correcto. No se admiten valores en formato científico (p.e. 1e5).
	
	No se admiten valores como .55 ó 22. ó 78. La lectura se etiqueta con 
	titulo. El funcionamiento es similar al de la función LeeEntero del 
	ejercicio 17.
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
double LeeReal (string titulo)
{
	
	//Declaración de datos
	
	bool es_real, ya_hay_punto;
	string str_real;
	double real; //Dato que devolverá la función
	
	
	do //while (!es_real);
	{
		cout<<titulo;
		getline(cin,str_real);
		
		es_real=true; //Supongo que es real
		ya_hay_punto=false;	//Supongo que aún no se ha introducido ningún punto
		
		
		for(int i=0;i<str_real.length();i++) //recorro cada caracter
		{
			
			if(str_real.at(i)<'0' || '9'<str_real.at(i)) // n no esta 0<=n<=9
			{
				
				if(str_real.at(i)=='.') //n es un punto
				{
					
					if(!ya_hay_punto) //es el primer punto que aparece 
					{
						
						ya_hay_punto=true; //ya hay un punto
						
						if(i==0 || i==(str_real.length()-1)) //Está en la 
						{						//primera o última posición
						
							es_real=false; 	//Hay un punto al principio o al 
						}					//final
						
					} //if(!ya_hay_punto)
					else //si hay ya otro punto
					{
						es_real=false; //Hay más de un punto
					}
					
				} //if(str_real.at(i)=='.')		(n es un punto)
				else
				{
					if(!(str_real.at(i)=='-' && i==0) ) //n no es un '-' en la 
					{									//primera posición
						es_real=false;	
					}
				}
				
			}//if(str_real.at(i)<'0' || '9'<str_real.at(i)) (n no esta 0<=n<=9)
			
		}	//for(int i=0;i<str_real.length();i++)  (recorro cada caracter)
		
	} while (!es_real);
	
	real=stod(str_real); //Conversión a double de la cadena que tiene el 
						 //formato de un número real
	
	return real;
}
/****************************************************************************/
int main()
{
	//Programa para probar la función
	
	//Declaración de datos
	
	double numero;
	
	//Entradas
	
	numero=LeeReal("Introduce un valor real: ");
	
	
	//Salidas
	
	cout<<"El numero real es: "<<numero<<endl;
	
	return 0;
}
