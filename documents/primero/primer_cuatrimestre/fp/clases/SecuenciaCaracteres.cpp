class SecuenciaCaracteres {

private:

    static const int TAMANIO = 50; // Núm.casillas disponibles
    char vector_privado[TAMANIO];

    // PRE: 0<=total_utilizados<TAMANIO

    int total_utilizados; // Núm.casillas ocupadas

public:

    /***********************************************************************/
    // Constructor sin argumentos

    SecuenciaCaracteres (void) : total_utilizados (0)
    {}
    
    //Constructor con argumentos 
    
    SecuenciaCaracteres(string cadena)
    {
    	total_utilizados=cadena.length();
    	
    	for(int i=0; i<total_utilizados ; i++)
    	{
    		vector_privado[i]=cadena.at(i);
		}
	}
	
	/***********************************************************************/
	//Actualiza la secuencia con una cadena dada
	
	SetSecuencia(string cadena)
	{
		total_utilizados=cadena.length();
    	
    	for(int i=0; i<total_utilizados ; i++)
    	{
    		vector_privado[i]=cadena.at(i);
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

    void Aniade (char nuevo)
    {
        if (total_utilizados < TAMANIO){
            vector_privado[total_utilizados] = nuevo;
            total_utilizados++;
        }
    }

    /***********************************************************************/
    // Devuelve el elemento de la casilla "indice"
    // PRE: 0 <= indice < total_utilizados

    char Elemento (int indice)
    {
        return (vector_privado[indice]);
    }

    /***********************************************************************/
    // Cambia el contenido de la casilla "indice" por el valor "nuevo"
    // PRE: 0 <= indice < total_utilizados

   void Modifica (int indice, char nuevo)
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
    // 		La inserción se realiza si hay alguna casilla disponible.
    // 		Si no hay espacio, no se hace nada.
    
	void Inserta (int indice, char valor_nuevo)
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

    string ToString()
    {
        string cadena;

        for (int i=0; i<total_utilizados; i++)
            cadena+=vector_privado[i];

        return (cadena);
    }
	
	/***********************************************************************/
	void EliminaOcurrencias (char caracter)
	{
		int posicion=0;
		while(posicion<total_utilizados)
		{
			if(vector_privado[posicion]==caracter)
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
	/***********************************************************************/
	//Devuelve una subsecuencia dada la posicion del primer elemento y la 
	//longitud que debera tener la subsecuencia
	//PRE: posicion_inicial+longitud <= total_utilizados
	
	string Subsecuencia(int posicion_inicial, int longitud)
	{
		string subsecuencia;
		
		for(int i=posicion_inicial; i<posicion_inicial+longitud; i++)
		{
			subsecuencia+=vector_privado[i];
		}
		
		return subsecuencia;
	}
	
	/***********************************************************************/
	//Verifica si una cadena es palíndroma
	
	bool EsPalindromo(string cadena)
	{
		
		string cadena_invertida;
		bool palindromo=false;
		
		
		for(int i=1; i<=cadena.length(); i++)
		{
			cadena_invertida+=cadena[cadena.length()-i];
		}
		
		if(cadena==cadena_invertida)
		{
			palindromo=true;
		}
		
		return palindromo;
		
	}
	/***********************************************************************/
	//Devuelve el palíndromo de mayor longitud de la secuencia
	
	string Palindromo_mayor_longitud ()
	{
		string mayor_palindromo;
		
		for(int longitud=1; longitud<=total_utilizados; longitud++)
		{
			for(int posicion_inicial=0; 
			   posicion_inicial<=total_utilizados-longitud; posicion_inicial++)
			{
				
				if(EsPalindromo(Subsecuencia(posicion_inicial,longitud)))
				{
					/*Dado que la longitud es un parámetro que toma valores 
					  cada vez mayores o iguales al anterior, cualquier nuevo
					  palíndromo que encuentre será de longitud mayor o igual a
					  la anterior y lo guardaré como el mayor */
					
					mayor_palindromo=Subsecuencia(posicion_inicial,longitud);
				}
				
			}
			
		}
		
		return mayor_palindromo;
	}
};
