#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"

#include <algorithm>
#include <iostream>

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0(sensores);
		break;
	case 1:
		accion = ComportamientoRescatadorNivel_1(sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2(sensores);
		break;
	case 3:
		accion = ComportamientoRescatadorNivel_3(sensores);
		break;
	case 4:
		accion = ComportamientoRescatadorNivel_4(sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
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
char ViableR(char casilla, int dif, bool zap)
{
	if ((abs(dif) <= 1 or (zap and abs(dif) <= 2)))
		return casilla;
	else
		return 'P';
}

// Dice si una casilla es transitable
bool TransitableR(char casilla, Sensores sensores)
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
		if (casilla == 'C' or casilla == 'D' or casilla == 'S')
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
int VecesPisadaR(int casilla, Sensores sensores, vector<vector<int>> vp)
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
int ComportamientoRescatador::TomarDecision(bool (*condicion)(char), vector<char> sup, vector<int> vp, int orientacion, Sensores sensores)
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
	if (condicion(sensores.superficie[6]))
	{
		if (TransitableR(sensores.superficie[2], sensores) and condicion(sup.at(6)))
		{
			// Orientación: -	Ruta: Directa
			posibles.push_back(6);
			pisada.push_back(vp.at(6));
			giros.push_back(0);

			if (DEBUG)
				cout << "Añado 6" << endl;
		}
		else if (TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sensores.superficie[6], sensores.cota[6] - sensores.cota[2], tiene_zapatillas), sensores))
		{
			// Orientación: -	Ruta: 2
			posibles.push_back(2);
			pisada.push_back(vp.at(6));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 6.2" << endl;
		}
		else if (orientacion == 0 and TransitableR(sup.at(3), sensores) and TransitableR(ViableR(sensores.superficie[6], sensores.cota[6] - sensores.cota[3], tiene_zapatillas), sensores))
		{
			// Orientación: 1	Ruta: 3
			posibles.push_back(3);
			pisada.push_back(vp.at(6));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 6.3" << endl;
		}
		else if (orientacion == 0 and TransitableR(sup.at(1), sensores) and TransitableR(ViableR(sensores.superficie[6], sensores.cota[6] - sensores.cota[1], tiene_zapatillas), sensores))
		{
			// Orientación: 1	Ruta: 1
			posibles.push_back(1);
			pisada.push_back(vp.at(6));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 6.1" << endl;
		}
	}

	// Ir a casilla 2 (c) Directa
	if (condicion(sup.at(2)))
	{
		// Orientación: -	Ruta: Directa
		posibles.push_back(2);
		pisada.push_back(vp.at(2));
		giros.push_back(0);
		if (DEBUG)
			cout << "Añado 2" << endl;
	}

	// Ir a casilla 8 (add)
	if (condicion(sup.at(8)))
	{
		if (TransitableR(sup.at(3), sensores) and TransitableR(ViableR(sup.at(8), sensores.cota[8] - sensores.cota[3], tiene_zapatillas), sensores))
		{
			// Orientación: -	Destino: 8	Ruta: 3
			posibles.push_back(3);
			pisada.push_back(vp.at(8));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 8.3" << endl;
		}
		else if (orientacion == 1 and TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sup.at(8), sensores.cota[8] - sensores.cota[2], tiene_zapatillas), sensores))
		{
			// Orientación: 1	Destino: 8	Ruta: 2
			posibles.push_back(2);
			pisada.push_back(vp.at(8));
			giros.push_back(2);
			if (DEBUG)
				cout << "Añado 8.2" << endl;
		}
		else if (orientacion == 0 and TransitableR(sup.at(6), sensores) and TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sup.at(8), sensores.cota[6] - sensores.cota[7], tiene_zapatillas), sensores) and TransitableR(sup.at(7), sensores) and TransitableR(ViableR(sup.at(8), sensores.cota[8] - sensores.cota[7], tiene_zapatillas), sensores))
		{
			// Orientación: 0	Destino: 8	Ruta: 2 6 7
			posibles.push_back(6);
			pisada.push_back(vp.at(8));
			giros.push_back(2);
			if (DEBUG)
				cout << "Añado 8.267" << endl;
		}
	}

	// Ir a casilla 4 (aii)
	if (condicion(sup.at(4)))
	{
		if (TransitableR(sup.at(1), sensores) and TransitableR(ViableR(sup.at(4), sensores.cota[4] - sensores.cota[1], tiene_zapatillas), sensores))
		{
			// Orientación: -	Ruta: 1
			posibles.push_back(1);
			pisada.push_back(vp.at(4));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 4.1" << endl;
		}
		else if (orientacion == 1 and TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sup.at(4), sensores.cota[4] - sensores.cota[2], tiene_zapatillas), sensores))
		{
			// Orientación: 1	Ruta 2
			posibles.push_back(2);
			pisada.push_back(vp.at(4));
			giros.push_back(-2);
			if (DEBUG)
				cout << "Añado 4.2" << endl;
		}
		else if (orientacion == 0 and TransitableR(sup.at(6), sensores) and TransitableR(sensores.superficie[2], sensores) and TransitableR(ViableR(sup.at(4), sensores.cota[6] - sensores.cota[5], tiene_zapatillas), sensores) and TransitableR(sup.at(5), sensores) and TransitableR(ViableR(sup.at(4), sensores.cota[4] - sensores.cota[5], tiene_zapatillas), sensores))
		{
			// Orientación: 0	Ruta: 2 6 5
			posibles.push_back(6);
			pisada.push_back(vp.at(4));
			giros.push_back(-2);
			if (DEBUG)
				cout << "Añado 4.265" << endl;
		}
	}

	// Ir a casilla 7 (ad)
	if (condicion(sup.at(7)))
	{
		if (TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sup.at(7), sensores.cota[7] - sensores.cota[2], tiene_zapatillas), sensores))
		{
			// Orientación: -	Ruta: 2
			posibles.push_back(2);
			pisada.push_back(vp.at(7));
			giros.push_back(1);
			if (DEBUG)
				cout << "Añado 7.2" << endl;
		}
		else if (TransitableR(sup.at(3), sensores) and TransitableR(ViableR(sup.at(7), sensores.cota[7] - sensores.cota[3], tiene_zapatillas), sensores))
		{
			// Orientación: -	Ruta: 3
			posibles.push_back(3);
			pisada.push_back(vp.at(7));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 7.3" << endl;
		}
	}

	// Ir a casilla 5 (ai)
	if (condicion(sup.at(5)))
	{
		if (TransitableR(sup.at(2), sensores) and TransitableR(ViableR(sup.at(5), sensores.cota[5] - sensores.cota[2], tiene_zapatillas), sensores))
		{
			// Orientación: - 	Ruta 2
			posibles.push_back(2);
			pisada.push_back(vp.at(5));
			giros.push_back(-1);
			if (DEBUG)
				cout << "Añado 5.2" << endl;
		}
		else if (TransitableR(sup.at(1), sensores) and TransitableR(ViableR(sup.at(5), sensores.cota[5] - sensores.cota[1], tiene_zapatillas), sensores))
		{
			// Otientación: -	Ruta 1
			posibles.push_back(1);
			pisada.push_back(vp.at(5));
			giros.push_back(0);
			if (DEBUG)
				cout << "Añado 5.1" << endl;
		}
	}

	// Ir a casilla 3 (d) Directa
	if (condicion(sup.at(3)))
	{
		// Orientación: -  Ruta: Directa
		posibles.push_back(3);
		pisada.push_back(vp.at(3));
		giros.push_back(0);
		if (DEBUG)
			cout << "Añado 3" << endl;
	}

	// Ir a casilla 1 (i) Directa
	if (condicion(sup.at(1)))
	{
		// Orientación: -	Ruta: Directa
		posibles.push_back(1);
		pisada.push_back(vp.at(1));
		giros.push_back(0);
		if (DEBUG)
			cout << "Añado 1" << endl;
	}

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

		giro45izq = giros.at(pos_min);

		if (DEBUG)
			cout << "Elijo la " << pos_min << endl;

		return posibles.at(pos_min);
	}
}

