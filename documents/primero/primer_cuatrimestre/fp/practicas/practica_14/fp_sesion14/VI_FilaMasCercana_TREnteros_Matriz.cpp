/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_51
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Ampliar la clase TablaRectangularEnteros con un método que busque la fila 
	de la matriz que más se parezca a un vector de enteros (clase 
	SecuenciaEnteros) al que llamaremos referencia. El método devolverá el 
	número de la fila.
	
	La similitud entre dos vectores x = (x1 · · · xp) e y = (y1 · · · yp) 
	vendrá dada por la distancia euclídea entre ambos:
						  ___________________________________
			dist(x, y) = V (x1 - y1)^2 + · · · + (xp - yp)^2
			
	Además, la búsqueda solo se hará sobre un conjunto seleccionado de filas 
	de la tabla, enumeradas en la secuencia de enteros filas_a_comparar 
	(un objeto de la clase SecuenciaEnteros).
	 
	Por ejemplo, dada la matriz M (7 × 4),
	
								->	3  1  0  8
									4  5  1  5
								->	5  7  1  7
									7  9  6  1
								->	4  9  5  5
								->	2  8  2  2
									7  3  2  5
								
	y los vectores referencia = 2, 8, 1, 1 y filas_a_comparar = 0, 2, 4, 5, el 
	programa deberá encontrar 5 como la fila más cercana a la secuencia 
	referencia (en el dibujo anterior se han marcado con una flecha las filas 
	indicadas por el vector filas_a_comparar).


*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
//Declaración de Clases/Objetos

class SecuenciaEnteros 
{

private:

    static const int TAMANIO = 50; // Núm.casillas disponibles
    int vector_privado[TAMANIO];

    // PRE: 0<=total_utilizados<TAMANIO

    int total_utilizados; // Núm.casillas ocupadas

public:

    /***********************************************************************/
    // Constructor sin argumentos

    SecuenciaEnteros (void) : total_utilizados (0)
    {}
    
    //Constructor con argumentos 
    
    SecuenciaEnteros(string cadena)
    {
    	total_utilizados=cadena.length();
    	
    	for(int i=0; i<total_utilizados ; i++)
    	{
    		vector_privado[i]=(cadena.at(i)-'0');
		}
	}
	
	//Constructor con argumentos
	
	SecuenciaEnteros(int entero)
	{
		int aux;
		int cifras=0;
			
		aux=entero;
		
		while(aux>0) //Cuento el número de cifras que tiene el número
		{
			cifras++;
			aux/=10;
		}
		
		total_utilizados=cifras;
		
		aux=entero;
		
		for(int i=0; i<cifras; i++) //Introduzco cada cifra en su posición
		{
			vector_privado[i]=(aux/(pow(10,cifras-1-i)));
			aux-=vector_privado[i]*pow(10,cifras-1-i);
		}
	}
	
	/***********************************************************************/
	//Actualiza la secuencia con un numero dado separandolo en cifras
	
	void SetSecuencia(int entero)
	{
		int aux;
		int cifras=0;
			
		aux=entero;
		
		while(aux>0) //Cuento el número de cifras que tiene el número
		{
			cifras++;
			aux/=10;
		}
		
		total_utilizados=cifras;
		
		aux=entero;
		
		for(int i=0; i<cifras; i++) //Introduzco cada cifra en su posición
		{
			vector_privado[i]=(aux/(pow(10,cifras-1-i)));
			aux-=vector_privado[i]*pow(10,cifras-1-i);
		}
	}
	
    /***********************************************************************/
    // Devuelve el número de casillas ocupadas

    int TotalUtilizados (void)
    {
        return (total_utilizados);
    }

    /***********************************************************************/
    // Devuelve el número de casillas disponibles

    int Capacidad (void)
    {
        return (TAMANIO);
    }

    /***********************************************************************/
    // "Vacía" completamente la secuencia

	void EliminaTodos()
	{
		total_utilizados = 0;
	}

    /***********************************************************************/
    // Añade un elemento ("nuevo") al vector.
    // PRE: total_utilizados < TAMANIO
    // 		La adición se realiza si hay alguna casilla disponible.
    // 		El nuevo elemento se coloca al final del vector.
    // 		Si no hay espacio, no se hace nada.

    void Aniade (int nuevo)
    {
        if (total_utilizados < TAMANIO){
            vector_privado[total_utilizados] = nuevo;
            total_utilizados++;
        }
    }

    /***********************************************************************/
    // Devuelve el elemento de la casilla "indice"
    // PRE: 0 <= indice < total_utilizados

    int Elemento (int indice)
    {
        return (vector_privado[indice]);
    }

