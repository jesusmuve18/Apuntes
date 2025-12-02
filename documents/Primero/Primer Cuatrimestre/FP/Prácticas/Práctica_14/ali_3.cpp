#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

/***************************************************************************/

/***************************************************************************/
class Fecha 
{
private: 

	int dia;	// 1 <= dia <= 31 
	int mes; 	// 1 <= mes <= 12
	int anio; 	// 1900 <= anio	
	
public:

/***********************************************************************/
	// Constructor sin argumentos
	
	Fecha(void){};
	
    // Constructor con argumentos

    Fecha (string cad_fecha)
	{
		dia  = stoi(cad_fecha.substr(0,2));	 
		mes  = stoi(cad_fecha.substr(3,2));	  	
		anio = stoi(cad_fecha.substr(6,4));
	}
		
/***************************************************************************/
/***************************************************************************/
// Compone y devuelve un dato string a partir de un dato Fecha.
// Parámetros: la_fecha, el dato a partir del que se hace la composición.
// Devuelve: Un string con la representación textual de "la_fecha".
//
// PRE: Los valores de "la_fecha" son correctos

string ToString ()
{
	const string MESES[] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun", 
							"Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
	string cad; 
	
	cad = cad + to_string(dia) + " " + 	 
	      MESES[mes-1] + " " + 
		  to_string(anio);
	
	return cad; 
}

/***************************************************************************/
/***************************************************************************/
// Compone y devuelve un dato Fecha a patir de un dato string, 
// Parámetros: cad_fecha, el dato string del que se extraen los valores 
//			   de los tres campos.
// Devuelve: Un dato Fecha.
//
// PRE: cad_fecha.length() == 10 y su formato es dd/mm/aaaa

string RellenaFecha (string cad_fecha)
{
	string una_fecha; 
	
	dia  = stoi(cad_fecha.substr(0,2));	 
	mes  = stoi(cad_fecha.substr(3,2));	  	
	anio = stoi(cad_fecha.substr(6,4));	 

	
	return una_fecha; 
}

bool EsMenorFecha (Fecha otro) 
{
	bool menor; 
	string fecha;
	
	if (otro.anio > anio) menor = true; 
	else {
		if (otro.anio < anio) menor = false;
		else { // años iguales
		
			if (otro.mes > mes) menor = true; 
			else {
				if (otro.mes < mes) menor = false; 
				else { // meses iguales
					
					if (otro.dia > dia) menor = true; 
					else {
						if (otro.dia < dia) menor = false; 
						else { // dias iguales
							menor = false; 
							
						} 
					} 
					
				} 
			} 
			
		} 
	} 

	
	return (menor);
}

};

/***********************************************************************/
class Empleado
{
private:
	
	string nombre, apellidos;
	double sueldo;
	Fecha fecha_nacimiento;

	
public:

/***********************************************************************/
    // Constructor sin argumentos


	Empleado(void){
		nombre="Ali";
		apellidos="Carv Paul";
		sueldo=0;
		fecha_nacimiento.RellenaFecha("09/01/2004");
	};
	
	// Constructor con argumentos
    Empleado (Fecha fecha_natal, string los_apellidos,
			  string el_nombre, double el_sueldo) : 
			  
