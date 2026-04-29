/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_V				EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Enunciado:
	
	Amplíe la clase GestorNumerosSecuenciables del ejercicio 18 de esta 
	Relación de Problemas. En esta nueva versión calculamos y guardamos todas 
	las secuencias (hasta un máximo de 100) asociadas a un número secuenciable.
	
	Una secuencia está determinada únicamente por una pareja de valores enteros 
	(p, q) siendo p < q de manera que si n es un número secuenciable, 
	entonces n = p + (p + 1) + . . . + q
	
	Para probar esta adición, amplíe la funcionalidad del apartado c de manera 
	que ahora deberá realizar estas tareas:
	Mostrar los números secuenciables (sus valores) cuyo número de 
	descomposiciones coincide con el número de referencia leido.
	Para cada uno de ellos deberá mostrar todas sus descomposiciones.
	Finalmente, mostrar el total de números secuenciables que cumplen esta
	última condición.
	
	Por ejemplo, para números entre 1 y 20 con dos secuencias, el resultado 
	sería:
	
			3. Numeros secuenciables con 2 secuencias:
			  9 ( 2 secuencias)
				9 = 2 + 3 + 4
				9 = 4 + 5
			  18 ( 2 secuencias)
				18 = 3 + 4 + 5 + 6
				18 = 5 + 6 + 7
				Hay 2 numeros con 2 secuencias

	Entradas: cotas del intervalo

	Salidas: numeros secuenciables, número de secuencias y cuales son

*/
/****************************************************************************/
#include<iostream>
#include<iomanip>

using namespace std;

/****************************************************************************/
//Declaración de Clases/Objetos

class Secuencia
{
	
private:
	
	int p;
	int q;

public:
	
	/**********************************************************************/
	//Constructor con argumentos
	
	Secuencia(int p, int q): p(p), q(q) {};
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	Secuencia() {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Para establecer los parámetros
	
	void SetSecuencia(int a, int b)
	{
		p=a;
		q=b;
	}	
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve una cadena
	
	string ToString()
	{
		string cad="";
		for(int i=p; i<=q; i++)
		{
			cad+=to_string(i);
			if(i<q)
			{
				cad+=" + ";
			}
		}
		return cad;
		
	}
	
};

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


/****************************************************************************/
/****************************************************************************/
int main()
{
	//Declaración de Datos

	int n1, n2; 
	int n_sec; 
	
	// Entrada (leer los extremos)
	
	do {
	    cout << "Introduzca un numero positivo: ";
    	cin >> n1;    	
	} while (n1<=0);

	do {
	    cout << "Introduzca otro numero positivo: ";
    	cin >> n2;    	
	} while (n2<=0);

	// Entrada (Número de secuencias)

	cout << endl; 
	do {
	    cout << "Introduzca numero de secuencias: ";
    	cin >> n_sec;    	
	} while (n_sec<=0);


	// Reajustar los extremos menor y mayor

    int extremo_menor, extremo_mayor;	
	
	if (n1<n2) {
		extremo_menor = n1;
		extremo_mayor = n2;	
	}
	else {
		extremo_menor = n2;
		extremo_mayor = n1;			
	}

	cout << endl; 
	cout << "Calculando numeros secuenciables entre " << setw(3) 
	     << extremo_menor << " y " << setw(3) << extremo_mayor << endl; 
	cout << endl; 
	
	cout << "................................................" << endl; 
	cout << endl; 


	GestorNumerosSecuenciables g;
	
	// Cálculos y resultado 
	
	// 1. Calcular total de números secuenciables 

	cout << endl; 
	cout << "1. Total de numeros secuenciables." << endl; 	
	cout << endl; 
		 	
	int contador_secuenciables = 0; 
	
	for (int n=extremo_menor; n<=extremo_mayor; n++) {
	
		g.SetValor(n); // Cambiar el valor objeto del cálculo 
		
		if (g.EsSecuenciable())	contador_secuenciables++;	
	}
	
	cout << "\tSe han encontrado " << setw(3) << contador_secuenciables 
	     << " numeros secuenciables." << endl; 
	cout << endl; 


	// 2. Mostrar numeros secuenciables y el número de secuencias
	
	cout << endl; 
	cout << "2. Los numeros secuenciables y el numero de secuencias." << endl; 	
	cout << endl; 

	for (int n=extremo_menor; n<=extremo_mayor; n++) {
		
		g.SetValor(n); // Cambiar el valor objeto del cálculo 
		
		if (g.EsSecuenciable())	 {
		   	cout << "\t" << setw(3) << g.GetValor() << " es secuenciable. ";	
	    	cout << "(" << g.NumSecuencias() << " secuencias)" << endl;
		}
	  	else 
		   	cout << "\t" << setw(3) << g.GetValor()  
			     << " NO es secuenciable." << endl;	
	}
	cout << endl; 
	
	// 3. Mostrar numeros secuenciables cuyo número de secuencias 
	// 	  coincida con el valor pedido y el total.  
	
	cout << endl; 
	cout << "3. Numeros secuenciables con " << 
	     setw(2) << n_sec << " secuencias:" << endl; 		
	cout << endl; 
	
	int contador = 0; 
	
	for (int n=extremo_menor; n<=extremo_mayor; n++) {
		
		g.SetValor(n); // Cambiar el valor objeto del cálculo 
		
		if (g.EsSecuenciable()) {
		
			int num_secuencias_calculadas = g.NumSecuencias();
			
			if (num_secuencias_calculadas == n_sec) {
				
			    contador++;
			
			   	cout << "\t" << setw(3) << contador << " --> " 
				     << setw(3) << g.GetValor() 
					 << " (" << setw(2) << num_secuencias_calculadas 
					 << " secuencias)" << endl;
					 
				for(int i=0; i<num_secuencias_calculadas; i++)	
				{
					cout << "\t\t    "<<g.ToString(i)<<endl;
				}
				cout<<endl;
				
			}
		}
	}
	cout << endl; 		
	
	cout << endl; 		
	if (contador > 0) 
		cout << "\tHay " << setw(3) << contador << " numeros con "
		     << n_sec << " secuencias" << endl; 		
	else 
		cout << "\tNo hay ningun numero con " << setw(3) << n_sec 
		     << " secuencias en el rango indicado." << endl; 			
	cout << endl; 		


	return 0;

	return 0;
}