    /***********************************************************************/
    // Cambia el contenido de la casilla "indice" por el valor "nuevo"
    // PRE: 0 <= indice < total_utilizados

   void Modifica (int indice, int nuevo)
   {
		if ((indice >= 0) && (indice < total_utilizados))
			vector_privado[indice] = nuevo;
   }


    /***********************************************************************/
    // Eliminar el carácter de la posición dada por "indice".
    // Realiza un borrado físico (desplazamiento y sustitución).
    // PRE: 0 <= indice < total_utilizados

    void Elimina (int indice)
    {
        if ((indice >= 0) && (indice < total_utilizados)) {

            int tope = total_utilizados-1; // posic. del último

            for (int i = indice ; i < tope ; i++)
                vector_privado[i] = vector_privado[i+1];

            total_utilizados--;
        }
    }
 

    /***********************************************************************/
    // Inserta el carácter "nuevo" en la posición dada por "indice".
    // Desplaza todos los caracteres una posición a la derecha antes de 
	// copiar en "indice" en valor "nuevo".
	// PRE: 0 <= indice < total_utilizados
    // PRE: total_utilizados < TAMANIO
    // 		La inseerción se realiza si hay alguna casilla disponible.
    // 		Si no hay espacio, no se hace nada.
    
	void Inserta (int indice, int valor_nuevo)
	{
        if ((indice >= 0) && (indice < total_utilizados) 
		    && (total_utilizados < TAMANIO)) {
		
			for (int i = total_utilizados ; i > indice ; i--)
				vector_privado[i] = vector_privado[i-1];
			
			vector_privado[indice] = valor_nuevo;
			total_utilizados++;		
		}
	}
   
    /***********************************************************************/    
    /***********************************************************************/
    // Compone un string con todos los caracteres que están
    // almacenados en la secuencia y lo devuelve.
    //Tiene el separador por defecto con posibildad de modificarlo
    
    string ToString(string separador=",")
    {
        string cadena;

        for (int i=0; i<total_utilizados; i++)
        {
        	cadena += to_string(vector_privado[i]);
        	
        	if(i<total_utilizados-1)
        	{
        		cadena+=separador;
			}
		}
            

        return (cadena);
    }
	
	/***********************************************************************/
	//Elimina el número dado todas las veces que aparece en la secuencia
	
	void EliminaOcurrencias (int a_borrar)
	{
		int posicion=0;
		while(posicion<total_utilizados)
		{
			if(vector_privado[posicion]==a_borrar)
			{
				Elimina(posicion);
			}
			else
			{
				posicion++;
			}
		}
	}
	
	
	/***********************************************************************/
	//Método de ordenación por Selección o Selection Sort
	
	void OrdenaSeleccion()
	{
		for (int izda = 0 ; izda < total_utilizados ; izda++) 
		{
			// Calcular el mínimo entre "izda" y "total_utilizados"-1
			int minimo = vector_privado[izda]; // Valor del mínimo
			
			int pos_min = izda; // Posición del mínimo
			
			for (int i = izda + 1; i < total_utilizados ; i++)
			{
				if (vector_privado[i] < minimo) // Nuevo mínimo
				{
					minimo = vector_privado[i];
					pos_min = i;
				}
				
			}
			
			// Intercambiar los valores guardados en "izda" y "pos_min"
			int intercambia = vector_privado[izda];
			vector_privado[izda] = vector_privado[pos_min];
			vector_privado[pos_min] = intercambia;	
			
		}

	}
	
	/***********************************************************************/
	//Método de ordenación por Inserción o Insertion Sort
	
	void OrdenaInsercion()
	{
		for (int izda=1; izda<total_utilizados; izda++)
		{
			// "a_insertar" es el valor que se va a insertar en
			// subvector izquierdo. Este subvector está ordenado y
			// comprende las posiciones entre 0 e "izda"-1
			
			int a_insertar=vector_privado[izda];
			
			// Se busca la posición en la zona ordenada y al mismo tiempo
			// se van desplazando hacia la derecha los valores mayores
			
			int i=izda;
			
			while ((i>0) && (a_insertar<vector_privado[i-1])) 
			{
				
				// Desplazar a la derecha los valores mayores que "a_insertar"
				vector_privado[i] = vector_privado[i-1]; 
				i--;
			}
			
			vector_privado[i]=a_insertar; // Copiar -insertar- en el "hueco"
		}

	}
	
	/***********************************************************************/
	//Método de ordenación por Intercambio Directo o Burbuja (Bubble Sort)
	//Primera Aproximación: 
	
