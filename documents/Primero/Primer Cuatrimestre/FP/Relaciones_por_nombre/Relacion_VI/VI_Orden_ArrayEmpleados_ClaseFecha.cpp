/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_24
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:

	Reutilice las clases Empleado y Fecha con las que ha trabajado en el 
	ejercicio 23 de esta misma Relación de Problemas para adaptar el ejercicio 
	40 de la Relación de Problemas IV de manera que las ordenaciones 
	propuestas se realicen sobre un array de objetos de la clase Empleado 
	(que contienen un campo de clase Fecha).
	
	Nota: El programa está diseñado para funcionar con redirección de entrada
		  por lo que será necesario ejecutarlo en la consola de comandos
		  con los datos de los empleados guardados en un archivo de texto

*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de tipos de datos

enum class TipoAjuste {izquierda, centro, derecha};
enum class TipoOrden {apellidos,nombre,fecha,sueldo};

/****************************************************************************/
//Declaración de Clases/Objetos

class Fecha
{

private:
	
	int dia; 	//PRE: 1 <= dia <= 31
	int mes;	//PRE: 1 <= mes <= 12
	int anio;	

public:
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Fecha(): dia(1), mes(1), anio(2000){}; //Valores or defecto
	
	/**********************************************************************/
	//Constructores con argumentos
	
	Fecha(int dia, int mes, int anio):
		dia(dia),
		mes(mes),
		anio(anio) {};
	
