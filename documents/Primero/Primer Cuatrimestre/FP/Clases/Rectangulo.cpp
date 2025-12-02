class Rectangulo
{
private:
	
	Punto2D punto_1; //coordenadas de uno de los vértices del rectángulo
	Punto2D punto_2; //coordenadas del punto opuesto con respecto a la 
				     //diagonal del punto_1
				     
public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Rectangulo(void){}; 
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor con argumentos
	
	Rectangulo(Punto2D punto1, Punto2D punto2):
		punto_1(punto1),
		punto_2(punto2) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve la coordenada de la esquina superior izquierda
	
	Punto2D GetSupIzda(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()<punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()>punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina superior derecha
	
	Punto2D GetSupDcha(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()>punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()>punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina inferior izquierda
	
	Punto2D GetInfIzda(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()<punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()<punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la coordenada de la esquina inferior derecha
	
	Punto2D GetInfDcha(void)
	{
		double coord_x;
		double coord_y;
		
		if(punto_1.GetX()>punto_2.GetX())
		{
			coord_x=punto_1.GetX();
		}
		else
		{
			coord_x=punto_2.GetX();
		}
		
		
		if(punto_1.GetY()<punto_2.GetY())
		{
			coord_y=punto_1.GetY();
		}
		else
		{
			coord_y=punto_2.GetY();
		}
		
		return Punto2D(coord_x, coord_y);
	}
	
	/**********************************************************************/
	//Devuelve la longitud de la base
	
	double GetBase(void)
	{
		return (GetInfDcha().GetX()-GetInfIzda().GetX());
	}
	
	/**********************************************************************/
	//Devuelve la longitud de la altura
	
	double GetAltura(void)
	{
		return (GetSupDcha().GetY()-GetInfDcha().GetY());
	}
	
	/**********************************************************************/
	//Devuelve el perímetro
	
	double GetPerimetro(void)
	{
		return (2*GetBase()+2*GetAltura());
	}
	
	/**********************************************************************/
	//Devuelve el área 
	
	double GetArea(void)
	{
		return (GetBase()*GetAltura());
	}

	/**********************************************************************/
	//Cálculo del centro del rectángulo (cruce de diagonales)	
															
	Punto2D GetCentro()
	{
		Punto2D centro; //Dato que devolverá la función

		centro.SetX(GetInfIzda().GetX()+(GetBase()/2));
		centro.SetY(GetInfIzda().GetY()+(GetAltura()/2));
		
		return centro;
	}
	
	/**********************************************************************/
	//Método Set
	
	//Actualiza el rectángulo 
	
	SetRectangulo(Punto2D punto1, Punto2D punto2)
	{
		punto_1=punto1;
		punto_2=punto2;
	}
	/**********************************************************************/
	/**********************************************************************/
	//Método que devuelve las características que definen al rectángulo
	
	string ToString(void)
	{	
	 	string str= "{" +GetSupIzda().ToString()+" , "
		 				+GetSupDcha().ToString()+"\n " 
						+GetInfIzda().ToString()+" , "
						+GetInfDcha().ToString()+"}";
						
		//Devuelve cada uno de los vértices en el orden del rectángulo				
		return str;
	}
	
	/**********************************************************************/
};
