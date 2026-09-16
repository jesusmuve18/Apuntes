/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_II				EJERCICIO_82
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
		Pueden emplearse aproximaciones numéricas para evitar el cálculo con 
		la función sqrt. Por ejemplo, en 
		https://www.youtube.com/watch?v=sOSdi9z8S6E puede ver un método muy 
		sencillo. Impleméntelo para el cálculo de la raiz cuadrada de datos 
		double.	
		
		Implemente a continuación una estimación basada en la 
		proporcionalidad entre triángulos. Se trata de aproximar la función 
		y =raiz(x) por rectas entre los tramos determinados por los valores 
		con raices exactas. Por ejemplo, en la figura 12.A mostramos la 
		función y =raiz(x) y remarcamos sobre el eje x los valores con raices 
		exactas (0, 1, 4, 9 y 16) y en el eje y los valores de las raices 
		exactas (0, 1, 2, 3 y 4). En la figura 12.B unimos los puntos 
		(x, raiz(x)) que verifican que x tiene raiz exacta.
		
		Si tomamos un segmento cualquiera en el eje x y lo vemos con detalle 
		observará que hay una región triangular bien delimitada. Por ejemplo 
		en la región entre x = 1 y x = 4 puede ver el triángulo delimitado por 
		los puntos (1, 1), (4, 1) y (4, 2) (figura 13.A). Por ser estrictos 
		se trata del triángulo delimitado por los puntos (1,raiz(1)), 
		(4,raiz(1)) y (4,raiz(4))
		
		La raiz de un valor x entre 1 y 4 se puede calcular geométricamente. 
		En la figura 13.B mostramos cómo se calcula la raiz cuadrada de x =3.5. 
		Observe que el triángulo delimitado por (1,raiz(1)), (3.5,raiz(1)) y 
		(3.5,raiz(3.5)) (triángulo amarillo) es equivalente al delimitado por 
		los puntos (1,raiz(1)), (4,raiz(1)) y (4,raiz(4)) (triángulo azul).
	
		Calcule la raiz cuadrada de un número real positivo usando esta 
		aproximación.
	
		Finalmente muestre una tabla comparativa en la que se muestren los 
		valores estimados por los dos métodos y por la función sqrt para 
		valores de x desde 0 a 16, con saltos de 0.5. ¿Puede extraer alguna 
		conclusión de esta tabla? ¿Puede proponer un método de estimación 
		mejor -más ajustado al verdadero valor- que los dos implementados?
	
