/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_68
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	En el ejercicio 57 de esta Relación de Problemas tuvo que escribir un 
	programa para la gestión de los empleados de una empresa que se organiza 
	en diferentes departamentos. En ese programa implementó una tabla 
	dentada que usaba una matriz clásica.
	
	Reescriba el programa modificando únicamente la clase TablaDentadaEmpleados
	para que se base en una secuencia de secuencias de datos Empleado. 
	Por ejemplo:
	
		class TablaDentadaEmpleados {
		
		private:
		
			static const int TAMANIO = 50; // "Filas" disponibles
			SecuenciaEmpleados vector_privado[TAMANIO];
			
			//PRE: 0 <= total_utilizados <= TAMANIO
			
			int total_utilizados; // Número de casillas ocupadas
		
		
	Tan sólo debería modificar la clase TablaDentadaEmpleados. Las demás 
	clases y, especialmente, la función main deberían permanecer sin cambios, 
	como en la versión anterior.

*/
/****************************************************************************/
#include<iostream>
using namespace std;

/****************************************************************************/
//Declaración de tipos de datos

enum class TipoAjuste {izquierda,centro,derecha};
enum class TipoOrden {apellidos,nombre,fecha,sueldo};

/****************************************************************************/
//Declaración de Clases/Objetos

class ParejaEnteros
{

private:
	
	int primero;
	int segundo;

public:
	
	/*********************************************************************/
	/*********************************************************************/
	//Constructor con argumentos
	
	ParejaEnteros (int el_primero, int el_segundo):
	 	primero (el_primero), 
		segundo (el_segundo) 
	{}
	
	/*********************************************************************/
	//Constructor sin argumentos
	
	ParejaEnteros (void):
	 	primero (0), 
		segundo (0) 
	{}
	
	/*********************************************************************/
	//Modifica el valor del primer entero
	
	void SetPrimero (int el_primero) 
	{ 
		primero = el_primero; 
	}
	
	/*********************************************************************/
	//Modifica el valor del segundo entero
	
	void SetSegundo (int el_segundo) 
	{ 
		segundo = el_segundo;
	}
	
	/*********************************************************************/
	//Modifica la pareja entera
	
	void SetPareja (int el_primero, int el_segundo)
	{
		primero=el_primero;
		segundo=el_segundo;
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Devuelve el primer valor entero
	
	int GetPrimero (void) 
	{ 
		return primero; 
	}
	
	/*********************************************************************/
	//Devuelve el segundo valor entero
	
	int GetSegundo (void) 
	{ 
		return segundo; 
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Construye una cadena de caracteres con la información de la pareja
	
	string ToString () 
	{
		return "{" + to_string(primero) + ", " +to_string(segundo) + "}";
	}
	
};

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
	
	void SetFecha(int el_dia, int el_mes, int el_anio)
	{
		dia=el_dia;
		mes=el_mes;
		anio=el_anio;
	}
	
	void SetFecha(string cadena)
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
	//Constructor con argumentos
	//Leera cadenas de la forma: "apellidos*nombre*dd/mm/aaa*sueldo*"
	
	Empleado(string cadena_en_formato)
	{
		SetEmpleado(cadena_en_formato);
	}
	
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
		string str_fecha=FormateaString(fecha.ToString(),14);
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

class SecuenciaEmpleados
{

private:
	
	static const int EMPLEADOS_MAX=100; //PRE: EMPLEADOS_MAX>0
	Empleado empleado[EMPLEADOS_MAX];
	
	int n_empleados; //PRE: 0<= n_empleados < EMPLEADOS_MAX

public:
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	SecuenciaEmpleados(void)
    {
		n_empleados=0;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve el número de empleados en la plantilla (secuencia)
	int GetNumeroEmpleados (void)
	{
		return n_empleados;
	}
	
	/**********************************************************************/
	//Devuelve el máximo número de empleados que se pueden almacenar
	int GetMaximoEmpleados (void)
	{
		return EMPLEADOS_MAX;
	}
	
	/**********************************************************************/
	//Devuelve el elemento de la casilla "indice"
	//PRE: 0 <= indice < n_empleados
	
	Empleado EmpleadoNumero(int indice)
	{
		return (empleado[indice]);
	}
	
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	//Sustituye a un empleado por otro nuevo
	//PRE: 0 <= indice < n_empleados
	void SetEmpleado(int indice, Empleado empleado_nuevo )
	{
		empleado[indice]=empleado_nuevo;
	}
		
	/**********************************************************************/
	/**********************************************************************/
	//"Vacía" la secuencia
	
	void EliminaTodos()
	{
		n_empleados=0;
	}
	
	/**********************************************************************/
	// Añade un elemento ("nuevo") al vector.
    // PRE: n_empleados < EMPLEADOS_MAX
    // 		La adición se realiza si hay alguna casilla disponible.
    // 		El nuevo elemento se coloca al final del vector.
    // 		Si no hay espacio, no se hace nada.

    void Aniade (Empleado empleado_nuevo)
    {
        if (n_empleados<EMPLEADOS_MAX)
		{
            empleado[n_empleados]=empleado_nuevo;
            n_empleados++;
        }
    }
    
    /**********************************************************************/
    // Elimina el empleado de la posición dada por "indice".
    // Realiza un borrado físico (desplazamiento y sustitución).
    // PRE: 0 <= indice < n_empleados

    void Elimina (int indice)
    {
        if ((indice>=0)&&(indice<n_empleados)) 
		{
            int tope=n_empleados-1; // posic. del último

            for (int i=indice; i<tope; i++)
            {
            	empleado[i] = empleado[i+1];
			}

            n_empleados--;
        }
    }
	
	/**********************************************************************/
	// Inserta el empleado "empleado_nuevo" en la posición dada por "indice".
    // Desplaza todos los empleados una posición a la derecha antes de 
	// copiar en "indice" en valor "empleado_nuevo".
	// PRE: 0 <= indice < n_empleados < EMPLEADOS_MAX
    // 		La inserción se realiza si hay alguna casilla disponible.
    // 		Si no hay espacio, no se hace nada.
    
	void Inserta (int indice, Empleado empleado_nuevo)
	{
        if ((indice>=0) && (indice<n_empleados)&&(n_empleados<EMPLEADOS_MAX)) 
		{
			for (int i=n_empleados; i>indice ;i--)
			{
				empleado[i]=empleado[i-1];	
			}
			
			empleado[indice]=empleado_nuevo;
			
			n_empleados++;		
		}
	}
	/**********************************************************************/
	//Devuelve en una string la plantilla de empleados
	
	string ToString(void)
	{
		string plantilla;
		
		for(int i=0; i<n_empleados ; i++)
		{
			plantilla+=empleado[i].ToStringEnFormato()+"\n";
		}
		
		return plantilla;
	}
	
	/**********************************************************************/
	//Ordena la secuencia según un criterio introducido
	//por defecto los ordenará por apellidos
	
	void OrdenaEmpleados(TipoOrden criterio=TipoOrden::apellidos)
	{
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
			
			aux=empleado[separador];      //Intercambio ambos valores con una 
			empleado[separador]=empleado[pos_min];	      //variable auxiliar
			empleado[pos_min]=aux;	
		}
	}

};

class TablaDentadaEmpleados							//Secuencia de secuencias
{

private:
	
	
	static const int TAMANIO = 50; 	// "Filas" disponibles
	SecuenciaEmpleados vector_privado[TAMANIO];
	
	//PRE: 0 <= total_utilizados <= TAMANIO
	
	int total_utilizados; // Número de casillas ocupadas

public:
	
	/*********************************************************************/
	//Constructor sin argumentos
	
	TablaDentadaEmpleados(void):
		total_utilizados(0)
	{}
	
	/*********************************************************************/
	//Constructor con argumentos
	// Recibe "primera_fila" (una secuencia de empleados).
	// PRE: primera_fila.TotalUtilizados() <= NUM_EMP
	
	TablaDentadaEmpleados(SecuenciaEmpleados primera_fila)
	{
		vector_privado[0]=primera_fila;
		total_utilizados=1;
	}
	/*********************************************************************/
	/*********************************************************************/
	//Métodos Get
	
	//Devuelve el número máximo de empleados por departamento
	
	int GetMaximoEmpleadosDept(void)
	{
		return (SecuenciaEmpleados().GetNumeroEmpleados());
	}
	
	/*********************************************************************/
	//Devuelve el número máximo de departamentos
	
	int GetMaximoNumDept(void)
	{
		return (TAMANIO);
	}
	
	/*********************************************************************/
	//Devuelve el número de departamentos de la tabla
	
	int NumDepartamentos(void)
	{
		return total_utilizados;
	}
	
	/*********************************************************************/
	//Devuelve el número de empleados de un departamento concreto
	//PRE:0 <= indice_departamento < num_departamentos
	
	int NumEmpDept(int indice_departamento)
	{
		return (vector_privado[indice_departamento].GetNumeroEmpleados());
	}
	
	/*********************************************************************/
	//Devuelve el número de empleados total
	
	int TotalEmpleados(void)
	{
		int suma=0;
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			suma+=NumEmpDept(dept);
		}
		
		return suma;
	}
	
	/*********************************************************************/
	//Devuelve un empleado dada su posicion en la tabla (departamento y numero)
	// PRE: 0 <= departamento < num_departamentos
	// PRE: 0 <= numero < num_empleados[fila]
	
	Empleado EmpleadoPosicion(int departamento, int numero)
	{
		return vector_privado[departamento].EmpleadoNumero(numero);
	}
	
	/*********************************************************************/
	// Devuelve una fila completa (un objeto "SecuenciaEmpleados")
	// PRE: 0 <= indice_dept < total_utilizados
	
	SecuenciaEmpleados Departamento (int indice_dept)
	{
		return (vector_privado[indice_dept]);
	}
	
	/*********************************************************************/
	// Añade una fila completa (un objeto "SecuenciaEmpleados") a la tabla
	// PRE: total_utilizados < TAMANIO
	
	void Aniade (SecuenciaEmpleados departamento)
	{
		if (total_utilizados < TAMANIO) 
		{
			vector_privado[total_utilizados] = departamento;
			total_utilizados++;
		}
	}
	
	/*********************************************************************/
	//Elimina todos los elementos de la tabla
	
	void EliminaTodos(void)
	{
		total_utilizados=0;
	}
	
	/*********************************************************************/
	//Elimina un Departamento
	// PRE: 0 <= indice_departamento < total_utilizados
	
	void Elimina (int indice_departamento)
	{
		if ((indice_departamento>=0) && (indice_departamento<total_utilizados)) 
		{
			int tope = total_utilizados-1; // posición del último
			for (int i = indice_departamento ; i < tope ; i++)
			{
				vector_privado[i] = vector_privado[i+1];
			}
			
			total_utilizados--;
		}
	}
	
	/*********************************************************************/
	//Devuelve las coordenadas de la pareja con menos salario
	
	ParejaEnteros LocalizacionSalarioMenor (void)
	{
		int fila;
		int columna;
		
		double salario_menor=999999999; //Cualquier salario será menor que este
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			for(int emp=0; emp<NumEmpDept(dept); emp++)
			{
				if(EmpleadoPosicion(dept,emp).GetSueldo()<=salario_menor)
				{
					salario_menor=EmpleadoPosicion(dept,emp).GetSueldo();
					fila=dept;
					columna=emp;
				}
			}
		}
		
		return(ParejaEnteros(fila,columna));
		
	}
	/*********************************************************************/
	//Devuelve las coordenadas de la pareja con mayor salario
	
	ParejaEnteros LocalizacionSalarioMayor (void)
	{
	
		int fila;
		int columna;
		
		double salario_mayor=-1; //Cualquier salario será mayor que este
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			for(int emp=0; emp<NumEmpDept(dept); emp++)
			{
				
				if(EmpleadoPosicion(dept,emp).GetSueldo()>=salario_mayor)
				{
					salario_mayor=EmpleadoPosicion(dept,emp).GetSueldo();
					fila=dept;
					columna=emp;
				}
			}
		}
		
		return ParejaEnteros(fila,columna);
		
	}
	/*********************************************************************/
	//Devuelve el salario medio de todos los departamentos
	
	double SalarioMedio(void)
	{
		double suma=0; //Acumulador
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			for(int emp=0; emp<NumEmpDept(dept); emp++)
			{
				suma+=Departamento(dept).EmpleadoNumero(emp).GetSueldo();
			}
		}
		
		return (suma/TotalEmpleados());
	}
	
	/*********************************************************************/
	//Devuelve una secuencia con los empleados por debajo del sueldo "valor"
	
	SecuenciaEmpleados EmpleadosSalarioMenor (double valor)
	{
		SecuenciaEmpleados secuencia;
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			for(int emp=0; emp<NumEmpDept(dept); emp++)
			{
				if(EmpleadoPosicion(dept,emp).GetSueldo()<valor)
				{
					secuencia.Aniade(EmpleadoPosicion(dept,emp));
				}
			}
		}
		
		return secuencia;
	}
	
	/*********************************************************************/
	//Devuelve una secuencia con los empleados por encima del sueldo "valor"
	
	SecuenciaEmpleados EmpleadosSalarioMayor (double valor)
	{
		SecuenciaEmpleados secuencia;
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			for(int emp=0; emp<NumEmpDept(dept); emp++)
			{
				if(EmpleadoPosicion(dept,emp).GetSueldo()>valor)
				{
					secuencia.Aniade(EmpleadoPosicion(dept,emp));
				}
			}
		}
		
		return secuencia;
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Devuelve una cadena de caracteres representativa de la tabla
	//organizada por departamentos
	
	string ToStringEnDept(void)
	{
		string cadena;
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			cadena+="Departamento "+to_string(dept+1)+"   ("
					+to_string(NumEmpDept(dept))+" empleados)"+ ":\n\n"
				    +Departamento(dept).ToString()+"\n";
		}
		
		return cadena;
	}
	
	/*********************************************************************/
	//Devuelve una cadena de caracteres representativa de la tabla
	//sin tener en cuenta los departamentos
	
	string ToStringLista(void)
	{
		string cadena;
		
		for(int dept=0; dept<total_utilizados; dept++)
		{
			cadena+=Departamento(dept).ToString();
		}
		
		return cadena;
	}
	
	/*********************************************************************/
	/*********************************************************************/

};

