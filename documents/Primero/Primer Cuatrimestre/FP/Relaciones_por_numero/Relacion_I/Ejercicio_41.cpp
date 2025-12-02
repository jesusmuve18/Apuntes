/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_I				EJERCICIO_41
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2

	Explicación del programa:
	Procesa el contenido de una cadena de caracteres extrayendo sus 
	componentes.
	
	Entradas: cadena de caracteres del índice
	
	Salidas: componentes de la cadena inicial:
				-sección
				-subsección
				-subsubsección
				-título
				-página
	
*/
/****************************************************************************/
#include<iostream>
#include<string.h>
#include<cmath>
using namespace std;
/****************************************************************************/
int main()
{
	
	string indice;
	
	/* Al introducir cada dato tipo string lo relleno con el número de 
	caracteres que tendrá al final y lo único que hago es ir sustituyendo
	cada char de las string por los elementos del índice que busco almacenar
	en cada variable*/
	
	string s_seccion="000";	
	string s_subseccion="000";	
	string s_subsubseccion="000";	
	string titulo="El.proceso.de.Compilacion...............";
	string s_pag="0000";
		
	//Entrada
	

	cout<<"Linea del indice del documento: "<<endl;
	
	//cout<<"..1..1..4El.proceso.de.Compilacion.................13"<<endl;
	//El comentario anterior lo puede usar de plantilla para que sea más 
	//sencillo escribir la línea del índice (quitándole la "//")
	
	getline(cin,indice);                				                     

	//Cálculos
	
	s_seccion.at(0)=indice.at(0);
	s_seccion.at(1)=indice.at(1);
	s_seccion.at(2)=indice.at(2);

	s_subseccion.at(0)=indice.at(3);
	s_subseccion.at(1)=indice.at(4);
	s_subseccion.at(2)=indice.at(5);
	
	s_subsubseccion.at(0)=indice.at(6);
	s_subsubseccion.at(1)=indice.at(7);
	s_subsubseccion.at(2)=indice.at(8);
	
	/*Para asignar el título normalmente usaría el siguiente código:
	
	for(int i=0;i<40;i++)
	{
		titulo.at(i)=indice.at(i+9);
	}
	
	Al no haber trabajado bucles de tipo for traduciré esa operación 
	escribiendo una por una sus operaciones.
	De igual forma se podría haber hecho para cada uno de los componentes
	del indice.*/
	
	titulo.at(0)=indice.at(9);
	titulo.at(1)=indice.at(10);
	titulo.at(2)=indice.at(11);	
	titulo.at(3)=indice.at(12);
	titulo.at(4)=indice.at(13);
	titulo.at(5)=indice.at(14);
	titulo.at(6)=indice.at(15);	
	titulo.at(7)=indice.at(16);
	titulo.at(8)=indice.at(17);
	titulo.at(9)=indice.at(18);
	titulo.at(10)=indice.at(19);	
	titulo.at(11)=indice.at(20);
	titulo.at(12)=indice.at(21);
	titulo.at(13)=indice.at(22);
	titulo.at(14)=indice.at(23);	
	titulo.at(15)=indice.at(24);
	titulo.at(16)=indice.at(25);
	titulo.at(17)=indice.at(26);
	titulo.at(18)=indice.at(27);	
	titulo.at(19)=indice.at(28);
	titulo.at(20)=indice.at(29);
	titulo.at(21)=indice.at(30);
	titulo.at(22)=indice.at(31);	
	titulo.at(23)=indice.at(32);
	titulo.at(24)=indice.at(33);
	titulo.at(25)=indice.at(34);
	titulo.at(26)=indice.at(35);	
	titulo.at(27)=indice.at(36);
	titulo.at(28)=indice.at(37);
	titulo.at(29)=indice.at(38);
	titulo.at(30)=indice.at(39);	
	titulo.at(31)=indice.at(40);
	titulo.at(32)=indice.at(41);
	titulo.at(33)=indice.at(42);
	titulo.at(34)=indice.at(43);	
	titulo.at(35)=indice.at(44);
	titulo.at(36)=indice.at(45);
	titulo.at(37)=indice.at(46);
	titulo.at(38)=indice.at(47);	
	titulo.at(39)=indice.at(48);
		
	s_pag.at(0)=indice.at(49);
	s_pag.at(1)=indice.at(50);
	s_pag.at(2)=indice.at(51);	
	s_pag.at(3)=indice.at(52);

	//Salidas
	
	cout<<endl;
	cout<<"Componentes: "<<endl;
	cout<<"\tSeccion:       "<<s_seccion<<endl;
	cout<<"\tSubseccion:    "<<s_subseccion<<endl;
	cout<<"\tSubsubseccion: "<<s_subsubseccion<<endl;
	cout<<"\tPagina:        "<<s_pag<<endl;
	cout<<"\tTitulo: "<<titulo<<endl;
	cout<<endl;
	cout<<"Linea procesada: "<<endl;
	cout<<s_seccion<<"."<<s_subseccion<<"."<<s_subsubseccion<<". "
		<<titulo<<" pag."<<s_pag;
	
	return 0;
}

/*Comentario extra
	Para escribir este programa más rapidamente hice otro programa de la 
	siguiente forma:
	
	for(int i=0;i<40;i++)
	{
		cout<<"titulo.at("<<i<<")=indice.at("<<i+9<<");"<<endl;
	}
	
	y así para cada uno de los componentes. Solo tuve que copiar el resultado
	y ponerlo en este programa.
*/










