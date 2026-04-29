#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <unordered_set>
#include <queue>
#include <unordered_map>

#include "comportamientos/comportamiento.hpp"

struct EstadoR
{
    int f;
    int c;
    int brujula;
    bool zapatillas;

    // Sobrecarga del operador '=='
    // Se considerará el mismo estado si coinciden todos los parámetros
    bool operator==(const EstadoR &st) const
    {
        return f == st.f and c == st.c and brujula == st.brujula and zapatillas == st.zapatillas;
    }

    // Sobrecarga del operador '<
    bool operator<(const EstadoR &st) const
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

// Hash para EstadoR
struct HashEstadoR {
    size_t operator()(const EstadoR &e) const {
        return ((std::hash<int>()(e.f) ^
                (std::hash<int>()(e.c) << 1)) >> 1) ^
                (std::hash<int>()(e.brujula) << 1) ^
                (std::hash<bool>()(e.zapatillas) << 2);
    }
};

struct NodoR
{
    EstadoR estado;
    vector<Action> secuencia;
    int costo;
    int estimacion;

    // Sobrecarga del operador '=='
    // Se considerarán iguales si tienen el mismo estado
    bool operator==(const NodoR &node) const
    {
        return estado == node.estado;
    }

    // Sobrecarga del operador '<'
    bool operator<(const NodoR &node) const {
        return estado < node.estado;
    }

    // Sobrecarga del operador '>'
    bool operator>(const NodoR &node) const {
        return costo > node.costo;
    }
};

// Hash para NodoR
struct HashNodoR {
    size_t operator()(const NodoR &n) const {
        return HashEstadoR()(n.estado); // Usa el hash del estado
    }
};

class ComportamientoRescatador : public Comportamiento
{

public:
    ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size), veces_pisada(size, std::vector<int>(size, 0)), mapaAgentes(size, vector<unsigned char>(size, '_'))
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

    ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : 
        Comportamiento(mapaR, mapaC),
        mapaAgentes(mapaR.size(), vector<unsigned char>(mapaR.size(), '_'))
    {
        // Inicializar Variables de Estado Niveles 2,3
        hayPlan = false;
        tiene_zapatillas = false;
        last_action = IDLE;
        paso = 0;

    }

    ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
    ~ComportamientoRescatador() {}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    Action ComportamientoRescatadorNivel_0(Sensores sensores);
    Action ComportamientoRescatadorNivel_1(Sensores sensores);
    Action ComportamientoRescatadorNivel_2(Sensores sensores);
    Action ComportamientoRescatadorNivel_3(Sensores sensores);
    Action ComportamientoRescatadorNivel_4(Sensores sensores);

    // Métodos auxiliares niveles 0 y 1
    int VeoCasillaTransitable(vector<char> superficie, Sensores sensores, bool (*condicion)(char));
    int TomarDecision(bool (*condicion)(char), vector<char> sup, vector<int> vp, int orientacion, Sensores sensores);

    // Métodos auxiliares niveles 2 y 3
    void VisualizaPlan(const EstadoR &st, const vector<Action> &plan);
    vector<Action> Dijkstra(const EstadoR &inicio, const EstadoR &final,
        const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &agentes);

    // Métodos auxiliares nivel 4
    vector<Action> BusquedaR(const EstadoR &inicio, const EstadoR &final,
        const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &agentes, int& costo);
    vector<Action> BusquedaBaseR(const EstadoR &inicio,
            const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &agentes, int& costo);

private:
    // Variables de Estado
    Action last_action;    // Almacena la última acción realizada por el agente
    bool tiene_zapatillas; // Indica si ya pasó por una casilla de tipo zapatillas
    int giro45izq;         // Indica si se está haciendo un TURN_SL (0 indica que no)

    vector<vector<int>> veces_pisada; // Número de veces que he pisado una casilla

    // Variables de estado para el nivel 2 y 3
    vector<Action> plan;
    int paso;
    bool hayPlan;

    // Variables de estado para el nivel 4
    vector<vector<unsigned char>> mapaAgentes;
    bool heVistoBase;
    vector<Action> planBase;
    bool voyABase;
};

#endif