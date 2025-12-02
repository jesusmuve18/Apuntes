//#define MOSTRAR_ERROR   	//Muestra la razón por la que no es válido el valor
							//Introducido en las funciones de lectura

/* Objeto que ofrece un conjunto de métodos seguros de lectura etiquetada de 
  datos int y double.
  
   Funciones incluidas:
  		• LeeReal 				(devuelve double)
  		• LeeRealEnRango 		(devuelve double)
  		• LeeRealMayorOIgual 	(devuelve double)
  		• LeeEntero 			(devuelve int)
  		• LeeEnteroEnRango 		(devuelve int)
  		• LeeEnteroMayorOIgual 	(devuelve int)
  		• SetTitulo 			(void)

   Construcctores:
		• De un parámetro: etiqueta (tipo string)
		• Sin parámetros
*/
class Lector
{
	private:
		
		string etiqueta;
	
	public:
		
		//Lee real devolviendo un valor double
		double LeeReal()
		{
			
			string lectura;
			string caracteres_inicio="+-1234567890.";
			string caracteres_resto="1234567890.";
			string caracteres_una_vez=".";
			
			bool todo_ok;
			
			
			do
			{
				do
				{
					cout<<etiqueta;
					getline(cin,lectura);
					
					string aux;
					
					for(int i=0; i<lectura.length(); i++) //Le quito los 
														  //espacios
					{
						if(lectura.at(i)!=' ')
						{
							aux+=lectura.at(i);
						}
					}
					
					lectura=aux;	
					
				} while(lectura.empty()); //Mientras no haya caracteres
			
				
				todo_ok=true;
				
				bool caracter_correcto;
				int siguiente_caracter=0;
				
				if(lectura.length()>1) //Si hay mas de un caracter
				{
					//Compruebo el primer caracter
		
					for(int i=0; i<caracteres_inicio.length(); i++)
					{
						if(lectura.at(0)==caracteres_inicio.at(i))
						{
							caracter_correcto=true;
						}
					}
					
					if(!caracter_correcto)
					{
						todo_ok=false;
						
						#ifdef MOSTRAR_ERROR
							
						cout<<"Caracter inicio incorrecto"<<endl;
						
						#endif
					}	
					
					siguiente_caracter=1;
				}
				
				
				//Compruebo el resto de caracteres
		
				
				for(int i=siguiente_caracter; i<lectura.length(); i++)
				{
					caracter_correcto=false;
					
					for(int j=0; j<caracteres_resto.length(); j++)
					{
						if(lectura.at(i)==caracteres_resto.at(j))
						{
							caracter_correcto=true;
						}
					}
					
					if(!caracter_correcto)
					{
						todo_ok=false;
						
						#ifdef MOSTRAR_ERROR
						
						cout<<"Caracter "<<i+1<<" incorrecto"<<endl;
						
						#endif
					}
					
				}	
				
				//Compruebo si hay caracteres repetidos que no deberían
				
				int contador=0;
				
				for(int j=0; j<caracteres_una_vez.length(); j++)
				{
					contador=0;
					
					for(int i=0; i<lectura.length(); i++)
					{
						if(lectura.at(i)==caracteres_resto.at(j))
						{
							contador++;
						}
					}
					
					if(contador>1)
					{
						todo_ok=false;
						
						#ifdef MOSTRAR_ERROR
						
						cout<<"Caracter "<<caracteres_una_vez.at(j)
							<<" repetido"<<endl;
						
						#endif
					}
					
				}	
				
				#ifdef MOSTRAR_ERROR
						
				cout<<endl;
				
				#endif
				
			} while(!todo_ok);
			
			return stod(lectura);
		}
	
		
		//Lee real en un rango dado en los parámetros
		double LeeRealEnRango (double num1, double num2)
		{
			double aux;
			double min=num1, max=num2;
			
			if(max<min)
			{
				max=num1;
				min=num2;
			}
			
			do
			{
				aux=LeeReal();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<min || aux>max)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<min || aux>max);
			
			return aux;
		}
		
		
		//Lee real mayor o igual a un parámetro dado
		double LeeRealMayorOIgual (double ref)
		{
			double aux;
			
			do
			{
				aux=LeeReal();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<ref)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<ref);
			
			return aux;
		}
		
		
		//Lee entero devolviendo un valor int
		int LeeEntero()
		{
			
			string lectura;
			string caracteres_inicio="+-1234567890";
			string caracteres_resto="1234567890";
			
			bool todo_ok;
		
		
			do
			{
				do
				{
					cout<<etiqueta;
					getline(cin,lectura);
					
					string aux;
					
					for(int i=0; i<lectura.length(); i++) //Le quito los 
														  //espacios
					{
						if(lectura.at(i)!=' ')
						{
							aux+=lectura.at(i);
						}
					}
					
					lectura=aux;	
					
				} while(lectura.empty()); //Mientras no haya caracteres
			
				
				todo_ok=true;
				
				bool caracter_correcto;
				int siguiente_caracter=0;
				
				if(lectura.length()>1) //Si hay mas de un caracter
				{
					//Compruebo el primer caracter
		
					for(int i=0; i<caracteres_inicio.length(); i++)
					{
						if(lectura.at(0)==caracteres_inicio.at(i))
						{
							caracter_correcto=true;
						}
					}
					
					if(!caracter_correcto)
					{
						todo_ok=false;
						
						#ifdef MOSTRAR_ERROR
							
						cout<<"Caracter inicio incorrecto"<<endl;
						
						#endif
					}	
					
					siguiente_caracter=1;
				}
				
				
				//Compruebo el resto de caracteres
		
				
				for(int i=siguiente_caracter; i<lectura.length(); i++)
				{
					caracter_correcto=false;
					
					for(int j=0; j<caracteres_resto.length(); j++)
					{
						if(lectura.at(i)==caracteres_resto.at(j))
						{
							caracter_correcto=true;
						}
					}
					
					if(!caracter_correcto)
					{
						todo_ok=false;
						
						#ifdef MOSTRAR_ERROR
						
						cout<<"Caracter "<<i+1<<" incorrecto"<<endl;
						
						#endif
					}
					
				}	
				
				#ifdef MOSTRAR_ERROR
						
				cout<<endl;
				
				#endif
				
			} while(!todo_ok);
			
			return stoi(lectura);
		}
		
		
		//Lee entero en un rango dado en los parámetros
		int LeeEnteroEnRango (int num1, int num2)
		{
			int aux;
			int min=num1, max=num2;
			
			if(max<min)
			{
				max=num1;
				min=num2;
			}
			
			do
			{
				aux=LeeEntero();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<min || aux>max)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<min || aux>max);
			
			return aux;
		}
		
		
		//Lee entero mayor o igual a un parámetro dado
		int LeeEnteroMayorOIgual (int ref)
		{
			int aux;
			
			do
			{
				aux=LeeEntero();
				
				#ifdef MOSTRAR_ERROR
				
				if(aux<ref)
				{
					cout<<"El valor introducido no se encuentra en el rango"
						<<endl;
				}
				
				#endif
				
			} while(aux<ref);
			
			return aux;
		}
		
		
		//Modificar Etiqueta
		void SetTitulo(string titulo)
		{
			etiqueta=titulo;
		}
		
		
		//Constructor de un parámetro
		Lector(string titulo): etiqueta(titulo) {};
		
		
		//Constructor sin parámetros
		Lector() {};

};

/****************************************************************************/

