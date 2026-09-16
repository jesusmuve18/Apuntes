#include <iostream>	// Inclusión de los recursos E/S
#include <string>
using namespace std;

enum class TipoAjuste { izquierda, centro, derecha} ;

string FormateaString (string cad, int num_casillas, TipoAjuste ajuste, 
													char relleno)
{
	string nueva_cad="";
	string cad_relleno="";

	
	int casillas_relleno=(num_casillas - cad.length());
	//bool ajuste_correcto = true;
	//bool relleno_correcto = true;

	if(casillas_relleno>0)
	{
		for(int i=0; i<casillas_relleno; i++)
		{
			cad_relleno+=relleno;
		}	
	}
	
	
	switch (ajuste)
	{
		case TipoAjuste::izquierda :
			{

				nueva_cad=cad_relleno + cad;
				break;
			}
		case TipoAjuste::derecha :
			{

				nueva_cad=cad+cad_relleno;
				break;
			}
		case TipoAjuste::centro:
			for(int i=0;i<(cad_relleno.length()/2);i++)
			{
				nueva_cad+=cad_relleno.at(i);
			}
			nueva_cad+=cad;
			for(int i=(cad_relleno.length()/2);i<cad_relleno.length();i++)
			{
				nueva_cad+=cad_relleno.at(i);
			}
	}
	
	return nueva_cad;
}
int main()	// Programa principal
{
	
	
	cout<<FormateaString ("Maria",10,TipoAjuste::derecha, '0')<<endl;
	cout<<FormateaString("Alison", 10, TipoAjuste::centro, ' ')<<endl;
	cout<<FormateaString("Alison", 10, TipoAjuste::izquierda, '-')<<endl;
	
	return 0;
}
