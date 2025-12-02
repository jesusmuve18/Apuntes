class Circunferencia
{
	
private:
	
	Punto2D centro; //coordenadas del centro de la circunferencia
	
	//PRE: radio > 0
	double radio;
	
	static const double RADIO_INICIAL;
	static const double PI;
	
public:
	
	/**********************************************************************/
	/**********************************************************************/
	//Constructor sin argumentos
	
	Circunferencia(void):
		centro({0,0}), 
		radio(RADIO_INICIAL){};
	
	/**********************************************************************/
	//Constructor con 1 argumento
	
	Circunferencia(Punto2D el_centro):
		centro(el_centro), 
		radio(RADIO_INICIAL) {};
	
	/**********************************************************************/
	//Constructor con 2 argumentos
	
	Circunferencia(Punto2D centro, double radio):
		centro(centro), 
		radio(radio) {};
		
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve las coordenadas del centro de la circunferencia
	
	Punto2D GetCentro(void)
	{
		return centro;
	}
	
	/**********************************************************************/
	//Devuelve el diámetro de la circunferencia
	
	double GetDiametro(void)
	{
		return (2*radio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Cálculo del área
	
	double GetArea(void)
	{
		return (PI*radio*radio);
	}
	
	/**********************************************************************/
	//Cálculo del perímetro
	
	double GetLongitud(void)
	{
		return (2*PI*radio);
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Verifica si un punto se encuentra contenido en la circunferencia
	
	bool Contiene(Punto2D punto)
	{
		return (radio >= centro.Distancia(punto));
	}

	/**********************************************************************/
	//Devuelve los datos que definen la circunferencia
	
	string ToString()
	{
		string str;
		str="\tCentro: "+ centro.ToString()+
			"\n\tRadio: "+to_string(radio)+" unidades";
			
		return str;
	}
	/**********************************************************************/
	
};

//Constantes estáticas
const double Circunferencia::RADIO_INICIAL=0.5;
const double Circunferencia::PI=6*asin(0.5);

