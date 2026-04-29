/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Este ejercicio está basado en el ejercicio 40 de la Relación de Problemas 
	IV. En ese ejercicio se leía un número indeterminado de datos referentes 
	a los empleados de una empresa que se almacenaban en registros, que a su 
	vez se organizaban en un array y después se ordenaban.
	
	Ahora deberá usar la clase Empleado para representar a los empleados. 
	Deberá diseñar la clase correctamente usando datos privados, constructores, 
	métodos Set, métodos Get y otros métodos de gestión si fueran necesarios.
	
	Use un array de objetos Empleado para almacenar los empleados leidos. 
	Muestre su contenido después de leer los datos de los empleados. Calcule 
	los salarios mínimos y máximos, muestre sus valores y los nombres y 
	apellidos de los empleados que ganan menos y más. Calcule el salario 
	medio, muestre su valor y después muestre un listado de los empleados cuyo 
	salario es superior al salario medio.

	Entradas: archivo de texto con los datos necesarios (redirección)

	Salidas: 1. Listado de empleados
			 2. Sueldo más alto
			 3. Sueldo más bajo
			 4. Sueldo medio
			 5. Empleados por encima del sueldo medio

*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

/****************************************************************************/
//Declaración de Clases/Objetos

class Empleado
{

private:
	
	string apellidos;
	string nombre;
	string fecha;
	double sueldo_bruto;

public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor con argumentos
	
	Empleado(string apellidos, string nombre, string fecha, double sueldo_bruto)
		:
		apellidos(apellidos), nombre(nombre),
		fecha(fecha), sueldo_bruto(sueldo_bruto){};
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	Empleado(void){};
	
	/**********************************************************************/
	/**********************************************************************/
	//Procesa una línea de texto formateada para establecer un nuevo empleado
	void SetEmpleado (string linea)
	{
		const char SEPARADOR='*';
		apellidos="";
		nombre="";
		fecha="";
		
		string sueldo;
		char letra;
		bool continuar=true;
		int i=0;
		
		do //Lee Apellidos
		{
			letra=linea.at(i);
			
			if(letra!=SEPARADOR)
			{
				apellidos+=letra;
			}
			else
			{
				continuar=false;
			}
			i++;
			
		} while(continuar);
		
		
		continuar=true;
		
		do //Lee Nombre
		{
			letra=linea.at(i);
			
			if(letra!=SEPARADOR)
			{
				nombre+=letra;
			}
			else
			{
				continuar=false;
			}
			i++;
			
		} while(continuar);
		
		
		continuar=true;
		
		do //Lee Fecha
		{
			letra=linea.at(i);
			
			if(letra!=SEPARADOR)
			{
				fecha+=letra;
			}
			else
			{
				continuar=false;
			}
			i++;
			
		} while(continuar);
		
		do //Lee Sueldo
		{
			letra=linea.at(i);
			
			if(letra!=SEPARADOR)
			{
				sueldo+=letra;	
			}
			else
			{
				continuar=false;
			}
			i++;
			
		} while(i<linea.length()-1);
		
		sueldo_bruto=stod(sueldo);
		
	}
	/**********************************************************************/
	//Establece el valor de Apellidos
	
	void SetApellidos(string apellidos_nuevos)
	{
		apellidos=apellidos_nuevos;
	}
	
	/**********************************************************************/
	//Establece el valor de Nombre
	
	void SetNombre(string nombre_nuevo)
	{
		nombre=nombre_nuevo;
	}
	
	/**********************************************************************/
	//Establece el valor de Fecha
	
	void SetFecha(string fecha_nueva)
	{
		fecha=fecha_nueva;
	}
	
	/**********************************************************************/
	//Establece el valor de Sueldo_bruto
	
	void SetSueldo(double sueldo_nuevo)
	{
		sueldo_bruto=sueldo_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Devuelve el valor de Apellidos
	
	string GetApellidos()
	{
		return (apellidos);
	}
	
	/**********************************************************************/
	//Devuelve el valor de Nombre
	
	string GetNombre()
	{
		return (nombre);
	}
	
	/**********************************************************************/
	//Devuelve el valor de Fecha
	
	string GetFecha()
	{
		return (fecha);
	}
	
	/**********************************************************************/
	//Devuelve el valor de Sueldo
	
	double GetSueldo()
	{
		return (sueldo_bruto);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Devuelve la cadena que representa a un empleado
	
	string ToString()
	{
		string separador=" , ";
		string cad=apellidos+separador+nombre+separador+fecha+separador
			+to_string(sueldo_bruto);
		
		return cad;
	}
	
};

/*****************************************************************************
  	Función:
	Parámetros:
	Devuelve:
............................................................................*/

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	const int EMPLEADOS_MAX=100;
	
	Empleado empleado[EMPLEADOS_MAX]; 
	int n_empleados=0;
	string linea;

	//Entradas

	while (getline(cin,linea))
	{
		empleado[n_empleados].SetEmpleado(linea);
		n_empleados++;
	}

	//Cálculos y Salidas
	
	//Muestro el contenido del array de empleados
	
	cout<<endl;
	cout<<"1. Listado de empleados: "<<endl;
	for(int i=0; i<n_empleados ; i++)
	{
		cout<<"\t"<<empleado[i].ToString()<<endl;
	}
	
	double sueldo_min=9999999;
	double sueldo_max=-sueldo_min;
	
	
	//Calculo el sueldo max y minimo
	
	for(int i=0; i<n_empleados ; i++)
	{
		if(empleado[i].GetSueldo()>sueldo_max)
		{
			sueldo_max=empleado[i].GetSueldo();
		}
		
		if(empleado[i].GetSueldo()<sueldo_min)
		{
			sueldo_min=empleado[i].GetSueldo();
		}
		
	}
	
	cout<<endl;
	cout<<"2. El sueldo mas alto es de "<<sueldo_max<<" euros"<<endl;
	cout<<"3. El sueldo mas bajo es de "<<sueldo_min<<" euros"<<endl;
	
	
	//Calculo el sueldo_medio
	
	double sueldo_medio;
	double suma=0;
	
	for(int i=0; i<n_empleados ; i++)
	{
		suma+=empleado[i].GetSueldo();
	}
	
	sueldo_medio=(suma/n_empleados);
	
	cout<<endl;
	cout<<"4. El sueldo medio es de "<<sueldo_medio<<" euros"<<endl;
	
	
	//Muestro los empleados por encima del sueldo medio
	
	cout<<endl;
	cout<<"5. Los empleados con un sueldo superior al medio son: "<<endl;
	for(int i=0; i<n_empleados ; i++)
	{
		if(empleado[i].GetSueldo()>sueldo_medio)
		{
			cout<<"\t"<<empleado[i].ToString()<<endl;
		}
	}

	return 0;
}

