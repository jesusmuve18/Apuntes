#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0(sensores);
		break;
	case 1:
		accion = ComportamientoAuxiliarNivel_1(sensores);
		break;
	case 2:
		accion = ComportamientoAuxiliarNivel_2(sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3(sensores);
		// accion = ComportamientoAuxiliarNivel_E (sensores);
		break;
	case 4:
		accion = ComportamientoAuxiliarNivel_4(sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

/********************************************************
 *   _   _ _           _              ___            _   *
 *  | \ | (_)_   _____| | ___  ___   / _ \   _   _  / |  *
 *  |  \| | \ \ / / _ \ |/ _ \/ __| | | | | | | | | | |  *
 *  | |\  | |\ V /  __/ |  __/\__ \ | |_| | | |_| | | |  *
 *  |_| \_|_| \_/ \___|_|\___||___/  \___/   \__, | |_|  *
 *                                           |___/       *
 ********************************************************/

// Comprueba si es viable por
// Altura
char ViableA(char casilla, int dif)
{
	if (abs(dif) < 2)
		return casilla;
	else
		return 'P';
}

// Dice si una casilla es transitable
bool TransitableA(char casilla, Sensores sensores, bool zap)
{
	switch (sensores.nivel)
	{
	case 0:
		if (casilla == 'C' or casilla == 'D')
			return true;
		else
			return false;
		break;
	case 1:
		if (casilla == 'C' or casilla == 'D' or casilla == 'S' or casilla == 'X' or (zap && casilla == 'B'))
			return true;
		else
			return false;
		break;
	case 2:
		if (casilla == 'C' or casilla == 'D')
			return true;
		else
			return false;
		break;
	case 3:
		if (casilla == 'C' or casilla == 'D')
			return true;
		else
			return false;
		break;
	case 4:
		if (casilla == 'C' or casilla == 'D')
			return true;
		else
			return false;
		break;
	default:
		return false;
	}
}

// Indica las veces que se ha pisado una casilla del campo de visión
// pre: 0 <= casilla <= 15
int VecesPisadaA(int casilla, Sensores sensores, vector<vector<int>> vp)
{

	const int ERROR = -1;

	if (casilla == 0)
		return vp[sensores.posF][sensores.posC];

	switch (sensores.rumbo)
	{
	case norte:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF - 1][sensores.posC - 1];
			break;
		case 2:
			return vp[sensores.posF - 1][sensores.posC];
			break;
		case 3:
			return vp[sensores.posF - 1][sensores.posC + 1];
			break;
		case 4:
			return vp[sensores.posF - 2][sensores.posC - 2];
			break;
		case 5:
			return vp[sensores.posF - 2][sensores.posC - 1];
			break;
		case 6:
			return vp[sensores.posF - 2][sensores.posC];
			break;
		case 7:
			return vp[sensores.posF - 2][sensores.posC + 1];
			break;
		case 8:
			return vp[sensores.posF - 2][sensores.posC + 2];
			break;
		default:
			return ERROR;
		}
		break;
	case noreste:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF - 1][sensores.posC];
			break;
		case 2:
			return vp[sensores.posF - 1][sensores.posC + 1];
			break;
		case 3:
			return vp[sensores.posF][sensores.posC + 1];
			break;
		case 4:
			return vp[sensores.posF - 2][sensores.posC];
			break;
		case 5:
			return vp[sensores.posF - 2][sensores.posC + 1];
			break;
		case 6:
			return vp[sensores.posF - 2][sensores.posC + 2];
			break;
		case 7:
			return vp[sensores.posF - 1][sensores.posC + 2];
			break;
		case 8:
			return vp[sensores.posF][sensores.posC + 2];
			break;
		default:
			return ERROR;
		}
		break;
	case este:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF - 1][sensores.posC + 1];
			break;
		case 2:
			return vp[sensores.posF][sensores.posC + 1];
			break;
		case 3:
			return vp[sensores.posF + 1][sensores.posC + 1];
			break;
		case 4:
			return vp[sensores.posF - 2][sensores.posC + 2];
			break;
		case 5:
			return vp[sensores.posF - 1][sensores.posC + 2];
			break;
		case 6:
			return vp[sensores.posF][sensores.posC + 2];
			break;
		case 7:
			return vp[sensores.posF + 1][sensores.posC + 2];
			break;
		case 8:
			return vp[sensores.posF + 2][sensores.posC + 2];
			break;
		default:
			return ERROR;
		}
		break;
	case sureste:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF][sensores.posC + 1];
			break;
		case 2:
			return vp[sensores.posF + 1][sensores.posC + 1];
			break;
		case 3:
			return vp[sensores.posF + 1][sensores.posC];
			break;
		case 4:
			return vp[sensores.posF][sensores.posC + 2];
			break;
		case 5:
			return vp[sensores.posF + 1][sensores.posC + 2];
			break;
		case 6:
			return vp[sensores.posF + 2][sensores.posC + 2];
			break;
		case 7:
			return vp[sensores.posF + 2][sensores.posC + 1];
			break;
		case 8:
			return vp[sensores.posF + 2][sensores.posC];
			break;
		default:
			return ERROR;
		}
		break;
	case sur:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF + 1][sensores.posC + 1];
			break;
		case 2:
			return vp[sensores.posF + 1][sensores.posC];
			break;
		case 3:
			return vp[sensores.posF + 1][sensores.posC - 1];
			break;
		case 4:
			return vp[sensores.posF + 2][sensores.posC + 2];
			break;
		case 5:
			return vp[sensores.posF + 2][sensores.posC + 1];
			break;
		case 6:
			return vp[sensores.posF + 2][sensores.posC];
			break;
		case 7:
			return vp[sensores.posF - 2][sensores.posC - 1];
			break;
		case 8:
			return vp[sensores.posF + 2][sensores.posC - 2];
			break;
		default:
			return ERROR;
		}
		break;
	case suroeste:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF + 1][sensores.posC];
			break;
		case 2:
			return vp[sensores.posF + 1][sensores.posC - 1];
			break;
		case 3:
			return vp[sensores.posF][sensores.posC - 1];
			break;
		case 4:
			return vp[sensores.posF + 2][sensores.posC];
			break;
		case 5:
			return vp[sensores.posF + 2][sensores.posC - 1];
			break;
		case 6:
			return vp[sensores.posF + 2][sensores.posC - 2];
			break;
		case 7:
			return vp[sensores.posF + 2][sensores.posC - 2];
			break;
		case 8:
			return vp[sensores.posF][sensores.posC - 2];
			break;
		default:
			return ERROR;
		}
		break;
	case oeste:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF + 1][sensores.posC - 1];
			break;
		case 2:
			return vp[sensores.posF][sensores.posC - 1];
			break;
		case 3:
			return vp[sensores.posF - 1][sensores.posC - 1];
			break;
		case 4:
			return vp[sensores.posF + 2][sensores.posC - 2];
			break;
		case 5:
			return vp[sensores.posF + 1][sensores.posC - 2];
			break;
		case 6:
			return vp[sensores.posF][sensores.posC - 2];
			break;
		case 7:
			return vp[sensores.posF - 1][sensores.posC - 2];
			break;
		case 8:
			return vp[sensores.posF - 2][sensores.posC - 2];
			break;
		default:
			return ERROR;
		}
		break;
	case noroeste:
		switch (casilla)
		{
		case 1:
			return vp[sensores.posF][sensores.posC - 1];
			break;
		case 2:
			return vp[sensores.posF - 1][sensores.posC - 1];
			break;
		case 3:
			return vp[sensores.posF - 1][sensores.posC];
			break;
		case 4:
			return vp[sensores.posF][sensores.posC - 2];
			break;
		case 5:
			return vp[sensores.posF - 1][sensores.posC - 2];
			break;
		case 6:
			return vp[sensores.posF - 2][sensores.posC - 2];
			break;
		case 7:
			return vp[sensores.posF - 2][sensores.posC - 1];
			break;
		case 8:
			return vp[sensores.posF - 2][sensores.posC];
			break;
		default:
			return ERROR;
		}
		break;
	default:
		return ERROR;
	}
}

