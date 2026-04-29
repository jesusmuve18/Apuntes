/****************************************************************/
/*  FUNDAMENTOS DE PROGRAMACIÓN					EJERCICIO-60


	ALISON CARVAJAL PAULINO

	1º A2 DOBLE GRADO EN INGENIERÍA INFORMÁTICA Y MATEMÁTICAS

	Reescribir el ejercicio 51 (fila más cercana de un conjunto de filas 
	preseleccionadas) usando la clase TablaRectangularEnteros que emplea 
	una secuencia de secuencias de enteros como dato privado.
	
	Emplear la misma función main que se usó para la solución propuesta usando 
	la clase que empela una matriz clásica como dato privado. 
	Si no ha tenido que cambiar nada, la clase está bien construida.

*/
/****************************************************************/


#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


///////////////////////////////////////////////////////////////////////

class SecuenciaEnteros  
{

private:

	static const int TAMANIO = 40; // Número de casillas disponibles
	int vector_privado[TAMANIO];
	
	//PRE: 0 <= total_utilizados <= TAMANIO
	int total_utilizados;	// Número de casillas ocupadas

public:
	
	/***********************************************************************/
	// Constructor sin argumentos

	SecuenciaEnteros (void) 
	{
		total_utilizados = 0;
	}

	/***********************************************************************/
	// Métodos de lectura (Get) de los datos individuales 

	// Devuelve el número de casillas ocupadas
	int TotalUtilizados (void)
	{
		return (total_utilizados);
	}

	// Devuelve el número de casillas disponibles
	int Capacidad (void)
	{
		return (TAMANIO);
	}

	/***********************************************************************/
	// Añade un elemento al vector.
	// Recibe: nuevo, el elemento que se va a añadir.
	//
	// PRE: total_utilizados < TAMANIO
	// 		La adición se realiza si hay espacio para el nuevo elemento. 
	// 		El nuevo elemento se coloca al final del vector. 
	// 		Si no hay espacio, no se hace nada.	

	void Aniade (int nuevo)
	{
		if (total_utilizados < TAMANIO) {
			vector_privado[total_utilizados] = nuevo;
			total_utilizados++;
		}
	}

	/***********************************************************************/
	// Devuelve el elemento de la casilla "indice" 
	//
	// PRE: 0 <= indice < total_utilizados 

	int Elemento (int indice)
	{
		return vector_privado[indice];
	}

	/***********************************************************************/
	// Devuelve true si la secuencia está vacía

	bool EstaVacia (void)
	{
		return (total_utilizados == 0);
	}

	/***********************************************************************/
	// "Vacía" una secuencia

	void EliminaTodos (void)
	{
		total_utilizados = 0;
	}
	
	/***********************************************************************/
	// Búsqueda de un valor en la secuencia, entre dos posiciones dadas. 
	// Realiza una búsqueda secuencial
	//
	// Recibe: 	buscado, el valor a buscar en la secuencia.
	//			izda y dcha, posiciones entre las que se realiza la búsqueda.
	// Devuelve: la posición del elemento "buscado", si está en la secuencia,
	//			 o -1, si no lo encuentra.
	// PRE: 0 <= izda <= dcha < total_utilizados

	int Posicion (int buscado, int izda, int dcha)
	{
		bool encontrado = false; 
		int pos = izda; 

		while ((pos <= dcha) && (!encontrado)) {

			if (vector_privado[pos] != buscado) // Seguir buscando
				pos++;
			else // Terminar la búsqueda: éxito
				encontrado =  true;
		} 

		if (encontrado)
			return (pos);
		else 
			return (-1);

	}
	
	/***********************************************************************/
	// Devuelve la posición del mínimo valor de la secuencia
	//
	// PRE: 0 <= izda <= dcha < total_utilizados
	
	int PosicionMinimo (int izda, int dcha)
	{
		int posicion_minimo;
		int minimo;
	
		minimo = vector_privado[izda];
		posicion_minimo = izda;
		
		for (int i = izda; i <= dcha ; i++) { 
			
			if (vector_privado[i] < minimo){
				minimo = vector_privado[i];
				posicion_minimo = i;
			}
		}
		
		return (posicion_minimo);		
	}
	
	/***********************************************************************/
	// Devuelve la posición del máximo valor de la secuencia
	//
	// PRE: 0 <= izda <= dcha < total_utilizados
	
