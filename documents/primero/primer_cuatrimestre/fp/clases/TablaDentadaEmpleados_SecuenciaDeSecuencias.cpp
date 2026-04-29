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