/**
 * Devuelve una decisión basándose en los parámetros
 * @param condicion Condición que establece a donde se puede ir
 * @param sup vector que indica el tipo de superficie del campo de visión
 * @param vp  vector que indica las veces que se ha pisado cada casilla del campo de visión
 * @param orientacion 	0 si se está mirando al norte, sur, este u oeste;
 * 						1 si se está mirando al noreste, sureste, suroeste o noroeste
 * @param sensores sensores del agente
 */
int ComportamientoAuxiliar::TomarDecision(bool (*condicion)(char, bool), vector<char> sup, vector<int> vp, int orientacion, Sensores sensores)
{

	// Orientación: Número que indica el rumbo que sigue el personaje
	//				- : Cualquiera
	//				0 : Norte, Sur, Este y Oeste 				(orientacion%2==0)
	//				1 : Noreste, sureste, suroeste, noroeste 	(orientacion%2!=0)
	//
	// Destino: Casilla a la que se pretende ir
	//
	// Ruta: Casillas por las que vamos a pasar para llegar a la casilla
	//
	// Las Casillas son:
	/*
		-----------------------------
		|   |   |   |   |   |   |   |
		-----------------------------
			| 4 | 5 | 6 | 7 | 8 |
			---------------------
				| 1 | 2 | 3 |
				-------------
					| 0 |
					-----
	*/

	vector<int> posibles; // Posibles decisiones que puedo tomar (hay camino)
	vector<int> pisada;
	vector<int> giros; // Veces que necesito girar tras tomar la decisión (positivo para derecha negativo para izquierda)

	const bool DEBUG = false;

	if (DEBUG)
	{
		cout << "-----------------------------------------------------------------------------" << endl;
		// cout<<"La casilla 6: "<<condicion(sup.at(6))<<":"<<sup.at(6)<<"y 2:"<<sup.at(2)<<endl;
		cout << "Veces pisada (Orientación: " << sensores.rumbo << "): ";
		for (int i = 0; i < vp.size(); i++)
		{
			cout << vp.at(i) << " ";
		}
		cout << endl;

		cout << "Superficie   (Orientación: " << sensores.rumbo << "): ";
		for (int i = 0; i < sup.size(); i++)
		{
			cout << sup.at(i) << " ";
		}
		cout << endl;
	}

	// Ir a casilla 6 (a)
	if (condicion(sup.at(6), tiene_zapatillas))
	{
		if (TransitableA(sup.at(2), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(6), sensores.cota[6] - sensores.cota[2]), sensores, tiene_zapatillas))
		{
			posibles.push_back(2);
			pisada.push_back(vp.at(6));
			giros.push_back(0);
		} // Orientación: -	Ruta: 2
	}

	// Ir a casilla 2 (c) Directa
	if (condicion(sup.at(2), tiene_zapatillas))
	{
		posibles.push_back(2);
		pisada.push_back(vp.at(2));
		giros.push_back(0);
	} // Orientación: -	Destino: 2 	Ruta: Directa

	// Ir a casilla 8 (add)
	if (condicion(sup.at(8), tiene_zapatillas))
	{
		if (TransitableA(sup.at(3), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(8), sensores.cota[8] - sensores.cota[3]), sensores, tiene_zapatillas))
		{
			posibles.push_back(3);
			pisada.push_back(vp.at(8));
			giros.push_back(0);
		} // Orientación: -	Destino: 8	Ruta: 3
		else if (orientacion == 1 and TransitableA(sup.at(2), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(8), sensores.cota[8] - sensores.cota[2]), sensores, tiene_zapatillas))
		{
			posibles.push_back(2);
			pisada.push_back(vp.at(8));
			giros.push_back(2);
		} // Orientación: 1	Destino: 8	Ruta: 2
	}

	// Ir a casilla 4 (aii)
	if (condicion(sup.at(4), tiene_zapatillas))
	{
		if (TransitableA(sup.at(1), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(4), sensores.cota[4] - sensores.cota[1]), sensores, tiene_zapatillas))
		{
			posibles.push_back(1);
			pisada.push_back(vp.at(4));
			giros.push_back(0);
		} // Orientación: -	Ruta: 1
		else if (orientacion == 1 and TransitableA(sup.at(2), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(4), sensores.cota[4] - sensores.cota[2]), sensores, tiene_zapatillas))
		{
			posibles.push_back(2);
			pisada.push_back(vp.at(4));
			giros.push_back(6);
		} // Orientación: 1	Ruta 2
	}

	// Ir a casilla 7 (ad)
	if (condicion(sup.at(7), tiene_zapatillas))
	{
		if (TransitableA(sup.at(2), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(7), sensores.cota[7] - sensores.cota[2]), sensores, tiene_zapatillas))
		{
			posibles.push_back(2);
			pisada.push_back(vp.at(7));
			giros.push_back(1);
		} // Orientación: -	Destino: 7	Ruta: 2
		else if (TransitableA(sup.at(3), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(7), sensores.cota[7] - sensores.cota[3]), sensores, tiene_zapatillas))
		{
			posibles.push_back(3);
			pisada.push_back(vp.at(7));
			giros.push_back(0);
		} // Orientación: -	Destino: 7	Ruta: 3
	}

	// Ir a casilla 5 (ai)
	if (condicion(sup.at(5), tiene_zapatillas) and TransitableA(sup.at(2), sensores, tiene_zapatillas) and TransitableA(ViableA(sup.at(5), sensores.cota[5] - sensores.cota[2]), sensores, tiene_zapatillas))
	{
		posibles.push_back(2);
		pisada.push_back(vp.at(5));
		giros.push_back(0);
	} // Orientación: -	Ruta: 2

	// Ir a casilla 3 (d) Directa
	if (condicion(sup.at(3), tiene_zapatillas))
	{
		posibles.push_back(3);
		pisada.push_back(vp.at(3));
		giros.push_back(0);
	} // Orientación: -   Destino: 3	Ruta: Directa

	// Ir a casilla 1 (i) Directa
	if (condicion(sup.at(1), tiene_zapatillas))
	{
		posibles.push_back(1);
		pisada.push_back(vp.at(1));
		giros.push_back(0);
	} // Orientación: -	Destino: 1	Ruta: Directa

	if (posibles.size() == 0)
	{
		return 0;
	}
	else
	{
		int pos_min = 0;
		int min = pisada.at(0);

		for (int i = 1; i < pisada.size(); i++)
		{
			if (pisada.at(i) < min)
			{
				pos_min = i;
				min = pisada.at(i);
			}
		}

		// cout << "[Rescatador] Movimientos posibles: ";

		// for(int i=0; i<posibles.size(); i++){
		// 	cout<<" ("<<posibles.at(i)<<", "<<pisada.at(i)<<", "<<giros.at(i)<<") ";
		// }
		// cout<<" . Decision: "<<posibles.at(pos_min)<<"ª "<<TransitableAA(sup.at(2), sensores)<<endl;

		giro45izq = giros.at(pos_min);

		// cout<<posibles.at(pos_min)<<":";
		// cout<<giro45izq<<endl;

		if (DEBUG)
			cout << "Elijo la " << pos_min << endl;

		return posibles.at(pos_min);
	}
}

