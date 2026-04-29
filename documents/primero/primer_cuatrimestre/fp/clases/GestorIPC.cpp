class GestorIPC
{

private:
	
	static const int PRIMER_ANIO=2001;
	static const int ULTIMO_ANIO=2021;
	static const int NUM_ANIOS = ULTIMO_ANIO - PRIMER_ANIO + 1;
	static const int NUM_PAISES = 4;
	static const string PAISES [NUM_PAISES];
    static const double IPC[NUM_PAISES][NUM_ANIOS];

public:
	
	/**********************************************************************/
	//Método que devuelve el primer año
	
	int PrimerAnio(void)
	{
		return (PRIMER_ANIO);
	}
	
	/**********************************************************************/
	//Método que devuelve el último año
	
	int UltimoAnio(void)
	{
		return (ULTIMO_ANIO);
	}
	
	/**********************************************************************/
	//Método que devuelve el número de años
	
	int NumAnios(void)
	{
		return (NUM_ANIOS);
	}
	
	/**********************************************************************/
	//Método que devuelve el número de países
	
	int NumPaises(void)
	{
		return (NUM_PAISES);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve el nombre del país asociado a un número
	
	string NombrePais(int pais)
	{
		return (PAISES[pais]);
	}
	
	/**********************************************************************/
	//Método que devuelve el IPC de un año en un país
	
	double ValorIPC(int anio, int pais)
	{
		return (IPC[pais][anio]);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve el IPC medio de un pais en un periodo de tiempo
	//PRE: no se el_primer_anio + los_anios no debe ser mayor que NUM_ANIOS
	
	double IPC_Medio_PaisPeriodo(int el_pais,int el_primer_anio,int los_anios)
	{
		double aux=1;
		double ipc_medio;
		
		for(int anio=el_primer_anio; anio<el_primer_anio+los_anios; anio++)
		{	
			aux*=(1+(IPC[el_pais][anio-PRIMER_ANIO])/100.0);			
		}
		
		ipc_medio=((aux-1)*100)/los_anios;
		
		return ipc_medio;
	}	
	
	/**********************************************************************/
	//Método que devuelve el mínimo IPC medio en un período
	
	InfoIPC Info_IPC_MinMedio_Periodo (int num_anios)
	{
		InfoIPC info_ipc_min;
		info_ipc_min.valor_medio_IPC=999999999;
		info_ipc_min.num_anios=num_anios;
		
		double ipc_medio; //tomará valores auxiliares
		
		for(int pais=0; pais<NUM_PAISES; pais++)
		{
			for(int anio_inicial=PRIMER_ANIO; 
						anio_inicial<=ULTIMO_ANIO-num_anios; anio_inicial++)
			{
				
				ipc_medio=IPC_Medio_PaisPeriodo(pais,anio_inicial,num_anios);
	
				if(ipc_medio<info_ipc_min.valor_medio_IPC)
				{
					info_ipc_min.indice_pais=pais;
					info_ipc_min.valor_medio_IPC=ipc_medio;
					info_ipc_min.anio_inicio=anio_inicial;	
				}
			}
		}	
		
		return info_ipc_min;
	} 
	/**********************************************************************/
	//Método que devuelve el máximo IPC medio en un período
	
	InfoIPC Info_IPC_MaxMedio_Periodo (int num_anios)
	{
		InfoIPC info_ipc_max;
		info_ipc_max.valor_medio_IPC=-999999999;
		info_ipc_max.num_anios=num_anios;
		
		double ipc_medio; //tomará valores auxiliares
		double ipc_max=-999999999;
		
		for(int pais=0; pais<NUM_PAISES; pais++)
		{
			for(int anio_inicial=PRIMER_ANIO; 
						anio_inicial<=ULTIMO_ANIO-num_anios; anio_inicial++)
			{
				
				ipc_medio=IPC_Medio_PaisPeriodo(pais,anio_inicial,num_anios);
	
				if(ipc_medio>info_ipc_max.valor_medio_IPC)
				{
					info_ipc_max.indice_pais=pais;
					info_ipc_max.valor_medio_IPC=ipc_medio;
					info_ipc_max.anio_inicio=anio_inicial;	
				}
			}
		}	
		
		return info_ipc_max;	
	} 
	
};

const string GestorIPC::PAISES[NUM_PAISES]=
						{"Espania","Inglaterra", "USA", "Zona Euro"};
const double GestorIPC::IPC[NUM_PAISES][NUM_ANIOS]=
					   {{2.70 , 4.00 , 2.60 , 3.20 , 3.70 ,		//España
					     2.70 , 4.20 , 1.40 , 0.80 , 3.00 ,
			   		     2.40 , 2.90 , 0.30 , -1.0 , 0.00 ,
					     1.60 , 1.10 , 1.20 , 0.80 , -0.5 ,
			   		     6.50 } ,
			   		  
			   		   { 1.07 , 1.69 , 1.25 , 1.64 , 1.92 ,		//Inglaterra
					     2.97 , 2.12 , 3.11 , 2.83 , 3.73 ,
					     4.20 , 2.71 , 2.00 , 0.55 , 0.23 ,
					     1.60 , 2.90 , 2.10 , 1.30 , 0.60 ,
					     5.40 } ,
					  
					   { 1.60 , 2.40 , 1.90 , 3.30 , 3.40 ,		//USA
					     2.50 , 4.10 , 0.10 , 2.70 , 1.50 ,
					     3.00 , 1.70 , 1.50 , 0.80 , 0.70 ,
					     2.10 , 2.10 , 1.90 , 2.30 , 1.40 ,
					     7.00 } ,
					  
					   { 2.10 , 2.30 , 2.00 , 2.40 , 2.20 ,		//Zona Euro
					     1.90 , 3.10 , 1.60 , 0.90 , 2.20 ,
					     2.80 , 2.20 , 0.80 , -0.2 , 0.20 ,
					     1.10 , 1.30 , 1.50 , 1.30 , -0.3 ,
					     5.00 }};
