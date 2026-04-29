/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			TEMA_1				EJERCICIO_40
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Crea una variable bool para cada una de las expresiones lógicas a 
	continuación basado en los datos introducidos.
	
	Apartados:
	a) El número de votos emitidos totales es mayor que el número de votantes.
	b) El número de votos válidos no es igual a la suma de los votos recibidos 
	por las	tres opciones.
	c) El número de votos registrados es igual a la suma de los votos válidos 
	y nulos.
	d) Gana la opción A.
	e) Gana la opción B.
	f) Gana la opción C.
	g) Empatan dos opciones.
	h) Hay un empate técnico entre las opciones A y B. Definimos que ocurre 
	un empate técnico cuando la diferencia entre los dos valores es menor que
	el 5 % de su suma.
	i) La opción A obtiene mayoría absoluta.
	j) La coalición A-C obtiene mayoría absoluta.
	k) Hay una coalición de dos opciones que obtiene mayoría absoluta.
	l) La opción B obtiene menos de 3 % de los votos válidos registrados.
	m) La participación es mayor que el 75 % del censo de votantes.
	n) Queremos saber si la opción A puede gobernar "sólo o en compañía de 
	otros".
	ñ) El número de abstenciones es mayor que el número de votos válidos.
	o) El número de abstenciones es mayor que el número de votos válidos, 
	pero las abstenciones no son mayores que los votos nulos.
	p) El número de votos por correo es mayor que el 20 % de los votos
	presenciales o el número de abstenciones.
	q) El número de votos nulos por correo es mayor que el número de votos
	nulos presenciales.
	r) El número de votos nulos por correo es mayor que el número de votos 
	válidos	por correo.

	Entradas:  
	
	Votos registrados
	Número de votos presenciales
	Número de votos por correo 
	Votos válidos y nulos
	Votos válidos por correo y válidos presenciales
	Número de votos que obtiene cada opción A, B, C
	
	Salidas:
	
	Resultado de cada una de las expresiones lógicas descritas previamente
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	//Declaración de datos
	
	const int VOTANTES=5000;
	
	int votos_emitidos , votos_presenciales , votos_correo;
	int votos_validos, votos_nulos;
	int votos_validos_correo , votos_validos_presenciales;
	int votos_A , votos_B , votos_C;
	int votos_registrados;
	int abstencion;
	int votos_nulos_correo=votos_correo-votos_validos_correo;
	int votos_nulos_presenciales=votos_presenciales-votos_validos_presenciales;  
	
	bool fraude_electoral;			//apartado a)
	bool error_recuento;			//apartado b)
	bool recuento_correcto;			//apartado c)
	bool gana_A , gana_B , gana_C ;	//apartados d), e), f)
	bool empate;					//apartado g)
	bool empate_tecnico;			//apartado h)
	bool abs_A ;					//apartado i)
	bool abs_AC;					//apartado j)
	bool abs_coalicion;				//apartado k)
	bool minoria_B;					//apartado l)
	bool gran_participacion;		//apartado m)
	bool solo_A;					//apartado n)
	bool mayoria_abstencion;		//apartado ñ)
	bool abstencion_nula;			//apartado o)
	bool correo_considerable;		//apartado p)
	bool nulo_correo;				//apartado q)
	bool may_nula_correo;			//apartado r)
	
	
	//Entrada
	
	cout<<"Votos emitidos: ";
	cin >>votos_emitidos;
	
	cout<<"Votos presenciales: ";
	cin >>votos_presenciales;
	
	cout<<"Votos correo: ";
	cin >>votos_correo;
	
	cout<<"Votos validos: ";
	cin >>votos_validos;
	
	cout<<"Votos nulos: ";
	cin >>votos_nulos;
	
	cout<<"Votos validos por correo: ";
	cin >>votos_validos_correo;
	
	cout<<"Votos validos presenciales: ";
	cin >>votos_validos_presenciales;
	
	cout<<"Votos de A: ";
	cin >>votos_A;
	
	cout<<"Votos de B: ";
	cin >>votos_B;
	
	cout<<"Votos de C: ";
	cin >>votos_C;
	
	
	//Cálculos
	//Traducción de cada una de las expresiones a c++
	
	votos_registrados=votos_A + votos_B + votos_C;
	abstencion=VOTANTES-votos_emitidos; //Operaciones previas
	
	fraude_electoral=(votos_emitidos>VOTANTES); 						//a)
	
	error_recuento=(votos_validos!=votos_registrados);					//b)
		 
	recuento_correcto=(votos_emitidos==(votos_validos+votos_nulos));	//c)
		 
	gana_A=((votos_A>votos_B) && (votos_A>votos_C)); 					//d)
	
	gana_B=((votos_B>votos_A) && (votos_B>votos_C)); 					//e)
	
	gana_C=((votos_C>votos_B) && (votos_C>votos_A)); 					//f) 
	
	empate=((votos_A==votos_B) || (votos_B==votos_C) 
			|| (votos_C==votos_A)); 									//g)
								 
	empate_tecnico=((abs(votos_A-votos_B))<(0.05*(votos_A+votos_B))); 	//h)
			 
	abs_A=(votos_A>(VOTANTES+1)); 										//i)
					 
	abs_AC=((votos_A+votos_C)>(VOTANTES+1)); 							//j)
					
	abs_coalicion=(((votos_A+votos_C)>((VOTANTES/2)+1)) 
					||((votos_B+votos_C)>((VOTANTES/2)+1))
					||((votos_A+votos_B)>((VOTANTES/2)+1))); 			//k)
								
	minoria_B=(votos_B<(0.03*votos_validos)); 							//l)
				
	gran_participacion=(votos_emitidos>(0.75*VOTANTES));				//m)
	
	solo_A=(abs_A); 													//n)
	
	mayoria_abstencion=(abstencion>votos_validos);						//ñ)
		
	abstencion_nula=((mayoria_abstencion)&&(abstencion<=votos_nulos)); 	//o)
				
	correo_considerable=((votos_correo>(0.2*votos_presenciales)) 
						||(votos_correo>(0.2*abstencion)));				//p)
						
	nulo_correo=(votos_nulos_correo>votos_nulos_presenciales); 			//q)
				
	may_nula_correo=(votos_nulos_correo>votos_validos_correo);			//r)
			
	
	//Salidas
	
	cout<<"Numero de votantes: "<<VOTANTES<<endl;
	cout<<boolalpha<<endl;	//Hago que se muestre en pantalla true o false
	cout<<"a) Votos emitidos totales > numero de votantes: "
		<<fraude_electoral<<endl;
	cout<<"b) Votos validos distinto de suma de los votos recibidos: "
		<<error_recuento<<endl;
	cout<<"c) Numero de votos registrados es igual a la suma de los votos "
		<<"validos y nulos: "<<recuento_correcto<<endl;
	cout<<"d) Gana la opcion A: "<<gana_A<<endl;
	cout<<"e) Gana la opcion B: "<<gana_B<<endl;
	cout<<"f) Gana la opcion C: "<<gana_C<<endl;
	cout<<"g) Hay un empate: "<<empate<<endl;
	cout<<"h) Hay un empate tecnico entre A y B: "<<empate_tecnico<<endl;
	cout<<"i) A tiene mayoria absoluta: "<<abs_A<<endl;
	cout<<"j) A-C tiene mayoria absoluta: "<<abs_AC<<endl;
	cout<<"k) Coalicion tiene mayoria absoluta: "<<abs_coalicion<<endl;
	cout<<"l) B tiene menos del 3% de los votos validos: "<<minoria_B<<endl;
	cout<<"m) La participacion es superior al 75%: "<<gran_participacion
		<<endl;
	cout<<"n) A puede gobernar solo: "<<solo_A<<endl;
	cout<<"n+1) Abstenciones mayores que votos validos: "<<mayoria_abstencion
		<<endl;
	cout<<"o) Abstenciones mayores que votos validos, pero abstenciones "
		<<"no son mayores que los votos nulos: "<<abstencion_nula<<endl;
	cout<<"p) Votos por correo mayor que el 20 % de los votos presenciales o "
		<<"el numero de abstenciones: "<<correo_considerable<<endl;
	cout<<"q) Votos nulos por correo mayor que numero de votos nulos "
		<<"presenciales: "<<nulo_correo<<endl;
	cout<<"r) Votos nulos por correo mayor que numero de votos validos "
		<<"por correo: "<<may_nula_correo<<endl;
	
	return 0;
}
