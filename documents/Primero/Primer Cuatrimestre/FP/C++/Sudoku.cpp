/****************************************************************************/
//	FUNDAMENTOS DE PROGRAMACIÓN			RELACIÓN_III			EJERCICIO_1
/****************************************************************************/
/*
	Jesús Muñoz Velasco 					Grupo:A2
	
	Enunciado:
	
	Resolver un sudoku
	
	Entradas: 
	
	Salidas: 
	
*/
/****************************************************************************/
#include<iostream>
#include<string>
using namespace std;
/****************************************************************************/
void ComprobarCuadros(int sudoku[9][9][10])
{
	for(int x=0;x<9; x++) //pongo los cuadros donde haya un numero a 0
	{
		for(int y=0;y<9;y++)
		{
			if(sudoku[x][y][0]!=0)
			{
				int numero=sudoku[x][y][0];
				for(int i=(3*((int)(x/3))) ; i<(3*((int)(x/3)))+3;i++)
				{
					for(int j=(3*((int)(y/3))) ; j<(3*((int)(y/3)))+3;j++)
					{
						sudoku[i][j][numero]=0;
					}
				}	
			}			
		}
	}
}
/****************************************************************************/
void ComprobarColumnasFilas (int sudoku[9][9][10])
{
	for(int numero=1; numero<=9;numero++) //relleno de ceros las filas y columnas donde esta el numero
	{
		for(int x=0; x<9;x++)
		{
			for(int y=0; y<9; y++)
			{
				if(sudoku[x][y][0]==numero)
				{
					for(int i=0; i<9;i++)
					{
						sudoku[i][y][numero]=0;
						sudoku[x][i][numero]=0;
					}
				}
			}
		}
	}
}
/****************************************************************************/
void MostrarMatriz(int sudoku[9][9][10])
{
	for(int k=0; k<10; k++) //Muestro la matiz tridimensional con cada posibilidad
	{
		for(int i=0; i<9; i++) //Muestro la matiz tridimensional 
		{
			for (int j=0; j<9; j++)
			{
				cout<<sudoku[i][j][k];
				if((j+1)%3==0)
				{
					cout<<" ";
				}
				
			}
			
			if((i+1)%3==0)
			{
				cout<<endl;;
			}	
			cout<<endl;
		}
	cout<<".............."<<endl;
	}
}
bool ComprobarVictoria(int sudoku[9][9][10])
{
	bool terminar=true;
	for(int x=0; x<9; x++)
	{
		for(int y=0; y<9; x++)
		{
			if(sudoku[x][y][0]==0)
			{
				terminar=false;
			}
		}
	}
	return terminar;
}
/****************************************************************************/
void RellenarCuadros(int sudoku[9][9][10])
{
	for(int num=1; num<=9;num++)
	{
		for(int x=0;x<9; x++) //pongo los cuadros donde haya un numero a 0
		{
			for(int y=0;y<9;y++)
			{
				if(sudoku[x][y][num]!=0)
				{
					int numero=sudoku[x][y][num];
					int contador=0;
					
					for(int i=(3*((int)(x/3))) ; i<(3*((int)(x/3)))+3;i++)
					{
						for(int j=(3*((int)(y/3))) ; j<(3*((int)(y/3)))+3;j++)
						{	
							
							if(sudoku[i][j][numero]!=0)
							{
								contador++;
							}
						}
					}
					if(contador==1)
					{
						sudoku[x][y][0]=sudoku[x][y][num];
						sudoku[x][y][num]=0;
					}	
				}			
			}
		}	
	}
}
/****************************************************************************/
void MostrarSudoku(int sudoku[9][9][10])
{
	for(int i=0; i<9; i++) //Muestro la matiz tridimensional 
	{
		for (int j=0; j<9; j++)
		{
			cout<<sudoku[i][j][0];
			if((j+1)%3==0)
			{
				cout<<" ";
			}
			
		}
		
		if((i+1)%3==0)
		{
			cout<<endl;;
		}	
		cout<<endl;
	}
	cout<<".............."<<endl;
}
/****************************************************************************/
void RellenarFilas(int sudoku[9][9][10])
{
	for(int numero=1; numero<=9;numero++) //Si en una fila hay un único valor
	{
		for(int x=0; x<9;x++)
		{
			for(int y=0; y<9; y++)
			{
				if(sudoku[x][y][numero]==numero)
				{
					int contador=0;
					for(int i=0; i<9;i++)
					{
						if(sudoku[i][y][numero]==0)
						{
							contador++;
						}
					}
					if(contador==1)
					{
						sudoku[x][y][0]=sudoku[x][y][numero];
						sudoku[x][y][numero]=0;
					}
				}
			}
		}
	}
}