// Devuelve la decisión tomada según las casillas transitables que se vean
int ComportamientoAuxiliar::VeoCasillaTransitable(vector<char> sup, Sensores sensores, bool (*condicion)(char, bool))
{

	const int SIZE = 16;
	const int DEFAULT_VP = 3000;

	vector<int> vp(SIZE, DEFAULT_VP); // Vector que indica las veces que se ha pisado una casilla vista

	// Relleno el vector de veces pisada
	for (int i = 0; i < SIZE; i++)
	{
		vp.at(i) = VecesPisadaA(i, sensores, veces_pisada);
	}

	int orientacion = (sensores.rumbo % 2); // Si estamos en diagonal o no

	if (sensores.nivel == 0)
	{ // En el nivel 0 buscamos la casilla de salida
		int res = TomarDecision([](char c, bool zap)
								{ return c == 'X'; }, sup, vp, orientacion, sensores);
		if (res != 0)
			return res;
	}

	if (!tiene_zapatillas)
	{ // Buscamos las zapatillas
		int res = TomarDecision([](char c, bool zap)
								{ return c == 'D'; }, sup, vp, orientacion, sensores);
		if (res != 0)
			return res;
	}

	// No voy ni a casilla de salida ni a por zapatillas
	// Entonces voy a tomar de las decisiones posibles la que menos veces haya pisado
	// En caso de empate se seguirá la preferencia

	return TomarDecision(condicion, sup, vp, orientacion, sensores);
}

// Dado un mapa, se asigna la superficie visible a sus coordenadas correspondientes
template <typename T>
void SituarSensorEnMapaA(vector<vector<T>> &m, int posF, int posC, vector<T> &a, int rumbo)
{
	m[posF][posC] = a[0];

	switch (rumbo)
	{
	case norte:
		m[posF - 1][posC - 1] = a[1];
		m[posF - 1][posC] = a[2];
		m[posF - 1][posC + 1] = a[3];
		m[posF - 2][posC - 2] = a[4];
		m[posF - 2][posC - 1] = a[5];
		m[posF - 2][posC] = a[6];
		m[posF - 2][posC + 1] = a[7];
		m[posF - 2][posC + 2] = a[8];
		m[posF - 3][posC - 3] = a[9];
		m[posF - 3][posC - 2] = a[10];
		m[posF - 3][posC - 1] = a[11];
		m[posF - 3][posC] = a[12];
		m[posF - 3][posC + 1] = a[13];
		m[posF - 3][posC + 2] = a[14];
		m[posF - 3][posC + 3] = a[15];
		break;
	case noreste:
		m[posF][posC + 1] = a[3];
		m[posF][posC + 2] = a[8];
		m[posF][posC + 3] = a[15];
		m[posF - 1][posC] = a[1];
		m[posF - 1][posC + 1] = a[2];
		m[posF - 1][posC + 2] = a[7];
		m[posF - 1][posC + 3] = a[14];
		m[posF - 2][posC] = a[4];
		m[posF - 2][posC + 1] = a[5];
		m[posF - 2][posC + 2] = a[6];
		m[posF - 2][posC + 3] = a[13];
		m[posF - 3][posC] = a[9];
		m[posF - 3][posC + 1] = a[10];
		m[posF - 3][posC + 2] = a[11];
		m[posF - 3][posC + 3] = a[12];
		break;
	case este:
		m[posF - 1][posC + 1] = a[1];
		m[posF][posC + 1] = a[2];
		m[posF + 1][posC + 1] = a[3];
		m[posF - 2][posC + 2] = a[4];
		m[posF - 1][posC + 2] = a[5];
		m[posF][posC + 2] = a[6];
		m[posF + 1][posC + 2] = a[7];
		m[posF + 2][posC + 2] = a[8];
		m[posF - 3][posC + 3] = a[9];
		m[posF - 2][posC + 3] = a[10];
		m[posF - 1][posC + 3] = a[11];
		m[posF][posC + 3] = a[12];
		m[posF + 1][posC + 3] = a[13];
		m[posF + 2][posC + 3] = a[14];
		m[posF + 3][posC + 3] = a[15];
		break;
	case sureste:
		m[posF][posC + 1] = a[1];
		m[posF][posC + 2] = a[4];
		m[posF][posC + 3] = a[9];
		m[posF + 1][posC] = a[3];
		m[posF + 1][posC + 1] = a[2];
		m[posF + 1][posC + 2] = a[5];
		m[posF + 1][posC + 3] = a[10];
		m[posF + 2][posC] = a[8];
		m[posF + 2][posC + 1] = a[7];
		m[posF + 2][posC + 2] = a[6];
		m[posF + 2][posC + 3] = a[11];
		m[posF + 3][posC] = a[15];
		m[posF + 3][posC + 1] = a[14];
		m[posF + 3][posC + 2] = a[13];
		m[posF + 3][posC + 3] = a[12];
		break;
	case sur:
		m[posF + 1][posC - 1] = a[3];
		m[posF + 1][posC] = a[2];
		m[posF + 1][posC + 1] = a[1];
		m[posF + 2][posC - 2] = a[8];
		m[posF + 2][posC - 1] = a[7];
		m[posF + 2][posC] = a[6];
		m[posF + 2][posC + 1] = a[5];
		m[posF + 2][posC + 2] = a[4];
		m[posF + 3][posC - 3] = a[15];
		m[posF + 3][posC - 2] = a[14];
		m[posF + 3][posC - 1] = a[13];
		m[posF + 3][posC] = a[12];
		m[posF + 3][posC + 1] = a[11];
		m[posF + 3][posC + 2] = a[10];
		m[posF + 3][posC + 3] = a[9];
		break;
	case suroeste:
		m[posF][posC - 1] = a[3];
		m[posF][posC - 2] = a[8];
		m[posF][posC - 3] = a[15];
		m[posF + 1][posC] = a[1];
		m[posF + 1][posC - 1] = a[2];
		m[posF + 1][posC - 2] = a[7];
		m[posF + 1][posC - 3] = a[14];
		m[posF + 2][posC] = a[4];
		m[posF + 2][posC - 1] = a[5];
		m[posF + 2][posC - 2] = a[6];
		m[posF + 2][posC - 3] = a[13];
		m[posF + 3][posC] = a[9];
		m[posF + 3][posC - 1] = a[10];
		m[posF + 3][posC - 2] = a[11];
		m[posF + 3][posC - 3] = a[12];
		break;
	case oeste:
		m[posF - 1][posC - 1] = a[3];
		m[posF][posC - 1] = a[2];
		m[posF + 1][posC - 1] = a[1];
		m[posF - 2][posC - 2] = a[8];
		m[posF - 1][posC - 2] = a[7];
		m[posF][posC - 2] = a[6];
		m[posF + 1][posC - 2] = a[5];
		m[posF + 2][posC - 2] = a[4];
		m[posF - 3][posC - 3] = a[15];
		m[posF - 2][posC - 3] = a[14];
		m[posF - 1][posC - 3] = a[13];
		m[posF][posC - 3] = a[12];
		m[posF + 1][posC - 3] = a[11];
		m[posF + 2][posC - 3] = a[10];
		m[posF + 3][posC - 3] = a[9];
		break;
	case noroeste:
		m[posF][posC - 1] = a[1];
		m[posF][posC - 2] = a[4];
		m[posF][posC - 3] = a[9];
		m[posF - 1][posC] = a[3];
		m[posF - 1][posC - 1] = a[2];
		m[posF - 1][posC - 2] = a[5];
		m[posF - 1][posC - 3] = a[10];
		m[posF - 2][posC] = a[8];
		m[posF - 2][posC - 1] = a[7];
		m[posF - 2][posC - 2] = a[6];
		m[posF - 2][posC - 3] = a[11];
		m[posF - 3][posC] = a[15];
		m[posF - 3][posC - 1] = a[14];
		m[posF - 3][posC - 2] = a[13];
		m[posF - 3][posC - 3] = a[12];
		break;
	}
}

