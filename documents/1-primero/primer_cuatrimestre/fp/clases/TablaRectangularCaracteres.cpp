class TablaRectangularCaracteres
{
	private:
		
		static const int NUM_FILS=100; //Filas reservadas
		static const int NUM_COLS=100; //Columnas reservadas
		
		char matriz_privada[NUM_FILS][NUM_COLS];
		
		//PRE: 0 <= filas_utilizadas <= NUM_FILS
		
		int filas_utilizadas; //Filas utilizadas
		
		//PRE: 0 <= cols_utilizadas <= NUM_COLS
		
		int cols_utilizadas; //Columnas utilizadas
		
	public:
		
		/*****************************************************************/
		//Constructor sin argumentos
		
		TablaRectangularCaracteres(void):
			filas_utilizadas(0),
			cols_utilizadas(0)
		{}
		
		/*****************************************************************/
		//Constructores con argumentos
		
		// Recibe "numero_de_columnas" que indica el número de
		// columnas que deben tener TODAS las filas.
		// PRE: numero_de_columnas <= NUM_COLS
		
		TablaRectangularCaracteres(int num_columnas):
			filas_utilizadas(0),
			cols_utilizadas(num_columnas)
		{}
		
		
		// Recibe "primera_fila" (una secuencia de caracteres).
		// El número de elementos de la secuencia es el valor que se
		// usará como "cols_utilizadas"
		// PRE: primera_fila.TotalUtilizados() <= NUM_COLS
		
		TablaRectangularCaracteres (SecuenciaCaracteres primera_fila):
			filas_utilizadas(0),
			cols_utilizadas (primera_fila.TotalUtilizados())
		{
			Aniade(primera_fila); // Actualiza "filas_utilizadas"
		}
		
		/*****************************************************************/
		/*****************************************************************/
		//Devuelve la capacidad de las filas 
		
		int CapacidadFilas(void)
		{
			return NUM_FILS;
		}
		
		/*****************************************************************/
		//Devuelve la capacidad de las columnas 
		
		int CapacidadColumnas(void)
		{
			return NUM_COLS;
		}
		
		/*****************************************************************/
		//Devuelve las filas utilizadas
		
		int FilasUtilizadas(void)
		{
			return filas_utilizadas;
		}
		
		/*****************************************************************/
		//Devuelve las columnas utilizadas
		
		int ColumnasUtilizadas(void)
		{
			return cols_utilizadas;
		}
		
		/*****************************************************************/
		//Devuelve el elemento dada su posición (fila y columna)
		// PRE: 0 <= fila < NUM_FILS
		// PRE: 0 <= columna < NUM_COLS
		
		char Elemento(int fila, int columna)
		{
			return matriz_privada[fila][columna];
		}
		
		/*****************************************************************/
		//Devuelve una fila en forma de secuencia de caracteres
		// PRE: 0 <= indice_fila < NUM_FILS
		
		SecuenciaCaracteres Fila(int indice_fila)
		{
			string cadena_caracteres;
			
			//Fijo la fila y voy variando la columna
			for(int i=0; i<cols_utilizadas; i++)
			{
				cadena_caracteres+=matriz_privada[indice_fila][i];
			}
			
			return (SecuenciaCaracteres(cadena_caracteres));
		}
		
		/*****************************************************************/
		//Devuelve una columna en forma de secuencia de caracteres
		// PRE: 0 <= indice_columna < NUM_COLS
		
		SecuenciaCaracteres Columna(int indice_columna)
		{
			string cadena_caracteres;
			
			//Fijo la columna y voy variando la fila
			for(int i=0; i<filas_utilizadas; i++)
			{
				cadena_caracteres+=matriz_privada[i][indice_columna];
			}
			
			return (SecuenciaCaracteres(cadena_caracteres));
		}
		
		/*****************************************************************/
		/*****************************************************************/
		//Añade una secuencia de caracteres como una nueva fila
		// PRE: filas_utilizadas < NUM_FILS
		// PRE: fila_nueva.TotalUtilizados() < NUM_COLS
		
		void Aniade(SecuenciaCaracteres fila_nueva)
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
		
		/*****************************************************************/
		//"Elimina" todos los elementos de la tabla
		
		void EliminaTodos(void)
		{
			filas_utilizadas=0;
			cols_utilizadas=0;
		}
		
		/*****************************************************************/
		//Consulta si la tabla está vacía
		
		bool EstaVacia (void)
		{
			return (filas_utilizadas || cols_utilizadas);
		}
		
		/*****************************************************************/
		//Comprueba si es igual a otra tabla rectangular
		
		bool EsIgual (TablaRectangularCaracteres otra)
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
		
		/*****************************************************************/
		/*****************************************************************/
		//Devuelve la tabla en formato string
		
		string ToString(void)
		{
			string cadena;
			
			for(int fila=0; fila<filas_utilizadas; fila++)// recorro filas
			{
				cadena+=Fila(fila).ToString()+"\n";
			}
			
			return cadena;
		}
	
};
