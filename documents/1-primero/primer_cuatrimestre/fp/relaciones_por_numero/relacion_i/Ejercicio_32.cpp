/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_32
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	
	Escribid una expresión lógica que sea verdadera si una variable de tipo 
	carácter llamada letra es una letra mayúscula y falso en otro caso.
	
	Escribid una expresión lógica que sea verdadera si una variable de tipo 
	entero llamada	edad es mayor o igual que 18 y menor que 67.
	
	Escribid una expresión lógica que nos informe cuando un año es bisiesto.
	(Los años bisiestos son aquellos que o bien son divisibles por 4 pero no 
	por 100, o bien son	divisibles por 400.)
	
	Escribid una expresión lógica que nos informe si el valor de una variable 
	double llamada distancia es menor que la constante LIMITE.
	
	Usad una variable lógica que registre si el valor de una variable int es 
	menor que otra, otra que informe si son iguales y otra que informe si es 
	mayor. (Asegúrese que sólo una, y sólo una de las tres trendrá el valor 
	true.)
	
	Entradas:  
	
	1) Caracter en mayúscula o minúscula
	2) Edad
	3) Año
	4) Distancia
	5) 2 números enteros
	
	Salidas:
	
	1) Si el caracter está en mayúscula: true
	2) Si la edad es 18<=edad<67 : true
	3) Si el año es bisiesto: true
	4) Si la distancia es menor que LIMITE: true
	5) Si el primer número es menor que el segundo: true
	   Si el primer número es igual que el segundo: true
	   Si el primer número es mayor que el segundo: true
	   Si solo se da una de las 3 condiciones anteriores: true
	
	
*/
/****************************************************************************/
#include<iostream>
using namespace std;
/****************************************************************************/
int main()
{
	
		
	bool mayuscula;
	char caracter;
	
	const int EDAD_MINIMA=18,EDAD_MAXIMA=67;
	bool edad_correcta;
	int edad;
	
	bool bisiesto;
	int year;
	
	const double LIMITE=500;
	bool d_correcta;
	double distancia;
	
	bool menor,iguales,mayor;
	int num1,num2;
	bool funciona;
	
	
	//Entrada
	
	cout<<"Caracter: ";
	cin>>caracter;
	cout<<"Edad: ";
	cin>>edad;
	cout<<" _"<<endl;
	cout<<"Ano: ";
	cin>>year;
	cout<<"Distancia: ";
	cin>>distancia;
	cout<<"\r\b(La distancia limite es "<<LIMITE<<")"<<endl;
	cout<<"Primer numero: ";
	cin>>num1;
	cout<<"Segundo numero: ";
	cin>>num2;
	
	
	//Cálculos
	//Traducción de las expresiones lógicas a lenguaje c++
	mayuscula=(caracter==toupper(caracter));
	edad_correcta=((edad>=EDAD_MINIMA)&&(edad<EDAD_MAXIMA));
	bisiesto=((((year%4)==0)&&((year%100!=0)))||(year%400==0));
	d_correcta=(distancia<LIMITE);
	menor=(num1<num2);
	iguales=(num1==num2);
	mayor=(num1>num2);
	funciona=((menor&& !iguales && !mayor)	  //Comprueba que solo se cumple 
			|| (!menor&& iguales && !mayor)   //una de las condiciones
			|| (!menor&& !iguales && mayor));
	
	
	//Salidas
	
	cout<<endl;
	cout<<"El caracter esta en mayuscula: "<<boolalpha<<mayuscula<<endl;
	cout<<"La edad es mayor o igual a "<<EDAD_MINIMA<<" y menor que "
		<<EDAD_MAXIMA<<": "<<edad_correcta<<endl;
	cout<<"    _"<<endl,
	cout<<"El ano es bisiesto: "<<bisiesto<<endl;
	cout<<"La distancia es menor que "<<LIMITE<<": "<<d_correcta<<endl;
	cout<<"El numero "<<num1<<" es menor que "<<num2<<": "<<menor<<endl;
	cout<<"El numero "<<num1<<" es igual que "<<num2<<": "<<iguales<<endl;
	cout<<"El numero "<<num1<<" es mayor que "<<num2<<": "<<mayor<<endl;
	cout<<"(El ultimo apartado funciona: "<<funciona<<")";
	
	
	return 0;
}
