/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_21
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escriba una función para dar formato a un string, obteniendo como resultado 
	un nuevo string de acuerdo al formato indicado en su llamada. La función 
	tendrá el prototipo:
	
		string FormateaString (string cad, int num_casillas,
								TipoAjuste ajuste, char relleno=’ ’);
								
	Transforma el contenido del dato cad a un nuevo string de acuerdo al 
	formato indicado. La función devuelve un nuevo string. El nuevo string 
	tendrá un mínimo de num_casillas casillas. Si el número de casillas 
	solicitado (num_casillas) es mayor que el tamaño de cad se 
	completan las casillas necesarias con el carácter relleno. 
	Si la longitud de cad fuera mayor simplemente se devuelve una copia de cad.
	
	Respecto al ajuste se usa un valor del tipo TipoAjuste para indicar la 
	alineación de la cadena resultado. Use la siguiente definición de tipo 
	enumerado TipoAjuste: 
				enum class TipoAjuste {izquierda, centro, derecha};
				 
 	Veamos unos ejemplos. Tenga en cuenta que los caracteres | se muestran para 
 	ayudar a visualizar el resultado, pero no forman parte de él:
				FormateaString ("Maria",10,TipoAjuste::derecha, ’*’)
				devuelve |*****Maria|
				FormateaString ("Maria",10,TipoAjuste::centro, ’ ’)
				devuelve | Maria |
				FormateaString ("Maria",10,TipoAjuste::izquierda, ’-’)
				devuelve |Maria-----|
				FormateaString ("Maria",3,TipoAjuste::derecha, ’*’)
				devuelve |Maria|
	
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
#include <stdio.h>
using namespace std;

enum class TipoAjuste {izquierda, centro, derecha};  //Defino el enumerado

#define VALORES_FIJOS 					//Activar o desactivar para distintas 
//#define VALORES_VARIABLES				//pruebas de la funcion

/****************************************************************************/


string FormateaString (string cad, int num_casillas,
											TipoAjuste ajuste, char relleno)
{
	string cadena; //Salida de la función
	
	int dif_casillas = num_casillas-cad.length();  //Casillas a rellenar
	
	switch (ajuste) //La función depende del ajuste
	{
		case TipoAjuste::izquierda:
			
			cadena+=cad; //Añado la cadena antes del relleno si lo hubiese
			
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				for(int i=0;i<dif_casillas;i++)
				{
					cadena+=relleno; //Añado el relleno después de la cadena
				}
			}
			
			
			break; //izquierda
		
		//...................................................................
		
		case TipoAjuste::derecha:
			
			cadena="";
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				for(int i=0;i<dif_casillas;i++)
				{
					cadena+=relleno; //Àñado el relleno antes de la cadena
				}
			}
			
			cadena+=cad; //Añado la cadena después del relleno si lo hubiese
			
			break; //derecha
			
		//...................................................................
			
		case TipoAjuste::centro:
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				cadena="";
				for(int i=0;i<(dif_casillas/2);i++)
				{
					cadena+=relleno; //Añado la mitad del relleno antes
				}
				
				cadena+=cad; //Añado en este punto la cadena
				
				//En caso de que el número a dividir entre 2 sea impar, le 
				//sumaré uno para añadir un caracter más al final y no perder 
				//información.
				
				if(dif_casillas%2!=0)  
					dif_casillas++;
				
				
				for(int i=0;i<(dif_casillas/2);i++)
				{
					cadena+=relleno; //Añado la otra mitad del relleno después
				}
				
				
			}	//if(dif_casillas>0)
			
			else //Si no hace falta añadir relleno 
			{
				cadena=cad;
			}
			
			
			break; //centro
			
	}
	
	return cadena; //Salida de la función
}
/****************************************************************************/
int main()
{
	#ifdef VALORES_FIJOS
	
	//Programa para probar la función con valores fijos:

	cout<<FormateaString ("Francisco", 120, TipoAjuste::izquierda, '_')<<endl;
	cout<<FormateaString ("Francisco", 120, TipoAjuste::centro, ' ')<<endl;
	cout<<FormateaString ("Francisco", 120, TipoAjuste::derecha, '*')<<endl;
	
	#endif
	
	//........................................................................
	
	#ifdef VALORES_VARIABLES
	
	//Programa para probar la función con variables:
	
	//Declaración de Datos
	
	TipoAjuste ajuste;
	string titulo;
	char relleno;
	int ancho;
	
	char opcion_ajuste;
	
	
	//Entradas
	
	cout<<"Titulo: ";
	getline(cin,titulo);
	
	do //Filtro para el ancho de línea
	{
		cout<<"Ancho de linea: ";
		cin>>ancho;	
	} while (ancho<=0);
	
	
	cout<<"Simbolo de relleno: ";
	cin.ignore(); 		//Limpio el buffer para poder poner un relleno del 
					 	//tipo ' ' con la instrucción cin.get()
	cin.get(relleno);
	
	cout<<"Alineacion: "<<endl;
	cout<<"\tIzquierda (I)"<<endl;
	cout<<"\tCentro (C)"<<endl;
	cout<<"\tDerecha (D)"<<endl;
	
	do //Filtro para elegir la opción que definirá el ajuste
	{
		cout<<">";
		cin>>opcion_ajuste;
		opcion_ajuste=toupper(opcion_ajuste);
	} while(opcion_ajuste!='I' && opcion_ajuste!='C' && opcion_ajuste!='D');
	
	
	switch (opcion_ajuste) //Asigno a la variable ajuste el valor elegido
	{
		case 'I':
			ajuste=TipoAjuste::izquierda;
			break;
		case 'C':
			ajuste=TipoAjuste::centro;
			break;
		case 'D':
			ajuste=TipoAjuste::derecha;
			break;
	}

	
	
	//Salidas
	
	cout<<FormateaString (titulo , ancho , ajuste , relleno);
	
	#endif
	
/****************************************************************************/
	
	return 0;
}
