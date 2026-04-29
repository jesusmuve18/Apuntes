#ifndef MAPA_H
#define MAPA_H

#include <GL/glut.h>

#include "motorlib/entidad.hpp"
#include "modelos_belkan.hpp"

// Macro clase para englobar Mapas de juego
// Contiene la informacion para pintar y ejecutar en el juego las Mapaes
// Las Mapaes son el jugador y los npcs.

class Mapa
{
private:
	std::vector<std::vector<unsigned char>> mapaCompleto;
	std::vector<std::vector<unsigned char>> mapaAlturas;
	std::vector<std::vector<bool>> mapaVisible;

	std::vector<Entidad *> *entidades;

	// Atributos necesarios y tal y cual...
	void colorCeldaMM(unsigned char celda, unsigned char color);
	void colorCeldaOpuestoMM(unsigned char celda);
	void colorCeldaOpuestoMM2(unsigned char celda);

	void complementosCelda(unsigned char celda, unsigned char altura);

	void formaEntidad(unsigned char tipoParam);
	void OrientacionEntidadMM(Orientacion orienParam);
	void OrientacionEntidadFP(Orientacion orienParam);

	float angulo = 0.0;
	float x = 5.0;
	float z = 5.0;

	float Y1 = 0, Y2 = 0.0, Y3 = 3.25;

	unsigned int nCols;
	unsigned int nFils;

public:
	Mapa(std::vector<std::vector<unsigned char>> mapaT, std::vector<std::vector<unsigned char>> mapaA, std::vector<Entidad *> *entidades_ptr) : mapaCompleto(mapaT), mapaAlturas(mapaA),entidades(entidades_ptr)
	{
		std::vector<bool> aux(mapaT[0].size(), true);
		for (unsigned int i = 0; i < mapaT.size(); i++)
		{
			mapaVisible.push_back(aux);
		}

		nCols = mapaT.size();
		nFils = mapaT[0].size();
	}
	~Mapa() {}

	void JoinMapasSuperficie (vector<vector<unsigned char>> &resultado, const vector<vector<unsigned char>> &fuente);
	void JoinMapasPlan (vector<vector<unsigned char>> &resultado, const vector<vector<unsigned char>> &fuente);

	void drawMM1(vector<unsigned int> objetivosActivos, int level);
	void drawMM2(vector<unsigned int> objetivosActivos, int level);
	void drawFirstPerson(int entidad);

	unsigned char entidadEnCelda(unsigned int x, unsigned int y);

	unsigned char getCelda(unsigned int i, unsigned int j) { return mapaCompleto[i][j]; }

	unsigned char alturaEnCelda(unsigned int i, unsigned int j) { return mapaAlturas[i][j];}

	float getMapDepth() { return sqrt(pow(mapaCompleto.size(), 2) + pow(mapaCompleto[0].size(), 2)); }

	int QuienEnCasilla(int f, int c);
	int casillaOcupada(unsigned int entidad);
	pair<int,int> NCasillasDelante(unsigned int entidad, int casillas);

	int EsPosibleCorrer(unsigned int entidad, int avance_size);

	int casillaOcupadaThrow(unsigned int entidad, int &fil, int &col);

	void girarCamaraIzquierda(int grados);
	void girarCamaraDerecha(int grados);

	unsigned int getNCols() { return nCols; }
	unsigned int getNFils() { return nFils; }

	std::vector<std::vector<unsigned char>> vision(unsigned int Entidad);

	float getAngle() const { return Y1; }
	float getParamX() const { return Y2; }
	float getParamZ() const { return Y3; }

	void putAngle(float x) { Y1 = x; }
	void PutParamX(float valor) { Y2 = valor; }
	void PutParamZ(float valor) { Y3 = valor; }
};
#endif
