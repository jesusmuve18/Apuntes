#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <unordered_set>
#include <queue>
#include <set>
#include <algorithm>
#include <unordered_map>

#include "comportamientos/comportamiento.hpp"

struct EstadoA
{
    int f;
    int c;
    int brujula;
    bool zapatillas;

    // Sobrecarga del operador '=='
    // Se considerará el mismo estado si coinciden todos los parámetros
    bool operator==(const EstadoA &st) const
    {
        return f == st.f and c == st.c and brujula == st.brujula and zapatillas == st.zapatillas;
    }

    // Sobrecarga del operador '<
    bool operator<(const EstadoA &st) const
    {
        if (f < st.f)
            return true;
        else if (f == st.f and c < st.c)
            return true;
        else if (f == st.f and c == st.c and brujula < st.brujula)
            return true;
        else if (f == st.f and c == st.c and brujula == st.brujula and zapatillas < st.zapatillas)
            return true;
        else
            return false;
    }
};

// Hash para EstadoA
struct HashEstadoA {
    size_t operator()(const EstadoA &e) const {
        return ((std::hash<int>()(e.f) ^
                (std::hash<int>()(e.c) << 1)) >> 1) ^
                (std::hash<int>()(e.brujula) << 1) ^
                (std::hash<bool>()(e.zapatillas) << 2);
    }
};

struct NodoA
{
    EstadoA estado;
    vector<Action> secuencia;
    int costo;
    int estimacion;

    // Sobrecarga del operador '=='
    // Se considerarán iguales si tienen el mismo estado
    bool operator==(const NodoA &node) const
    {
        return estado == node.estado;
    }

    // Sobrecarga del operador '<'
    bool operator<(const NodoA &node) const {
        return estado < node.estado;
    }

    // Sobrecarga del operador '>'
    bool operator>(const NodoA &node) const {
        return costo + estimacion > node.costo + node.estimacion;
    }
};

// Hash para NodoA
struct HashNodoA {
    size_t operator()(const NodoA &n) const {
        return HashEstadoA()(n.estado); // Usa el hash del estado
    }
};

class ComportamientoAuxiliar : public Comportamiento
{

public:
    ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size), veces_pisada(size, std::vector<int>(size, 0))
    {
        // Inicializar Variables de Estado Niveles 0,1,4
        last_action = IDLE;
        tiene_zapatillas = false;
        giro45izq = 0;

         // Nivel 4
         hayPlan = false;
         paso = 0;
         heVistoBase = false;
         voyABase = false;
    }
    ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR, mapaC)
    {
        // Inicializar Variables de Estado Niveles 2,3
        hayPlan = false;
        tiene_zapatillas = false;
        last_action = IDLE;
        paso = 0;
    }
    ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
    ~ComportamientoAuxiliar() {}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    Action ComportamientoAuxiliarNivel_0(Sensores sensores);
    Action ComportamientoAuxiliarNivel_1(Sensores sensores);
    Action ComportamientoAuxiliarNivel_2(Sensores sensores);
    Action ComportamientoAuxiliarNivel_3(Sensores sensores);
    Action ComportamientoAuxiliarNivel_4(Sensores sensores);

    // Métodos usados en los niveles 0 y 1
    int VeoCasillaTransitable(vector<char> superficie, Sensores sensores, bool (*condicion)(char, bool));
    int TomarDecision(bool (*condicion)(char, bool), vector<char> sup, vector<int> vp, int orientacion, Sensores sensores);

    // Métodos usados en los niveles 2 y 3
    Action ComportamientoAuxiliarNivel_E(Sensores sensores);
    vector<Action> AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,
                                 const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
    void VisualizaPlan(const EstadoA &st, const vector<Action> &plan);
    vector<Action> BusquedaA(const EstadoA &inicio, const EstadoA &final,
        const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);

    // Métodos usados en el nivel 4
    vector<Action> BusquedaADesconocido(const EstadoA &inicio, const EstadoA &final,
        const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, int& costo);
    vector<Action> BusquedaBaseA(const EstadoA &inicio,
            const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, int& costo);


private:
    // Definir Variables de Estado
    Action last_action;    // Almacena la última acción realizada por el agente
    bool tiene_zapatillas; // Indica si ya pasó por una casilla de tipo zapatillas
    int giro45izq;         // Indica si se está haciendo un TURN_SL (0 indica que no)

    vector<vector<int>> veces_pisada; // Número de veces que he pisado una casilla

    // Variables de estado para el nivel E
    vector<Action> plan;
    int paso; // Número por el que se va
    bool hayPlan;

    // Variables de estado para el nivel 4
    vector<vector<unsigned char>> mapaAgentes;
    bool heVistoBase;
    vector<Action> planBase;
    bool voyABase;
};

#endif