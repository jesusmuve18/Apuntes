/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_VI				EJERCICIO_6
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Queremos disponer de una clase que nos permita trabajar con datos binarios 
	(la clase Bin) y realizar las operaciones básicas típicas sobre éstos.
	
	Vamos a restringir el tamaño a un máximo de 32 bits y sólo consideraremos 
	valores positivos. Así, podremos operar con valores (decimales) entre 0 y 
	2^{32}-1 = 4294967295.
	
	Investigue y vea que el tipo unsigned int requiere 4 bytes (32 bits) para 
	su almacenamiento y que el máximo valor que puede representarse es 
	4294967295 (consulte UINT_MAX en climits). Esto nos ayudará a adoptar un 
	dato unsigned int como campo privado de la clase. Es económico 
	(4 bytes para cada valor a representar) y disponemos de un amplio surtido 
	de operaciones aritméticas que pueden realizarse con datos unsigned int.

*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
//Declaración de Clases/Objetos

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
/****************************************************************************/
/****************************************************************************/
//Declaración de tipos de Datos

//Para mejorar la presentación
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
string FormateaString (string cad, int num_casillas
			,TipoAjuste ajuste=TipoAjuste::derecha, char relleno=' ')
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
					cadena+=relleno; //Àñado el relleno antes de la cadena
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

/****************************************************************************/
/****************************************************************************/

int main()
{
	//Declaración de Datos
	
	const int MAX_BITS=32; //Para mejorar la representación de resultados
	
	Bin binario1("100101001");  //Prueba del constructor a partir de string
	Bin binario2(221);			//Prueba del constructor a partir de int
	
	//Cálculos para una mejor representación
	
	int max_n_bits=binario1.MinNumBits();
	
	if(binario2.MinNumBits()>max_n_bits)
	{
		max_n_bits=binario2.MinNumBits();
	}
	
	string separador; //Para separar representación de cálculos del resultado
	
	for(int i=0; i<max_n_bits; i++)
	{
		separador+="-";
	}
	

	//Salidas
	
	//Prueba del constructor, método ToString y GetValorDecimal
	
	cout<<"Binario1: "<<binario1.GetValorDecimal()<<" (Dec) = "
		<<binario1.ToString()<<" (Bin)"<<endl;
	cout<<"Binario2: "<<binario2.GetValorDecimal()<<" (Dec) = "
		<<binario2.ToString()<<" (Bin)"<<endl;
	
	
	//Prueba de la función suma
	
	cout<<endl;
		cout<<"Binario1 + Binario2: "<<endl;
		cout<<endl;
		cout<<FormateaString(binario1.ToString(),MAX_BITS)<<endl;
		cout<<FormateaString(("+ "+binario2.ToString()),MAX_BITS)<<endl;
		
		cout<<FormateaString(separador,MAX_BITS)<<endl;
		cout<<FormateaString(binario1.Suma(binario2).ToString(),MAX_BITS)
			<<" (Dec: "<<binario1.Suma(binario2).GetValorDecimal()<<")"<<endl;
	
	//Prueba de la función resta
	
	if(binario1.GetValorDecimal()>binario2.GetValorDecimal())
	{
		cout<<endl;
		cout<<"Binario1 - Binario2: "<<endl;
		cout<<endl;
		cout<<FormateaString(binario1.ToString(),MAX_BITS)<<endl;
		cout<<FormateaString(("- "+binario2.ToString()),MAX_BITS)<<endl;
		
		cout<<FormateaString(separador,MAX_BITS)<<endl;
		cout<<FormateaString(binario1.Resta(binario2).ToString(),MAX_BITS)
			<<" (Dec: "<<binario1.Resta(binario2).GetValorDecimal()<<")"<<endl;
	}
	else
	{
		cout<<endl;
		cout<<"No es posible realizar la resta";
		cout<<endl;
	}
	
	
	//Prueba de la función AND
	
	cout<<endl;
	cout<<"Binario1 AND Binario2 (bit a bit): "<<endl;
	cout<<endl;
	cout<<FormateaString(binario1.ToString(),MAX_BITS)<<endl;
	cout<<FormateaString(("& "+binario2.ToString()),MAX_BITS)<<endl;
	
	cout<<FormateaString(separador,MAX_BITS)<<endl;
	cout<<FormateaString(binario1.AND(binario2).ToString(),MAX_BITS)
		<<" (Dec: "<<binario1.AND(binario2).GetValorDecimal()<<")"<<endl;
	
	
	//Prueba de la función OR
	
	cout<<endl;
	cout<<"Binario1 OR Binario2 (bit a bit): "<<endl;
	cout<<endl;
	cout<<FormateaString(binario1.ToString(),MAX_BITS)<<endl;
	cout<<FormateaString(("OR "+binario2.ToString()),MAX_BITS)<<endl;
	
	cout<<FormateaString(separador,MAX_BITS)<<endl;
	cout<<FormateaString(binario1.OR(binario2).ToString(),MAX_BITS)
		<<" (Dec: "<<binario1.OR(binario2).GetValorDecimal()<<")"<<endl;
	
	
	//Prueba de la función DespDcha
	
	int veces=3;
	
	cout<<endl;
	cout<<"Deplazamiento Derecha de Binario1 ( "<<veces<<" veces ): "
		<<binario1.DespDcha(veces).ToString()<<endl;
	cout<<"Deplazamiento Derecha de Binario2 ( "<<veces<<" veces ): "
		<<binario2.DespDcha(veces).ToString()<<endl;
	
	
	//Prueba de la función DespIzda
	
	veces=2;
	
	cout<<endl;
	cout<<"Deplazamiento Izquierda de Binario1 ( "<<veces<<" veces ): "
		<<binario1.DespIzda(veces).ToString()<<endl;
	cout<<"Deplazamiento Izquierda de Binario2 ( "<<veces<<" veces ): "
		<<binario2.DespIzda(veces).ToString()<<endl;

	return 0;
}