	int PosicionMaximo (int izda, int dcha)
	{
		int posicion_maximo;
		int maximo;
	
		maximo = vector_privado[izda];
		posicion_maximo = izda;
		
		for (int i = izda; i <= dcha ; i++) { 
			
			if (vector_privado[i] > maximo){
				maximo = vector_privado[i];
				posicion_maximo = i;
			}
		}
		
		return (posicion_maximo);		
	}
	
	/***********************************************************************/
	// Devuelve el mínimo de la secuencia. 
	//
	// PRE: 0 <= izda <= dcha < total_utilizados

	int Minimo (int izda, int dcha)
	{
		return (vector_privado[PosicionMinimo(izda, dcha)]);
	}


	/***********************************************************************/
	// Devuelve el máximo de la secuencia. 
	//
	// PRE: 0 <= izda <= dcha < total_utilizados

	int Maximo (int izda, int dcha)
	{
		return (vector_privado[PosicionMaximo(izda, dcha)]);
	}
	
	/***********************************************************************/
	// Devuelve una secuencia incluida en la secuencia implícita, que empieza 
	// en la posición "pos_init" y tiene "num_datos" valores
	//
	// Recibe: pos_init, posición inicial desde donde empieza la copia a la 
	//					 subsecuencia. 
	//		   num_datos, número de valores que se copian a la subsecuencia. 
	// Devuelve: una subsecuencia de la secuencia implícita. 
	// 
	// Casos: 1) Si 0 <= pos_init < total_utilizados Y 
	//			 pos_init+num_datos<=total_utilizados
	//			 se devuelve una subsecuencia de tamaño "num_datos"
	//		  2) Si 0 <= pos_init < total_utilizados Y 
	//			 pos_init+num_datos>total_utilizados, se devuelve una 
	//			 subsecuencia de tamaño total_utilizados-pos_init
	//		  3) Si pos_init < 0 Ó pos_init >= total_utilizados, 
	//			 se devuelve una subsecuencia vacía.
	//		  4) Si num_datos = 0, se devuelve una subsecuencia vacía.

	SecuenciaEnteros Subsecuencia (int pos_init, int num_datos)
	{
		SecuenciaEnteros a_devolver; // inicialmente vacía
		
		if ((pos_init>=0) && (pos_init<total_utilizados) && (num_datos>0)) {
			
			int pos_end;
			
			if (pos_init+num_datos>total_utilizados) 
				pos_end = total_utilizados;
			else 
				pos_end = pos_init+num_datos;
			
			for (int pos=pos_init; pos<pos_end; pos++)
				a_devolver.Aniade(vector_privado[pos]);
		}
		
		return (a_devolver);
	}
			
	/***********************************************************************/
	// Compara dos secuencias
	
	bool EsIgual (SecuenciaEnteros otra_secuencia) 
	{
		bool son_iguales = true; 
		
		if (total_utilizados != otra_secuencia.total_utilizados)
		
			son_iguales = false;
		
		else {
				
			bool sigo = true;
			int pos = 0;
			
			while (sigo && (pos < total_utilizados)) {
	
				if (vector_privado[pos]!=otra_secuencia.vector_privado[pos]) {
					sigo = false;
					son_iguales = false;
				}
				else
					pos++; 
			} // while (sigo && pos < total_utilizados) 
		}
			
		return (son_iguales);
	}
	

	/***********************************************************************/
	// Calcula la distancia Euclídea entre dos secuencias
	//
	// PRE: total_utiizados = otra_secuencia.TotalUtilizados()

	double DistanciaEuclidea (SecuenciaEnteros otra_secuencia)
	{
		double suma = 0.0;

		for (int pos=0; pos<total_utilizados; pos++) {

			double dif=vector_privado[pos]-otra_secuencia.vector_privado[pos];
			suma += dif*dif;
		}

		return (sqrt(suma));
	}

    /***********************************************************************/
    // Compone un string con todos los valores que están
    // almacenados en la secuencia y lo devuelve.

    string ToString()
    {
        string cadena = "{";

		if (total_utilizados > 0) {

	        for (int i=0; i<total_utilizados-1; i++)
	        	cadena = cadena + to_string(vector_privado[i])+", ";

			// Añadie el último seguido de '}'
	        cadena = cadena + to_string(vector_privado[total_utilizados-1]);
		}

        cadena = cadena + "}";

        return (cadena);
    }
    