			  apellidos (los_apellidos), nombre (el_nombre), 
			  fecha_nacimiento(fecha_natal), sueldo(el_sueldo){};
	


/***************************************************************************/
/***************************************************************************/
// Compone y devuelve un dato string a partir de un dato RegEmpleado.
// Parámetros: empleado, el dato a partir del que se hace la composición.
// Devuelve: Un string con la representación textual de "empleado".
//
// PRE: Los valores de "empleado" son correctos

string ToStringEmpleado (void) 
{
	string cad = apellidos + "  |" + nombre + "  |" + 
		  			fecha_nacimiento.ToString() + "  |" + 
		  			to_string (sueldo); 
		  
	return cad; 	
}

/***************************************************************************/
/***************************************************************************/
// Compone y devuelve un dato string a partir de otro, tomando los 
// 		caracteres desde una posición dada hasta encontrar un carcater dado.
// Parámetros: cad_fuente, string desde donde se extrae la palabra.
//			   pos, posición inicial.
//			   delim, carácter que marca el final de la zona de interés.
//
// PRE: 0 <= pos 

string FormaPalabra (string cad_fuente, int pos, char delimitador) 
{
	string cad; 
	
	bool sigo_leyendo = true; 
	
	while (sigo_leyendo && pos<(int)cad_fuente.length()) {
		
		if (cad_fuente.at(pos) != delimitador) {
			cad += cad_fuente.at(pos); 
			pos++;
		}
		else sigo_leyendo = false; 
	}
		
	return cad; 
}

/***************************************************************************/
/***************************************************************************/
// Compone y devuelve un dato RegEmpleado a patir de un dato string. 
// Parámetros: linea, el dato string del que se extraen los valores 
//			   de los campos del dato RegEmpleado.
// Devuelve: Un dato RegEmpleado.

void RellenaEmpleado (string linea, char delimitador)
{ 

	int pos = 0; // posicion en la cadena 

	// Copiar apellidos en "un_empleado"
	apellidos =	FormaPalabra (linea, pos, delimitador); 
	pos += apellidos.length()+1;


	// Copiar nombre en "un_empleado"
	nombre = FormaPalabra (linea, pos, delimitador); 
	pos += nombre.length()+1;
	

	// Copiar fecha en "un_empleado". Hay que convertir la fecha leida 
	// (cadena, dd/mm/aaaa) en un dato Fecha. Esta conversión se hace 
	// en la función RellenaFecha
	
	string str_fecha = FormaPalabra (linea, pos, delimitador);
	fecha_nacimiento.RellenaFecha(str_fecha);

	pos += str_fecha.length()+1;


	// Copiar sueldo en "un_empleado"	
	string str_sueldo = FormaPalabra (linea, pos, delimitador);
	sueldo = stod(str_sueldo); 

	 
}


/***********************************************************************/
/***********************************************************************/
// Comprueba si un campo sueldo es menor que otro campo sueldo. 
// Parámetros: sueldo1, sueldo2, los datos que se comparan.
// Devuelve: true, si sueldo1 es menor que sueldo2. 

bool EsMenorSueldo (Empleado otro)
{
	return (otro.sueldo > sueldo); 
}



/***********************************************************************/
/***********************************************************************/
// Comprueba si un campo apellido es menor que otro campo apellido. 
// Parámetros: apellido1, apellido2, los datos que se comparan.
// Devuelve: true, si apellido1 es menor que apellido2. 

bool EsMenorApellido (Empleado otro)
{
	return (otro.apellidos > apellidos); 	
}


/***********************************************************************/
/***********************************************************************/
// Comprueba si un dato "RegEmpleado" es menor que otro dato "RegEmpleado".
// Parámetros: reg1, reg2, los datos "RegEmpleado" que se comparan.
// Devuelve: true, si reg1 es menor que reg2.

bool EsMenor (Empleado otro) 
{	
//	return (EsMenorSueldo(otro));	
//	return (EsMenorApellido(otro));	
	return (fecha_nacimiento.EsMenorFecha(otro.fecha_nacimiento));	
}

		
};
/***************************************************************************/
/***************************************************************************/
class SecuenciaEmpleados {

private:

    static const int TAMANIO = 50; // Num.casillas disponibles
    Empleado vector_privado[TAMANIO];

    // PRE: 0<=total_utilizados<TAMANIO

    int total_utilizados; // Num.casillas ocupadas

public:

    /***********************************************************************/
    // Constructor sin argumentos

    SecuenciaEmpleados (void) : total_utilizados (0)
    {}

	// Constructor con argumentos
	
	SecuenciaEmpleados (string linea, char delimitador ='*')
	{
		CompletaSecuencia (linea, delimitador);
	}
	
	/***********************************************************************/
	//Este método se encarga de completar la secuencia de empleados
	