// Dada una superficie visible, se asigna el mapa a sus coordenadas correspondientes
template <typename T>
void SituarMapaEnSensorA(vector<vector<T>> &m, int posF, int posC, vector<T> &a, int rumbo)
{
	a[0] = m[posF][posC];

	switch (rumbo)
	{
	case norte:
		a[1] = m[posF - 1][posC - 1];
		a[2] = m[posF - 1][posC];
		a[3] = m[posF - 1][posC + 1];
		a[4] = m[posF - 2][posC - 2];
		a[5] = m[posF - 2][posC - 1];
		a[6] = m[posF - 2][posC];
		a[7] = m[posF - 2][posC + 1];
		a[8] = m[posF - 2][posC + 2];
		a[9] = m[posF - 3][posC - 3];
		a[10] = m[posF - 3][posC - 2];
		a[11] = m[posF - 3][posC - 1];
		a[12] = m[posF - 3][posC];
		a[13] = m[posF - 3][posC + 1];
		a[14] = m[posF - 3][posC + 2];
		a[15] = m[posF - 3][posC + 3];
		break;
	case noreste:
		a[3] = m[posF][posC + 1];
		a[8] = m[posF][posC + 2];
		a[15] = m[posF][posC + 3];
		a[1] = m[posF - 1][posC];
		a[2] = m[posF - 1][posC + 1];
		a[7] = m[posF - 1][posC + 2];
		a[14] = m[posF - 1][posC + 3];
		a[4] = m[posF - 2][posC];
		a[5] = m[posF - 2][posC + 1];
		a[6] = m[posF - 2][posC + 2];
		a[13] = m[posF - 2][posC + 3];
		a[9] = m[posF - 3][posC];
		a[10] = m[posF - 3][posC + 1];
		a[11] = m[posF - 3][posC + 2];
		a[12] = m[posF - 3][posC + 3];
		break;
	case este:
		a[1] = m[posF - 1][posC + 1];
		a[2] = m[posF][posC + 1];
		a[3] = m[posF + 1][posC + 1];
		a[4] = m[posF - 2][posC + 2];
		a[5] = m[posF - 1][posC + 2];
		a[6] = m[posF][posC + 2];
		a[7] = m[posF + 1][posC + 2];
		a[8] = m[posF + 2][posC + 2];
		a[9] = m[posF - 3][posC + 3];
		a[10] = m[posF - 2][posC + 3];
		a[11] = m[posF - 1][posC + 3];
		a[12] = m[posF][posC + 3];
		a[13] = m[posF + 1][posC + 3];
		a[14] = m[posF + 2][posC + 3];
		a[15] = m[posF + 3][posC + 3];
		break;
	case sureste:
		a[1] = m[posF][posC + 1];
		a[4] = m[posF][posC + 2];
		a[9] = m[posF][posC + 3];
		a[3] = m[posF + 1][posC];
		a[2] = m[posF + 1][posC + 1];
		a[5] = m[posF + 1][posC + 2];
		a[10] = m[posF + 1][posC + 3];
		a[8] = m[posF + 2][posC];
		a[7] = m[posF + 2][posC + 1];
		a[6] = m[posF + 2][posC + 2];
		a[11] = m[posF + 2][posC + 3];
		a[15] = m[posF + 3][posC];
		a[14] = m[posF + 3][posC + 1];
		a[13] = m[posF + 3][posC + 2];
		a[12] = m[posF + 3][posC + 3];
		break;
	case sur:
		a[3] = m[posF + 1][posC - 1];
		a[2] = m[posF + 1][posC];
		a[1] = m[posF + 1][posC + 1];
		a[8] = m[posF + 2][posC - 2];
		a[7] = m[posF + 2][posC - 1];
		a[6] = m[posF + 2][posC];
		a[5] = m[posF + 2][posC + 1];
		a[4] = m[posF + 2][posC + 2];
		a[15] = m[posF + 3][posC - 3];
		a[14] = m[posF + 3][posC - 2];
		a[13] = m[posF + 3][posC - 1];
		a[12] = m[posF + 3][posC];
		a[11] = m[posF + 3][posC + 1];
		a[10] = m[posF + 3][posC + 2];
		a[9] = m[posF + 3][posC + 3];
		break;
	case suroeste:
		a[3] = m[posF][posC - 1];
		a[8] = m[posF][posC - 2];
		a[15] = m[posF][posC - 3];
		a[11] = m[posF + 1][posC];
		a[2] = m[posF + 1][posC - 1];
		a[7] = m[posF + 1][posC - 2];
		a[14] = m[posF + 1][posC - 3];
		a[4] = m[posF + 2][posC];
		a[5] = m[posF + 2][posC - 1];
		a[6] = m[posF + 2][posC - 2];
		a[13] = m[posF + 2][posC - 3];
		a[9] = m[posF + 3][posC];
		a[10] = m[posF + 3][posC - 1];
		a[11] = m[posF + 3][posC - 2];
		a[12] = m[posF + 3][posC - 3];
		break;
	case oeste:
		a[3] = m[posF - 1][posC - 1];
		a[2] = m[posF][posC - 1];
		a[1] = m[posF + 1][posC - 1];
		a[8] = m[posF - 2][posC - 2];
		a[7] = m[posF - 1][posC - 2];
		a[6] = m[posF][posC - 2];
		a[5] = m[posF + 1][posC - 2];
		a[4] = m[posF + 2][posC - 2];
		a[15] = m[posF - 3][posC - 3];
		a[14] = m[posF - 2][posC - 3];
		a[13] = m[posF - 1][posC - 3];
		a[12] = m[posF][posC - 3];
		a[11] = m[posF + 1][posC - 3];
		a[10] = m[posF + 2][posC - 3];
		a[9] = m[posF + 3][posC - 3];
		break;
	case noroeste:
		a[1] = m[posF][posC - 1];
		a[4] = m[posF][posC - 2];
		a[9] = m[posF][posC - 3];
		a[3] = m[posF - 1][posC];
		a[2] = m[posF - 1][posC - 1];
		a[5] = m[posF - 1][posC - 2];
		a[10] = m[posF - 1][posC - 3];
		a[8] = m[posF - 2][posC];
		a[7] = m[posF - 2][posC - 1];
		a[6] = m[posF - 2][posC - 2];
		a[11] = m[posF - 2][posC - 3];
		a[15] = m[posF - 3][posC];
		a[14] = m[posF - 3][posC - 1];
		a[13] = m[posF - 3][posC - 2];
		a[12] = m[posF - 3][posC - 3];
		break;
	}
}

