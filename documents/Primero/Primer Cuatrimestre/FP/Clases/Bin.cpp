class Bin
{

private:
	
	unsigned int valor_bin;  // PRE: 0 <= valor_bin < 2^{32}-1
	
public:
	
	/**********************************************************************/
	//Constructor sin argumentos
	
	Bin(void): valor_bin(0) {};
	
	/**********************************************************************/
	//Constructor con argumentos
	
	Bin(unsigned int el_valor_inicial): valor_bin(el_valor_inicial){}
	
	/**********************************************************************/
	//Constructor con argumentos
	//PRE: EsBinario(el_valor_inicial) == true
	
	Bin(string el_valor_inicial)
	{
		//Declaración de Datos
	const int BASE=2;
		
		int decimal=0;
		
		for(int i=0; i<el_valor_inicial.length(); i++)
		{
			decimal+=((el_valor_inicial.at(i)-'0')
						*pow(BASE,el_valor_inicial.length()-i-1));
		}
		
		valor_bin=decimal;	
		
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Set
	
	void SetValor(unsigned int el_valor)
	{
		valor_bin=el_valor;
	}
	
	//PRE: EsBinario(el_valor) == true
	
	void SetValor(string el_valor)
	{
		int decimal;
		int cifra, exponente;
		
		for(int i=0;i<=el_valor.length();i++)
		{
			cifra=el_valor.at(i)-'0'; 		//Le resto el caracter '0'para 
											//pasarlo a entero
			exponente=el_valor.length()-i;
			decimal= (decimal+(cifra*(pow(2,exponente))));
		}
		
		valor_bin=decimal;
	}
	
	/**********************************************************************/
	/**********************************************************************/
	//Métodos Get
	
	//Devuelve el número mínimo de bits necesarios para su representación
	//binaria
	
	int MinNumBits(void)
	{
		const int BASE=2;
		
		int min_bits=log2(valor_bin)+1;
		
		return min_bits;
	}
	
	/**********************************************************************/
	//Devuelve el valor decimal 
	
	unsigned int GetValorDecimal(void)
	{
		return valor_bin;
	}
	
	/**********************************************************************/
	//Devuelve el valor_bin en formato binario mediante una string
	
	string ToString()
	{
		int n_decimal; 	 // Número del que calculamos la representación binaria
		string n_binario; 	// Resultado (string formado por '0' y '1') 

		
		int n=valor_bin; // Se trabaja con una copia de "valor_bin" 
		
		while (n != 0) 
		{
	
			// Comparar el bit menos significativo con 1  
			
			if (n & 1) 
				n_binario="1"+n_binario;  	// Es 1
			else 
				n_binario="0"+n_binario;	// No es 1 (es 0)
			
			n>>=1; // Desplazar a la derecha n (n /= 2)
		}
		
		return n_binario;
			
	}
	
	/**********************************************************************/
	//Verifica si una cadena está en formato binario
	
	bool EsBinario(string cadena)
	{
		bool binario=true;
		int pos=0;
		
		while (pos<cadena.length() && binario)
		{
			if(cadena.at(pos)!='0' && cadena.at(pos)!='1')
			{
				binario=false;
			}
		}
		
		return binario;
	}
	/**********************************************************************/
	/**********************************************************************/
	//Métodos de cálculo:
	
	//Devuelve la suma de dos objetos de tipo binario
	Bin Suma (Bin otro)
	{
		Bin suma(valor_bin+otro.GetValorDecimal());
		return (suma);
	}
	
	//Devuelve la resta de dos objetos de tipo binario
	//PRE: GetValorDecimal() > otro.GetValorDecimal()
	Bin Resta (Bin otro)
	{
		Bin resta(valor_bin-otro.GetValorDecimal());
		
		return (resta);
	}
	
	//Devuelve el AND bit a bit de dos objetos de tipo binario

	Bin AND (Bin otro)
	{
		string bin_valor_bin=ToString();
		string bin_otro=otro.ToString();
		string resultado;
		
		int n_bits=MinNumBits();
		
		if(otro.MinNumBits()>n_bits)
		{
			n_bits=otro.MinNumBits();
			
			//Relleno de ceros la cadena más corta
			string aux;
			
			for(int i=0; i<n_bits-MinNumBits(); i++)
			{
				aux+='0';
			}
			
			bin_valor_bin=aux+bin_valor_bin;
			
		}
		else
		{
			//Relleno de ceros la cadena más corta
			string aux;
			
			for(int i=0; i<n_bits-otro.MinNumBits(); i++)
			{
				aux+='0';
			}
			
			bin_otro=aux+bin_otro;
			
		}

		
		//Relleno la cadena resultado
		for(int i=0; i<n_bits; i++)
		{
			resultado+='0';
		}

		//AND bit a bit
		for(int pos=0; pos<n_bits; pos++)
		{
			
			if(bin_valor_bin.at(pos)=='1' && bin_otro.at(pos)=='1')
			{
				resultado.at(pos)='1';
			}
			else
			{
				resultado.at(pos)='0';
			}
			
		}
		
		Bin valor_resultado(resultado);
		
		return valor_resultado;
	}
	
	//Delvuelve el OR bit a bit de dos objetos de tipo binario
	
	Bin OR (Bin otro)
	{
		string bin_valor_bin=ToString();
		string bin_otro=otro.ToString();
		string resultado;
		
		int n_bits=MinNumBits();
		
		if(otro.MinNumBits()>n_bits)
		{
			n_bits=otro.MinNumBits();
			
			//Relleno de ceros la cadena más corta
			string aux;
			
			for(int i=0; i<n_bits-MinNumBits(); i++)
			{
				aux+='0';
			}
			
			bin_valor_bin=aux+bin_valor_bin;
			
		}
		else
		{
			//Relleno de ceros la cadena más corta
			string aux;
			
			for(int i=0; i<n_bits-otro.MinNumBits(); i++)
			{
				aux+='0';
			}
			
			bin_otro=aux+bin_otro;
			
		}

		
		//Relleno la cadena resultado
		for(int i=0; i<n_bits; i++)
		{
			resultado+='0';
		}

		//AND bit a bit
		for(int pos=0; pos<n_bits; pos++)
		{
			
			if(bin_valor_bin.at(pos)=='1' || bin_otro.at(pos)=='1')
			{
				resultado.at(pos)='1';
			}
			else
			{
				resultado.at(pos)='0';
			}
			
		}
		
		Bin valor_resultado(resultado);
		
		return valor_resultado;
	}
	/**********************************************************************/
	/**********************************************************************/
	
	//Desplaza los bits hacia la derecha
	
	Bin DespDcha (int veces)
	{
		string binario=ToString();
		string resultado;
		
		for(int i=0; i<MinNumBits()-veces; i++)
		{
			resultado+=binario.at(i);
		}
		
		Bin valor_resultado (resultado);
		
		return valor_resultado;
		
	}
	
	//Desplaza los bits hacia la izquierda 
	
	Bin DespIzda (int veces)
	{
		string binario=ToString();
		
		for(int i=0; i<veces; i++)
		{
			binario+="0";
		}
		
		Bin valor_resultado(binario);
		
		return valor_resultado;
	}
};
