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
