/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_43
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Explicación del programa:
	
	Construya un programa que calcule cuándo se produjo la mayor secuencia de 
	días consecutivos con temperaturas crecientes. El programa leerá una 
	secuencia de reales representando temperaturas, hasta llegar a un número 
	fuera de un rango establecido entre los valores MIN_TEMP_VALIDA y 
	MAX_TEMP_VALIDA y debe calcular la subsecuencia de números ordenada, de 
	menor a mayor, de mayor longitud. El programa nos debe decir la posición 
	donde comienza la subsecuencia y su longitud.
	
	Entradas: Temperaturas 
	
	Salidas: Posición y longitud de la subcadena que se pide
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const int TEMPERATURA_MINIMA=-50, TEMPERATURA_MAXIMA=50;
	double temperatura,temperatura_anterior;
	int posicion, posicion_resultado;
	int longitud=0, longitud_resultado=0,
	contador=0;
	bool orden=false;
	
	
	//Entradas y cálculos
	
	cout<<"Intoduzca temperaturas entre "<<TEMPERATURA_MINIMA<<" y "
		<<TEMPERATURA_MAXIMA<<":"<<endl;
	
	//cout<<"Temperatura dia "<<contador+1<<": ";
	cin>>temperatura;
	
	temperatura_anterior=temperatura;
	contador++;
	
	while(TEMPERATURA_MINIMA<temperatura && temperatura<TEMPERATURA_MAXIMA)
	{
		//cout<<"Temperatura dia "<<contador+1<<": ";
		cin>>temperatura;
		if(temperatura_anterior<temperatura)
		{
			if(orden==false) //no había un orden previo
			{
				posicion=contador;
				longitud=2;
				orden=true;
			}
			else
			{		
				if(longitud>longitud_resultado)
				{
					longitud_resultado=longitud;
					posicion_resultado=posicion;
				}
				longitud++;
			}
		}
		else	//termina una secuencia ordenada
		{
			if(longitud>longitud_resultado)
				{
					longitud_resultado=longitud;
					posicion_resultado=posicion;
				}
			
			orden=false;
			longitud=0;
		}
			
		contador++;
		temperatura_anterior=temperatura;
	}
	
		
	//Salidas
	
	if(contador>2)
	{
		if(posicion_resultado==0 && longitud_resultado==0) //Valores iniciales
		{
			cout<<"No hay elementos ordenados de forma ascendente"<<endl;
		}
		else
		{
			cout<<"Posicion: "<<posicion_resultado<<endl;
			cout<<"Longitud: "<<longitud_resultado<<endl;	
		}
		
	}
	else
	{
		cout<<"No hay suficientes valores validos para establecer un orden"
			<<endl;
	}
	
	return 0;
}

/*EXPLICACIÓN DETALLADA:

El programa comienza pidiendo un dato (en este caso he puesto como comentarios 
las peticiones de datos para que funcionase mejor con ficheros tipo texto).
Seguirá pidiendo datos hasta que detecte uno que esté fuera del intervalo
prefijado. Entre cada lectura de datos hará el siguiente cálculo:
Posibilidades:
a) No había orden previamente pero esta vez sí:
	- Guardo en una variable el dato de la posición que voy calculando gracias
	  a un contador
	- Inicializo la longitud. Dado que he dicho que el elemento introducido es
	  mayor que el previo ya tendré 2 elementos ordenados por lo que le doy 
	  a la longitud el valor 2.
	- Defino orden como true para indicar que ahora sí hay orden previo.
	
b) Había orden previo y ahora también:
	- Le añado a la longitud 1 ya que he interpretado que ahora la secuencia
	  es más larga que antes.
	- Si la longitud que llevo hasta el momento es mayor que la mayor de todas
	  (que es la longitud_resultado) guardo en la longitud_resultado dicha 
	  longitud. Además guardaré la posición de inicio de la secuencia asociada
	  a la longitud.

c) El elemento introducido no es mayor que el anterior:
	- En este caso se interrumpiría la secuencia en caso de que la hubiera por
	  lo que le doy a orden el valor false.
	- Vuelvo a comprobar si la longitud que llevo hasta ese momento es mayor 
	  que la del resultado y en caso afirmativo le asigno al resultado los 
	  valores asociados a dicha longitud.
	- Una vez almacenada la mayor longitud vuelvo a resetear la variable 
	  longitud para evitar errores.
	  
Una vez analizados estos casos le añado uno al contador y almaceno la 
temperatura actual en la temperatura anterior para no perder dicho valor al 
volver a leer otra temperatura.

Finalmente al cerrar este bucle introduciendo un valor fuera del intervalo 
se imprime por pantalla la posicion y la longitud correspondientes al resultado


NOTA:

Los datos pueden ser introducidos entre espacios de la forma:
	1 2 3 4 5 1 43 ... n ... INTRO (siendo n un valor fuera del intervalo)
	(se leeran los valores anteriores a n ignorando los que le sigan)
	
O también en líneas diferentes de la forma:
	1
	2
	3
	4
	5				(siendo n un valor fuera del intervalo)
	1				(al pulsar la tecla intro después de introducir el valor n
	43				no se podrán introducir más valores)
   ...
	n
	

*/
