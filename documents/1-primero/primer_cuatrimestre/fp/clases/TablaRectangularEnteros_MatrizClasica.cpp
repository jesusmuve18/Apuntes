class TablaRectangularEnteros							//Matriz clásica		
{
	
private:
	
	static const int NUM_FILS=100; //Filas reservadas
	static const int NUM_COLS=100; //Columnas reservadas
	
	int matriz_privada[NUM_FILS][NUM_COLS];
	
	//PRE: 0 <= filas_utilizadas <= NUM_FILS
	
	int filas_utilizadas; //Filas utilizadas
	
	//PRE: 0 <= cols_utilizadas <= NUM_COLS
	
	int cols_utilizadas; //Columnas utilizadas
	
public:
	
	/*********************************************************************/
	//Constructor sin argumentos
	
	TablaRectangularEnteros(void):
		filas_utilizadas(0),
		cols_utilizadas(0)
	{}
	
	/*********************************************************************/
	//Constructores con argumentos
	
	// Recibe "numero_de_columnas" que indica el número de
	// columnas que deben tener TODAS las filas.
	// PRE: numero_de_columnas <= NUM_COLS
	
	TablaRectangularEnteros(int num_columnas):
		filas_utilizadas(0),
		cols_utilizadas(num_columnas)
	{}
	
	
	// Recibe "primera_fila" (una secuencia de enteros).
	// El número de elementos de la secuencia es el valor que se
	// usará como "cols_utilizadas"
	// PRE: primera_fila.TotalUtilizados() <= NUM_COLS
	
	TablaRectangularEnteros (SecuenciaEnteros primera_fila):
		filas_utilizadas(0),
		cols_utilizadas (primera_fila.TotalUtilizados())
	{
		Aniade(primera_fila); // Actualiza "filas_utilizadas"
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Devuelve la capacidad de las filas 
	
	int CapacidadFilas(void)
	{
		return NUM_FILS;
	}
	
	/*********************************************************************/
	//Devuelve la capacidad de las columnas 
	
	int CapacidadColumnas(void)
	{
		return NUM_COLS;
	}
	
	/*********************************************************************/
	//Devuelve las filas utilizadas
	
	int FilasUtilizadas(void)
	{
		return filas_utilizadas;
	}
	
	/*********************************************************************/
	//Devuelve las columnas utilizadas
	
	int ColumnasUtilizadas(void)
	{
		return cols_utilizadas;
	}
	
	/*********************************************************************/
	//Devuelve el elemento dada su posición (fila y columna)
	// PRE: 0 <= fila < filas_utilizadas
	// PRE: 0 <= columna < cols_utilizadas
	
	int Elemento(int fila, int columna)
	{
		return matriz_privada[fila][columna];
	}
	
	/*********************************************************************/
	//Devuelve una fila en forma de secuencia de enteros
	// PRE: 0 <= indice_fila < NUM_FILS
	
	SecuenciaEnteros Fila(int indice_fila)
	{
		SecuenciaEnteros secuencia;
		
		//Fijo la fila y voy variando la columna
		for(int i=0; i<cols_utilizadas; i++)
		{
			secuencia.Aniade(matriz_privada[indice_fila][i]);
		}
		
		return (secuencia);
	}
	
	/*********************************************************************/
	//Devuelve una columna en forma de secuencia de enteros
	// PRE: 0 <= indice_columna < NUM_COLS
	
	SecuenciaEnteros Columna(int indice_columna)
	{
		SecuenciaEnteros secuencia;
		
		//Fijo la columna y voy variando la fila
		for(int i=0; i<filas_utilizadas; i++)
		{
			secuencia.Aniade(matriz_privada[i][indice_columna]);
		}
		
		return (secuencia);
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Añade una secuencia de enteros como una nueva fila
	// PRE: filas_utilizadas < NUM_FILS
	// PRE: fila_nueva.TotalUtilizados() < NUM_COLS
	
	void Aniade(SecuenciaEnteros fila_nueva)
	{
		for(int i=0; i<fila_nueva.TotalUtilizados(); i++)
		{
			matriz_privada[filas_utilizadas][i]=fila_nueva.Elemento(i);
		}
		
		filas_utilizadas++;
		
		if(fila_nueva.TotalUtilizados()>cols_utilizadas)
		{
			cols_utilizadas=fila_nueva.TotalUtilizados();
		}
	}
	
	/*********************************************************************/
	//"Elimina" todos los elementos de la tabla
	
	void EliminaTodos(void)
	{
		filas_utilizadas=0;
		cols_utilizadas=0;
	}
	
	/*********************************************************************/
	//Consulta si la tabla está vacía
	
	bool EstaVacia (void)
	{
		return (filas_utilizadas || cols_utilizadas);
	}
	
	/*********************************************************************/
	//Comprueba si es igual a otra tabla rectangular
	
	bool EsIgual (TablaRectangularEnteros otra)
	{
		bool iguales=true;
		
		//Si las dimensiones no coinciden ya son diferentes
		
		if(cols_utilizadas!=otra.ColumnasUtilizadas() || 
		   				filas_utilizadas!=otra.FilasUtilizadas())
		{
			iguales=false;
		}
		else 
		{
			//Compruebo elemento a elemento que sean iguales
			
			for(int fila=0; fila<filas_utilizadas;fila++)//Recorro filas
			{
				for(int col=0; col<cols_utilizadas;col++)//Recorro columnas
				{
					if(Elemento(fila,col)!=otra.Elemento(fila,col))
					{
						iguales=false;
					}
				}
			}
		}		
		
		return iguales;	
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Devuelve la tabla en formato string
	//Tiene el separador por defecto con posibildad de cambiarlo
	//Se lee puede agregar un margen a la izquierda
	
	string ToString(string separador=" ", int margen=0)
	{
		string cadena;
		string str_margen;
		
		if(margen>0)
		{
			for(int i=0; i<margen; i++)
			{
				str_margen+=" ";
			}
		}
		
		for(int fila=0; fila<filas_utilizadas; fila++)// recorro filas
		{
			cadena+=str_margen+Fila(fila).ToString(separador)+"\n";
		}
		
		return cadena;
	}
	
	/*********************************************************************/
	/*********************************************************************/
	// Devuelve el índice de la fila que más se parezca a un vector dado
	// referencia de enteros buscando en las filas dadas por el vector
	// filas_a_comparar. La similitud vendrá dada por la distancia euclídea
	// entre ambos vectores
	
	int FilaSimilarMasCercana(SecuenciaEnteros referencia, 
							  SecuenciaEnteros filas_a_comparar )
	{
		int distancia_minima=-1; //Le doy un valor negativo para que 
								 //cualquiera que calcule sea mayor
		int indice_fila;
		
		
		for(int i=0; i<filas_a_comparar.TotalUtilizados(); i++)
		{
			int diferencia_coordenadas;
			int suma_cuadrados=0;
			int distancia_aux;
			
			
			for(int j=0; j<referencia.TotalUtilizados(); j++)
			{
				//Calculo la distancia a cada vector
				diferencia_coordenadas+=
						Fila(filas_a_comparar.Elemento(i)).Elemento(j)
					  - referencia.Elemento(j);
					  
				suma_cuadrados+=pow(diferencia_coordenadas,2);
					
			}
			
			distancia_aux=sqrt(suma_cuadrados);
			
			if(distancia_aux<=distancia_minima)
			{
				distancia_minima=distancia_aux;
				indice_fila=filas_a_comparar.Elemento(i);
			}
			
		}
		
		return indice_fila;
		
	}
};