*/
/****************************************************************************/
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;
/****************************************************************************/
int main()
{
	
	//Declaración de datos
	
	const double SALTO=0.5;
	const double LIMITE=16; //Número hasta el que se calcular raíces
	
	double x; //Numero al que aplicar la raíz cuadrada
	int dif_abajo, dif_arriba; 	//Los utilizaré para elegir el 
								//valor cuya raiz este mas cercana
	int dif_abajo_p2, dif_arriba_p2;//El cuadrado de las magnitudes anteriores					
	int min_dif; //Valor cuyo cuadrado se aproxima más a x
	double resultado_1, resultado_2, resultado_3; 
	bool condicion, datos_ok;
	int contador;
	double error_1, error_2;
	double h1, x1, h2, x2; 

	
	//Entradas
	
	cout<<"Calculadora de raices cuadradas:"<<endl;
	cout<<"Valor sobre el que calcular: ";
	cin>>x;
		
	cout<<"  -------------------------------------------------------- "<<endl;
	cout<<" |   x  |    Metodo 1    |     Metodo 2     |   sqrt(x)   |"<<endl;
	cout<<" |--------------------------------------------------------|"<<endl;
	
	
	
	//Cálculos
	datos_ok=(x>=0);
	
	if(datos_ok)
	{
		///Método 1:
		
		contador=0;
		condicion=true; //para entrar al bucle de una forma única
		while(condicion)
		{
			if((contador*contador)<x)
			{
				dif_abajo=contador;
			}
			else
			{
				dif_arriba=contador;
				condicion=false;
			}
			contador++;
		}
		
		dif_abajo_p2=dif_abajo*dif_abajo;
		dif_arriba_p2=dif_arriba*dif_arriba;
		
		if((x-dif_abajo_p2)>=(dif_arriba_p2-x))
		{
			min_dif=dif_arriba;
		}
		else
		{
			min_dif=dif_abajo;
		}
		
		resultado_1=min_dif+((x-(min_dif*min_dif))/(min_dif*2.0));
		
		/********************************************************************/	
		
		//Método 2:
		
		dif_abajo_p2=dif_abajo*dif_abajo;
		dif_arriba_p2=dif_arriba*dif_arriba;
		
		h1 = dif_arriba_p2-dif_abajo_p2;
		x1 = dif_arriba-dif_abajo;
		h2 = x-dif_abajo_p2;
		x2;
		
		x2=((h2*x1)/h1);
		
		resultado_2= x2 + dif_abajo;
		
		resultado_3=sqrt(x);
		
		
		error_1=abs(resultado_1-resultado_3); //Aplicando teoría de errores 
		error_2=abs(resultado_2-resultado_3); //para saber cual es más preciso
		
		//Salidas
		
		cout<<" | "<<x<<"\t|   "
			<<resultado_1<<"   \t |     "
			<<resultado_2<<"\t    |  "
			<<resultado_3<<"\t  | ";
		if(error_1!=0 || error_2!=0)
		{
			if(error_1<error_2)
				cout<<"  Error 1: "<<error_1<<" *"<<" \t Error 2: "<<error_2;
			else
				cout<<"  Error 1: "<<error_1<<"   \t Error 2: "<<error_2<<" *";
		}
		else
		{
			cout<<"  La raiz es exacta";
		}
		cout<<endl;	
	}
	
	
	/************************************************************************/	
	/************************************************************************/	
	//Desarrollo del resto de la tabla
	
	dif_abajo=0; //Por si no encuentra uno para evitar errores (en el 0 , 0.5)
	
	for(x=0; x<=LIMITE; x+=SALTO)
	{
		//Método 1:
		
		contador=0;
		condicion=true;
		
		while(condicion)
		{
			if((contador*contador)<x)
			{
				dif_abajo=contador;
			}
			else
			{
				dif_arriba=contador;
				condicion=false;
			}
			contador++;
		}
		
		dif_abajo_p2=dif_abajo*dif_abajo;
		dif_arriba_p2=dif_arriba*dif_arriba;
		
		if((x-dif_abajo_p2)>=(dif_arriba_p2-x))
		{
			min_dif=dif_arriba;
		}
		else
		{
			min_dif=dif_abajo;
		}
		
		resultado_1=min_dif+((x-(min_dif*min_dif))/(min_dif*2.0));
				
		/********************************************************************/
		/*El valor estará en el intervalo (dif_abajo,dif_arriba) que serán los 
		  valores sobre los que se creará el triángulo:
		                                    
		  						       		 __ 
		  					     /|			  |
		  					   /  |   __	  |
		     				 /	  |     |	  |
						   / |	  |	    |	  |				
			             /	 |	  |	    |	  | 		
			           /	 |	  |	    |	  |	h1		
			         /		 |	  |	    | h2  |			
			       /		 |    |	    |	  |	
		         /			 |	  |	    |	  |					
			   /			 |	  |	    |     |					
			 /__a			 |	  |	    |     |			
		   /___|_____________|____|	  __|   __|			
		   
		   |_________________|    
		   			x2
		   |______________________|
		  	          x1
		  	          
		Basándonos en la simetría de triángulos sabemos que:
		
		tg(a)=h1/x1; y además tg(a)=h2/x2; ==>   h1/x1=h2/x2;
		
		h1= dif_arriba_p2-dif_abajo_p2  
		h2= x-dif_abajo_p2
		x1= dif_arriba-dif_abajo
		x2= resultado-dif_abajo
		
		despejando x2 que es donde se encuentra el resultado:
		
		     h2*x1
		x2= ______;
		      h1
		     
		 y despejando por último el resultado:
		 
		 resultado= x2 + dif_abajo
		*/
		
		//Método 2:
		
		
		
		h1 = dif_arriba_p2-dif_abajo_p2;
		x1 = dif_arriba-dif_abajo;
		h2 = x-dif_abajo_p2;
		x2;
		
		x2=((h2*x1)/h1);
		
		resultado_2= x2 + dif_abajo;
		
		resultado_3=sqrt(x);
		
		error_1=abs(resultado_1-resultado_3); //Aplicando teoría de errores 
		error_2=abs(resultado_2-resultado_3); //para saber cual es más preciso
		
		//Salidas
		
		cout<<" | "<<x<<"\t|   "
			<<resultado_1<<"   \t |     "
			<<resultado_2<<"\t    |  "
			<<resultado_3<<"\t  | ";
		if(error_1!=0 || error_2!=0)
		{
			if(error_1<error_2)
				cout<<"  Error 1: "<<error_1<<" *"<<" \t Error 2: "<<error_2;
			else
				cout<<"  Error 1: "<<error_1<<"   \t Error 2: "<<error_2<<" *";
		}
		else
		{
			cout<<"  La raiz es exacta";
		}
		cout<<endl;
		//Con el símbolo '*' doy a conocer de manera visual el resultado más 
		//pequeño de los errores, es decir, el método más preciso (entre 1 y 2)	
		
	}
	
	cout<<"  --------------------------------------------------------"<<endl;   
	
	if(!datos_ok)
	{
		cout<<"El valor introducido no era valido"<<endl;
	}
	
	return 0;
}
/* CONCLUSIONES:
	Con el primer método los resultados siempre son ligeramente superiores al 
	real mientras que por el segundo método los valores resultan ligeramente
	inferiores pero ninguno demuestra ser sistemáticamente más preciso que 
	otro.
	Un método más preciso sin duda sería la media de los resultados calculados
	por el método 1 y por el 2 ya que el error sería menor.
*/
