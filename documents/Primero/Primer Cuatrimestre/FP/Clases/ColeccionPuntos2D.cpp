class ColeccionPuntos2D
{
	
private: 

	static const int MAX_PUNTOS=100;
	Punto2D coleccion[MAX_PUNTOS];
	int total_utilizados;

public:
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	ColeccionPuntos2D(void): 	total_utilizados(0) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Método Set
	
	//Sustituye un punto de la colección (indicado por índice) por otro nuevo
	// PRE: 0 <= indice < total_utilizados
	
	void SetPunto(int indice, Punto2D punto_nuevo)
	{
		coleccion[indice]=punto_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve un punto señalado por un índice
	// PRE: 0 <= indice < total_utilizados
	
	Punto2D GetPunto(int indice)
	{
		return coleccion[indice];
	}
	
	/**********************************************************************/
	//Devuelve el número de puntos que hay en la colección
	
	int GetNumeroPuntos(void)
	{
		return total_utilizados;
	}

	/**********************************************************************/
	/**********************************************************************/
	//Añade nuevos puntos al final de la colección
	// PRE: total_utilizados < MAX_PUNTOS
	
	Aniade(Punto2D punto_nuevo)
	{
		if(total_utilizados<MAX_PUNTOS)
		{
			coleccion[total_utilizados]=punto_nuevo;
			total_utilizados++;
		}
		
	}

	/**********************************************************************/
	//Elimina un punto de la colección y desplaza los demás para no dejar
	//huecos en blanco
	// PRE: 0 <= indice < total_utilizados
	
	Elimina(int indice)
	{
		if((indice>=0)&&(indice<total_utilizados)) {

            int tope=total_utilizados-1; // posic. del último

            for(int i=indice; i<tope; i++)
            {
            	coleccion[i]=coleccion[i+1];
			}
               

            total_utilizados--;
        }
	}
	
	/**********************************************************************/
	// Inserta el punto "punto_nuevo" en la posición dada por "indice".
    // Desplaza todos los caracteres una posición a la derecha antes de 
	// copiar en "indice" en valor "punto_nuevo".
	// PRE: 0 <= indice < total_utilizados
    // PRE: total_utilizados < MAX_PUNTOS
    // 		La inserción se realiza si hay alguna casilla disponible.
    // 		Si no hay espacio, no se hace nada.
    
	void Inserta (int indice, Punto2D punto_nuevo)
	{
        if((indice>=0)&&(indice<total_utilizados)
			&&(total_utilizados<MAX_PUNTOS)) 
		{
		
			for(int i=total_utilizados; i>indice; i--)
			{
				coleccion[i]=coleccion[i-1];
			}

			coleccion[indice]=punto_nuevo;
			
			total_utilizados++;		
		}
	}
	
	/**********************************************************************/
	/**********************************************************************/
	// Compone un string con todos los caracteres que están
    // almacenados en la coleccioni y lo devuelve.

    string ToString()
    {
        string cadena=coleccion[0].ToString();
	
        for (int i=1; i<total_utilizados; i++)
        {
        	cadena+=" , "+coleccion[i].ToString();
		}

        return (cadena);
    }
    
    /**********************************************************************/
};