	void CompletaSecuencia (string linea, char delimitador ='*')
	{  
		total_utilizados=0;
	
		while (getline (cin, linea)) 
		{ 
			
		 vector_privado[total_utilizados].RellenaEmpleado(linea, delimitador);
			
		 total_utilizados++;
		}
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

    void Aniade (Empleado nuevo)
    {
        if (total_utilizados < TAMANIO){
            vector_privado[total_utilizados] = nuevo;
            total_utilizados++;
        }
    }

    /***********************************************************************/
    // Devuelve el elemento de la casilla "indice"
    // PRE: 0 <= indice < total_utilizados

    Empleado Elemento (int indice)
    {
        return (vector_privado[indice]);
    }

    /***********************************************************************/
    // Cambia el contenido de la casilla "indice" por el valor "nuevo"
    // PRE: 0 <= indice < total_utilizados

   void Modifica (int indice, Empleado nuevo)
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
    
	void Inserta (int indice, Empleado nuevo)
	{
        if ((indice >= 0) && (indice < total_utilizados) 
		    && (total_utilizados < TAMANIO)) {
		
			for (int i = total_utilizados ; i > indice ; i--)
				vector_privado[i] = vector_privado[i-1];
			
			vector_privado[indice] = nuevo;
			total_utilizados++;		
		}
	}
   
    /***********************************************************************/
    //Este método se encarga de mostrar la lista de empleados
    //(Similar a ToString)

    string MuestraEmpleado()
    {
    	string cad;
    	
        for (int i=0; i<total_utilizados; i++)
        {
        	cad += vector_privado[i].ToStringEmpleado() + " ";
		}
		
		return cad;  
    }
	
	/***********************************************************************/
	//Este método ordena la secuencia de empleados por sueldo
	//Se hace uso del metodo EsMenorSueldo de la clase Empleado
	
	void OrdenaEmpleadoSueldo()
	{
		
		for (int izda = 0; izda < total_utilizados; izda++) {
	   
			// Calcular el mínimo entre "izda" y "total_utilizados"-1
			
			Empleado minimo = vector_privado[izda]; // Valor del mínimo
			int pos_min = izda;       		   	  // Posición del mínimo
			
			for (int i = izda + 1; i < total_utilizados; i++) {
			
				if (vector_privado[i].EsMenorSueldo(minimo)) { //Nuevo mínimo
					minimo = vector_privado[i];
					pos_min = i;
				}
				
			} // for i
		
			// Intercambiar los valores guardados en "izda" y "pos_min"
			      
			Empleado intercambia = vector_privado[izda];
			vector_privado[izda] = vector_privado[pos_min];
			vector_privado[pos_min] = intercambia;
			
		} // for izda
		
	}
	
	/***********************************************************************/
	//Este método ordena la secuencia de empleados por apellido
	//Hace uso del método EsMenorApellido de la clase Empleado
	
	void OrdenaEmpleadoApellido()
	{
		
		for (int izda = 0; izda < total_utilizados; izda++) {
	   
			// Calcular el mínimo entre "izda" y "total_utilizados"-1
			
			Empleado minimo = vector_privado[izda]; // Valor del mínimo
			int pos_min = izda;       		   	  // Posición del mínimo
			
			for (int i = izda + 1; i < total_utilizados; i++) {
			
				if (vector_privado[i].EsMenorApellido(minimo)) { //Nuevo mínimo
					minimo = vector_privado[i];
					pos_min = i;
				}
				
			} // for i
		
			// Intercambiar los valores guardados en "izda" y "pos_min"
			      
			Empleado intercambia = vector_privado[izda];
			vector_privado[izda] = vector_privado[pos_min];
			vector_privado[pos_min] = intercambia;
			
		} // for izda
	}	
	
	/***********************************************************************/
	//Este método ordena la secuencia de empleados por fecha de nacimiento
	//Hace uso del método EsMenor de la clase Empleado 
	//(con la opción de orden por fecha)
		
	void OrdenaEmpleadoFecha()
	{
		
		for (int izda = 0; izda < total_utilizados; izda++) {
	   
			// Calcular el mínimo entre "izda" y "total_utilizados"-1
			
			Empleado minimo = vector_privado[izda]; // Valor del mínimo
			int pos_min = izda;       		   	  // Posición del mínimo
			
			for (int i = izda + 1; i < total_utilizados; i++) {
			
				if (vector_privado[i].EsMenor(minimo)) { //Nuevo mínimo
					minimo = vector_privado[i];
					pos_min = i;
				}
				
			} // for i
		
			// Intercambiar los valores guardados en "izda" y "pos_min"
			      
			Empleado intercambia = vector_privado[izda];
			vector_privado[izda] = vector_privado[pos_min];
			vector_privado[pos_min] = intercambia;
			
		} // for izda
		
	}
		
	
	
};




class ParejaEnteros 
{
private:
	int primero, segundo;
	
public:
	
