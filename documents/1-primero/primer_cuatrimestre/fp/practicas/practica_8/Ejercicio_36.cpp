/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_36
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Escriba una función que verifique la validez de un NIF (de españoles 
	residentes mayores de edad) y del NIE (de extranjeros residentes en 
	España).
	
	En http://www.interior.gob.es/web/servicios-al-ciudadano/dni/calculo-del-
	digito-de-control-del-nif-nie encontrará las especificaciones técnicas 
	para este cometido.
	
	Escriba un programa que lea un número indefinido de NIFs/NIEs (finaliza 
	cuando se introduzca FIN) y verifique su validez.
	
	Modularice el programa adecuadamente, usando funciones.
	
	Entradas: Documento NIF/NIE
	
	Salidas: Si es correcto o no
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
using namespace std;
/****************************************************************************/
const int LONG_DOC=9;
const string LETRAS_ORDEN="TRWAGMYFPDXBNJZSQVHLCKE";
const string TERMINADOR="FIN";
/****************************************************************************/
char DigitoControl(int numero)
{
	char letra;
	int resto=numero%(LETRAS_ORDEN.length());
	letra=LETRAS_ORDEN.at(resto);
	
	/*Esta función consiste en asignar una letra según el resto que se obtiene
	 de dividir el numero del documento entre 23. Para ello creo una string
	 con tantos caracteres como se requieran (en este caso 23) y ordenados
	 de forma que su posición en la string sea el resto calculado previamente.
	 Esta sería la string constante LETRAS_ORDEN.
	 
	 Ejemplo:
	 	Si al dividir entre 23 se obtiene resto 0 a ese número le corresponde
	 	el caracter T, que ocupa la posición 0 en la cadena de letras ordenadas
	 */
	 
	return letra;
}

/****************************************************************************/

double Pow(int base, int exponente) //Función para las potencias
{
	//Declaración de datos
	
	double potencia=1; //Inicializo con el neutro del producto
	
	
	//Cálculos
	for(int i=0;i<exponente;i++)
	{
		potencia*=base;
	}
	
	
	//Salida de la función
	return potencia;
}

/****************************************************************************/

string LeeDocumentoCorrecto(string titulo)
{
	//Declaración de Datos
	
	bool documento_ok, ultima_letra_ok;
	string documento;
	
	
	
	do //while(!documento_ok);
	{
		documento_ok=true; //Supongo que es correcto
		
		cout<<titulo; //Muestro la etiqueta de petición por pantalla
		getline(cin,documento);	
		
		if(documento!=TERMINADOR) //La cadena no es el terminador
		{
			
			if(documento.length()!=LONG_DOC)
			{
				documento_ok=false;
				cout<<"Longitud de documento no valida"<<endl;
			}
			else //La cadena tiene la longitud adecuada
			{
				
				if(documento.at(0)<'0' || '9'<documento.at(0))  
				{
					//El primer caracter no es un número
					
					//Paso a mayúscula para reducir casos
					documento.at(0)=toupper(documento.at(0)); 
					
					switch (documento.at(0))
					{
						case 'X':
							
							//Sustituyo por el número correspondiente
							documento.at(0)='0'; 
							
							break;
							
							
						case 'Y':
							
							//Sustituyo por el número correspondiente
							documento.at(0)='1';
							
							break;
							
							
						case 'Z':
							
							//Sustituyo por el número correspondiente
							documento.at(0)='2';
							
							break;
							
							
						default:
							documento_ok=false;
							cout<<"Primer elemento no valido"<<endl;
							break;
							
					} //switch (documento.at(0))
					
				} //if(documento.at(0)<'0' || '9'<documento.at(0))  
				
				
				//Filtro el resto de la cadena hasta la penúltima posición
				for(int i=1;i<documento.length()-1;i++)
				{
					//Si el caracter no es un número
					if(documento.at(i)<'0' || '9'<documento.at(i))
					{
						documento_ok=false;
						cout<<"Elemento de la posicion"<<i+1<<" no valido"
							<<endl;
					}
				}
				
				ultima_letra_ok=false; //Supongo que no es válida la letra
				
				
				//Paso la letra a mayúscula para reducir casos
				(documento.at(documento.length()-1))=
								toupper((documento.at(documento.length()-1)));
				
				
				//Comparo la letra con cada caracter de la cadena LETRAS_ORDEN
				for(int i=0;i<LETRAS_ORDEN.length();i++)
				{
					
					if((documento.at(documento.length()-1))
														==LETRAS_ORDEN.at(i))
					{
						ultima_letra_ok=true;
					}
				}
				
				
				if(ultima_letra_ok==false)	
				{
					cout<<"Ultima letra no valida"<<endl;
					documento_ok=false;
				}
				
			} //La cadena tiene la longitud adecuada
			
		} //if(documento!=TERMINADOR)
		
	} while(!documento_ok);
	
	
	//Salida de la función
	return documento;
}

/****************************************************************************/

bool ValidezDocumento(string documento) 
{
	//Declaración de Datos
	
	string documento_sin_letra;
	char letra;
	bool valido;
	
	
	//Cálculos
	
	//Separo el documento en el documento sin letra y la letra por separado
	for(int i=0;i<documento.length()-1;i++)
	{
		documento_sin_letra+=documento.at(i);
	}
	
	letra=documento.at(documento.length()-1);
	
	//Compruebo si el dígito que le corresponde a ese documento sin letra
	//es la letra separada anteriormente
	if(DigitoControl(stoi(documento_sin_letra))!=letra)
	{
		valido=false;
	}
	else
	{
		valido=true;
	}
	
	//Salida de la función
	return valido;
}
/****************************************************************************/
int main()
{
	//Programa para probar la función
	
	
	//Declaración de Datos
	
	string documento;
	bool terminar=false;
	
		//Indicador del terminador
	cout<<"Para salir introduzca \""<<TERMINADOR<<"\""<<endl;
	
	
	
	//Cálculos
	
	while(!terminar)
	{
		documento=LeeDocumentoCorrecto("Introduzca su documento (NIF/NIE): ");
		
		if(documento!=TERMINADOR)
		{
			if(ValidezDocumento(documento)==true)
			{
				cout<<"El documento es correcto"<<endl;
			}
			else
			{
				cout<<"La letra de control no es la que corresponde"<<endl;
				cout<<"La letra deberia ser "
					<<DigitoControl(stoi(documento))<<endl;
			}	
		}
		else //El documento es el terminador
		{
			terminar=true;	//Se sale del bucle
		}
		
		
		cout<<endl;
			
	} //while(!terminar)
	
	
	return 0;
}
