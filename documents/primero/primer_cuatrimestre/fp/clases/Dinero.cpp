class Dinero
{
	private:
		int euros;
		int cents;
	public:
		
		//PRE: euros>=0 && cents>=0
		void Ajustar() //Ajusta las unidades en sus respectivos rangos
		{
			euros+=(int)(cents/CENTS_EN_EUR);
			cents=cents%CENTS_EN_EUR;
			
		}
		
		void Incrementar(double incremento)
		{
			double euros_incr=euros*(incremento/100);
			
			euros+=(int)euros_incr;
			cents*=(1+(incremento/100));
			cents+=((euros_incr-((int)euros_incr))*CENTS_EN_EUR);
			
			Ajustar();			
		}
		
		//PRE: euros>=0 && cents>=0
		Dinero(int euros, int cents):euros(euros),cents(cents)
		{
			Ajustar(); //Ajusto los datos al declarar un nuevo objeto
		}
		
		//PRE: euros>=0 && cents>=0
		Dinero():euros(0),cents(0) 
		{
			Ajustar(); //Ajusto los datos al declarar un nuevo objeto
		}
		
		
		string ToString()
		{
			string str_euros=FormateaString(to_string(euros),6,
							TipoAjuste::derecha, RELLENO);
							
			string str_cents=FormateaString(to_string(cents),2,
							TipoAjuste::derecha, RELLENO);
							
			return (str_euros+" Euros y "+str_cents+" Centimos");
		}
		
};
