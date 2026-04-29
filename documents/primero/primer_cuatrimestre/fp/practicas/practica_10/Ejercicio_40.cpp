/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_IV				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
using namespace std;

//#define DATOS_EXTRA

/****************************************************************************/
//Declaración de constantes globales

const int EMPLEADOS_MAX=100;
const char SEPARADOR='*';

/****************************************************************************/
//Declaración de tipos de datos


struct Empleado
{
	string apellidos;
	string nombre;
	string fecha;
	double sueldo_bruto;
};

/*****************************************************************************
  	Función: Transforma la fecha dd/mm/aaaa en un formato de Fecha adecuado
  			 para ordenar de forma que el campo más relevante es el año
  			 y el menor el día.
	Parámetros: la linea que conforma la fecha (dd/mm/aaaa)
	Devuelve: Fecha de tipo aaammdd		
............................................................................*/
string ProcesaFecha(string linea) 
{
	string fecha;
	int i=0;
	char letra;
	string dia, mes, anio;
	bool continuar=true;
	
	do //Lee Dia
	{
		letra=linea.at(i);
		
		if(letra!='/')
		{
			dia+=letra;
		}
		else
		{
			continuar=false;
		}
		i++;
		
	} while(continuar);
	
	
	continuar=true;
	
	do //Lee Mes
	{
		letra=linea.at(i);
		
		if(letra!='/')
		{
			mes+=letra;
		}
		else
		{
			continuar=false;
		}
		i++;
		
	} while(continuar);
	
		
	do //Lee Año
	{
		letra=linea.at(i);
		
		if(letra!='/')
		{
			anio+=letra;	
		}
		else
		{
			continuar=false;
		}
		i++;
		
	} while(i<linea.length());
	
	fecha=anio+mes+dia;
	
	return fecha;
}

/*****************************************************************************
  	Función: Lee una linea de la entrada y separa la linea en cada uno de 
  			 los campos de un empleado
	Parámetros: Linea de la entrada
	Devuelve: Dato de tipo Empleado		
............................................................................*/
Empleado LeeEmpleado (string linea)
{
	//Empleado empleado;
	empleado.apellidos="";
	empleado.nombre="";
	empleado.fecha="";
	
	string sueldo;
	char letra;
	bool continuar=true;
	int i=0;
	
	do //Lee Apellidos
	{
		letra=linea.at(i);
		
		if(letra!=SEPARADOR)
		{
			empleado.apellidos+=letra;
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
			empleado.nombre+=letra;
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
			empleado.fecha+=letra;
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
	
	empleado.sueldo_bruto=stod(sueldo);
	
	return empleado;
}

/*****************************************************************************
  	Función: Compara 2 string elemento a elemento
	Parámetros: las 2 string a comparar
	Devuelve: El minimo string (caracter a caracter)	
............................................................................*/
string MinimoString (string cadena1, string cadena2)
{
	int pasos=(min(cadena1.length(), cadena2.length()))-1;
	string minimo; //Dato que devolverá la función
	bool encontrado=false;
	
	int i=0;
	
	while(i<pasos && !encontrado)
	{
		if(cadena1.at(i)<cadena2.at(i))
		{
			minimo=cadena1;
			encontrado=true;
		}
		else
		{
			
			if(cadena1.at(i)>cadena2.at(i))
			{
				minimo=cadena2;
				encontrado=true;
			}
			else
			{
				i++;
			}
			
		} //else
		
		if(minimo.empty()) //Si uno esta totalmente contenido en el otro
						   //El minimo sera el mas corto
		{
			if(cadena1.length()<cadena2.length())
			{
				minimo=cadena1;
			}
			else
			{
				minimo=cadena2;
			}
		}
		
	} //while(i<pasos && !encontrado)
	
	return minimo;
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	Empleado empleado[EMPLEADOS_MAX];
	string linea;
	bool acabar_lectura=false;
	int n_empleados=0;
	
	//Entradas
	
	//Se realizarán mediante un archivo de texto
	
	
	//Cálculos
	
	while (getline(cin,linea))
	{
		empleado[n_empleados]=LeeEmpleado(linea);
		n_empleados++;
	}
	
	#ifdef DATOS_EXTRA
	
	for(int i=0; i<n_empleados; i++) //Muestra el listado de empleados
	{
		cout<<"................................................"<<endl;
		cout<<"Empleado "<<i+1<<": "<<endl;
		cout<<"\tNombre: "<<empleado[i].nombre<<endl;
		cout<<"\tApellidos: "<<empleado[i].apellidos<<endl;
		cout<<"\tFecha de Nacimiento: "<<empleado[i].fecha<<endl;
		cout<<"\tSueldo: "<<empleado[i].sueldo_bruto<<endl;	
	}
	
	#endif
	
	//Aplicaré la Ordenación por selección
	
	Empleado aux;
	string string1, string2;
	for (int separador=0 ; separador<n_empleados ; separador++) 
	{
		
		Empleado min=empleado[separador]; // Supongo que es el minimo
		int pos_min=separador; // Posición del mínimo
		
		for (int i=separador+1; i<n_empleados ; i++)
		{
			
			//Aquí se implementa el criterio de ordenación
			
			string2=to_string(min.sueldo_bruto);		  	//por sueldo							 
			string1=to_string(empleado[i].sueldo_bruto);  
														 
			//string1=empleado[i].apellidos;				//por apellidos
			//string2=min.apellidos;
			
			//string1=empleado[i].nombre;					//por nombre
			//string2=min.nombre;
			
			//string1=ProcesaFecha(empleado[i].fecha);		//por fecha
			//string2=ProcesaFecha(min.fecha);
			
			if (MinimoString(string1,string2)==string1)
			{
				min=empleado[i];	
				pos_min=i;	
			}
		}
		
		aux=empleado[separador]; //Intercambio ambos valores con una variable
		empleado[separador]=empleado[pos_min];				     //auxiliar
		empleado[pos_min]=aux;
		
	}
	
	
	//Salidas
	
	for(int i=0; i<n_empleados; i++) //Muestra el listado de empleados ordenado
	{
		cout<<"................................................"<<endl;
		cout<<"Empleado "<<i+1<<": "<<endl;
		cout<<"\tNombre: "<<empleado[i].nombre<<endl;
		cout<<"\tApellidos: "<<empleado[i].apellidos<<endl;
		cout<<"\tFecha de Nacimiento: "<<empleado[i].fecha<<endl;
		cout<<"\tSueldo: "<<empleado[i].sueldo_bruto<<endl;	
	}
	
	
	return 0;
}