// Devuelve la decisión tomada según las casillas transitables que se vean
int ComportamientoRescatador::VeoCasillaTransitable(vector<char> sup, Sensores sensores, bool (*condicion)(char))
{

	const int SIZE = 16;
	const int DEFAULT_VP = 3000;

	vector<int> vp(SIZE, DEFAULT_VP); // Vector que indica las veces que se ha pisado una casilla vista

	// Relleno el vector de veces pisada
	for (int i = 0; i < SIZE; i++)
	{
		vp.at(i) = VecesPisadaR(i, sensores, veces_pisada);
	}

	int orientacion = (sensores.rumbo % 2); // Si estamos en diagonal o no

	if (sensores.nivel == 0)
	{ // En el nivel 0 buscamos la casilla de salida
		int res = TomarDecision([](char c)
								{ return c == 'X'; }, sup, vp, orientacion, sensores);
		if (res != 0)
			return res;
	}

	if (!tiene_zapatillas)
	{ // Buscamos las zapatillas
		int res = TomarDecision([](char c)
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
void SituarSensorEnMapaR(vector<vector<T>> &m, int posF, int posC, vector<T> &a, int rumbo)
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
void SituarMapaEnSensorR(vector<vector<T>> &m, int posF, int posC, vector<T> &a, int rumbo)
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
 *
 * @return True si ve a un agente
 * @return False si no ve a un agente
 */
bool EvitarAgentesR(vector<char> &sup, vector<unsigned char> agentes, int width, int height, Sensores sensores)
{

	// Genero 2 mapas auxiliares
	vector<vector<unsigned char>> mapaAgentes(width, vector<unsigned char>(height, '_'));
	vector<vector<char>> mapaSuperficie(width, vector<char>(height, '_'));

	bool hayAgente = false;
	bool DEBUG = false;

	if (DEBUG)
	{

		cout << "Agentes: ";
		for (int i = 0; i < agentes.size(); i++)
		{
			cout << agentes[i] << " ";
		}
	}

	// Compruebo si hay algún agente a la vista y si es así elimino la posibilidad de ir a alguna casilla
	// a la que pueda llegar en un solo movimiento
	for (int i = 1; i < agentes.size(); i++)
	{
		if (agentes.at(i) == 'a')
		{
			SituarSensorEnMapaR(mapaAgentes, sensores.posF, sensores.posC, agentes, sensores.rumbo);
			SituarSensorEnMapaR(mapaSuperficie, sensores.posF, sensores.posC, sup, sensores.rumbo);

			// Busco al auxiliar en el nuevo mapa
			int f = 0, c = 0;

			if (DEBUG)
			{
				cout << "Mapa auxiliar:" << endl;
				for (int a = 0; a < mapaAgentes.size(); a++)
				{
					for (int b = 0; b < mapaAgentes[0].size(); b++)
					{
						cout << mapaAgentes[a][b] << " ";
					}
					cout << endl;
				}
			}

			for (int j = 0; j < mapaAgentes.size() and !hayAgente; j++)
			{
				for (int k = 0; k < mapaAgentes[0].size() and !hayAgente; k++)
				{
					if (mapaAgentes.at(j).at(k) == 'a')
					{
						hayAgente = true;
						f = j;
						c = k;
					}
				}
			}

			// Pongo como precipicio en el mapa de superficie las posiciones a las que podría llegar en un movimiento
			mapaSuperficie.at(f + 1).at(c - 1) = 'P';
			mapaSuperficie.at(f + 1).at(c) = 'P';
			mapaSuperficie.at(f + 1).at(c + 1) = 'P';
			mapaSuperficie.at(f).at(c - 1) = 'P';
			mapaSuperficie.at(f).at(c) = 'P';
			mapaSuperficie.at(f).at(c + 1) = 'P';
			mapaSuperficie.at(f - 1).at(c - 1) = 'P';
			mapaSuperficie.at(f - 1).at(c) = 'P';
			mapaSuperficie.at(f - 1).at(c + 1) = 'P';

			// Devuelvo la parte visible de este mapa a sup
			SituarMapaEnSensorR(mapaSuperficie, sensores.posF, sensores.posC, sup, sensores.rumbo);
		}
	}

	if (DEBUG)
		cout << "Hay agente: " << hayAgente << endl;

	return hayAgente;
}

// Comportamiento del nivel 0
Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{

	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base

	// Actualización de variables de estado

	// Ajusto el mapa resultado y el de cotas
	SituarSensorEnMapaR(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
	SituarSensorEnMapaR(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

	veces_pisada[sensores.posF][sensores.posC]++;
	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	// Definición del comportamiento
	if (sensores.superficie[0] == 'X')
	{ // Llegué al objetivo
		accion = IDLE;
	}
	else if (giro45izq != 0)
	{ // Estoy haciendo un TURN_SL
		if (giro45izq < 0)
		{
			accion = TURN_L;
			giro45izq += 2;
		}
		else
		{
			accion = TURN_SR;
			giro45izq--;
		}
	}
	else
	{
		const int SIZE = 16;
		const int DEFAULT_SUPERFICIE = 'P';

		vector<char> sup(SIZE, DEFAULT_SUPERFICIE); // Vector de superficie

		sup.at(1) = ViableR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		sup.at(2) = ViableR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		sup.at(3) = ViableR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
		sup.at(4) = sensores.superficie[4];																	// alante izquierda (para correr)
		sup.at(5) = sensores.superficie[5];																	// alante izquierda (para andar)
		sup.at(6) = ViableR(sensores.superficie[6], sensores.cota[6] - sensores.cota[0], tiene_zapatillas); // alante (para correr)
		sup.at(7) = sensores.superficie[7];																	// alante izquierda (para andar)
		sup.at(8) = sensores.superficie[8];																	// alante derecha (para correr)

		EvitarAgentesR(sup, sensores.agentes, mapaResultado.size(), mapaResultado[0].size(), sensores);

		int pos = VeoCasillaTransitable(sup, sensores, [](char c)
										{ return c == 'C' or c == 'D'; });

		switch (pos)
		{
		case 6: // Correr (avanzar 2 casillas)
			accion = RUN;
			break;
		case 2: // Seguir recto
			accion = WALK;
			break;
		case 3: // Girar hacia la derecha 45 grados
			accion = TURN_SR;
			break;
		case 1: // Girar hacia la izquierda 45 grados
			giro45izq = 1;
			accion = TURN_L;
			break;
		case 0: // Girar hacia la izquierda 90 grados (si no puede hacer nada)
			accion = TURN_L;
			break;
		}
	}

	// Devolver la siguiente acción a hacer
	last_action = accion;
	return accion;
}

// Comportamiento del nivel 1
Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{

	Action accion;
	// El comportamiento de encontrar todo el camino y sendero posible

	if (sensores.energia < 200)
	{
		return IDLE;
	}

	// Actualización de variables de estado

	// Ajusto el mapa resultado y el de cotas
	SituarSensorEnMapaR(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
	SituarSensorEnMapaR(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

	veces_pisada[sensores.posF][sensores.posC]++;

	if (sensores.superficie[0] == 'D')
		tiene_zapatillas = true;

	// Definición del comportamiento
	if (giro45izq != 0)
	{ // Estoy haciendo un TURN_SL
		if (giro45izq < 0)
		{
			accion = TURN_L;
			giro45izq += 2;
		}
		else
		{
			accion = TURN_SR;
			giro45izq--;
		}
	}
	else
	{
		const int SIZE = 16;
		const int DEFAULT_SUPERFICIE = 'P';

		vector<char> sup(SIZE, DEFAULT_SUPERFICIE); // Vector de superficie

		sup.at(1) = ViableR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		sup.at(2) = ViableR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		sup.at(3) = ViableR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
		sup.at(4) = sensores.superficie[4];																	// alante izquierda (para correr)
		sup.at(5) = sensores.superficie[5];																	// alante izquierda (para andar)
		sup.at(6) = ViableR(sensores.superficie[6], sensores.cota[6] - sensores.cota[0], tiene_zapatillas); // alante (para correr)
		sup.at(7) = sensores.superficie[7];																	// alante izquierda (para andar)
		sup.at(8) = sensores.superficie[8];																	// alante derecha (para correr)

		bool agente = EvitarAgentesR(sup, sensores.agentes, mapaResultado.size(), mapaResultado[0].size(), sensores);

		int pos = VeoCasillaTransitable(sup, sensores, [](char c)
										{ return c == 'C' or c == 'S' or c == 'D' or c == 'X'; });

		vector<int> tmp(SIZE);

		switch (pos)
		{
		case 6: // Correr (avanzar 2 casillas)
			accion = RUN;

			// Aumento el contador de la casilla de enmedio
			SituarMapaEnSensorR(veces_pisada, sensores.posF, sensores.posC, tmp, sensores.rumbo);
			tmp.at(2)++;
			SituarSensorEnMapaR(veces_pisada, sensores.posF, sensores.posC, tmp, sensores.rumbo);

			break;
		case 2: // Seguir recto
			accion = WALK;
			break;
		case 3: // Girar hacia la derecha 45 grados
			accion = TURN_SR;
			break;
		case 1: // Girar hacia la izquierda 45 grados
			giro45izq = 1;
			accion = TURN_L;
			break;
		case 0: // Girar hacia la izquierda 90 grados (si no puede hacer nada)

			if (veces_pisada[sensores.posF][sensores.posC] % 4 == 0 or
				veces_pisada[sensores.posF][sensores.posC] % 7 == 0)
				accion = TURN_SR; // con 4 flama
			else
				accion = TURN_L;
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
int CosteR(Action a, char c, int dif)
{

	int coste = 0;

	if (dif > 0)
	{
		dif = 1;
	}
	else if (dif < 0)
	{
		dif = -1;
	}

	switch (a)
	{
	case WALK:
		switch (c)
		{
		case 'A':
			coste = 100 + dif * 10;
			break;
		case 'T':
			coste = 20 + dif * 5;
			break;
		case 'S':
			coste = 2 + dif;
			break;
		default:
			coste = 1;
			break;
		}
		break;

	case RUN:
		switch (c)
		{
		case 'A':
			coste = 150 + dif * 15;
			break;
		case 'T':
			coste = 35 + dif * 5;
			break;
		case 'S':
			coste = 3 + dif * 2;
			break;
		default:
			coste = 1;
			break;
		}
		break;
	case TURN_L:
		switch (c)
		{
		case 'A':
			coste = 30;
			break;
		case 'T':
			coste = 5;
			break;
		case 'S':
			coste = 1;
			break;
		default:
			coste = 1;
			break;
		}
		break;
	case TURN_SR:
		switch (c)
		{
		case 'A':
			coste = 16;
			break;
		case 'T':
			coste = 3;
			break;
		case 'S':
			coste = 1;
			break;
		default:
			coste = 1;
			break;
		}
		break;
	}

	return coste;
}

// Anula todos los elementos de una matriz
void AnularMatrizR(vector<vector<unsigned char>> &m, unsigned char value = 0)
{
	for (int i = 0; i < m[0].size(); i++)
	{
		for (int j = 0; j < m.size(); j++)
		{
			m[i][j] = value;
		}
	}
}

// Muestra el recorrido en el mapa marcando con un punto naranja las casillas que serán transitadas
void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const vector<Action> &plan)
{
	AnularMatrizR(mapaConPlan);
	EstadoR cst = st;
	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case RUN:
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
			mapaConPlan[cst.f][cst.c] = 3;
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
			mapaConPlan[cst.f][cst.c] = 1;
			break;
		case TURN_SR:
			cst.brujula = (cst.brujula + 1) % 8;
			break;
		case TURN_L:
			cst.brujula = (cst.brujula + 6) % 8;
			break;
		}
		it++;
	}
}

// Devuelve el estado en el que quedaría el agente después de hacer WALK o RUN
EstadoR NextCasillaR(const EstadoR &st, Action a)
{
	EstadoR siguiente = st;

	int d = (a == WALK) ? 1 : 2; // Distancia a la que se avanza (WALK:1 , RUN:2)

	switch (st.brujula)
	{
	case norte:
		siguiente.f = st.f - d;
		break;
	case noreste:
		siguiente.f = st.f - d;
		siguiente.c = st.c + d;
		break;
	case este:
		siguiente.c = st.c + d;
		break;
	case sureste:
		siguiente.f = st.f + d;
		siguiente.c = st.c + d;
		break;
	case sur:
		siguiente.f = st.f + d;
		break;
	case suroeste:
		siguiente.f = st.f + d;
		siguiente.c = st.c - d;
		break;
	case oeste:
		siguiente.c = st.c - d;
		break;
	case noroeste:
		siguiente.f = st.f - d;
		siguiente.c = st.c - d;
	}

	return siguiente;
}

// Devuelve si sería posible hacer la acción determinada desde la posición actual
bool CasillaAccesibleR(const EstadoR &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura,
					   const vector<vector<unsigned char>> agentes, const Action &a)
{
	EstadoR next = NextCasillaR(st, a);
	bool check1 = false, check2 = false, check3 = false;

	// Que esté en el mapa
	if (next.f < 0 or next.f >= terreno.size() || next.c < 0 || next.c >= terreno[0].size())
	{
		return false;
	}

	// Que sea transitable
	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M' and terreno[next.f][next.c] != 'B';

	if (a == RUN and check1)
	{
		EstadoR enmedio = NextCasillaR(st, WALK);
		check1 = terreno[enmedio.f][enmedio.c] != 'P' and terreno[enmedio.f][enmedio.c] != 'M' and terreno[enmedio.f][enmedio.c] != 'B';
	}

	// Que sea viable por altura
	int dif = abs(altura[next.f][next.c] - altura[st.f][st.c]);

	check2 = (dif <= 1 or (next.zapatillas and dif <= 2));

	// Si no la conozco asumo que es viable por altura
	if (terreno[next.f][next.c] == '?')
	{
		check2 = true;
	}
	else if (a == RUN)
	{ // Si voy a correr y no conozco la de enmedio
		EstadoR enmedio = NextCasillaR(st, WALK);

		if (terreno[enmedio.f][enmedio.c] == '?')
		{
			check2 = true;
		}
	}

	// Que no haya agentes

	check3 = agentes[next.f][next.c] != 'a';

	if(a == RUN and check3) {
		EstadoR enmedio = NextCasillaR(st, WALK);
		check3 = agentes[enmedio.f][enmedio.c] != 'a';
	}


	return check1 and check2 and check3;
}

// Devuelve el estado resultante tras aplicar la acción
EstadoR applyR(Action accion, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
			   const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &agentes)
{
	EstadoR next = st;

	switch (accion)
	{
	case WALK:
		if (CasillaAccesibleR(st, terreno, altura, agentes, WALK))
		{
			next = NextCasillaR(st, WALK);
		}
		break;
	case RUN:
		if (CasillaAccesibleR(st, terreno, altura, agentes, RUN))
		{
			next = NextCasillaR(st, RUN);
		}
		break;
	case TURN_SR:
		next.brujula = (next.brujula + 1) % 8;
		break;
	case TURN_L:
		next.brujula = (next.brujula + 6) % 8;
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
 * @brief Algoritmo de búsqueda de Dijkstra para el recatador
 *
 * @param inicio Estado inicial de la búsqueda
 * @param final Estado final de la búsqueda, Solo es relevante la fila y la columna
 * @param terreno Matriz que contiene información sobre el tipo de terreno
 * @param altura Matriz que contiene información sobre la altura de las casillas
 *
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
vector<Action> ComportamientoRescatador::Dijkstra(const EstadoR &inicio, const EstadoR &final,
												  const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura,
												  const vector<vector<unsigned char>> &agentes)
{
	NodoR current_node;
	priority_queue<NodoR, vector<NodoR>, greater<NodoR>> frontier;
	unordered_set<NodoR, HashNodoR> explored;
	list<Action> path;

	// Acciones posibles
	const vector<Action> acciones = {RUN, WALK, TURN_SR, TURN_L};

	current_node.estado = inicio;
	current_node.costo = 0;

	frontier.emplace(current_node);

	int n = 0;

	while (!frontier.empty())
	{
		current_node = frontier.top();
		frontier.pop();

		if (explored.find(current_node) == explored.end())
		{
			n++;

			if (current_node.estado.f == final.f && current_node.estado.c == final.c)
			{
				cout << "Iteraciones: " << n << endl;
				cout << "Abiertos: " << frontier.size() << endl;
				cout << "Cerrados: " << explored.size() << endl;

				priority_queue<NodoR, vector<NodoR>, greater<NodoR>> tmp = frontier;

				cout << "Energía final: " << 3000 - current_node.costo << endl;

				return current_node.secuencia;
			}

			explored.emplace(current_node);

			for (int i = 0; i < acciones.size(); ++i)
			{
				Action a = acciones[i];

				NodoR hijo = current_node;
				hijo.estado = applyR(a, current_node.estado, terreno, altura, agentes);

				int alt_actual = altura[current_node.estado.f][current_node.estado.c];
				int alt_destino = altura[hijo.estado.f][hijo.estado.c];
				int diff_altura = alt_destino - alt_actual;

				hijo.costo += CosteR(a, terreno[current_node.estado.f][current_node.estado.c], diff_altura);

				hijo.secuencia.emplace_back(a);

				if (explored.find(hijo) == explored.end())
				{
					frontier.emplace(hijo);
				}
			}
		}
	}

	return {};
}

// Comportamiento del nivel 2
Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;

	if (!hayPlan)
	{
		// Invocar al método de búsqueda
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = mapaResultado[inicio.f][inicio.c] == 'D';
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = Dijkstra(inicio, fin, mapaResultado, mapaCotas, mapaAgentes);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
		paso = 0;
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

// Comportamiento del nivel 3
Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
	return IDLE;
}

/************************************
 *   _   _ _           _   _  _      *
 *  | \ | (_)_   _____| | | || |     *
 *  |  \| | \ \ / / _ \ | | || |_    *
 *  | |\  | |\ V /  __/ | |__   _|   *
 *  |_| \_|_| \_/ \___|_|    |_|     *
 *  							     *
 ************************************/

// Función de Heureística del Algoritmo A* (coste estimado del camino más barato desde n al objetivo)
int HeureisticaR(const EstadoR &st, const EstadoR &objetivo)
{

	int f = abs(st.f - objetivo.f);
	int c = abs(st.c - objetivo.c);

	int g = (objetivo.brujula + 8 - st.brujula) % 8;

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
 * @brief Algoritmo de búsqueda A* para el rescatador
 *
 * @param inicio Estado inicial de la búsqueda
 * @param final Estado final de la búsqueda, Solo es relevante la fila y la columna
 * @param terreno Matriz que contiene información sobre el tipo de terreno
 * @param altura Matriz que contiene información sobre la altura de las casillas
 *
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
vector<Action> ComportamientoRescatador::BusquedaR(const EstadoR &inicio, const EstadoR &final, const vector<vector<unsigned char>> &terreno,
												   const vector<vector<unsigned char>> &altura,
												   const vector<vector<unsigned char>> &agentes, int &costo)
{
	priority_queue<NodoR, vector<NodoR>, greater<NodoR>> frontier;
	unordered_map<EstadoR, int, HashEstadoR> coste_minimo;

	const vector<Action> acciones = {RUN, WALK, TURN_SR, TURN_L};

	const bool DEBUG = false;

	NodoR start_node;
	start_node.estado = inicio;
	start_node.costo = 0;
	start_node.estimacion = HeureisticaR(inicio, final);

	frontier.emplace(start_node);
	coste_minimo[inicio] = 0;

	while (!frontier.empty())
	{
		NodoR actual = frontier.top();
		frontier.pop();

		const EstadoR &estado_actual = actual.estado;

		if (estado_actual.f == final.f && estado_actual.c == final.c)
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
			NodoR hijo = actual;
			hijo.estado = applyR(a, estado_actual, terreno, altura, agentes);

			const int f_hijo = hijo.estado.f;
			const int c_hijo = hijo.estado.c;

			// Si la casilla no es accesible salto el resto del estudio
			if (f_hijo == f_actual and c_hijo == c_actual and hijo.estado.brujula == estado_actual.brujula)
				continue;

			int diff_altura = altura[f_hijo][c_hijo] - altura_actual;
			diff_altura = clamp(diff_altura, -1, 1);

			hijo.costo++; // Priorizo reducir los movimientos
			hijo.estimacion = HeureisticaR(hijo.estado, final);
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

/**
 * @brief Algoritmo de búsqueda A* para el rescatador
 *
 * @param inicio Estado inicial de la búsqueda
 * @param final Estado final de la búsqueda, Solo es relevante la fila y la columna
 * @param terreno Matriz que contiene información sobre el tipo de terreno
 * @param altura Matriz que contiene información sobre la altura de las casillas
 *
 * @return La secuencia de acciones para llegar al estado final
 * @note Devuelve un plan vacío si no es posible encontrar un plan válido
 */
vector<Action> ComportamientoRescatador::BusquedaBaseR(const EstadoR &inicio, const vector<vector<unsigned char>> &terreno,
												   const vector<vector<unsigned char>> &altura,
												   const vector<vector<unsigned char>> &agentes, int& costo)
{
	priority_queue<NodoR, vector<NodoR>, greater<NodoR>> frontier;
	unordered_map<EstadoR, int, HashEstadoR> coste_minimo;

	const vector<Action> acciones = {RUN, WALK, TURN_SR, TURN_L};

	const bool DEBUG = false;

	NodoR start_node;
	start_node.estado = inicio;
	start_node.costo = 0;
	start_node.estimacion = 0;

	frontier.emplace(start_node);
	coste_minimo[inicio] = 0;

	while (!frontier.empty())
	{
		NodoR actual = frontier.top();
		frontier.pop();

		const EstadoR &estado_actual = actual.estado;

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
			NodoR hijo = actual;
			hijo.estado = applyR(a, estado_actual, terreno, altura, agentes);

			const int f_hijo = hijo.estado.f;
			const int c_hijo = hijo.estado.c;

			// Si la casilla no es accesible salto el resto del estudio
			if (f_hijo == f_actual and c_hijo == c_actual and hijo.estado.brujula == estado_actual.brujula)
				continue;

			int diff_altura = altura[f_hijo][c_hijo] - altura_actual;
			diff_altura = clamp(diff_altura, -1, 1);

			// Priorizo reducir la energía
			if(terreno_actual == '?'){
				hijo.costo += 165; // Si no sé la casilla le aumento el máximo posible
			} else {
				hijo.costo+=CosteR(a, terreno_actual, diff_altura); 
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

// Indica si el auxiliar se encuentra en el campo de visión
bool VeoAuxiliarR(const vector<unsigned char> agentes)
{
	for (int i = 0; i < agentes.size(); i++)
	{
		if (agentes[i] == 'a')
			return true;
	}

	return false;
}

// Indica si hay algún agente en el campo de visión
bool VeoBaseR(vector<unsigned char> sup)
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
Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
	Action accion = IDLE;
	int costo, costoBase;
	const int UMBRAL = 500;

	const bool DEBUG = false;

	// Ajusto el mapa resultado y el de cotas
	SituarSensorEnMapaR(mapaResultado, sensores.posF, sensores.posC, sensores.superficie, sensores.rumbo);
	SituarSensorEnMapaR(mapaCotas, sensores.posF, sensores.posC, sensores.cota, sensores.rumbo);

	// Si veo al auxiliar
	if (VeoAuxiliarR(sensores.agentes))
	{
		AnularMatrizR(mapaAgentes, '_');
		SituarSensorEnMapaR(mapaAgentes, sensores.posF, sensores.posC, sensores.agentes, sensores.rumbo);
	}

	// Si he visto una base por primera vez (para poder descansar)
	if (!heVistoBase and VeoBaseR(sensores.superficie))
	{
		heVistoBase = true;
		if (DEBUG)
		{
			cout << "[R] He visto una base"<<endl;
		}
	}

	// Si cojo las zapatillas
	if(!tiene_zapatillas and mapaResultado[sensores.posF][sensores.posC] == 'D') {
		tiene_zapatillas = true;

		if(DEBUG) {
			cout<<"[R] He cogido las zapatillas"<<endl;
		}
	}

	if (DEBUG)
	{
		cout << "----------------------------------------" << endl;

		// for (int i = 0; i < mapaResultado.size(); i++)
		// {
		// 	for (int j = 0; j < mapaResultado.size(); j++)
		// 	{
		// 		cout << mapaResultado[i][j] << " ";
		// 	}
		// 	cout << endl;
		// }

		// cout << "----------------------------------------" << endl;

		// for (int i = 0; i < mapaCotas.size(); i++)
		// {
		// 	for (int j = 0; j < mapaCotas.size(); j++)
		// 	{
		// 		cout << mapaCotas[i][j] << " ";
		// 	}
		// 	cout << endl;
		// }
	}

	// Si llego al destino y tiene gravedad
	if (sensores.posF == sensores.destinoF and sensores.posC == sensores.destinoC)
	{
		if (sensores.gravedad)
		{
			if (last_action != CALL_ON)
			{
				last_action = CALL_ON;

				if(DEBUG) {
					cout<<"[R] Llamo al auxiliar con un CALL_ON"<<endl;
				}

				return CALL_ON;
			}
			else
			{
				last_action = TURN_L;

				if(DEBUG) {
					cout<<"[R] Me quedo dando vueltas hasta ver al auxiliar"<<endl;
				}

				return TURN_L;
			}
		}
	}

	

	// Invocar al método de búsqueda
	EstadoR inicio, fin;
	inicio.f = sensores.posF;
	inicio.c = sensores.posC;
	inicio.brujula = sensores.rumbo;
	inicio.zapatillas = (mapaResultado[inicio.f][inicio.c] == 'D' or tiene_zapatillas);

	fin.f = sensores.destinoF;
	fin.c = sensores.destinoC;

	plan = BusquedaR(inicio, fin, mapaResultado, mapaCotas, mapaAgentes, costo);

	if (heVistoBase)
	{
		planBase = BusquedaBaseR(inicio, mapaResultado, mapaCotas, mapaAgentes, costoBase);
	}

	hayPlan = plan.size() != 0;	

	if(heVistoBase and planBase.size() > 0 and costoBase + UMBRAL >= sensores.energia) {
		voyABase = true;
	}

	if(voyABase) {
		if(sensores.superficie[0]=='X') {
			if(2*(costo + UMBRAL) >= sensores.energia) {

				if(DEBUG) {
					cout<<"[R] Estoy en una base"<<endl;
				}

				return IDLE;
			} else {
				voyABase = false;
			}
		} else {

			if(DEBUG) {
				cout<<"[R] Voy a ir a una base"<<endl;
			}

			VisualizaPlan(inicio, planBase);

			return planBase.at(0);
		}
	} else if (hayPlan and paso < plan.size())
	{	
		VisualizaPlan(inicio, plan);

		accion = plan.at(0);

		if(DEBUG) {
			cout<<"[R] Voy a por un excurisionista en apuros"<<endl;
		}
	}

	return accion;
}