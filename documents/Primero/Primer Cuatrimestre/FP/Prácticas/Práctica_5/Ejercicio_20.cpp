/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_20
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Lee el valor de la edad (dato de tipo entero) y salario (dato de tipo 
	real) de una persona. Sube el salario un 4 % si es mayor de 65 o menor de 
	35 años. Si además de cumplir la anterior condición, también tiene un 
	salario inferior a 300 euros, se le subirá otro 3 %, mientras que si su 
	salario es mayor o igual que 300 euros pero inferior a 900 euros se le 
	subirá un sólo el 1.5 %
	
	Entradas: Edad y salario base
	
	Salidas: Salario final
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int EDAD_JUBILACION=65;				//En años
	const double INCREMENTO_EDAD_TRABAJADOR=4;	//En tanto por ciento
	const int LIMITE_JOVEN=35;					//En años
	const double LIMITE_SALARIO_BAJO=300;		//En euros
	const double INCREMENTO_SALARIO_BAJO=3;		//En tanto por ciento
	const double INCREMENTO_SALARIO_MEDIO=1.5;	//En tanto por ciento
	const double LIMITE_SALARIO_ALTO=900;		//En euros
	
	
	
	enum class ClaseEdad {Jubilado, Joven, Trabajador};
	ClaseEdad edad;
	
	enum class ClaseSueldo {Bajo, Medio, Alto};
	ClaseSueldo salario;
	
	int i_edad;
	double d_salario, incremento=0, salario_final;
	bool datos_ok;
	
	
	//Entradas
	
	cout<<"Edad: ";
	cin>>i_edad;
	
	cout<<"Sueldo: ";
	cin>>d_salario;
	
	
	//Cálculos
	
	datos_ok=(i_edad>=0 && d_salario>=0);
	
	if(datos_ok)
	{
		if(i_edad<EDAD_JUBILACION) {
			edad=ClaseEdad::Trabajador;
			if(i_edad<LIMITE_JOVEN)		
					edad=ClaseEdad::Joven;
		}
		else
			edad=ClaseEdad::Jubilado;
		
		if(d_salario<LIMITE_SALARIO_ALTO) {
			salario=ClaseSueldo::Medio;
			if(d_salario<LIMITE_SALARIO_BAJO)
				salario=ClaseSueldo::Bajo;
		}
		else
			salario=ClaseSueldo::Alto;
		
		if (edad==ClaseEdad::Trabajador)
			incremento=incremento+INCREMENTO_EDAD_TRABAJADOR;
		
		switch (salario) 
		{
			case (ClaseSueldo::Bajo):
				incremento=incremento+INCREMENTO_SALARIO_BAJO;
				break;
			case (ClaseSueldo::Medio):
				incremento=incremento+INCREMENTO_SALARIO_MEDIO;
				break;
		}

		salario_final=d_salario*(1+(incremento/100.0));	
	}
	
	
	//Salidas
	
	cout<<endl;
	
	if(datos_ok)
	{
		cout<<"El salario resultante es de "<<salario_final<<" euros"<<endl;
	}
	else
		cout<<"Los datos introducidos no son validos"<<endl;
	
	
	
	return 0;
}
