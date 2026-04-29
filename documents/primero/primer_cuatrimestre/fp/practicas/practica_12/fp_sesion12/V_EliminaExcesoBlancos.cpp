/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_43
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:

	Añadir un método EliminaExcesoBlancos() para eliminar el exceso de 
	caracteres en blanco, es decir, que sustituya todas las secuencias de 
	espacios en blanco por un sólo espacio. Por ejemplo, si el vector original 
	es {' ','a','h',' ',' ',' ','c'}, que contiene una secuencia de tres 
	espacios consecutivos, el vector resultante debe ser {' ','a','h',' ','c'}.

*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
//Declaración de constantes globales

/****************************************************************************/
//Declaración de tipos de datos

/****************************************************************************/
//Declaración de Clases/Objetos

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
    // 		La inseerción se realiza si hay alguna casilla disponible.
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
            cadena = cadena + vector_privado[i];

        return (cadena);
    }
	
	/***********************************************************************/
	//Elimina un caracter de la secuencia todas las veces que aparezca
	
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
	//Elimina los espacios en blanco consecutivos hasta dejar uno solo
	
	void EliminaExcesoBlancos()
	{
		bool espacio=false;
		int posicion=0;
		
		while(posicion<total_utilizados)
		{
			if(vector_privado[posicion]==' ')
			{
				if(!espacio) //Si no había espacio antes
				{
					espacio=true; //Ahora sí hay espacio
					posicion++; 
				}
				else //Si había espacio antes
				{
					Elimina(posicion);
				}
			}
			else
			{
				espacio=false;
				posicion++;
			}
		}
	}
	
	
	/***********************************************************************/
};
/*****************************************************************************
  	Función:
	Parámetros:
	Devuelve:
............................................................................*/

/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos

	string cadena_aux;
	
	//Entradas

	cout<<"Introduce cadena de caracteres: ";
	getline(cin,cadena_aux);
	
	SecuenciaCaracteres secuencia(cadena_aux);

	//Salidas

	cout<<"Secuencia generada: "<<secuencia.ToString()<<endl;
	secuencia.EliminaExcesoBlancos();
	cout<<"Secuencia procesada: "<<secuencia.ToString()<<endl;


	return 0;
}

