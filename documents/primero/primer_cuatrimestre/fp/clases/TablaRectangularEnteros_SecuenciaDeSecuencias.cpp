class TablaRectangularEnteros 						//Secuencia de secuencias
{
	
private:
	
	static const int NUM_FILS=100; 	//Filas reservadas
	
	// (NUM_COlS = TAMANIO de SecuenciaEnteros)
	static const int NUM_COLS=4; 	//Columnas reservadas 
	
	SecuenciaEnteros vector_privado[NUM_FILS];
	
	//PRE: 0 <= filas_utilizadas <= NUM_FILS
	// PRE: 0 <= cols_utilizadas <= NUM_COLS

	
	int filas_utilizadas; 	//Filas utilizadas	
	int cols_utilizadas; 	//Columnas utilizadas
	
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
		return (vector_privado[fila].Elemento(columna));
	}
	
	/*********************************************************************/
	//Devuelve una fila en forma de secuencia de enteros
	// PRE: 0 <= indice_fila < NUM_FILS
	
	SecuenciaEnteros Fila(int indice_fila)
	{
		return (vector_privado[indice_fila]);
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
			secuencia.Aniade(vector_privado[i].Elemento(indice_columna));
		}
		
		return (secuencia);
	}
	
	/*********************************************************************/
	/*********************************************************************/
	// Añade una secuencia de enteros como una nueva fila
	// PRE: fila.TotalUtilizados() == cols_utilizadas
	// PRE: filas_utilizadas < NUM_FILS
	
	void Aniade(SecuenciaEnteros fila_nueva)
	{
		int numero_cols_fila=fila_nueva.TotalUtilizados();
		
		
		if ((filas_utilizadas<NUM_FILS)&&(numero_cols_fila==NUM_COLS)) 
		{
			vector_privado[filas_utilizadas]=fila_nueva;
			filas_utilizadas++;
		}

	}
	
	/*********************************************************************/
	// Elimina una fila completa (un objeto "SecuenciaCaracteres").
	// PRE: 0 <= num_fila < TotalUtilizados()
	
	void EliminaFila (int indice_fila)
	{
		if ((0<=indice_fila) && (indice_fila <=filas_utilizadas)) 
		{
			// "Desplazar" las filas hacia posiciones bajas.
			// En la posición "num_fila" se copia la que está
			// en la siguiente ("num_fila"+1), ... y en la posición
			// "total_utilizados"-2 se copia la de "total_utilizados"-1.
			
			for (int fil=indice_fila; fil<filas_utilizadas-1; fil++)
			{
				vector_privado[fil]=vector_privado[fil+1];	
			}
		
			filas_utilizadas--; // Actualizar el tamaño de la tabla.
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
