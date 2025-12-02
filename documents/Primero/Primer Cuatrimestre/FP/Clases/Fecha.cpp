class Fecha
{

private:
	
	int dia; 	//PRE: 1 <= dia <= 31
	int mes;	//PRE: 1 <= mes <= 12
	int anio;	

public:
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Fecha(): dia(1), mes(1), anio(2000){}; //Valores or defecto
	
	/**********************************************************************/
	//Constructores con argumentos
	
	Fecha(int dia, int mes, int anio):
		dia(dia),
		mes(mes),
		anio(anio) {};
	
	Fecha(string cadena) //Formato cadena: dd/mm/aaaa
	{
		string str_dia;
		string str_mes;
		string str_anio;
		
		for(int i=0; i<2; i++) //Guardo el dia
		{
			str_dia+=cadena.at(i);
		}
		
		for(int i=3; i<5; i++) //Guardo el mes
		{
			str_mes+=cadena.at(i);
		}
		
		for(int i=6; i<cadena.length(); i++) //Guardo el año
		{
			str_anio+=cadena.at(i);
		}
		
		
		//Los guardo en el objeto
		
		dia=stoi(str_dia);
		mes=stoi(str_mes);
		anio=stoi(str_anio);
		
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	int GetDia()
	{
		return dia;
	}
	
	
	int GetMes()
	{
		return mes;
	}
	
	
	string GetNombreMes()
	{
		const string meses[12]={"Enero", "Febrero", "Marzo", "Abril", "Mayo", 
						  		"Junio", "Julio", "Agosto", "Septiembre", 
								"Octubre", "Noviembre", "Diciembre"};
		return meses[(mes-1)];
	}
	
	
	string GetNombreCortoMes()
	{
		const string meses[12]={"Ene", "Feb", "Mar", "Abr", "May", 
						  		"Jun", "Jul", "Ago", "Sep", 
								"Oct", "Nov", "Dic"};
		return meses[(mes-1)];
	}
	
	
	int GetAnio()
	{
		return anio;
	}
	
	//Útil para la ordenación
	string GetValor()
	{
		return(to_string(anio)+to_string(mes)+to_string(dia));
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	void SetFecha(int el_dia, int el_mes, int el_anio)
	{
		dia=el_dia;
		mes=el_mes;
		anio=el_anio;
	}
	
	void SetFecha(string cadena)
	{
		string str_dia;
		string str_mes;
		string str_anio;
		
		for(int i=0; i<2; i++) //Guardo el dia
		{
			str_dia+=cadena.at(i);
		}
		
		for(int i=4; i<6; i++) //Guardo el mes
		{
			str_mes+=cadena.at(i);
		}
		
		for(int i=8; i<cadena.length(); i++) //Guardo el año
		{
			str_anio+=cadena.at(i);
		}
		
		
		//Los guardo en el objeto
		
		dia=stoi(str_dia);
		mes=stoi(str_mes);
		anio=stoi(str_anio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método ToString
	
	string ToString()
	{
		const string SEPARADOR=" ";
		string str_dia;
		
		if(dia<10)
		{
			str_dia="0"+to_string(dia);
		}
		else
		{
			str_dia=to_string(dia);
		}
		
		return (str_dia+SEPARADOR+GetNombreCortoMes()+SEPARADOR+
				to_string(anio));
		
	}

};
