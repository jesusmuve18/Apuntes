/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:

	1. Crear una secuencia de enteros de 50 enteros aleatorios entre 1 y 100
	2. Ordenar con Quick Sort (última versión)
		- Método básico a elección del programador
		- UMBRAL= 5
	

	Entradas:

	Salidas:

*/
/****************************************************************************/
#include<iostream>
#include <random>  // para la generación de números pseudoaleatorios
#include <chrono>  // para la semilla

using namespace std;
/****************************************************************************/
//Declaración de Clases/Objetos

class GeneradorAleatorioEnteros
{  
private:
	
	mt19937 generador_mersenne;    // Mersenne twister
	uniform_int_distribution<int>  distribucion_uniforme;
	
	/************************************************************************/
	
	long long Nanosec(){
		return(chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	
	/************************************************************************/ 
	
public:
	
	/************************************************************************/
		
	GeneradorAleatorioEnteros() : GeneradorAleatorioEnteros(0, 1) 
	{ }
	
	/************************************************************************/  
	GeneradorAleatorioEnteros(int min, int max) {
	
		const int A_DESCARTAR = 70000;
		// ACM TOMS Volume 32 Issue 1, March 2006
		
		auto semilla = Nanosec();
		generador_mersenne.seed(semilla);
		generador_mersenne.discard(A_DESCARTAR);
		distribucion_uniforme = uniform_int_distribution<int> (min, max);
	}
	
	/************************************************************************/
	
	int Siguiente(){
	  return (distribucion_uniforme(generador_mersenne));
	}
	
	/************************************************************************/

};

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
    // Compone un string con todos los caracteres que están
    // almacenados en la secuencia y lo devuelve.
    //Tiene el separador por defecto
    
    string ToString()
    {
        string cadena;

        for (int i=0; i<total_utilizados; i++)
        {
        	cadena += to_string(vector_privado[i]);
        	
        	if(i<total_utilizados-1)
        	{
        		cadena+=" , ";
			}
		}
            

        return (cadena);
    }
    
    /***********************************************************************/
    // Compone un string con todos los caracteres que están
    // almacenados en la secuencia y lo devuelve.
    //El separador es introducido como parámetro
    
    string ToString(string separador)
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
	
	/**********************************************************************/
	//Método que particiona un vector divido por el elemento pivote
	
	int partir (int primero, int ultimo)
	{
		int intercambia;
		int izda, dcha;
		int valor_pivote = vector_privado[primero];
		
		izda = primero + 1; // izda avanza hacia delante
		dcha = ultimo; // dcha retrocede hacia atrás
		
		while (izda <= dcha)
		{
			while (izda <= dcha && vector_privado[izda] <= valor_pivote)
			{
				izda++;
			}
			
			while (izda <= dcha && vector_privado[dcha] >= valor_pivote)
			{
				dcha--;	
			}
			
			if (izda < dcha) 
			{
				intercambia = vector_privado[izda];
				vector_privado[izda] = vector_privado[dcha];
				vector_privado[dcha] = intercambia;
				dcha--;
				izda++;
			}
		}
		intercambia = vector_privado[primero];
		vector_privado[primero] = vector_privado[dcha];
		vector_privado[dcha] = intercambia;
		return dcha;
	}

	/***********************************************************************/
	//Método que intercambia dos valores en la secuencia dadas sus posiciones	
	IntercambiaComponentes_en_Posiciones(int pos1, int pos2)
	{
		int auxiliar=Elemento(pos1);
		Modifica(pos1,pos2);
		Modifica(pos2,auxiliar);
	}
	/***********************************************************************/
	//Método que devuelve la posicion del valor minimo en un intervalo
	
	int PosicionMinimoEntre (int izda, int dcha) 
	{
		char minimo = vector_privado[izda];
		int posicion_minimo = izda;
		
		for (int i = izda+1 ; i <= dcha ; i++)
		{
			if (vector_privado[i] < minimo)
			{
				minimo = vector_privado[i];
				posicion_minimo = i;
			}
		}
		
		return posicion_minimo;
	}
	/**********************************************************************/
	//Método que ordena por seleccion una parte de la secuencia
	//PRE: primero >= 0 && ultimo < total_utilizados
	
	void Ordena_por_Seleccion_entre(int primero, int ultimo)
	{
		int pos_min;
		
		for (int izda=primero ; izda<ultimo ; izda++)
		{
			pos_min = PosicionMinimoEntre (izda, ultimo);
			IntercambiaComponentes_en_Posiciones (izda, pos_min);
		}
		
	}
	
	/**********************************************************************/
	//Método que ordena usando la técnica quicksort
	//Necesita el método Ordena_por_Seleccion_entre()
	
	void QuickSort(int inicio, int final)
	{
		const int UMBRAL = 5;
		int pos_pivote;
		
		if (final - inicio < UMBRAL)
		{
			Ordena_por_Seleccion_entre(inicio, final);
		}
		
		else
		{
			if (inicio < final) 
			{
				pos_pivote = partir (inicio, final);
				QuickSort (inicio, pos_pivote - 1); // Ordena primera mitad
				QuickSort (pos_pivote + 1, final); // Ordena segunda mitad
			}
		}
	}
	
};

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos
	
	const int MIN=1;
	const int MAX=100;
	
	int n_datos_cadena=50;
	
	//Inicializo el generador de aleatorios
	
	GeneradorAleatorioEnteros generador(MIN,MAX);
	
	//Inicializo la cadena de enteros
	
	SecuenciaEnteros secuencia;
	
	//Relleno la cadena
	
	for(int i=0; i<n_datos_cadena ; i++)
	{
		int valor=generador.Siguiente();
		secuencia.Aniade(valor);
	}
	
	cout<<"Cadena generada: "<< secuencia.ToString() <<endl;

	//Ordeno la cadena utilizando el método quicksort

	secuencia.QuickSort(0,(secuencia.TotalUtilizados()-1));
	
	cout<<endl;
	cout<<"Secuencia Ordenada: "<<secuencia.ToString()<<endl;



	return 0;
}

