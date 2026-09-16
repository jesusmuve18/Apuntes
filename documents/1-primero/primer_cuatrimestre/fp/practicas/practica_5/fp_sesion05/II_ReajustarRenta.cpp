/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_15
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Cread un programa que lea los datos fiscales de una persona, reajuste su 
	renta bruta según el criterio que se indica posteriormente e imprima su 
	renta neta final.
	
	• La renta bruta es la cantidad de dinero íntegra que el trabajador gana.
	• La retención fiscal es el tanto por ciento que el gobierno se queda.
	• La renta neta es la cantidad que le queda al trabajador después de 
	  quitarle el porcentaje de retención fiscal, es decir:
		Renta_neta = Renta_bruta - Renta_bruta * Retención / 100
		
	Los datos a leer son:
	• Si la persona es un trabajador autónomo o no.
	• Si es pensionista o no.
	• Si está casado o no.
	• Renta bruta (total de ingresos obtenidos).
	• Retención inicial.
	
	La modificación se hará de la siguiente forma:
	• Bajar un 3 % la retención fiscal a los autónomos
	• Para los no autónomos:
	– Se sube un 1 % la retención fiscal a todos los pensionistas
	– Al resto de los trabajadores se les aplica una subida lineal del 2 %.
	  Una vez hecha esta subida, se le aplica (sobre el resultado anterior) 
	  las siguientes subidas adicionales a la retención fiscal:
		-Subir otro 2 % si la renta bruta es menor de 20.000€
		-Subir otro 2.5 % a los casados con renta bruta superior a 20.000€
		-Subir otro 3 % a los solteros con renta bruta superior a 20.000 €
	
	Entradas: 
		• Si la persona es un trabajador autónomo o no.
		• Si es pensionista o no.
		• Si está casado o no.
		• Renta bruta (total de ingresos obtenidos).
		• Retención inicial.
	
	Salidas: Renta neta
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const double DESCUENTO_AUTONOMOS=3;			//En tanto por ciento
	const double INCREMENTO_PENSIONISTAS=1;	 	//En tanto por ciento
	const double INCREMENTO_NO_PENSIONISTAS=2;	//En tanto por ciento
	const double RENTA_BRUTA_MEDIA=20000;		//En euros
	const double INCREMENTO_RENTA_BAJA=2;		//En tanto por ciento
	const double INCREMENTO_CASADOS=2.5;		//En tanto por ciento
	const double INCREMENTO_NO_CASADOS=3;		//En tanto por ciento

	
	double renta_bruta, retencion_inicial, retencion_final, renta_neta;
	char c_autonomo, c_pensionista, c_casado;
	bool autonomo, pensionista, casado;
	bool datos_correctos=true;
	
	//Entradas
	
	cout<<"Autonomo (S/N): ";
	cin>>c_autonomo;
	
	cout<<"Pensionista (S/N): ";
	cin>>c_pensionista;
	
	cout<<"casado (S/N): ";
	cin>>c_casado;
	
	cout<<"Renta bruta (euros): ";
	cin>>renta_bruta;
	
	cout<<"Retencion inicial (%): ";
	cin>>retencion_inicial;
	
	//Cálculos
	
	c_autonomo=toupper(c_autonomo); 		//Paso los caracteres a mayúsculas
	c_pensionista=toupper(c_pensionista); 	//para evitar errores y limitar 
	c_casado=toupper(c_casado);				//casos
	
	
	switch (c_autonomo)
	{
		case 'S':
			autonomo=true;
			break;
		case 'N':
			autonomo=false;
			break;
		default:
			datos_correctos=false; 	//Si no corresponde a ningún grupo anterior
			break;					//consideraré que no se han introducido
	}								//los datos adecuadamente
		
	switch (c_pensionista)
	{
		case 'S':
			pensionista=true;
			break;
		case 'N':
			pensionista=false;
			break;
		default:
			datos_correctos=false; 	
			break;				
	}
	
	switch (c_casado)
	{
		case 'S':
			casado=true;
			break;
		case 'N':
			casado=false;
			break;
		default:
			datos_correctos=false; 	
			break;				
	}
	
	if(renta_bruta<0)	//Considero que la renta inicial debe ser positiva
		datos_correctos=false;
	if(retencion_inicial<0 || retencion_inicial>100)
		datos_correctos=false;
	
	//En este punto ya tengo todos los datos necesarios para empezar a calcular
	
	if(datos_correctos)
	{
		double retencion_aux=retencion_inicial; //porcentaje de retencion 
		
		if(autonomo)
			retencion_aux=retencion_aux-DESCUENTO_AUTONOMOS;
		else
		{
			if(pensionista)
				retencion_aux=retencion_aux+INCREMENTO_PENSIONISTAS;
			else
			{
				retencion_aux=retencion_aux+INCREMENTO_NO_PENSIONISTAS;
				
				if(renta_bruta<RENTA_BRUTA_MEDIA)
				{
					retencion_aux=retencion_aux+INCREMENTO_RENTA_BAJA;
				}
				else
				{
					if(casado)
					{
						retencion_aux=retencion_aux+INCREMENTO_CASADOS;
					}
					else
					{
						retencion_aux=retencion_aux+INCREMENTO_NO_CASADOS;
					}
				}
			}
		}	
		retencion_final=retencion_aux;
		renta_neta=renta_bruta-(renta_bruta*(retencion_final/100.0));
	}
	
	//Salidas
	
	cout<<endl;
	
	if(datos_correctos)
	{
		cout<<"Usted reune las siguientes caracteristicas:"<<endl;
		
		if(autonomo)
			cout<<"\t-Autonomo"<<endl;
		else
			cout<<"\t-No es autonomo"<<endl;
		
		if(pensionista)
			cout<<"\t-Pensionista"<<endl;
		else
			cout<<"\t-No es pensionista"<<endl;
		
		if(casado)
			cout<<"\t-Casado"<<endl;
		else
			cout<<"\t-Soltero"<<endl;
			
		cout<<endl;
		cout<<"Por tanto su retencion es del "<<retencion_final<<"%"<<endl;
		cout<<"La renta final correspondiente es de "<<renta_neta<<" euros";
	}
	else
		cout<<"Los datos introducidos no son validos";
		
	return 0;
}
