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