   	/***********************************************************************/
   	
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// El tipo de dato PosicionEnMatriz se emplea para guardar posiciones 	
// dentro de una matriz: se registra la pareja (fila, columna)
	
struct PosicionEnMatriz {
	int fila;
	int columna;
};

/////////////////////////////////////////////////////////////////////////////
// La clase "SecuenciaPosicionEnMatriz" es útil para representar una secuencia
// de  posiciones (coordenadas) en una tabla bidimensional.	
	
class SecuenciaPosicionEnMatriz
{
private:

	static const int TAMANIO = 50; // Número de casillas disponibles
	PosicionEnMatriz vector_privado[TAMANIO];
	
	//PRE: 0 <= total_utilizados <= TAMANIO
	int total_utilizados;	// Número de casillas ocupadas

public: 
	
	/***********************************************************************/
	// Constructor sin argumentos

	SecuenciaPosicionEnMatriz (void) 
	{
		total_utilizados = 0;
	}

	/***********************************************************************/
	// Métodos de lectura (Get) de los datos individuales 

	// Devuelve el número de casillas ocupadas
	int TotalUtilizados (void)
	{
		return (total_utilizados);
	}

	// Devuelve el número de casillas disponibles
	int Capacidad (void)
	{
		return (TAMANIO);
	}

	/***********************************************************************/
	// Añade un elemento al vector.
	// Recibe: nuevo, el elemento que se va a añadir.
	//
	// PRE: total_utilizados < TAMANIO
	// 		La adición se realiza si hay espacio para el nuevo elemento. 
	// 		El nuevo elemento se coloca al final del vector. 
	// 		Si no hay espacio, no se hace nada.	

	void Aniade (PosicionEnMatriz nuevo)
	{
		if (total_utilizados < TAMANIO) {
			vector_privado[total_utilizados] = nuevo;
			total_utilizados++;
		}
	}

	/***********************************************************************/
	// Devuelve el elemento de la casilla "indice" 
	//
	// PRE: 0 <= indice < total_utilizados 

	PosicionEnMatriz Elemento (int indice)
	{
		return vector_privado[indice];
	}
			
	/***********************************************************************/
	// Compone y devuelve un string con el contenido de la secuencia
	
	string ToString ()
	{
	    string cadena = "{";

		if (total_utilizados > 0) {

	        for (int indice=0; indice<total_utilizados-1; indice++) {
	        	PosicionEnMatriz pos = vector_privado[indice];
	    	    cadena = cadena + "(" + to_string(pos.fila) + ", " + 
				                        to_string(pos.columna) + "), ";
			}
	
			// Añadie el último seguido de '}'
			PosicionEnMatriz pos = vector_privado[total_utilizados-1];
	        cadena = cadena + "(" + to_string(pos.fila) + ", " + 
			                        to_string(pos.columna) + ")";
		}
        cadena = cadena + "}";		
		
		return cadena;
	}
	
	/***********************************************************************/
};

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Tabla rectangular de enteros

class TablaRectangularEnteros
{

private:

    static const int NUM_FILS = 50; // Filas disponibles
    static const int NUM_COLS = 4; // Columnas disponibles
    
    SecuenciaEnteros  matriz_privada[NUM_FILS];
	
	
    // PRE: 0 <= filas_utilizadas <= NUM_FILS
    // PRE: 0 <= col_utilizadas < NUM_COLS
    
    int filas_utilizadas;
    int cols_utilizadas;

public:

	/***********************************************************************/
	// Constructor sin argumentos--> Crea matriz nula

	TablaRectangularEnteros (void)
		: filas_utilizadas(0), 
		  cols_utilizadas(0)
	{
	}

	/***********************************************************************/
	// Constructor--> Recibe "numero_de_columnas" que indica el número de 
	// columnas que deben tener TODAS las filas. 
	// PRE: numero_de_columnas <= MAX_COL

	TablaRectangularEnteros (int numero_de_columnas)
		: filas_utilizadas(0), 
		  cols_utilizadas(numero_de_columnas)
	{
	}

	/***********************************************************************/
	// Constructor--> Recibe una secuencia de enteros.  El número de 
	// elementos de la secuencia es el valor que se usa como "col_utilizadas"
	// PRE: primera_fila.TotalUtilizados() <= MAX_COL

	TablaRectangularEnteros (SecuenciaEnteros primera_fila)
		: filas_utilizadas(0), 
		  cols_utilizadas (primera_fila.TotalUtilizados())
	{
		Aniade(primera_fila); // Actualiza "filas_utilizadas"
	}


	/***********************************************************************/
	// Método de lectura: número máximo de filas