	void OrdenaIntercambio()
	{
		for (int izquierda = 0; izquierda<total_utilizados; izquierda++) 
		{
			for (int i = total_utilizados-1 ; i > izquierda ; i--)
			{
				if (vector_privado[i] < vector_privado[i-1]) // Intercambiar
				{	
					int intercambia = vector_privado[i];
					vector_privado[i] = vector_privado[i-1];
					vector_privado[i-1] = intercambia;
				}
			}		
		}	
	}
	
	/***********************************************************************/
	//Método de ordenación por Intercambio Directo o Burbuja (Bubble Sort)
	//Segunda Aproximación: 
	
	void OrdenaIntercambioMejorado()
	{
		bool cambio=true; // Para obligar a entrar al ciclo
		
		for (int izda=0; izda<total_utilizados && cambio;izda++)
		{
			// En cada pasada iniciamos "cambio" a false.
			// Se pondrá a true si y solo si hay algún intercambio
			
			cambio=false;
			
			for(int i=total_utilizados-1; i>izda ;i--)
			{
				if(vector_privado[i]<vector_privado[i-1])	// Intercambiar
				{ 
					int intercambia=vector_privado[i];
					vector_privado[i]=vector_privado[i-1];
					vector_privado[i-1]=intercambia;
					
					// Se ha hecho un intercambio.
					// Se obliga a una nueva iteración del ciclo externo.
					
					cambio=true;
				}	
			}
		}	
	}
	
	/***********************************************************************/
	//Método de comparación entre 2 enteros que devuelve el mayor de los 2
	
	int NuevoOrden_Mayor (int a, int b)
	{
		const char NUM_INICIO='9';
		
		string num1=to_string(a);
		string num2=to_string(b);
		
		int mayor;
		
		bool acabar=false;
		char num_a_contar=NUM_INICIO;
		
		int contador1=0;
		int contador2=0;
		
		while(!acabar)
		{
			//Reseteo los contadores para cada numero a contar
			contador1=0;
			contador2=0;
			
			for(int i=0; i<num1.length(); i++) //Cuento las apariciones en num1
			{
				if(num1.at(i)==num_a_contar)
				{
					contador1++;
				}
			}
			
			for(int i=0; i<num2.length(); i++) //Cuento las apariciones en num2
			{
				if(num2.at(i)==num_a_contar)
				{
					contador2++;
				}
			}
			
			if(contador1>contador2)//El mayor será el que tenga más apariciones
			{
				mayor=stoi(num1);
				acabar=true;
			}
			else
			{
				if(contador1<contador2)
				{
					mayor=stoi(num2);
					acabar=true;
				}
				else
				{
					if(num_a_contar>0)
					{
						num_a_contar--;
					}
					else
					{
						mayor=stoi(num1);
						acabar=true;
					}
				}
			}
		}
		return mayor;
	}
	
	/***********************************************************************/
	//Método de ordenación por Selección o Selection Sort
	
	void OrdenaSeleccion_NuevoOrden()
	{
		for (int izda = 0 ; izda < total_utilizados ; izda++) 
		{
			// Calcular el mínimo entre "izda" y "total_utilizados"-1
			int minimo = vector_privado[izda]; // Valor del mínimo
			
			int pos_min = izda; // Posición del mínimo
			
			for (int i = izda + 1; i < total_utilizados ; i++)
			{
				if (NuevoOrden_Mayor(vector_privado[i], minimo)==minimo) 
				{											// Nuevo mínimo
					minimo = vector_privado[i];
					pos_min = i;
				}
				
			}
			
			// Intercambiar los valores guardados en "izda" y "pos_min"
			int intercambia = vector_privado[izda];
			vector_privado[izda] = vector_privado[pos_min];
			vector_privado[pos_min] = intercambia;	
			
		}

	}
	
	/***********************************************************************/
	//Método de ordenación por Inserción o Insertion Sort
	
	void OrdenaInsercion_NuevoOrden()
	{
		for (int izda=1; izda<total_utilizados; izda++)
		{
			// "a_insertar" es el valor que se va a insertar en
			// subvector izquierdo. Este subvector está ordenado y
			// comprende las posiciones entre 0 e "izda"-1
			
			int a_insertar=vector_privado[izda];
			
			// Se busca la posición en la zona ordenada y al mismo tiempo
			// se van desplazando hacia la derecha los valores mayores
			
			int i=izda;
			
			while ((i>0)&&(NuevoOrden_Mayor(vector_privado[i-1],a_insertar)
														==vector_privado[i-1]))
			{
				
				// Desplazar a la derecha los valores mayores que "a_insertar"
				vector_privado[i] = vector_privado[i-1]; 
				i--;
			}
			
			vector_privado[i]=a_insertar; // Copiar -insertar- en el "hueco"
		}

	}
	