	ParejaEnteros (int el_primero, int el_segundo)
	: primero (el_primero), segundo (el_segundo) { }
	
	void SetPrimero (int el_primero) 
	{
	 	primero = el_primero; 
	}
	
	void SetSegundo (int el_primero) 
	{ 
		segundo = el_primero; 
	}
	
	int GetPrimero (void) 
	{ 
		return primero; 
	}
	
	int GetSegundo (void) 
	{ 
		return segundo; 
	}
	
	string ToString () 
	{
		return "{" + to_string(primero) + ", " +
		to_string(segundo) + "}";
	}
};

class TablaDentadaEmpleados
{
	
private:

    static const int NUM_FILS = 50;  // Filas disponibles
    static const int NUM_COLS = 40;  // Columnas disponibles

    Empleado matriz_privada[NUM_FILS][NUM_COLS];

    // PRE: 0 <= filas_utilizadas <= NUM_FILS

    int filas_utilizadas;

    // PRE: 0 <= num_cols_utilizadas[i] <= NUM_COLS
    //      para i=0,1,...,filas_utilizadas-1
    // Número de columnas ocupadas en cada fila

    int num_cols_utilizadas[NUM_FILS] = {0};

public:

	/***********************************************************************/
	// Constructor sin argumentos--> Crea matriz nula

	TablaDentadaEmpleados (void) : filas_utilizadas(0) { }
	
	/***********************************************************************/
	// Constructor--> Recibe una secuencia de caracteres que será la primera 
	// fila de la tabla. 

	TablaDentadaEmpleados (SecuenciaEmpleados primera_fila)
		: filas_utilizadas(0)
	{
		Aniade(primera_fila); // Actualiza "filas_utilizadas"
	}
	
	/***********************************************************************/
	// Método de lectura: número máximo de filas

	int CapacidadFilas (void)
	{
		return (NUM_FILS);
	}

	/*****************************************************************/
	// Método de lectura: número máximo de columnas

	int CapacidadColumnas (void)
	{
		return (NUM_COLS);
	}

	/*****************************************************************/
	// Método de lectura: número real de filas usadas

	int FilasUtilizadas (void)
	{
		return (filas_utilizadas);
	}

	/*****************************************************************/
	// Método de lectura: número real de columnas usadas en la fila "fila"
	// PRE: 0 <= fila < filas_utilizadas
	
	int ColumnasUtilizadas (int fila)
	{
		return (num_cols_utilizadas[fila]);
	}

	/*****************************************************************/
	// Método de lectura: devuelve el dato que ocupa la casilla 
	// de coordenadas (fila, columna)
	// PRE: 0 <= fila < filas_utilizadas
	// PRE: 0 <= columna < num_cols_utilizadas[fila]	
	
	string Elemento (int fila, int columna)
	{
		return (matriz_privada[fila][columna].ToStringEmpleado());
	}

	
	/***********************************************************************/
	// Devuelve true si la tabla está vacía

	bool EstaVacia (void) 
	{
		return (filas_utilizadas == 0);
	}
	
	/***********************************************************************/
	// "Vacía" una tabla
	
	void EliminaTodos (void)
	{
		filas_utilizadas = 0;
	}
	
	/*****************************************************************/
	// Devuelve una fila completa (un objeto "SecuenciaCaracteres")
	// PRE: 0 <= indice_fila < filas_utilizadas
	
	SecuenciaEmpleados Fila (int indice_fila)
	{
		SecuenciaEmpleados la_fila;
				
		int num_columnas = num_cols_utilizadas[indice_fila];
			
		for (int c=0; c<num_columnas; c++) 
			la_fila.Aniade (matriz_privada[indice_fila][c]);
		
		return (la_fila);
	}

