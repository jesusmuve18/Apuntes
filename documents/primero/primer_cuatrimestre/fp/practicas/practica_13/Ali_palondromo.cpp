/****************************************************************/
/*  FUNDAMENTOS DE PROGRAMACIÓN					EJERCICIO-20


	ALISON CARVAJAL PAULINO

	1º A2 DOBLE GRADO EN INGENIERÍA INFORMÁTICA Y MATEMÁTICAS

	 Un palíndromo es una secuencia que se lee igual de izquierda a derecha 
	 que de derecha a izquierda. Se pide que defina un método que calcule y 
	 devuelva el palíndromo de mayor longitud contenido en una secuencia. 
	 Dicho palíndromo siempre existe y será un objeto de la clase
	 SecuenciaCaracteres.


*/
/****************************************************************/
#include <iostream>	// Inclusión de los recursos E/S
using namespace std;



/////////////////////////////////////////////////////////////////////////////

class SecuenciaCaracteres {

private:

    static const int TAMANIO = 50; // N?m.casillas disponibles
    char vector_privado[TAMANIO];

    // PRE: 0<=total_utilizados<TAMANIO

    int total_utilizados; // N?m.casillas ocupadas

public:

    /***********************************************************************/
    // Constructor sin argumentos

    SecuenciaCaracteres (void) : total_utilizados (0)
    {}
    
    /***********************************************************************/
	//Constructor con argumentos
	
	SecuenciaCaracteres (string cad)
	{
		for(int i=0; i<cad.length(); i++)
		{
			vector_privado[i]=cad.at(i);
			
		}
		
		total_utilizados=cad.length();
	}
	
	
    /***********************************************************************/
    // Devuelve el n?mero de casillas ocupadas

    int TotalUtilizados (void)
    {
        return (total_utilizados);
    }

    /***********************************************************************/
    // Devuelve el n?mero de casillas disponibles

    int Capacidad (void)
    {
        return (TAMANIO);
    }

    /***********************************************************************/
    // "Vac?a" completamente la secuencia

	void EliminaTodos()
	{
		total_utilizados = 0;
	}

    /***********************************************************************/
    // A?ade un elemento ("nuevo") al vector.
    // PRE: total_utilizados < TAMANIO
    // 		La adici?n se realiza si hay alguna casilla disponible.
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
    // Eliminar el car?cter de la posici?n dada por "indice".
    // Realiza un borrado f?sico (desplazamiento y sustituci?n).
    // PRE: 0 <= indice < total_utilizados

    void Elimina (int indice)
    {
        if ((indice >= 0) && (indice < total_utilizados)) {

            int tope = total_utilizados-1; // posic. del ?ltimo

            for (int i = indice ; i < tope ; i++)
                vector_privado[i] = vector_privado[i+1];

            total_utilizados--;
        }
    }
 

    /***********************************************************************/
    // Inserta el car?cter "nuevo" en la posici?n dada por "indice".
    // Desplaza todos los caracteres una posici?n a la derecha antes de 
	// copiar en "indice" en valor "nuevo".
	// PRE: 0 <= indice < total_utilizados
    // PRE: total_utilizados < TAMANIO
    // 		La inseerci?n se realiza si hay alguna casilla disponible.
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
    // Compone un string con todos los caracteres que est?n
    // almacenados en la secuencia y lo devuelve.

    string ToString()
    {
        string cadena;

        for (int i=0; i<total_utilizados; i++)
            cadena = cadena + vector_privado[i];

        return (cadena);
    }

	
	/*****************************************************************/
	
	bool EsPalindromo (string cadena)
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
		
		cout<<"Palindromo: "<<boolalpha<<palindromo;
		
		return (palindromo);
	}
	
	/*****************************************************************/
	string Subsecuencia (int comienzo, int longitud)
	{
		string subsecuencia;
		
		for(int i=comienzo; i<longitud; i++)
		{
			char caracter = vector_privado[i];
				subsecuencia += caracter;
		}
		
		return subsecuencia;
	}
	
	/*****************************************************************/

	string MayorSubsecuencia (void)
	{
		string subsecuencia="", mayor_subsecuencia;
		
		
		//Recorre las posiciones del vector
		for(int longitud=1; longitud<=total_utilizados; longitud++)	
		{
			//Crea una subcadena y comprueba si es palindromo
			for(int comienzo=0; comienzo <total_utilizados; comienzo++)	
			{
				subsecuencia = Subsecuencia(comienzo, longitud);
				
				cout<<"Subsecuencia generada:" <<subsecuencia<<endl;
				
				if(EsPalindromo(subsecuencia) 
					&& mayor_subsecuencia.length()<=subsecuencia.length())
				{
					mayor_subsecuencia = subsecuencia;
						
				}
				
				cout<<"  Mayor subsecuencia: "<<mayor_subsecuencia<<endl;		
			}
			cout<<endl;
		
		}
	
		return mayor_subsecuencia;
	}
	
	
	
	/*****************************************************************/
};

/////////////////////////////////////////////////////////////////////////////
int main()	// Programa principal
{
	//Declaración de datos
	
	string cad;
	
	
	
	//Entrada de datos
	
	cout <<"Secuencia: ";
	getline(cin, cad);
	
	
	//Cálculos

	SecuenciaCaracteres MayorPalindromoContenido(cad);
	
	cout<<"Secuencia generada: "<<MayorPalindromoContenido.ToString()<<endl;
	
	cout<<"Mayor palindromo contenido: ";
	cout<<MayorPalindromoContenido.MayorSubsecuencia();
	
	return 0;
}