	/***********************************************************************/
	//Método de ordenación por Intercambio Directo o Burbuja (Bubble Sort)
	//Primera Aproximación: 
	
	void OrdenaIntercambio_NuevoOrden()
	{
		for (int izquierda = 0; izquierda<total_utilizados; izquierda++) 
		{
			for (int i = total_utilizados-1 ; i > izquierda ; i--)
			{
				if (NuevoOrden_Mayor(vector_privado[i], vector_privado[i-1])
					==vector_privado[i-1]) // Intercambiar
				{	
					int intercambia = vector_privado[i];
					vector_privado[i] = vector_privado[i-1];
					vector_privado[i-1] = intercambia;
				}
			}		
		}	
	}
	
	/***********************************************************************/
	//Método de ordenación por Intercambio Directo o Burbuja (Bubble Sort)
	//Segunda Aproximación: 
	
	void OrdenaIntercambioMejorado_NuevoOrden()
	{
		bool cambio=true; // Para obligar a entrar al ciclo
		
		for (int izda=0; izda<total_utilizados && cambio;izda++)
		{
			// En cada pasada iniciamos "cambio" a false.
			// Se pondrá a true si y solo si hay algún intercambio
			
			cambio=false;
			
			for(int i=total_utilizados-1; i>izda ;i--)
			{
				if(NuevoOrden_Mayor(vector_privado[i], vector_privado[i-1])
					==vector_privado[i-1])	// Intercambiar
				{ 
					int intercambia=vector_privado[i];
					vector_privado[i]=vector_privado[i-1];
					vector_privado[i-1]=intercambia;
					
					// Se ha hecho un intercambio.
					// Se obliga a una nueva iteración del ciclo externo.
					
					cambio=true;
				}	
			}
		}	
	}
	
	/***********************************************************************/
	//Método que devuelve la distancia entre dos secuencias de enteros
	//			  		  ___________________________________
	//		dist(x, y) = V (x1 - y1)^2 + · · · + (xp - yp)^2
	
	double DistanciaEuclidea (SecuenciaEnteros otra_secuencia)
	{
		
		double distancia;
		double diferencia_coordenadas;
		double suma_cuadrados=0;
		
		for(int i=0; i<TotalUtilizados(); i++)
		{
			//Calculo la distancia a cada vector
			diferencia_coordenadas = Elemento(i)-otra_secuencia.Elemento(i);
				  
			suma_cuadrados+=pow(diferencia_coordenadas,2);
				
		}
		
		distancia=sqrt(suma_cuadrados);
				
		return distancia;
	}
	
};

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
	//Se le puede agregar un margen a la izquierda
	
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
		double distancia_minima=9999999999; //Le doy un valor alto para que 
								 			//cualquiera que calcule sea menor
		int indice_fila;
		
		
		for(int i=0; i<filas_a_comparar.TotalUtilizados(); i++)
		{
		
			double distancia_aux=
			
			referencia.DistanciaEuclidea(Fila(filas_a_comparar.Elemento(i)));
						
			
			if(distancia_aux<=distancia_minima)
			{
				distancia_minima=distancia_aux;
				indice_fila=filas_a_comparar.Elemento(i);
			}
			
		}
		
		return indice_fila;
		
	}
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	TablaRectangularEnteros tabla1;
	
	SecuenciaEnteros referencia("2811");
	SecuenciaEnteros filas_a_comparar("0245");
	
	//Relleno la tabla con vectores
	tabla1.Aniade(SecuenciaEnteros("3108"));
	tabla1.Aniade(SecuenciaEnteros("4515"));
	tabla1.Aniade(SecuenciaEnteros("5717"));
	tabla1.Aniade(SecuenciaEnteros("7961"));
	tabla1.Aniade(SecuenciaEnteros("4955"));
	tabla1.Aniade(SecuenciaEnteros("2822"));
	tabla1.Aniade(SecuenciaEnteros("7325"));
	
	int fila_similar=tabla1.FilaSimilarMasCercana(referencia,filas_a_comparar);
	
	
	//Salidas 
	
	cout<<"Tabla generada:"<<endl;
	
	cout<<endl;
	cout<<tabla1.ToString(" ",8);
	cout<<endl;
	
	cout<<"Vector Referencia: "<<referencia.ToString(",")<<endl;
	cout<<"Filas a comparar: "<<filas_a_comparar.ToString(",")<<endl;
	
	cout<<endl;
	cout<<"El metodo FilaSimilarMasCercana devuelve: "<<fila_similar<<endl;
	
	cout<<"La fila "<<fila_similar<<" es: "
		<<tabla1.Fila(fila_similar).ToString()<<endl;
		

	return 0;
}

