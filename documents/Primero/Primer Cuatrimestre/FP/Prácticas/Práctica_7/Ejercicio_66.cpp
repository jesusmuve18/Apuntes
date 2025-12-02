/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_66
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Sobre la solución del ejercicio 29 de esta relación de problemas, se pide 
	lo siguiente:
	
	Supondremos que sólo pueden introducirse intereses enteros 
	(1, 2, 3, . . . ). Calcular el capital obtenido al término de cada año, 
	pero realizando los cálculos para todos los	tipos de interés enteros 
	menores o iguales que el introducido (en pasos de 1).
	
	Por ejemplo, si interés es 5, y un número de años es 3, hay que mostrar 
	el capital ganado al término de cada uno de los tres años a un interés del 
	1 %; a continuación para un interés del 2 %, y así hasta llegar al 5 %. 
	El programa debe mostrar:
	
			Cálculos realizados al 1%:
			 Dinero obtenido en el año número 1 = 2020.00
			 Dinero obtenido en el año número 2 = 2040.20
			 Dinero obtenido en el año número 3 = 2060.60
			
			Cálculos realizados al 2%:
			 Dinero obtenido en el año número 1 = 2040.00
			 Dinero obtenido en el año número 2 = 2080.80
			 Dinero obtenido en el año número 3 = 2122.42
			.......

	
	Entradas: capital a invertir (capital_inicial)
			  interés a aplicar (interes)
			  número de años de la inversión (numero_anios)
	
	Salidas: dinero total obtenido por la inversión 
			 (capital + ganancia obtenida) calculado -anualmente- por: 
				total = capital + (capital * interes/100)
			  reinvirtiendo cada año el nuevo total.
	
*/
/****************************************************************************/
#include<iostream>
#include<iomanip>
using namespace std;
/****************************************************************************/
int main()
{
	cout.setf(ios::fixed);		// Notación de punto fijo para los reales
	cout.setf(ios::showpoint);	// Mostrar siempre decimales 
	
	
	const int MIN_CAPITAL = 1000;
	const int MIN_ANIOS   =    1;
	const int MAX_ANIOS   =   20;
	const int MIN_INTERES =    0; // En porcentaje 0-100
	const int MAX_INTERES =   10; // En porcentaje 0-100
	
	// Entradas: 
	double capital_inicial;
	int interes, interes_aux; 
	int numero_anios;
	   
	// Salidas
	double capital_acumulado;
  
	// Auxiliares 
	int anio;
	
	

	// Lectura y filtrado de datos

	const string mensaje_capital =  "Introduzca capital inicial (capital >= "
	      + to_string(MIN_CAPITAL) + "): ";
	const string mensaje_anios =  "Introduzca numero de anios (" 
	      + to_string(MIN_ANIOS) + "-" +  to_string(MAX_ANIOS) + "): ";
	const string mensaje_interes =  "Introduzca % interes (" 
	      + to_string(MIN_INTERES) + "-" +  to_string(MAX_INTERES) + "]: ";
	      	      
	do {	
		cout << mensaje_capital;
		cin >> capital_inicial;
	} while (capital_inicial < MIN_CAPITAL);

	do {	
		cout << mensaje_anios;
		cin >> numero_anios;
	} while ((numero_anios<MIN_ANIOS) || (numero_anios>MAX_ANIOS));
	
	
	do {	
		cout << mensaje_interes;
		cin >> interes;   
   	} while ((interes<= MIN_INTERES) || (interes>MAX_INTERES));

	cout << endl;

	// En este punto aseguro que 
	//	1)	MIN_CAPITAL <= capital_inicial  
	//	2)	MIN_ANIOS <= numero_anios <= MAX_ANIOS
	// 	3)	MIN_INTERES < interes <= MAX_INTERES


	// Cálculos y presentación de resultados
	
	capital_acumulado = capital_inicial; // Capital inicial
	
	double capital_anterior = capital_inicial; // Capital del año anterior
	double ganancia_acumulada = 0; // Suma de los incrementos de capital, 
									// año a año.
	
	  
	interes_aux=1; // contador de interes auxiliares (temporales)
	
   while(interes_aux<=interes) //recorro los intereses hasta el introducido
   {
   		anio  = 1; // contador de años
   		cout<<"Calculos realizados al "<<interes_aux<<"%:"<<endl;
   		cout<<endl;
   		
	   	while (anio <= numero_anios) {
		
			// Nuevo capital pasado un año
			capital_acumulado = capital_acumulado  +  
			                   (capital_acumulado * (interes_aux / 100.0));   
		
			double ganancia_anio = capital_acumulado - capital_anterior;
			ganancia_acumulada += ganancia_anio; 
			
			capital_anterior = capital_acumulado;
			
			// No es preciso mostrar esta información, pero ayuda a entender
			cout << "  Capital obtenido transcurrido el anio numero "
				 << setw(2)  << anio << " = " 
				 << setw(10) << setprecision(2) << capital_acumulado << " ";
			cout << "(" << setw(10) << setprecision(2) << ganancia_anio << ")";
			cout << endl;
		
			anio++; // actualizar el año de inversión 
		}
	   
		double incremento_capital =  capital_acumulado-capital_inicial;
	
		cout << setw(62) << " " << "  ----------" << endl; 
		cout << setw(62) << "     " << setw(10) << setprecision(2) 
		     << incremento_capital << endl; 
		cout << endl;
	
		bool control_integridad = (incremento_capital == ganancia_acumulada);
		
		if (control_integridad) 		
			cout << "Incremento de capital = " << setw(10) << setprecision(2) 
				 << incremento_capital << endl;
		else 
			cout << "Error en los calculos realizados. ";
		
		cout<<endl;
		cout<<"------------------------------------------------------"
			<<"----------------------"<<endl;
		cout << endl;
		interes_aux++;
   }
	
	     
	return 0;
}
