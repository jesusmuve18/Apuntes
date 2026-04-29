class ParejaEnteros
{

private:
	
	int primero;
	int segundo;

public:
	
	/*********************************************************************/
	/*********************************************************************/
	//Constructor con argumentos
	
	ParejaEnteros (int el_primero, int el_segundo):
	 	primero (el_primero), 
		segundo (el_segundo) 
	{}
	
	/*********************************************************************/
	//Constructor sin argumentos
	
	ParejaEnteros (void):
	 	primero (0), 
		segundo (0) 
	{}
	
	/*********************************************************************/
	//Modifica el valor del primer entero
	
	void SetPrimero (int el_primero) 
	{ 
		primero = el_primero; 
	}
	
	/*********************************************************************/
	//Modifica el valor del segundo entero
	
	void SetSegundo (int el_segundo) 
	{ 
		segundo = el_segundo;
	}
	
	/*********************************************************************/
	//Modifica la pareja entera
	
	void SetPareja (int el_primero, int el_segundo)
	{
		primero=el_primero;
		segundo=el_segundo;
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Devuelve el primer valor entero
	
	int GetPrimero (void) 
	{ 
		return primero; 
	}
	
	/*********************************************************************/
	//Devuelve el segundo valor entero
	
	int GetSegundo (void) 
	{ 
		return segundo; 
	}
	
	/*********************************************************************/
	/*********************************************************************/
	//Construye una cadena de caracteres con la información de la pareja
	
	string ToString () 
	{
		return "{" + to_string(primero) + ", " +to_string(segundo) + "}";
	}
	
};
