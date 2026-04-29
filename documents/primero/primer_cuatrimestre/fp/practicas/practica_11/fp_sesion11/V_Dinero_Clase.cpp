/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_4
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Definid una clase Dinero para trabajar de forma precisa con datos 
	monetarios. La clase tendrá dos datos miembro: euros y centimos.
		
	La clase deberá permitir que se introduzca un número de céntimos arbitrario 
	(siempre que sea positivo o cero) y se encargará de ajustar las cantidades 
	debidamente. Por ejemplo, si asignamos 20 euros y 115 céntimos, el objeto 
	deberá almacenar el valor 21 en euros y 15 en centimos. No olvide añadir un 
	método ToString() para reperesentar de forma textual el contenido de un 
	objeto Dinero.
	
	Hacer un programa que calcule el precio final de un producto a partir de 
	su precio inicial, de un incremento fijo mensual y de un número de meses. 
	El programa irá mostrando, mes a mes, el precio del producto.
	
	Nota: Implemente métodos que modifiquen una cantidad de dinero.


*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

const int CENTS_EN_EUR=100;
const char RELLENO=' ';

/****************************************************************************/
//Declaración de tipos de Datos

enum class TipoAjuste {izquierda, centro, derecha};

/****************************************************************************/
//Funciones

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
//Declaración de clases/objetos

class Dinero
{
	private:
		int euros;
		int cents;
	public:
		
		//PRE: euros>=0 && cents>=0
		void Ajustar() //Ajusta las unidades en sus respectivos rangos
		{
			euros+=(int)(cents/CENTS_EN_EUR);
			cents=cents%CENTS_EN_EUR;
			
		}
		
		void Incrementar(double incremento)
		{
			double euros_incr=euros*(incremento/100);
			
			euros+=(int)euros_incr;
			cents*=(1+(incremento/100));
			cents+=((euros_incr-((int)euros_incr))*CENTS_EN_EUR);
			
			Ajustar();			
		}
		
		//PRE: euros>=0 && cents>=0
		Dinero(int euros, int cents):euros(euros),cents(cents)
		{
			Ajustar(); //Ajusto los datos al declarar un nuevo objeto
		}
		
		//PRE: euros>=0 && cents>=0
		Dinero():euros(0),cents(0) 
		{
			Ajustar(); //Ajusto los datos al declarar un nuevo objeto
		}
		
		
		string ToString()
		{
			string str_euros=FormateaString(to_string(euros),6,
							TipoAjuste::derecha, RELLENO);
							
			string str_cents=FormateaString(to_string(cents),2,
							TipoAjuste::derecha, RELLENO);
							
			return (str_euros+" Euros y "+str_cents+" Centimos");
		}
		
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos

	int euros, cents;
	int meses, incremento;

	//Entradas

	do
	{
		cout<<"Euros: ";
		cin>>euros;
		
	} while(euros<0);

	do
	{
		cout<<"Centimos: ";
		cin>>cents;
		
	} while(cents<0);
	
	Dinero precio_producto(euros, cents);  //Creo el objeto con el constructor  
										  // de dos parámetros	
	
	do
	{
		cout<<"Meses: ";
		cin>>meses;
		
	} while(meses<=0);
	
	
	cout<<"Incremento mensual fijo (%): ";
	cin>>incremento;
	
	

	//Cálculos y Salidas
	
	cout<<endl;
	
	//Muestro el precio inicial
	cout<<FormateaString("Precio inicial: ", 16 , TipoAjuste::derecha, 
		  RELLENO);
	cout<<precio_producto.ToString()<<endl;
	
	string cad_auxiliar;
	
	for(int i=1; i<=meses; i++)
	{
		precio_producto.Incrementar(incremento);
		cad_auxiliar="Mes "+to_string(i)+": ";
		
		cout<<FormateaString(cad_auxiliar, 16 , TipoAjuste::derecha, 
		      RELLENO);
		cout<<precio_producto.ToString()<<endl;
	}

	return 0;
}

