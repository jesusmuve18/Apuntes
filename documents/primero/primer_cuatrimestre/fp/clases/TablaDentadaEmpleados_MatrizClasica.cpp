class TablaDentadaEmpleados								//Matriz clásica
{
	/*********************************************************************/
	/*********************************************************************/
	/*ESTRUCTURA DE LA TABLA:
				
				  --------------------------------------------------------
			     |	 EMP_1	   EMP_2	 EMP_3	   EMP_4	 EMP_5	  ... |
		 --------|--------------------------------------------------------|
		| DEPT_1 | Empleado  Empleado  Empleado  Empleado  Empleado	   	  |
		| DEPT_2 | Empleado  Empleado  Empleado  	-		  -    	 	  |
		| DEPT_3 | Empleado  Empleado  Empleado  Empleado     -    		  |
		| DEPT_4 | Empleado  Empleado      -        -         -  	  	  |
		|   ...  | Empleado  Empleado  Empleado  Empleado  Empleado   ... |
		 -----------------------------------------------------------------
		
	*/
	/*********************************************************************/
	/*********************************************************************/
private:
	
	
	//Número de Departamentos máximos
	static const int  NUM_DEPT= 10; // Filas reservadas
	
	//Número de empleados máximo por departamento
	static const int NUM_EMP = 50; // Columnas reservadas
	
	Empleado matriz_privada[NUM_DEPT][NUM_EMP];
	
	// PRE: 0 <= filas_utilizadas <= NUM_FILS
	int num_departamentos; // Filas utilizadas
	
	// PRE: 0 <= num_cols_utilizadas[i] <= NUM_COLS
	// para i=0,1,...,filas_utilizadas-1
	
	// Número de empleados que hay en cada departamento
	int num_empleados[NUM_DEPT] = {0}; // Columnas utilizadas

public:
	
	/*********************************************************************/
	//Constructor sin argumentos
	
	TablaDentadaEmpleados(void):
		num_departamentos(0)
	{}
	
	/*********************************************************************/
	//Constructor con argumentos
	// Recibe "primera_fila" (una secuencia de empleados).
	// PRE: primera_fila.TotalUtilizados() <= NUM_EMP
	
	TablaDentadaEmpleados(SecuenciaEmpleados primera_fila):
		num_departamentos(0)
	{
		Aniade(primera_fila); //Actualiza "num_departamentos"
	}
	/*********************************************************************/
	/*********************************************************************/
	//Métodos Get
	
	//Devuelve el número máximo de empleados por departamento
	
	int GetMaximoEmpleadosDept(void)
	{
		return NUM_EMP;
	}
	
	/*********************************************************************/
	//Devuelve el número máximo de departamentos
	
	int GetMaximoNumDept(void)
	{
		return NUM_DEPT;
	}
	
	/*********************************************************************/
	//Devuelve el número de departamentos de la tabla
	
	int NumDepartamentos(void)
	{
		return num_departamentos;
	}
	
	/*********************************************************************/
	//Devuelve el número de empleados de un departamento concreto
	//PRE:0 <= indice_departamento < num_departamentos
	
	int NumEmpDept(int indice_departamento)
	{
		return num_empleados[indice_departamento];
	}
	
	/*********************************************************************/
	//Devuelve el número de empleados total
	
	int TotalEmpleados(void)
	{
		int suma=0;
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			suma+=num_empleados[dept];
		}
		
		return suma;
	}
	
	/*********************************************************************/
	//Devuelve un empleado dada su posicion en la tabla (departamento y numero)
	// PRE: 0 <= departamento < num_departamentos
	// PRE: 0 <= numero < num_empleados[fila]
	
	Empleado EmpleadoPosicion(int departamento, int numero)
	{
		return matriz_privada[departamento][numero];
	}
	
	/*********************************************************************/
	// Devuelve una fila completa (un objeto "SecuenciaEmpleados")
	// PRE: 0 <= indice_dept < num_departamentos
	
	SecuenciaEmpleados Departamento (int indice_dept)
	{
		SecuenciaEmpleados fila;
		
		if ((0 <= indice_dept) && (indice_dept < num_departamentos)) 
		{
			int num_columnas = num_empleados[indice_dept];
			
			for (int emp=0; emp < num_columnas; emp++)
			{
				fila.Aniade(matriz_privada[indice_dept][emp]);
			}
			
		}
		
		return (fila);
	}
	
	/*********************************************************************/
	// Añade una fila completa (un objeto "SecuenciaEmpleados") a la tabla
	// PRE: departamento.TotalUtilizados() = NUM_EMP
	// PRE: num_departamentos < NUM_DEPT
	
	void Aniade (SecuenciaEmpleados departamento)
	{
		int numero_emp_dept=departamento.GetNumeroEmpleados();
		
		if ((num_departamentos < NUM_DEPT) && (numero_emp_dept <= NUM_EMP)) 
		{
			for (int c=0; c<numero_emp_dept; c++)
			{
				matriz_privada[num_departamentos][c]=
									departamento.EmpleadoNumero(c);
			}
			
			num_empleados[num_departamentos]=numero_emp_dept;
			num_departamentos++;
		}
	}
	
	/*********************************************************************/
	//Elimina todos los elementos de la tabla
	
	
	void EliminaTodos(void)
	{
		num_departamentos=0;
	}
	
	/*********************************************************************/
	/*Elimina un Departamento
	
	void EliminaDepartamento(int indice_Departamento)
	{
		
	}
	/*
	/*********************************************************************/
	//Devuelve las coordenadas de la pareja con menos salario
	
	ParejaEnteros LocalizacionSalarioMenor (void)
	{
		int fila;
		int columna;
		
		double salario_menor=999999999; //Cualquier salario será menor que este
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			for(int emp=0; emp<num_empleados[dept]; emp++)
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
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			for(int emp=0; emp<num_empleados[dept]; emp++)
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
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			for(int emp=0; emp<num_empleados[dept]; emp++)
			{
				suma+=EmpleadoPosicion(dept,emp).GetSueldo();
			}
		}
		
		return (suma/TotalEmpleados());
	}
	
	/*********************************************************************/
	//Devuelve una secuencia con los empleados por debajo del sueldo "valor"
	
	SecuenciaEmpleados EmpleadosSalarioMenor (double valor)
	{
		SecuenciaEmpleados secuencia;
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			for(int emp=0; emp<num_empleados[dept]; emp++)
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
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			for(int emp=0; emp<num_empleados[dept]; emp++)
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
		
		for(int dept=0; dept<num_departamentos; dept++)
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
		
		for(int dept=0; dept<num_departamentos; dept++)
		{
			cadena+=Departamento(dept).ToString();
		}
		
		return cadena;
	}
	
	/*********************************************************************/
	/*********************************************************************/
	


};