	int CapacidadFilas (void)
	{
		return (NUM_FILS);
	}

	/***********************************************************************/
	// Método de lectura: número máximo de columnas

	int CapacidadColumnas (void)
	{
		return (NUM_COLS);
	}

	/***********************************************************************/
	// Método de lectura: número real de filas usadas

	int FilasUtilizadas (void)
	{
		return (filas_utilizadas);
	}

	/***********************************************************************/
	// Método de lectura: número real de columnas usadas

	int ColumnasUtilizadas (void)
	{
		return (cols_utilizadas);
	}

	/***********************************************************************/
	// Método de lectura: devuelve el dato que ocupa la casilla 
	// de coordenadas (fila, columna)
	// PRE: 0 <= fila < filas_utilizadas
	// PRE: 0 <= columna < cols_utilizadas	
	
	int Elemento (int fila, int columna)
	{
		return (matriz_privada[fila].Elemento(columna));
	}

	/***********************************************************************/
	// Devuelve true si la tabla está vacía

	bool EstaVacia (void)
	{
		return (filas_utilizadas == 0);
	}
	
	/*****************************************************************/
	// Devuelve una fila completa (un objeto "SecuenciaEnteros")
	// PRE: 0 <= indice_fila < filas_utilizadas
	
	SecuenciaEnteros Fila (int indice_fila)
	{

		return (matriz_privada[indice_fila]);
	}

	/***********************************************************************/
	// Devuelve una columna completa (un objeto "SecuenciaEnteros")
	// PRE: 0 <= indice_columna < cols_utilizadas
	
	SecuenciaEnteros Columna (int indice_columna)
	{

		return (matriz_privada[indice_columna]);
	}

	/***********************************************************************/
	// Añade una fila completa (un objeto "SecuenciaEnteros")
	// PRE:  fila_nueva.TotalUtilizados() = cols_utilizadas
	// PRE:  filas_utilizadas < MAX_FIL
	
	void Aniade (SecuenciaEnteros fila_nueva)
	{
		int numero_columnas_fila_nueva = fila_nueva.TotalUtilizados();
					
		if ((filas_utilizadas < NUM_FILS) &&
			(numero_columnas_fila_nueva == NUM_COLS)) {

		
				matriz_privada[filas_utilizadas]=fila_nueva;

			filas_utilizadas++;
		}
	}

	/***********************************************************************/
	// "Vacía" una tabla

	void EliminaTodos (void)
	{
		filas_utilizadas = 0;
	}
	
	

	/***********************************************************************/
	// Devuelve la traspuesta de la matriz (si es posible calcularse)
	// PRE: filas_utilizadas <= MAX_COL
	// PRE: cols_utilizadas  <= MAX_FIL
	// Si no se puede calcular la traspuesta se devuelve una matriz nula. 

	TablaRectangularEnteros Traspuesta (void)
	{		
	    TablaRectangularEnteros traspuesta; // Inicialmente vacía
		
		if ((filas_utilizadas <= NUM_COLS) && (cols_utilizadas  <= NUM_FILS)) {
		
			// La traspuesta tiene tantas filas como columnas tiene la
			// matriz original
	
			TablaRectangularEnteros solucion (filas_utilizadas);
	
	
			SecuenciaEnteros columna; // Las filas de "traspuesta" serán las
										 // columnas de la tabla implícita
	
			for (int col = 0; col < cols_utilizadas; col++){
	
				columna = Columna(col);
				solucion.Aniade(columna);
			}
			
			traspuesta = solucion;
		} 

		return (traspuesta);
	}			
	

	
	/***********************************************************************/
	// Inserta una fila completa en una posición dada. 
	// La fila se porporciona en un objeto "SecuenciaEnteros".
	// 
	// Recibe: num_fila, la posición que ocupará "fila_nueva" cuando se 
	//		 		inserte en la tabla.
	//		   fila_nueva, la secuencia que se va a insertar. Se trata 
	//				de un objeto de la clase "SecuenciaEnteros". 
	//
	// PRE:  fila_nueva.TotalUtilizados() = cols_utilizadas
	// PRE:  filas_utilizadas < MAX_FIL
	// PRE:  0 <= num_fila <= TotalUtilizados()
	//		 Si num_fila = 0, "nueva_fila" será la nueva primera fila. 
	//		 Si num_fila = TotalUtilizados(), "nueva_fila" será la nueva 
	//		 última fila (el resultado será equivalente al de "Aniade()" 
	