void RellenarColumnas(int sudoku[9][9][10])
{
	for(int numero=1; numero<=9;numero++) //Si en una columna hay un único valor
	{
		for(int x=0; x<9;x++)
		{
			for(int y=0; y<9; y++)
			{
				if(sudoku[x][y][numero]==numero)
				{
					int contador=0;
					for(int i=0; i<9;i++)
					{
						if(sudoku[x][i][numero]==0)
						{
							contador++;
						}
					}
					if(contador==1)
					{
						sudoku[x][y][0]=sudoku[x][y][numero];
						sudoku[x][y][numero]=0;
					}
				}
			}
		}
	}
}
/****************************************************************************/
void RellenarUnicidad(int sudoku[9][9][10])
{
	int contador;
	for(int z=1; z<10; z++)
	{
		for(int x=0; x<9; x++)
		{
			for(int y=0; y<9;y++)
			{
				if(sudoku[x][y][z]!=0)
				{
					contador=0;
					for(int i=1; i<10; i++)
					{
						if(sudoku[x][y][i]!=0)
						{
							contador++;
						}
					}
					if(contador==1)
					{
						sudoku[x][y][0]=sudoku[x][y][z];
						sudoku[x][y][z]=0;
					}
				}
			}
		}
	}
}
/****************************************************************************/
/****************************************************************************/
int main()
{
	
	//Entradas
	
	int sudoku[9][9][10]; //x, y, z
	int huecos_blanco;
	bool posibilidad;
	
	string str_sudoku_inicial[9];
	
	str_sudoku_inicial[0]="700 000 508";
	str_sudoku_inicial[1]="105 062 000";
	str_sudoku_inicial[2]="000 004 100";
	
	str_sudoku_inicial[3]="000 008 076";
	str_sudoku_inicial[4]="500 017 300";
	str_sudoku_inicial[5]="208 000 000";
	
	str_sudoku_inicial[6]="300 020 000";
	str_sudoku_inicial[7]="000 000 610";
	str_sudoku_inicial[8]="012 470 000";
	
	for(int i=0; i<9;i++) //Relleno la matriz tridimensional con el sudoku inicial
	{
		huecos_blanco=0;
		for(int j=0;j<11;j++)
		{
			if(str_sudoku_inicial[i].at(j)!=' ')
			{
				sudoku[i][j-huecos_blanco][0]=(str_sudoku_inicial[i].at(j)-'0');
			}
			else
			{
				huecos_blanco++;
			}
		}
	}
	
	for(int i=0; i<9; i++) //Muestro la matiz tridimensional 
	{
		for (int j=0; j<9; j++)
		{
			cout<<sudoku[i][j][0];
			if((j+1)%3==0)
			{
				cout<<" ";
			}
			
		}
		
		if((i+1)%3==0)
		{
			cout<<endl;;
		}	
		cout<<endl;
	}
	cout<<".............."<<endl;
	
	for(int x=0;x<9;x++) //Relleno las matrices numerales auxiliares
	{
		for(int y=0; y<9; y++)
		{
			for(int z=1;z<10;z++)
			{
				if(sudoku[x][y][0]==0)
				{
					sudoku[x][y][z]=z;	
				}
				else
				{
					sudoku[x][y][z]=0;
				}
			}
		}
	}
	int a=1;
	while(a!=0)
	{
		ComprobarColumnasFilas(sudoku);
		ComprobarCuadros(sudoku);
		MostrarMatriz(sudoku);
		RellenarCuadros(sudoku);
		RellenarFilas(sudoku);
		RellenarColumnas(sudoku);
		RellenarUnicidad(sudoku);
		MostrarSudoku(sudoku);
		cin>>a;
		system("cls");
	}
	
	
		

	
	
	
	
	
	return 0;
}