	/*****************************************************************/
	// Añade una fila completa. La fila se porporciona en un objeto de 
	// la clase "SecuenciaCaracteres".
	// PRE:  fila_nueva.TotalUtilizados() <= NUM_COLS
	// PRE:  filas_utilizadas < NUM_FILS
	
	void Aniade (SecuenciaEmpleados fila_nueva)
	{
		int numero_columnas_fila_nueva = fila_nueva.TotalUtilizados();
					
		if ((filas_utilizadas < NUM_FILS) &&
			(numero_columnas_fila_nueva <= NUM_COLS)) {

			for (int c=0; c<numero_columnas_fila_nueva; c++) 
				matriz_privada[filas_utilizadas][c] = fila_nueva.Elemento(c);

			num_cols_utilizadas[filas_utilizadas]=numero_columnas_fila_nueva;
			
			filas_utilizadas++;
		}
	}

	/*****************************************************************/
	// Calcula si dos matrices dentadas son iguales

	bool EsIgual (TablaDentadaEmpleados otra_tabla)
	{
		bool son_iguales = true; 
		
		if (filas_utilizadas != otra_tabla.FilasUtilizadas())
			
			son_iguales = false;
		
		else {
			
			int f=0; 
			
			while (f<filas_utilizadas && son_iguales) {
				
				if (num_cols_utilizadas[f] != otra_tabla.ColumnasUtilizadas(f)) 
				
					son_iguales = false;	
				
				else {
					
					int c=0; 
					
					while (c<num_cols_utilizadas[f] && son_iguales) {
				
						if (matriz_privada[f][c].ToStringEmpleado() != otra_tabla.Elemento(f,c)) 
							son_iguales = false;
							
						c++;
						
					} // while c
		
					f++;
					
				} // else 
					
			} // while f
					
		} // else 
	
		return (son_iguales);
	}
	
	/*****************************************************************/
	// Inserta una fila completa en una posición dada. 
	// La fila se porporciona en un objeto "SecuenciaCaracteres".
	// 
	// Recibe: num_fila, la posición que ocupará "fila_nueva" cuando se 
	//		 		inserte en la tabla.
	//		   fila_nueva, la secuencia que se va a insertar. Se trata 
	//				de un objeto de la clase SecuenciaCaracteres. 
	//
	// PRE:  fila_nueva.TotalUtilizados() <= NUM_COLS
	// PRE:  filas_utilizadas <= NUM_FILS
	// PRE:  0 <= num_fila <= TotalUtilizados()
	//		 Si num_fila = 0, "nueva_fila" será la nueva primera fila. 
	//		 Si num_fila = TotalUtilizados(), "nueva_fila" será la nueva 
	//		 última fila (el resultado será equivalente al de "Aniade()" 
	
	void Inserta (int num_fila, SecuenciaEmpleados fila_nueva)
	{
		int numero_columnas_fila_nueva = fila_nueva.TotalUtilizados();
					
		// Comprobar precondiciones
		
		if ((filas_utilizadas < NUM_FILS) &&
			(numero_columnas_fila_nueva <= NUM_COLS) &&
			(0<=num_fila) && (num_fila <=filas_utilizadas)) {

			// "Desplazar" las filas hacia posiciones altas. 
			// La última fila se copia en una NUEVA FILA que ocupa la 
			// posición "filas_utilizadas", la penúltima se copia en 
			// "filas_utilizadas"-1, ... y se queda un "hueco" en 
			// la fila "num_fila ".
			// NOTA: no se modifica (todavía) "filas_utilizadas" 
			
			for (int fil=filas_utilizadas; fil>num_fila; fil--) {	
			
				int num_columnas = num_cols_utilizadas[fil-1];
					
				for (int c=0; c<num_columnas; c++) 
					matriz_privada[fil][c] = matriz_privada[fil-1][c]; 
					
				num_cols_utilizadas[fil] = num_cols_utilizadas[fil-1];
			}
					
			// Copiar en el "hueco" (fila "num_fila") el contenido de 
			// la secuencia "fila_nueva"
			
			for (int c=0; c<numero_columnas_fila_nueva; c++) 
				matriz_privada[num_fila][c] = fila_nueva.Elemento(c);
			
			num_cols_utilizadas[num_fila] = numero_columnas_fila_nueva;
			
			filas_utilizadas++; // Actualización del tamaño de la tabla.
		}
	}