	void Inserta (int num_fila, SecuenciaEnteros fila_nueva)
	{
		int numero_columnas_fila_nueva = fila_nueva.TotalUtilizados();
					
		// Comprobar precondiciones
		
		if ((filas_utilizadas < NUM_FILS) &&
			(numero_columnas_fila_nueva == cols_utilizadas) && 
			(0<=num_fila) && (num_fila <=filas_utilizadas)) {

			// "Desplazar" las filas hacia posiciones altas. 
			// La última fila se copia en una NUEVA FILA que ocupa la 
			// posición "filas_utilizadas", la penúltima se copia en 
			// "filas_utilizadas"-1, ... y se queda un "hueco" en 
			// la fila "num_fila ".
			// NOTA: no se modifica (todavía) "filas_utilizadas" 
			
			for (int fil=filas_utilizadas; fil>num_fila; fil--) {
				
				
					matriz_privada[fil]=matriz_privada[fil-1];
			}
			
			// Copiar en el "hueco" (fila "num_fila") el contenido de 
			// la secuencia "fila_nueva"
			
			
				matriz_privada[num_fila]=fila_nueva;	
			
			filas_utilizadas++; // Actualización del tamaño de la tabla.
		}
	}

	/***********************************************************************/
	// Elimina una fila completa, dada una posición. 
	// 
	// Recibe: num_fila, la posición de la fila a eliminar.
	// PRE:  0 <= num_fila < TotalUtilizados()
	
	void Elimina (int num_fila)
	{		
		// Comprobar precondiciones
		
		if ((0<=num_fila) && (num_fila <=filas_utilizadas)) {

			// "Desplazar" las filas hacia posiciones bajas. 
			// En la posición "num_fila" se copia la que está en la posición
			// siguiente ("num_fila"+1), en su lugar se copia que está en 
			// "num_fila"+2, ... y en la posición "total_utilizados"-2 se 
			// copia la de "total_utilizados"-1. 
			
			for (int fil=num_fila; fil<filas_utilizadas-1; fil++) {
			
				matriz_privada[fil]=matriz_privada[fil+1];
			}
			
			filas_utilizadas--; // Actualización del tamaño de la tabla.
		}
	}	
	
	/***********************************************************************/
	// Devuelve TRUE si la matriz contiene el valor "buscado"
	/*
	bool Contiene (int buscado)
	{
		bool encontrado = false;

		// Recorrido por columnas 

		int col = 0;

	//	while (col < cols_utilizadas && !encontrado) {

			int fil=0;

			while (fil < filas_utilizadas && !encontrado) {

				if (matriz_privada[fil] == buscado)
					encontrado = true;
				
				fil++;

			} // while fil

			//col++; 

		//} // while col

		return (encontrado);
	}
		
	
	/***********************************************************************/
	// Compone y devuelve un string con el contenido de la matriz
	
	string ToString ()
	{
		string cad; 
		
		for (int fila=0; fila<filas_utilizadas; fila++) {
				cad += Fila(fila).ToString() + "\n";
			
		}
		
		return (cad);
	}
	
	/***********************************************************************/
	//Este método se encarga de comparar las filas elegidas de la matriz 
	//con un vector. Aquella con menor distancia euclídea será la más similar.
	
	//Parámetros: el vector de referencia, las filas a comparar, y la matriz
	//(de la cual obtendremos dichas filas)
	/***********************************************************************/
	