/** Dado un vector elimina las posibilidades de entrar en una zona a la que pudiera llegar otro agente con un solo movimiento
 *
 * @param sup Vector de superficie visible
 * @param agentes Vector de agentes visibles
 * @param size Tamaño del mapa completo
 */
void EvitarAgentesA(vector<char> &sup, vector<unsigned char> agentes, int width, int height, Sensores sensores)
{

	// Genero 2 mapas auxiliares
	vector<vector<unsigned char>> mapaAgentes(width, vector<unsigned char>(height, '_'));
	vector<vector<char>> mapaSuperficie(width, vector<char>(height, '_'));

	// Compruebo si hay algún agente a la vista y si es así elimino la posibilidad de ir a alguna casilla
	// a la que pueda llegar en un solo movimiento
	for (int i = 1; i < agentes.size(); i++)
	{
		if (agentes.at(i) == 'r')
		{
			SituarSensorEnMapaA(mapaAgentes, sensores.posF, sensores.posC, agentes, sensores.rumbo);
			SituarSensorEnMapaA(mapaSuperficie, sensores.posF, sensores.posC, sup, sensores.rumbo);

			// Busco al auxiliar en el nuevo mapa
			bool found = false;
			int f = 0, c = 0;

			for (int j = 0; j < mapaAgentes.size() and !found; j++)
			{
				for (int k = 0; k < mapaAgentes.at(j).size() and !found; k++)
				{
					if (mapaAgentes.at(j).at(k) == 'r')
					{
						found = true;
						f = j;
						c = k;
					}
				}
			}

			// Pongo como precipicio en el mapa de superficie las posiciones a las que podría llegar en un movimiento

			mapaSuperficie.at(f - 2).at(c - 2) = 'P';
			mapaSuperficie.at(f - 2).at(c) = 'P';
			mapaSuperficie.at(f - 2).at(c + 2) = 'P';

			mapaSuperficie.at(f - 1).at(c - 1) = 'P';
			mapaSuperficie.at(f - 1).at(c) = 'P';
			mapaSuperficie.at(f - 1).at(c + 1) = 'P';

			mapaSuperficie.at(f).at(c - 2) = 'P';
			mapaSuperficie.at(f).at(c - 1) = 'P';
			mapaSuperficie.at(f).at(c) = 'P';
			mapaSuperficie.at(f).at(c + 1) = 'P';
			mapaSuperficie.at(f).at(c + 2) = 'P';

			mapaSuperficie.at(f + 1).at(c - 1) = 'P';
			mapaSuperficie.at(f + 1).at(c) = 'P';
			mapaSuperficie.at(f + 1).at(c + 1) = 'P';

			mapaSuperficie.at(f + 2).at(c - 2) = 'P';
			mapaSuperficie.at(f + 2).at(c) = 'P';
			mapaSuperficie.at(f + 2).at(c + 2) = 'P';

			// Devuelvo la parte visible de este mapa a sup
			SituarMapaEnSensorA(mapaSuperficie, sensores.posF, sensores.posC, sup, sensores.rumbo);
		}
	}
}

// Comportamiento del nivel 0
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base

	// Actualización de variables de estado

	// Ajusto el mapa resultado y el de cotas
	SituarSensorEnMapaA(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
	SituarSensorEnMapaA(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

	veces_pisada[sensores.posF][sensores.posC]++;
	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	// Definición del comportamiento
	if (sensores.superficie[0] == 'X')
	{ // Llegué al objetivo
		accion = IDLE;
	}
	else if (giro45izq > 0)
	{ // Estoy haciendo un TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else
	{
		const int SIZE = 16;
		const int DEFAULT_SUPERFICIE = 'P';

		vector<char> sup(SIZE, DEFAULT_SUPERFICIE); // Vector de superficie visible

		sup.at(1) = ViableA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		sup.at(2) = ViableA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		sup.at(3) = ViableA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		sup.at(4) = sensores.superficie[4];												  // alante izquierda (para correr)
		sup.at(5) = sensores.superficie[5];												  // alante izquierda (para andar)
		sup.at(6) = ViableA(sensores.superficie[6], sensores.cota[6] - sensores.cota[0]); // alante (para correr)
		sup.at(7) = sensores.superficie[7];												  // alante izquierda (para andar)
		sup.at(8) = sensores.superficie[8];												  // alante derecha (para correr)

		EvitarAgentesA(sup, sensores.agentes, mapaResultado.size(), mapaResultado[0].size(), sensores);

		int pos = VeoCasillaTransitable(sup, sensores, [](char c, bool zap)
										{ return c == 'C' or c == 'D'; });

		switch (pos)
		{
		case 2: // Seguir recto
			accion = WALK;
			break;
		case 3: // Girar hacia la derecha 45 grados
			accion = TURN_SR;
			break;
		case 1: // Girar hacia la izquierda 45 grados
			giro45izq = 6;
			accion = TURN_SR;
			break;
		case 0: // Girar hacia la izquierda 90 grados (si no puede hacer nada)
			accion = TURN_SR;
			break;
		}
	}

	// Devolver la siguiente acción a hacer
	last_action = accion;
	return accion;
}

// Comportamiento del nivel 1
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;
	// El comportamiento de encontrar todo el camino y sendero posible

	if (sensores.energia < 200)
	{
		return IDLE;
	}

	// Actualización de variables de estado

	// Ajusto el mapa resultado y el de cotas
	SituarSensorEnMapaA(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
	SituarSensorEnMapaA(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

	veces_pisada[sensores.posF][sensores.posC]++;
	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	// Definición del comportamiento
	if (giro45izq > 0)
	{ // Estoy haciendo un TURN_SL
		accion = TURN_SR;
		giro45izq--;
	}
	else
	{
		const int SIZE = 16;
		const int DEFAULT_SUPERFICIE = 'P';

		vector<char> sup(SIZE, DEFAULT_SUPERFICIE); // Vector de superficie visible

		sup.at(1) = ViableA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		sup.at(2) = ViableA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		sup.at(3) = ViableA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		sup.at(4) = sensores.superficie[4]; // alante izquierda (para correr)
		sup.at(5) = sensores.superficie[5]; // alante izquierda (para andar)
		sup.at(6) = sensores.superficie[6]; // alante (para correr)
		sup.at(7) = sensores.superficie[7]; // alante izquierda (para andar)
		sup.at(8) = sensores.superficie[8]; // alante derecha (para correr)

		EvitarAgentesA(sup, sensores.agentes, mapaResultado.size(), mapaResultado[0].size(), sensores);

		int pos = VeoCasillaTransitable(sup, sensores, [](char c, bool zap)
										{ return c == 'C' or c == 'S' or c == 'D' or c == 'X' or (c == 'B' and zap); });

		switch (pos)
		{
		case 2: // Seguir recto
			accion = WALK;
			break;
		case 3: // Girar hacia la derecha 45 grados
			accion = TURN_SR;
			break;
		case 1: // Girar hacia la izquierda 45 grados
			giro45izq = 6;
			accion = TURN_SR;
			break;
		case 0: // Girar hacia la izquierda 90 grados (si no puede hacer nada)
			accion = TURN_SR;
			break;
		}
	}

	// Devolver la siguiente acción a hacer
	last_action = accion;
	return accion;
}

/************************************************************
 *   _   _ _           _             ____            _____   *
 *  | \ | (_)_   _____| | ___  ___  |___ \   _   _  |___ /   *
 *  |  \| | \ \ / / _ \ |/ _ \/ __|   __) | | | | |   |_ \   *
 *  | |\  | |\ V /  __/ |  __/\__ \  / __/  | |_| |  ___) |  *
 *  |_| \_|_| \_/ \___|_|\___||___/ |_____|  \__, | |____/   *
 *                                           |___/           *
 ************************************************************/

/** Tablas de costes
 * @param a Acción a realizar
 * @param c Casilla inicial
 * @param dif Diferencia de altura
 */
int CosteA(Action a, char c, int dif)
{
	switch (a)
	{
	case WALK:
		switch (c)
		{
		case 'A':
			return 100 + dif * 10;
		case 'T':
			return 20 + dif * 5;
		case 'S':
			return 2 + dif;
		default:
			return 1;
		}
		break;
	case TURN_SR:
		switch (c)
		{
		case 'A':
			return 16;
		case 'T':
			return 3;
		case 'S':
			return 1;
		default:
			return 1;
		}
	default:
		return 0;
	}
}

// Anula todos los valores de la matriz
void AnularMatrizA(vector<vector<unsigned char>> &m)
{
	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m[i].size(); j++)
		{
			m[i][j] = 0;
		}
	}
}

// Muestra el recorrido en el mapa marcando con un punto naranja las casillas que serán transitadas
void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const vector<Action> &plan)
{
	AnularMatrizA(mapaConPlan);
	EstadoA cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case WALK:
			switch (cst.brujula)
			{
			case 0:
				cst.f--;
				break;
			case 1:
				cst.f--;
				cst.c++;
				break;
			case 2:
				cst.c++;
				break;
			case 3:
				cst.f++;
				cst.c++;
				break;
			case 4:
				cst.f++;
				break;
			case 5:
				cst.f++;
				cst.c--;
				break;
			case 6:
				cst.c--;
				break;
			case 7:
				cst.f--;
				cst.c--;
				break;
			}
			mapaConPlan[cst.f][cst.c] = 2;
			break;
		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;
		}

		it++;
	}
}

