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
