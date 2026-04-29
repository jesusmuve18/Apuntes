class Punto2D
{

private:
	
	double x; //coordenada x
	double y; //coordenada y

public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Punto2D(void): x(0), y(0) {};
	
	/**********************************************************************/
	//Constructor con argumentos
	
	Punto2D(double coord_x, double coord_y):
		x(coord_x),
		y(coord_y) {};
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	//Modifica el valor de la coordenada x
	
	SetX(double x_nuevo)
	{
		x=x_nuevo;
	}
	
	/**********************************************************************/
	//Modifica el valor de la coordenada y
	
	SetY(double y_nuevo)
	{
		y=y_nuevo;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve el valor de la coordenada x
	
	double GetX(void)
	{
		return x;
	}
	
	/**********************************************************************/
	//Devuelve el valor de la coordenada y
	
	double GetY(void)
	{
		return y;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Devuelve una cadena con las características del punto
	
	string ToString(void)
	{
		return ("("+to_string(x)+","+to_string(y)+")");
	}
	
	/***********************************************************************/
	//Devuelve la distancia entre el objeto y otro punto de tipo Punto2D
	
	double Distancia(Punto2D otro_punto)
	{
		return sqrt(pow((otro_punto.GetX()-x),2)+pow((otro_punto.GetY()-y),2));
	}
	
};