	/*****************************************************************/
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
			
				int num_columnas = num_cols_utilizadas[fil+1];
					
				for (int c=0; c<num_columnas; c++) 
					matriz_privada[fil][c]=matriz_privada[fil+1][c];
					
				num_cols_utilizadas[fil] = num_cols_utilizadas[fil+1];
			}
			
			filas_utilizadas--; // Actualización del tamaño de la tabla.
		}
	}
		
	/*********************************************************************/
	// Forma un string con el contenido de la tabla.  
	// La primera línea es el encabezamiento indicado en "titulo".
	
	string ToString (string titulo)
	{
		string cad; 
		
		cad = "\n" + titulo + "\n"; 	
		cad = cad + "\n--------------------------------------\n"; 
		
		for (int fila=0; fila<filas_utilizadas; fila++) {
	
			SecuenciaEmpleados la_secuencia = Fila(fila);
			
			int num_datos = la_secuencia.TotalUtilizados();
	
			cad = cad + "Fila " + to_string(fila); 
			cad = cad + " (" + to_string(num_datos) + " datos): "; 
			cad = cad + la_secuencia.MuestraEmpleado() + "\n";
		}
		
		cad = cad + "--------------------------------------\n"; 
		cad = cad + "\n\n";
		
		return (cad); 
	}
	
	
	ParejaEnteros LocalizacionSalarioMenor (void);
	ParejaEnteros LocalizacionSalarioMayor (void);
	double SalarioMedio (void);
	SecuenciaEmpleados EmpleadosSalarioMenor (double valor);
	SecuenciaEmpleados EmpleadosSalarioMayor (double valor);
	/*****************************************************************/

};

/***************************************************************************/

int main ()
{
	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	

	int utilizados_empleados = 0;
	
	// Delimitador del final de un campo en una línea de datos
	const char DELIMITADOR = '*';

	/***********************************************************************/
	// Lectura de datos
	
	cout << endl;
	cout << "Leyendo datos ... ";

	TablaDentadaEmpleados empleados;
	string linea; 
	SecuenciaEmpleados secuencia, trabajador;
	int pos=0;
	
	//Relleno la secuencia

	getline (cin, linea);
	
	int num_departamentos = stoi(linea);

	
	
	for(int i=0; i < num_departamentos; i++)
	{
		getline (cin, linea);
		
		int num_empleados=stoi(linea);
		
		for(int j=0; j < num_empleados; j++)
		{
			getline (cin, linea);
			secuencia.CompletaSecuencia(linea, DELIMITADOR);
		}
		
		empleados.Aniade(secuencia);
		
		secuencia.EliminaTodos();
	}
	
	
	

	cout << "... Datos leidos" << endl;
	cout << endl;
	
	
	/***********************************************************************/
	// Mostrar listado de datos leidos

	cout << endl;
	empleados.ToString("LISTADO DE EMPLEADOS");
	cout << endl; 
	 
	/*	
	//Muestro todos los tipos de ordenación 
	//Se podría haber creado un solo método que hiciera uso de EsMenor
	//y se fuera cambiando la ordenación en función de lo deseado
	
	cout << "LISTADO ORDENADO POR SUELDO" <<endl;
	cout <<endl;
	
	secuencia.OrdenaEmpleadoSueldo();
	secuencia.MuestraEmpleado();
	cout <<endl;
	
	cout << "LISTADO ORDENADO POR APELLIDO" <<endl;
	cout <<endl;
	
	secuencia.OrdenaEmpleadoApellido();
	secuencia.MuestraEmpleado();
	cout <<endl;
	
	cout << "LISTADO ORDENADO POR FECHA" <<endl;
	cout <<endl;
	
	secuencia.OrdenaEmpleadoFecha();
	secuencia.MuestraEmpleado();
	cout <<endl;
	

*/
	return 0;
}