// Devuelve el estado en el que quedaría el agente después de hacer WALK
EstadoA NextCasillaA(const EstadoA &st)
{
	// EstadoA siguiente = st;
	// switch (st.brujula)
	// {
	// case norte:
	// 	siguiente.f = st.f - 1;
	// 	break;
	// case noreste:
	// 	siguiente.f = st.f - 1;
	// 	siguiente.c = st.c + 1;
	// 	break;
	// case este:
	// 	siguiente.c = st.c + 1;
	// 	break;
	// case sureste:
	// 	siguiente.f = st.f + 1;
	// 	siguiente.c = st.c + 1;
	// 	break;
	// case sur:
	// 	siguiente.f = st.f + 1;
	// 	break;
	// case suroeste:
	// 	siguiente.f = st.f + 1;
	// 	siguiente.c = st.c - 1;
	// 	break;
	// case oeste:
	// 	siguiente.c = st.c - 1;
	// 	break;
	// case noroeste:
	// 	siguiente.f = st.f - 1;
	// 	siguiente.c = st.c - 1;
	// }

	// return siguiente;

	static const int df[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
	static const int dc[8] = {0, 1, 1, 1, 0, -1, -1, -1};

	EstadoA siguiente = st;
	siguiente.f += df[st.brujula];
	siguiente.c += dc[st.brujula];

	return siguiente;
}

// Devuelve si sería posible hacer WALK desde la posición actual
bool CasillaAccesibleA(const EstadoA &st, const vector<vector<unsigned char>> &terreno,
					   const vector<vector<unsigned char>> &altura)
{
	EstadoA next = NextCasillaA(st);

	// Que esté en el mapa
	if (next.f < 0 or next.f >= terreno.size() || next.c < 0 || next.c >= terreno[0].size())
	{
		return false;
	}


	unsigned char c = terreno[next.f][next.c];
	bool check1 = false, check2 = false, check3 = false;

	// Que sea transitable
	check1 = c != 'P' and c != 'M';
	check2 = c != 'B' or (c == 'B' and st.zapatillas);

	// Que sea viable por altura
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

	// Si no la conozco asumo que es viable por altura
	if (terreno[next.f][next.c] == '?')
	{
		check3 = true;
	}

	return check1 and check2 and check3;
}

// Devuelve el estado resultante tras aplicar la acción
EstadoA applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura)
{
	EstadoA next = st;

	switch (accion)
	{
	case WALK:
		if (CasillaAccesibleA(st, terreno, altura))
		{
			next = NextCasillaA(st);
		}
		break;
	case TURN_SR:
		next.brujula = (next.brujula + 1) % 8;
		break;
	}

	// Compruebo si tiene zapatillas
	if (terreno[next.f][next.c] == 'D')
	{
		next.zapatillas = true;
	}

	return next;
}