	Fecha(string cadena) //Formato cadena: dd/mm/aaaa
	{
		string str_dia;
		string str_mes;
		string str_anio;
		
		for(int i=0; i<2; i++) //Guardo el dia
		{
			str_dia+=cadena.at(i);
		}
		
		for(int i=3; i<5; i++) //Guardo el mes
		{
			str_mes+=cadena.at(i);
		}
		
		for(int i=6; i<cadena.length(); i++) //Guardo el año
		{
			str_anio+=cadena.at(i);
		}
		
		
		//Los guardo en el objeto
		
		dia=stoi(str_dia);
		mes=stoi(str_mes);
		anio=stoi(str_anio);
		
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	int GetDia()
	{
		return dia;
	}
	
	
	int GetMes()
	{
		return mes;
	}
	
	
	string GetNombreMes()
	{
		const string meses[12]={"Enero", "Febrero", "Marzo", "Abril", "Mayo", 
						  		"Junio", "Julio", "Agosto", "Septiembre", 
								"Octubre", "Noviembre", "Diciembre"};
		return meses[(mes-1)];
	}
	
	
	string GetNombreCortoMes()
	{
		const string meses[12]={"Ene", "Feb", "Mar", "Abr", "May", 
						  		"Jun", "Jul", "Ago", "Sep", 
								"Oct", "Nov", "Dic"};
		return meses[(mes-1)];
	}
	
	
	int GetAnio()
	{
		return anio;
	}
	
	//Útil para la ordenación
	string GetValor()
	{
		return(to_string(anio)+to_string(mes)+to_string(dia));
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	SetFecha(int el_dia, int el_mes, int el_anio)
	{
		dia=el_dia;
		mes=el_mes;
		anio=el_anio;
	}
	
	SetFecha(string cadena)
	{
		string str_dia;
		string str_mes;
		string str_anio;
		
		for(int i=0; i<2; i++) //Guardo el dia
		{
			str_dia+=cadena.at(i);
		}
		
		for(int i=4; i<6; i++) //Guardo el mes
		{
			str_mes+=cadena.at(i);
		}
		
		for(int i=8; i<cadena.length(); i++) //Guardo el año
		{
			str_anio+=cadena.at(i);
		}
		
		
		//Los guardo en el objeto
		
		dia=stoi(str_dia);
		mes=stoi(str_mes);
		anio=stoi(str_anio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método ToString
	
	string ToString()
	{
		const string SEPARADOR=" ";
		string str_dia;
		
		if(dia<10)
		{
			str_dia="0"+to_string(dia);
		}
		else
		{
			str_dia=to_string(dia);
		}
		
		return (str_dia+SEPARADOR+GetNombreCortoMes()+SEPARADOR+
				to_string(anio));
		
	}

};

class Empleado
{

private:
	
	string apellidos;
	string nombre;
	Fecha fecha;
	double sueldo_bruto;

public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor con argumentos
	
	Empleado(string apellidos, string nombre, string fecha, double sueldo_bruto)
		:
		apellidos(apellidos), nombre(nombre),
		fecha(Fecha(fecha)), sueldo_bruto(sueldo_bruto){};
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	Empleado(void): //Valores por defecto
		apellidos("Munoz Velasco"), 
		nombre("Jesus"),
		fecha(Fecha("27/11/2004")),
		sueldo_bruto(0) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Procesa una línea de texto formateada para establecer un nuevo empleado
	void SetEmpleado (string linea)
	{
		const char SEPARADOR='*';
		string str_apellidos;
		string str_nombre;
		string str_fecha;
		
		string str_sueldo;
		char letra;
		bool continuar=true;
		int i=0;
		
		do //Lee Apellidos
		{
			letra=linea.at(i);
			
			if(letra!=SEPARADOR)
			{
				str_apellidos+=letra;
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
				str_nombre+=letra;
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
				str_fecha+=letra;
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
				str_sueldo+=letra;	
			}
			else
			{
				continuar=false;
			}
			i++;
			
		} while(i<linea.length()-1);
		
		
		
		apellidos=str_apellidos;
		nombre=str_nombre;
		fecha=Fecha(str_fecha);
		sueldo_bruto=stod(str_sueldo);
		
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
		fecha=Fecha(fecha_nueva);
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
	
	Fecha GetFecha()
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
		const string SEPARADOR=" , ";
		string cad=(apellidos + SEPARADOR + nombre + SEPARADOR 
					+ fecha.ToString()+ SEPARADOR + to_string(sueldo_bruto));
		
		return cad;
	}
	
	//Cadena en formato de un empleado
	string ToStringEnFormato()
	{
		string str_apellidos=FormateaString(apellidos,25);
		string str_nombre=FormateaString(nombre,25);
		string str_fecha=FormateaString(fecha.ToString(),25);
		string str_sueldo=FormateaString(to_string(sueldo_bruto),25);
		
		return(str_apellidos+str_nombre+str_fecha+str_sueldo);
	}

	/**********************************************************************/
	/**********************************************************************/
	//Compara al objeto con otro y devuelve "true" en caso de que sea menor
	//que el introducido en los argumentos del método
	
	bool EsMenor(Empleado empleado, TipoOrden orden=TipoOrden::apellidos)
	{
		bool es_menor=false;
		
		switch(orden)
		{
			case TipoOrden::apellidos:
				
				if(apellidos<empleado.GetApellidos())
				{
					es_menor=true;
				}
				
				break;
			
			
			case TipoOrden::nombre:
				
				if(nombre<empleado.GetNombre())
				{
					es_menor=true;
				}
				
				break;
			
			
			case TipoOrden::fecha:
				
				if(fecha.GetValor()<empleado.GetFecha().GetValor())
				{
					es_menor=true;
				}
				
				break;
			
			
			case TipoOrden::sueldo:
				
				if(sueldo_bruto<empleado.GetSueldo())
				{
					es_menor=true;
				}
				
				break;

		}
		
		return es_menor;
	}
	
private:
	
	/***********************************************************************
		Formatea una string dandole una longitud y alineación predefinidas
		en los parámetros
		
		Parámetros: • Cadena a formatear
					• Número de casillas que se quieren en la cadena de salida
					• Tipo de ajuste o alineación:
						a) ajuste::izquierda
						b) ajuste::centro
						c) ajuste::derecha
					• Relleno de las casillas que quedan libres (tipo char)
		Salida: Cadena formateada
	/***********************************************************************/
	string FormateaString (string cad, int num_casillas,
		TipoAjuste ajuste=TipoAjuste::izquierda, char relleno=' ')
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
						cadena+=relleno; //Añado el relleno después de la 
										 //cadena
					}
				}
				
				
				break; //izquierda
			
			//..............................................................
			
			case TipoAjuste::derecha:
				
				cadena="";
				
				if(dif_casillas>0) //Solo añado el relleno si se requiere
				{
					for(int i=0;i<dif_casillas;i++)
					{
						cadena+=relleno; //Àñado el relleno antes de la cadena
					}
				}
				
				cadena+=cad; //Añado la cadena después del relleno si lo 
							 //hubiese
				
				break; //derecha
				
			//..............................................................
				
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
					//sumaré uno para añadir un caracter más al final y no 
					//perder información.
					
					if(dif_casillas%2!=0)  
						dif_casillas++;
					
					
					for(int i=0;i<(dif_casillas/2);i++)
					{
						cadena+=relleno; //Añado la otra mitad del relleno 
										 //después
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
	
};
/****************************************************************************/
/****************************************************************************/
//Función auxiliar para la prueba de funciones que devuelve un nombre
//indicativo del criterio de orden seleccionado

string Nombre(TipoOrden criterio)
{
	string nombre;
	
	switch(criterio)
	{
		case TipoOrden::apellidos:
			
			nombre="apellidos";
			
			break;
		
		
		case TipoOrden::nombre:
			
			nombre="nombre";
			
			break;
		
		
		case TipoOrden::fecha:
			
			nombre="fecha de nacimiento";
			
			break;
		
		
		case TipoOrden::sueldo:
			
			nombre="sueldo bruto mensual";
			
			break;

	}
	
	return nombre;
}

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos

	const int EMPLEADOS_MAX=100; //Indica el máximo de empleados que pueden 
								 //generarse
	const TipoOrden criterio=TipoOrden::fecha; //Se define el criterio de orden
	
	Empleado empleado[EMPLEADOS_MAX]; 
	
	int n_empleados=1; //Creo un empleado para probar el constructor sin 
					   //argumentos
	string linea;

	//Entradas

	while (getline(cin,linea)) //Realizo la lectura desde un archivo de texto
	{
		empleado[n_empleados].SetEmpleado(linea);
		n_empleados++;
	}
	
	//Muestro el listado de empleados
	
	cout<<endl;
	cout<<"1. Listado de empleados: "<<endl;
	cout<<endl;
	
	for(int i=0; i<n_empleados ; i++)
	{
		cout<<"\t"<<empleado[i].ToStringEnFormato()<<endl;
	}
	

	cout<<endl;
	
	for(int i=0; i<100; i++) //Separador de apartados
	{
		cout<<".";
	}
	
	cout<<endl;
	
	
	
	//Aplicaré la Ordenación por selección
	
	Empleado aux;
	
	for (int separador=0 ; separador<n_empleados ; separador++) 
	{
		
		Empleado min=empleado[separador]; // Supongo que es el minimo
		int pos_min=separador; // Posición del mínimo
		
		for (int i=separador+1; i<n_empleados ; i++)
		{
			if (empleado[i].EsMenor(min, criterio))
			{
				min=empleado[i];	
				pos_min=i;	
			}
		}
		
		aux=empleado[separador]; //Intercambio ambos valores con una variable
		empleado[separador]=empleado[pos_min];				     //auxiliar
		empleado[pos_min]=aux;	
	}
	
	
	//Muestro el resultado de la ordenación
	
	cout<<endl;
	cout<<"2. Empleados ordenados por " <<Nombre(criterio)<<": "<<endl;
	cout<<endl;
	for(int i=0; i<n_empleados ; i++)
	{
		cout<<"\t"<<empleado[i].ToStringEnFormato()<<endl;
	}


	return 0;
}

