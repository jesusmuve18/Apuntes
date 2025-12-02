/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_21
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	Lee un caracter introducido por teclado y el programa lo clasifica en
	los grupos: Mayúscula, Minúscula, Dígito y Otro. Además hace las 
	siguientes transformaciones:
	- En caso de mayúscula lo pasa a minúscula
	- En caso de minúscula lo pasa a mayúscula
	- En cualquier otro caso lo deja igual
	
	
	Entradas: Caracter
	
	Salidas: Tipo de caracter y su transformación si corresponde
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	char letra_original, letra_convertida;
	enum class TipoLetra{Mayuscula, Minuscula, Digito, Otro};
	TipoLetra tipo;
	
	//Entradas
	
	cout<<"Caracter: ";
	cin>>letra_original;
	
	//Cálculos
	
	tipo=TipoLetra::Otro; 	//Inicializo el tipo como "Otro" para después 
							//compararlo y añadirlo a un tipo diferente si
							//corresponde
	
	if('A'<=letra_original && letra_original<='Z')
		tipo=TipoLetra::Mayuscula;
		
	if('a'<=letra_original && letra_original<='z')
		tipo=TipoLetra::Minuscula;
		
	if('0'<=letra_original && letra_original<='9')
		tipo=TipoLetra::Digito;
	
	switch (tipo)	
	{
		case (TipoLetra::Mayuscula):
			letra_convertida=tolower(letra_original);
			break;
		
		case (TipoLetra::Minuscula):
			letra_convertida=toupper(letra_original);
			break;
			
		default:
			letra_convertida=letra_original;
			break;
	}
	
	//Salidas
	
	cout<<endl;
	
	switch (tipo)	
	{
		case (TipoLetra::Mayuscula):
			cout<<"La letra era una mayuscula. Una vez convertida es "
				<<letra_convertida;
			break;
		
		case (TipoLetra::Minuscula):
			cout<<"La letra era una minuscula. Una vez convertida es "
				<<letra_convertida;
			break;
			
		case (TipoLetra::Digito):
			cout<<"El caracter es un digito: "<<letra_convertida;
			break;
			
		case (TipoLetra::Otro):
			cout<<"El caracter no es una letra ni un digito validos: "
				<<letra_convertida;
			break;
	}
	
	
	return 0;
}