/**
 * @brief Primera versión del algoritmo de Búsqueda en Anchura para el agente Auxiliar
 *
 * @param inicio Estado inicial de la búsqueda
 * @param final Estado final de la búsqueda, Solo es relevante la fila y la columna
 * @param terreno Matriz que contiene información sobre el tipo de terreno
 * @param altura Matriz que contiene información sobre la altura de las casillas
 *
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
vector<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,
													   const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{

	NodoA current_node;
	list<NodoA> frontier;
	set<NodoA> explored;
	vector<Action> path;

	current_node.estado = inicio;	  // Asignamos el estado inicial al nodo actual
	frontier.push_back(current_node); // Lo añadimos a la cola de abiertos

	bool SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);

	while (!SolutionFound and !frontier.empty())
	{

		// Actualización de las listas
		frontier.pop_front();
		explored.insert(current_node);

		// Compruebo si estoy en una casilla que dé las zapatillas
		if (terreno[current_node.estado.f][current_node.estado.c] == 'D')
		{
			current_node.estado.zapatillas = true;
		}

		// Genero el hijo resultante de aplicar la acción WALK
		NodoA child_WALK = current_node;
		child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
		if (child_WALK.estado.f == final.f and child_WALK.estado.c == final.c)
		{
			// El hijo generado es solución
			child_WALK.secuencia.push_back(WALK);
			current_node = child_WALK;
			SolutionFound = true;
		}
		else if (explored.find(child_WALK) == explored.end())
		{
			// Se mete en la lista frontier después de añadir a secuencia la acción
			child_WALK.secuencia.push_back(WALK);
			frontier.push_back(child_WALK);
		}

		// Genero el hijo resultante de aplicar la acción TURN_SR
		if (!SolutionFound)
		{
			NodoA child_TURN_SR = current_node;
			child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
			if (explored.find(child_TURN_SR) == explored.end())
			{
				child_TURN_SR.secuencia.push_back(TURN_SR);
				frontier.push_back(child_TURN_SR);
			}
		}

		// Paso a evaluar el siguiente nodo en la lista "frontier"
		if (!SolutionFound and !frontier.empty())
		{
			current_node = frontier.front();
			while (explored.find(current_node) != explored.end() and !frontier.empty())
			{
				frontier.pop_front();
				current_node = frontier.front();
			}
		}
	}

	// Devuelvo el camino si se ha encontrado solución
	if (SolutionFound)
		path = current_node.secuencia;

	return path;
}

// Función de Heureística del Algoritmo A* (coste estimado del camino más barato desde n al objetivo)
int HeureisticaA(const EstadoA &st, const EstadoA &objetivo)
{

	int f = abs(st.f - objetivo.f);
	int c = abs(st.c - objetivo.c);

	int res;

	if (f > c)
	{
		return f;
	}
	else
	{
		return c;
	}
}

/**
 * @brief Algoritmo de búsqueda A* para el auxiliar
 *
 * @param inicio Estado inicial de la búsqueda
 * @param final Estado final de la búsqueda, Solo es relevante la fila y la columna
 * @param terreno Matriz que contiene información sobre el tipo de terreno
 * @param altura Matriz que contiene información sobre la altura de las casillas
 *
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
vector<Action> ComportamientoAuxiliar::BusquedaA(const EstadoA &inicio, const EstadoA &final,
												 const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura)
{
	// NodoA current_node;
	// priority_queue<NodoA, vector<NodoA>, greater<NodoA>> frontier;
	// unordered_set<NodoA, HashNodoA> explored;

	// // Acciones posibles
	// const vector<Action> acciones = {WALK, TURN_SR};

	// current_node.estado = inicio;
	// current_node.costo = 0;
	// current_node.estimacion = HeureisticaA(inicio, final);

	// frontier.emplace(current_node);

	// int n = 0;

	// while (!frontier.empty())
	// {
	// 	current_node = frontier.top();
	// 	frontier.pop();

	// 	if (explored.find(current_node) == explored.end())
	// 	{
	// 		n++;

	// 		const EstadoA& st_actual = current_node.estado;

	// 		if (st_actual.f == final.f && st_actual.c == final.c)
	// 		{
	// 			// cout << "Iteraciones: " << n << endl;
	// 			// cout << "Abiertos: " << frontier.size() << endl;
	// 			// cout << "Cerrados: " << explored.size() << endl;

	// 			// cout << "Energía final: " << 3000 - current_node.costo << endl;

	// 			return current_node.secuencia;
	// 		}

	// 		explored.emplace(current_node);
	// 		int alt_actual = altura[current_node.estado.f][current_node.estado.c];
	// 		int terr_actual = terreno[current_node.estado.f][current_node.estado.c];

	// 		for (const Action &a : acciones)
	// 		{
	// 			NodoA hijo = current_node;
	// 			hijo.estado = applyA(a, st_actual, terreno, altura);

	// 			int dif_altura = clamp(altura[hijo.estado.f][hijo.estado.c] - alt_actual, -1, 1);

	// 			hijo.costo += CosteA(a, terr_actual, dif_altura);
	// 			hijo.estimacion = HeureisticaA(hijo.estado, final);

	// 			hijo.secuencia.emplace_back(a);

	// 			if (explored.find(hijo) == explored.end())
	// 			{
	// 				frontier.emplace(std::move(hijo)); // Forma más eficiente
	// 			}
	// 		}
	// 	}
	// }

	// cout<<"No solution found"<<endl;

	// return {};

	priority_queue<NodoA, vector<NodoA>, greater<NodoA>> frontier;
	unordered_map<EstadoA, int, HashEstadoA> coste_minimo;

	const vector<Action> acciones = {WALK, TURN_SR};

	NodoA start_node;
	start_node.estado = inicio;
	start_node.costo = 0;
	start_node.estimacion = HeureisticaA(inicio, final);

	frontier.emplace(start_node);
	coste_minimo[inicio] = 0;

	while (!frontier.empty())
	{
		NodoA actual = frontier.top();
		frontier.pop();

		const EstadoA &estado_actual = actual.estado;

		if (estado_actual.f == final.f && estado_actual.c == final.c)
		{
			return actual.secuencia;
		}

		const int f_actual = estado_actual.f;
		const int c_actual = estado_actual.c;
		const unsigned char terreno_actual = terreno[f_actual][c_actual];
		const int altura_actual = altura[f_actual][c_actual];

		for (const Action &a : acciones)
		{
			NodoA hijo = actual;
			hijo.estado = applyA(a, estado_actual, terreno, altura);

			const int f_hijo = hijo.estado.f;
			const int c_hijo = hijo.estado.c;

			if (f_hijo < 0 || f_hijo >= int(terreno.size()) || c_hijo < 0 || c_hijo >= int(terreno[0].size()))
				continue;

			int diff_altura = altura[f_hijo][c_hijo] - altura_actual;
			diff_altura = clamp(diff_altura, -1, 1);

			hijo.costo += CosteA(a, terreno_actual, diff_altura);
			hijo.estimacion = HeureisticaA(hijo.estado, final);
			hijo.secuencia.push_back(a);

			auto it = coste_minimo.find(hijo.estado);
			if (it == coste_minimo.end() || hijo.costo < it->second)
			{
				coste_minimo[hijo.estado] = hijo.costo;
				frontier.emplace(std::move(hijo));
			}
		}
	}

	return {};
}

// Comportamiento del nivel Extra (Búsqueda Anchura)
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		// Invocar al método de búsqueda

		// plan = AvanzaASaltosDeCaballo();
		// hayPlan = true;

		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}

	if (hayPlan and paso < plan.size())
	{
		accion = plan.at(paso);
		paso++;
	}

	if (plan.size() == 0)
	{
		hayPlan = false;
	}

	return accion;
}

// Comportamiento del nivel 2
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
	// No tiene que hacer nada
	return IDLE;
}

// Comportamiento del nivel 3
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		// Invocar al método de búsqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = mapaResultado[inicio.f][inicio.c] == 'D';
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = BusquedaA(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}

	if (hayPlan and paso < plan.size())
	{
		accion = plan.at(paso);
		paso++;
	}

	if (plan.size() == 0)
	{
		hayPlan = false;
	}

	return accion;
}

/************************************
 *   _   _ _           _   _  _      *
 *  | \ | (_)_   _____| | | || |     *
 *  |  \| | \ \ / / _ \ | | || |_    *
 *  | |\  | |\ V /  __/ | |__   _|   *
 *  |_| \_|_| \_/ \___|_|    |_|     *
 *  							     *
 ************************************/

vector<Action> ComportamientoAuxiliar::BusquedaADesconocido(const EstadoA &inicio, const EstadoA &final,
															const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, int& costo)
{
	priority_queue<NodoA, vector<NodoA>, greater<NodoA>> frontier;
	unordered_map<EstadoA, int, HashEstadoA> coste_minimo;

	const vector<Action> acciones = {WALK, TURN_SR};

	const bool DEBUG = false;

	NodoA start_node;
	start_node.estado = inicio;
	start_node.costo = 0;
	start_node.estimacion = HeureisticaA(inicio, final);

	frontier.emplace(start_node);
	coste_minimo[inicio] = 0;

	while (!frontier.empty())
	{
		NodoA actual = frontier.top();
		frontier.pop();

		const EstadoA &estado_actual = actual.estado;

		if (estado_actual.f == final.f && estado_actual.c == final.c)
		{
			return actual.secuencia;
		}

		const int f_actual = estado_actual.f;
		const int c_actual = estado_actual.c;
		const unsigned char terreno_actual = terreno[f_actual][c_actual];

		// Le pongo altura -1 si no se sabe
		int altura_actual = altura[f_actual][c_actual];

		if (DEBUG)
		{
			cout << "Paso a estudiar (" << f_actual << "," << c_actual << "," << estado_actual.brujula << "," << altura_actual << ")" << endl;
		}

		for (const Action &a : acciones)
		{
			NodoA hijo = actual;

			hijo.estado = applyA(a, estado_actual, terreno, altura);

			const int f_hijo = hijo.estado.f;
			const int c_hijo = hijo.estado.c;

			// Si la casilla no es accesible salto el resto del estudio
			if (f_hijo == f_actual and c_hijo == c_actual and hijo.estado.brujula == estado_actual.brujula)
				continue;

			// Si alguna de las alturas es desconocida supongo que es 0
			int diff_altura = altura[f_hijo][c_hijo] - altura_actual;
			diff_altura = clamp(diff_altura, -1, 1);

			// hijo.costo += CosteR(a, terreno_actual, diff_altura);

			hijo.costo++; // Priorizo reducir los movimientos
			hijo.estimacion = HeureisticaA(hijo.estado, final);
			hijo.secuencia.push_back(a);

			if (DEBUG)
			{
				cout << "Genero (" << hijo.estado.f << "," << hijo.estado.c << "," << hijo.estado.brujula << "," << diff_altura << ")" << endl;
			}

			auto it = coste_minimo.find(hijo.estado);
			if (it == coste_minimo.end() || hijo.costo < it->second)
			{
				coste_minimo[hijo.estado] = hijo.costo;
				frontier.emplace(std::move(hijo));
			}
		}
	}

	return {};
}