	int FilaMasParecida (SecuenciaEnteros referencia, SecuenciaEnteros 
							filas_a_comparar, TablaRectangularEnteros matriz )
	{
		int fila;
		int a;
		
		//Distancias euclídeas
		double nueva=0;
		double anterior=99999999999999.9;	//Para comparar
		
		SecuenciaEnteros otra_secuencia;
		
		
		for(int i = 0; i < filas_a_comparar.TotalUtilizados(); i++)
		{
			//Número de la fila a comparar
			
			a = filas_a_comparar.Elemento(i);
			
			for(int j=0; j < filas_utilizadas; j++)
			{
				
				//Se iguala la secuencia a la fila elegida
				otra_secuencia = matriz.Fila(a);
				
				//Se calcula la distancia euclídea
				nueva = referencia.DistanciaEuclidea(otra_secuencia);
				cout<<"nueva"<<nueva<<endl;
				//Si la nueva distancia es menor que la anterior
				if(nueva < anterior)
				{
					anterior = nueva;
					fila = a;	//Se guarda la posición de la fila 
					cout<<"fila"<<a<<endl;
				}	
			}	
		}
		
		//Se le suma 1 para que siga el orden correcto (ya que empieza en 0)
		return(fila+1);
	}
	
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/***************************************************************************/
/***************************************************************************/
	
int main (void)
{
	//Crear la tabla rectangular. 

/***************************************************************************/
/***************************************************************************/
	// 	Trabajaremos con una matriz de cuatro columnas: 
	// 		1) 	Crear la primera fila (secuencia de enteros) y crear la 
	//    		tabla a partir de esa secuencia. 
	// 		2) 	Las demás filas deberán tener los mismos elementos que 
	//			la primera. El procedimiento para cada nueva fila será: 
	//			rellenar una secuencia y añadirla a la tabla.
	
	SecuenciaEnteros una_fila; // Secuencia vacía
	
	una_fila.Aniade (3); una_fila.Aniade (1); 
	una_fila.Aniade (0); una_fila.Aniade (8);

	// Crear la tabla a partir de la primera fila
	// Las demás filas deberán tener la misma longitud. 
		
	TablaRectangularEnteros matriz (una_fila);


	// Vaciar la secuencia, y añadir una nueva fila a "matriz" 
	
	una_fila.EliminaTodos();	// Para poder utilizarse posteriormente
		
	una_fila.Aniade (4); una_fila.Aniade (5);	
	una_fila.Aniade (1); una_fila.Aniade (5);
	matriz.Aniade (una_fila);
		
	// Vaciar la secuencia, y añadir una nueva fila a "matriz" 
		
	una_fila.EliminaTodos();	// Para poder utilizarse posteriormente
	
	una_fila.Aniade (4); una_fila.Aniade (9);	
	una_fila.Aniade (5); una_fila.Aniade (5);
	matriz.Aniade (una_fila);
	
	
	//Fila siguiente
	una_fila.EliminaTodos();	// Para poder utilizarse posteriormente
	
	una_fila.Aniade (7); una_fila.Aniade (9);	
	una_fila.Aniade (6); una_fila.Aniade (1);
	matriz.Aniade (una_fila);
	
	///Fila siguiente/
	una_fila.EliminaTodos();	// Para poder utilizarse posteriormente
	
	una_fila.Aniade (2); una_fila.Aniade (8);	
	una_fila.Aniade (2); una_fila.Aniade (2);
	matriz.Aniade (una_fila);
	
	
	///Fila siguiente/
	una_fila.EliminaTodos();	// Para poder utilizarse posteriormente
	
	una_fila.Aniade (5); una_fila.Aniade (7);	
	una_fila.Aniade (1); una_fila.Aniade (7);
	matriz.Aniade (una_fila);
	
	
	//Fila siguiente
	una_fila.EliminaTodos();
	
	una_fila.Aniade (7); una_fila.Aniade (3);	
	una_fila.Aniade (2); una_fila.Aniade (5);
	matriz.Aniade (una_fila);
	
/***************************************************************************/
/***************************************************************************/	
	
	//Mostrar la tabla rectangular. 

	// Consultar el número de filas y columnas de "matriz"
	
	int num_filas_matriz = matriz.FilasUtilizadas();
	int num_cols_matriz  = matriz.ColumnasUtilizadas();
	
	// Mostrar los datos de la matriz
	
	cout <<endl;
	
	cout << "Matriz orginal: ";	
	cout << "(" << setw(2) << num_filas_matriz << " x " 
	     << setw(2) << num_cols_matriz << ") : " << endl;
	cout << endl;		
	cout << matriz.ToString() << endl;



	//Relleno el vector de referencia y las filas a comparar
	 
	SecuenciaEnteros referencia, filas_a_comparar;
	
	//Vector de referencia a comparar
	
	referencia.EliminaTodos();
	
	referencia.Aniade (2); 
	referencia.Aniade (8); 
	referencia.Aniade (1); 
	referencia.Aniade (1);
	
	
	
	//Filas de la matriz a comparar
	
	filas_a_comparar.Aniade (0); 
	filas_a_comparar.Aniade (2); 
	filas_a_comparar.Aniade (4); 
	filas_a_comparar.Aniade (5); 
	
	
	//Mostrar resultado

	cout <<"Fila con mayor similitud: ";
	cout<<matriz.FilaMasParecida(referencia, filas_a_comparar, matriz);
	
	return 0;
}
