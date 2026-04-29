class GestorNumerosSecuenciables
{

private:
	
	static const int MAX_SECUENCIAS=20;
	int valor; //PRE: 1 <= valor
	Secuencia secuencias[MAX_SECUENCIAS];
	int total_utilizados;

public:
	
	/**********************************************************************/
	//Constructor con 1 argumento
	
	GestorNumerosSecuenciables(int n): valor(n),total_utilizados(0){};
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	GestorNumerosSecuenciables(void) {};
	
	/**********************************************************************/
	//Método para establecer un valor 
	//PRE: n>=0
	
	void SetValor (int n)
	{
		valor=n;
		total_utilizados=0;
	}
	
	/**********************************************************************/
	//Método que devuelve el valor en ese instante
	
	int GetValor (void)
	{
		return (valor);
	}
	
	/***********************************************************************/
	/***********************************************************************/
	// Calcula y devuelve si un número es secuenciable o no. 
	
	bool EsSecuenciable (void) 
	{		
		bool es_secuenciable = false; 
		
		int inicio = 1; 			
		int ultimo_inicio = valor/2;   
		
		// Exploramos hasta encontrar una secuencia
		
		while (inicio <= ultimo_inicio && !es_secuenciable) {
			
			int suma  = inicio;  // suma inicial (primer valor de la sec.)
			int nuevo = inicio+1; 	// segundo valor de la sec. 
	
			// Mientras que la suma de la secuencia sea menor que "valor", 
			// seguir añadiendo términos consecutivos. 
			 
			while (suma < valor) {
				
				suma += nuevo; // Nueva suma 
				nuevo++; // El nuevo término a considerar es el siguiente 
			}
			
			// Si "suma" es igual a "valor" hemos encontrado una secuencia. 
			// Si no, la suma es necesariamente mayor.  
			
			if (suma == valor) es_secuenciable = true; 
			else inicio++;
		}
		
		return es_secuenciable;
	}

	/***********************************************************************/	
	/***********************************************************************/
	// Calcula y devuelve el número de secuencias de un número secuenciable. 
	// PRE: EsSecuenciable() == true
	
	int NumSecuencias (void) 
	{
		int ultimo_inicio = valor/2;   
		int cont_secuencias = 0; 
				
		for (int inicio = 1; inicio <= ultimo_inicio; inicio++) {
			
			int suma  = inicio;  // suma inicial (primer valor de la sec.)
			int nuevo = inicio+1; 	// segundo valor de la sec. 
	
			// Mientras que la suma de la secuencia sea menor que "valor", 
			// seguir añadiendo términos consecutivos. 
			 
			while (suma < valor) {
				suma += nuevo; // Nueva suma 
				nuevo++; // El nuevo término a considerar es el siguiente 
			}
			
			// Si "suma" es igual a "n" hemos encontrado una secuencia. 
			// Si no, la suma es necesariamente mayor.  
			if (suma == valor)
			{
				cont_secuencias++;
				secuencias[total_utilizados].SetSecuencia(inicio,(nuevo-1));	
				total_utilizados++;	
			} 
		}
		
		return cont_secuencias;
	}
	/***********************************************************************/	
	/***********************************************************************/
	// Devuelve las secuencias 
	//PRE: indice<= total_utilizados
	
	string ToString(int indice)
	{
		string cad;
		
		cad=to_string(valor)+" = "+secuencias[indice].ToString();
		
		return cad;
	}
	
};