/****************************************************************************/
/****************************************************************************/
int main()
{
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	
	//#define RELLENA_MANUAL 			//Recibe los datos del propio programa
	#define REDIRECCION_ENTRADA			//Recibe los datos de un archivo txt
	
	//Declaración de Datos

	TablaDentadaEmpleados tabla1;
	
	#ifdef RELLENA_MANUAL
	
	//Relleno la tabla manualmente para facilitar la prueba del programa
	
	SecuenciaEmpleados dept_1;
	SecuenciaEmpleados dept_2;
	SecuenciaEmpleados dept_3;
	
	
	dept_1.Aniade(Empleado("Martinez Lopez*Juan Carlos*10/12/2000*2345.55*"));
	dept_1.Aniade(Empleado("Mendez Morales*Paula*04/06/1998*1234.59*"));
	dept_1.Aniade(Empleado(
				"Abad y Paz de Molina*Maria del Carmen*01/01/1999*3002.88*"));
	dept_1.Aniade(Empleado("Pi Garcia*Jose*05/05/1987*1987.44*"));
	dept_1.Aniade(Empleado("Martin Martinez*Patricia*04/06/1998*1234.56*"));
	
	dept_2.Aniade(Empleado("Santa Claus*Noel*24/12/1990*2142.55*"));
	dept_2.Aniade(Empleado("Ferrero Roche*Bombon*25/12/1991*1123.66*"));
	dept_2.Aniade(Empleado("Gaspar y Baltasar*Melchor*06/01/1985*2222.22*"));
	
	dept_3.Aniade(Empleado("Martin Berlanga*Joaquin*10/10/1990*1145.33*"));
	dept_3.Aniade(Empleado("Bernal Bernardilla*Bernarda*02/10/1986*1356.00*"));
	dept_3.Aniade(Empleado("Belmez de Jaen*Cesar Augusto*01/01/1998*999.99*"));
	dept_3.Aniade(Empleado(
						"Joaquina Melgarejo*Maria Luisa*10/10/1984*1345.66*"));
	
	
	tabla1.Aniade(dept_1);
	tabla1.Aniade(dept_2);
	tabla1.Aniade(dept_3);
	
	//En este punto ya tengo rellena la tabla manualmente
	
	#endif
	
	
	#ifdef REDIRECCION_ENTRADA
	
	SecuenciaEmpleados dept[tabla1.GetMaximoNumDept()];
	
	//Leo del documento de texto
	string linea;
	
	//La primera línea me indica el número de departamentos que hay 
	getline(cin,linea);
	int n_dept=stoi(linea);
	
	for(int i=0; i<n_dept; i++)
	{
		//La primera línea indica el número de empleados que hay en el dpto
		getline(cin,linea);
		int n_emp=stoi(linea);
		
		for(int j=0; j<n_emp; j++)
		{
			getline(cin,linea);
			dept[i].Aniade(Empleado(linea));
		}
	}
	
	//Relleno la tabla con los datos leídos
	
	for(int i=0; i<n_dept; i++)
	{
		tabla1.Aniade(dept[i]);
	}
	
	
	#endif
	
	cout<<"LISTADO DE EMPLEADOS: "<<endl;
	
	cout<<endl;
	cout<<"Num. total de empleados = "<<tabla1.TotalEmpleados()<<endl;
	cout<<endl;
	
	cout<<tabla1.ToStringLista();
	
	cout<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
	
	cout<<"LISTADO DE EMPLEADOS POR DEPARTAMENTO"<<endl;
	
	cout<<endl;
	cout<<"Num. Departamentos = "<<tabla1.NumDepartamentos()<<endl;
	cout<<endl;

	cout<<tabla1.ToStringEnDept();
	
	cout<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
	
	cout<<"Sueldo minimo = "<<tabla1.EmpleadoPosicion
				(tabla1.LocalizacionSalarioMenor().GetPrimero(),
				 tabla1.LocalizacionSalarioMenor().GetSegundo()).GetSueldo()
				 <<endl;
						 
	cout<<tabla1.EmpleadoPosicion
				(tabla1.LocalizacionSalarioMenor().GetPrimero(),
				 tabla1.LocalizacionSalarioMenor().GetSegundo()).
				 ToStringEnFormato()<<endl;
	
	
	cout<<"Departamento = "<<tabla1.LocalizacionSalarioMenor().GetPrimero()+1
		<<endl;
	cout<<endl;
	
	cout<<"Sueldo maximo = "<<tabla1.EmpleadoPosicion
				(tabla1.LocalizacionSalarioMayor().GetPrimero(),
				 tabla1.LocalizacionSalarioMayor().GetSegundo()).GetSueldo()
				 <<endl;
						 
	cout<<tabla1.EmpleadoPosicion
				(tabla1.LocalizacionSalarioMayor().GetPrimero(),
				 tabla1.LocalizacionSalarioMayor().GetSegundo()).
				 ToStringEnFormato()<<endl;
	
	cout<<"Departamento = "<<tabla1.LocalizacionSalarioMayor().GetPrimero()+1
		<<endl;
		
	cout<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<endl;
	
	cout<<"Sueldo medio = "<<tabla1.SalarioMedio()<<endl;
	
	cout<<endl;
	cout<<"Empleados con salario mayor que el sueldo medio:"<<endl;
	cout<<endl;
	
	SecuenciaEmpleados sueldo_alto=
						tabla1.EmpleadosSalarioMayor(tabla1.SalarioMedio());
	
	sueldo_alto.OrdenaEmpleados();
	
	cout<<sueldo_alto.ToString();
	
	cout<<endl;
	cout<<"Empleados con salario menor que el sueldo medio:"<<endl;
	cout<<endl;
	
	SecuenciaEmpleados sueldo_bajo=
						tabla1.EmpleadosSalarioMenor(tabla1.SalarioMedio());
	
	sueldo_bajo.OrdenaEmpleados();
	
	cout<<sueldo_bajo.ToString();
	
	
	return 0;
}