vector<Action> ComportamientoAuxiliar::BusquedaBaseA(const EstadoA &inicio, const vector<vector<unsigned char>> &terreno,
													   const vector<vector<unsigned char>> &altura, int &costo)
{
	priority_queue<NodoA, vector<NodoA>, greater<NodoA>> frontier;
	unordered_map<EstadoA, int, HashEstadoA> coste_minimo;

	const vector<Action> acciones = {WALK, TURN_SR};

	const bool DEBUG = false;

	NodoA start_node;
	start_node.estado = inicio;
	start_node.costo = 0;
	start_node.estimacion = 0;

	frontier.emplace(start_node);
	coste_minimo[inicio] = 0;

	while (!frontier.empty())
	{
		NodoA actual = frontier.top();
		frontier.pop();

		const EstadoA &estado_actual = actual.estado;

		if (terreno[actual.estado.f][actual.estado.c] == 'X')
		{
			costo = actual.costo;
			return actual.secuencia;
		}

		const int f_actual = estado_actual.f;
		const int c_actual = estado_actual.c;
		const unsigned char terreno_actual = terreno[f_actual][c_actual];

		// Le pongo altura -1 si no se sabe
		int altura_actual = altura[f_actual][c_actual];

		if (DEBUG)
		{
			cout << "Paso a estudiar (" << f_actual << "," << c_actual << "," << estado_actual.brujula << "," << altura_actual << ")" << endl;
		}

		for (const Action &a : acciones)
		{
			NodoA hijo = actual;
			hijo.estado = applyA(a, estado_actual, terreno, altura);

			const int f_hijo = hijo.estado.f;
			const int c_hijo = hijo.estado.c;

			// Si la casilla no es accesible salto el resto del estudio
			if (f_hijo == f_actual and c_hijo == c_actual and hijo.estado.brujula == estado_actual.brujula)
				continue;

			int diff_altura = altura[f_hijo][c_hijo] - altura_actual;
			diff_altura = clamp(diff_altura, -1, 1);

			// Priorizo reducir la energía
			if (terreno_actual == '?')
			{
				hijo.costo += 110; // Si no sé la casilla le aumento el máximo posible
			}
			else
			{
				hijo.costo += CosteA(a, terreno_actual, diff_altura);
			}

			hijo.secuencia.push_back(a);

			if (DEBUG)
			{
				cout << "Genero (" << hijo.estado.f << "," << hijo.estado.c << "," << hijo.estado.brujula << "," << diff_altura << ")" << endl;
			}

			auto it = coste_minimo.find(hijo.estado);
			if (it == coste_minimo.end() || hijo.costo < it->second)
			{
				coste_minimo[hijo.estado] = hijo.costo;
				frontier.emplace(std::move(hijo));
			}
		}
	}

	return {};
}

// Indica si el rescatador se encuentra en el campo de visión
bool VeoRescatadorA(const vector<unsigned char> agentes)
{
	for (int i = 0; i < agentes.size(); i++)
	{
		if (agentes[i] == 'r')
			return true;
	}

	return false;
}

// Indica si hay algún agente en el campo de visión
bool VeoBaseA(vector<unsigned char> sup)
{

	for (int i = 0; i < sup.size(); i++)
	{
		if (sup[i] == 'X')
		{
			return true;
		}
	}

	return false;
}

// Comportamiento del nivel 4
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
	Action accion = IDLE;
	int costo, costoBase;
	const int UMBRAL = 500;

	const bool DEBUG = false;


	// Si le llaman
	if (sensores.venpaca)
	{

		if (DEBUG)
		{
			cout << "[A] Me han llamado" << endl;
		}

		// Ajusto el mapa resultado y el de cotas
		SituarSensorEnMapaA(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
		SituarSensorEnMapaA(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

		// Si ya he llegado (veo al rescatador)
		if (VeoRescatadorA(sensores.agentes))
		{

			if (DEBUG)
			{
				cout << "[A] Veo al rescatador" << endl;
			}

			accion = IDLE;
		}
		else
		{
			if (DEBUG)
			{
				cout << "[A] Aún no veo al rescatador" << endl;
			}

			// Si he visto una base por primera vez (para poder descansar)
			if (!heVistoBase and VeoBaseA(sensores.superficie))
			{
				heVistoBase = true;
				if (DEBUG)
				{
					cout << "[A] He visto una base" << endl;
				}
			}

			// Si cojo las zapatillas
			if(!tiene_zapatillas and mapaResultado[sensores.posF][sensores.posC] == 'D') {
				tiene_zapatillas = true;

				if(DEBUG) {
					cout<<"[A] He cogido las zapatillas"<<endl;
				}
			}

			// Invocar al método de búsqueda
			EstadoA inicio, fin;

			inicio.f = sensores.posF;
			inicio.c = sensores.posC;
			inicio.brujula = sensores.rumbo;
			inicio.zapatillas = (mapaResultado[inicio.f][inicio.c] == 'D' or tiene_zapatillas);

			fin.f = sensores.destinoF;
			fin.c = sensores.destinoC;

			if(DEBUG) {
				cout<<"[A] He iniciado el proceso de búsqueda del herido"<<endl;
			}

			plan = BusquedaADesconocido(inicio, fin, mapaResultado, mapaCotas, costo);

			if(DEBUG) {
				cout<<"[A] He finalizado el proceso de búsqueda del herido"<<endl;
			}

			if (heVistoBase)
			{
				planBase = BusquedaBaseA(inicio, mapaResultado, mapaCotas, costoBase);
			}

			hayPlan = plan.size() != 0;

			if(heVistoBase and planBase.size() > 0 and costoBase + UMBRAL >= sensores.energia) {
				voyABase = true;
			}

			if(voyABase) {
				if(sensores.superficie[0]=='X') {
					if(2*(costo + UMBRAL) >= sensores.energia) {
		
						if(DEBUG) {
							cout<<"[A] Estoy en una base"<<endl;
						}
		
						return IDLE;
					} else {
						voyABase = false;
					}
				} else {
		
					if(DEBUG) {
						cout<<"[A] Voy a ir a una base"<<endl;
					}
		
					VisualizaPlan(inicio, planBase);
		
					return planBase.at(0);
				}
			} else if (hayPlan and paso < plan.size())
			{	
				VisualizaPlan(inicio, plan);
		
				if(DEBUG) {
					cout<<"[A] Voy a atender la llamada del rescatador"<<endl;
				}

				accion = plan.at(0);
			}
		}
	}
	else
	{ // Si no me llaman
		accion = IDLE;
	}

	return accion;
}