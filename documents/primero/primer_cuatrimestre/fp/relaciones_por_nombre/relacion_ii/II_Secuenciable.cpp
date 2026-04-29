/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_76
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Diremos que un número entero positivo es secuenciable si se puede generar 
	como suma de números consecutivos. Por ejemplo, 6 = 1 + 2 + 3, 15 = 7 + 8.
	Esta descomposición no tiene por qué ser única. Por ejemplo, 
	15 = 7 + 8 = 4 + 5 + 6 = 1 + 2 + 3 + 4 + 5.
	
	Escribir un programa que lea un entero n y nos diga cuántas 
	descomposiciones posibles tiene. Por ejemplo:
	
						15 -> 3 descomposiciones
						94 -> 1 descomposición
						108 -> 3 descomposiciones
						
	Curiosidad: los únicos números con 0 descomposiciones son las potencias 
	de 2.
	
	Entradas: Numero 
	
	Salidas: Numero de descomposiciones posibles y cuales son
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	int n, sumatoria, descomposiciones=0;
	string s="", str_desc="";
	
	//Entradas
	
	do
	{
		cout<<"Numero: ";
		cin>>n;
		if(n<=0)
			cout<<"El numero introducido debe ser mayor que 0"<<endl;
	} while(n<=0);
	
	
	//Cálculos
	
	for(int i=2; i<=n;i++) 
	{								
		for(int j=1; j<((n/i)+1) ; j++)
		{
			for(int k=0; k<i; k++)
			{
				sumatoria+=j+k;
				
				if(k!=0)
				{
					s+=" + "+(to_string(j+k));	
				}
				else
				{
					s+=(to_string(j+k));
				}
			}
			
			if(sumatoria==n)
			{
				descomposiciones++;
				str_desc+="\t"+(to_string(n))+" = "+s+"\n";
			}
				
			sumatoria=0;
			s="";
		}	
	}
	
	//Salidas
	
	cout<<endl;
	
	if(descomposiciones==1)
	{
		cout<<n<<" -> "<<descomposiciones<<" descomposicion"<<endl;
	}
	else
	{
		cout<<n<<" -> "<<descomposiciones<<" descomposiciones"<<endl;
	}
	
	cout<<endl;
	cout<<str_desc;
	
	return 0;
}
/*	EXPLICACION DEL PROGRAMA:
	Mi programa se basa en buscar secuencias cada vez más largas y ver 
	cuanto suman. Para ello empiezo en 2, es decir voy a ver si existen 2
	números consecutivos cuya suma sea igual al número dado. Lógicamente
	tan solo tendré que buscar hasta ((numero/2)+1) ya que 2 numeros 
	consecutivos mayores a la mitad del número siempre van a ser mayores que 
	el número.
	
	Ahora este proceso lo iré haciendo pero en lugar de con 2 con i, cuyos 
	valores van desde el 2 hasta el número. Cuando i valga por ejemplo 4 
	empezaré a sumar los primeros 4 números (1+2+3+4) y comprobaré el 
	resultado. Después de los siguientes 4 (2+3+4+5) y así. 
	
	En general, compruebo si existen i numeros consecutivos cuya suma sea 
	igual al número dado. Lo haré desde el 1 hasta ((numero/i)+1) (por la 
	razón explicada anteriormente). 
	
	Si en este proceso encuentro alguna secuencia que verifique la igualdad
	sumaré 1 al contador de descomposiciones y además guardaré en una string
	el equivalente a:
	
		numero = primera descomposición
		numero = segunda descomposición
		......
		
	para mostrarlo al final del programa
*/
