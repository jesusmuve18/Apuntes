/****************************************************************************/
//Declaración de tipos de Datos

enum class TipoAjuste {izquierda, centro, derecha};

/****************************************************************************
	Función: Formatea una string dandole una longitud y alineación predefinidas
			 en los parámetros
	Parámetros: • Cadena a formatear
				• Número de casillas que se quieren en la cadena de salida
				• Tipo de ajuste o alineación:
					a) ajuste::izquierda
					b) ajuste::centro
					c) ajuste::derecha
				• Relleno de las casillas que quedan libres (tipo char)
	Salida: Cadena formateada
/****************************************************************************/
string FormateaString (string cad, int num_casillas,
											TipoAjuste ajuste, char relleno)
{
	string cadena; //Salida de la función
	
	int dif_casillas = num_casillas-cad.length();  //Casillas a rellenar
	
	switch (ajuste) //La función depende del ajuste
	{
		case TipoAjuste::izquierda:
			
			cadena+=cad; //Añado la cadena antes del relleno si lo hubiese
			
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				for(int i=0;i<dif_casillas;i++)
				{
					cadena+=relleno; //Añado el relleno después de la cadena
				}
			}
			
			
			break; //izquierda
		
		//...................................................................
		
		case TipoAjuste::derecha:
			
			cadena="";
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				for(int i=0;i<dif_casillas;i++)
				{
					cadena+=relleno; //Añado el relleno antes de la cadena
				}
			}
			
			cadena+=cad; //Añado la cadena después del relleno si lo hubiese
			
			break; //derecha
			
		//...................................................................
			
		case TipoAjuste::centro:
			
			if(dif_casillas>0) //Solo añado el relleno si se requiere
			{
				cadena="";
				for(int i=0;i<(dif_casillas/2);i++)
				{
					cadena+=relleno; //Añado la mitad del relleno antes
				}
				
				cadena+=cad; //Añado en este punto la cadena
				
				//En caso de que el número a dividir entre 2 sea impar, le 
				//sumaré uno para añadir un caracter más al final y no perder 
				//información.
				
				if(dif_casillas%2!=0)  
					dif_casillas++;
				
				
				for(int i=0;i<(dif_casillas/2);i++)
				{
					cadena+=relleno; //Añado la otra mitad del relleno después
				}
				
				
			}	//if(dif_casillas>0)
			
			else //Si no hace falta añadir relleno 
			{
				cadena=cad;
			}
			
			
			break; //centro
			
	}
	
	return cadena; //Salida de la función
}
