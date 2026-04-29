#ifndef NODE_COUNTER_H
#define NODE_COUNTER_H

#include <iostream>
#include <chrono>

class Parchis;
class Heuristic;

class NodeCounter
{
private:
    static const int MAX_NODES = 3500000;
    static const float MAX_TIME; // En segundos.
    // Margen de cortesía que os damos antes de suspenderos
    static const int NODE_MARGIN = 625;
    static const float TIME_MARGIN; // En segundos.

    static NodeCounter *instance;
    int nodesGenerated;
    int nodesEvaluated;

    std::chrono::steady_clock::time_point startTime;

    float accumulatedTime;
    bool running;

    // Constructor privado
    NodeCounter();

    // Métodos privados
    void incrGenerated(int n=1);
    void incrEvaluated(int n=1);
    void resetCounters();
    
    bool limitExceeded() const;
    bool timeExceeded() const;
    bool nodeLimitExceeded() const;
    
    void beginTimer();
    void endTimer();

    // Métodos estáticos de incremento y reseteo (para ser llamados por amiguis <3)
    inline static void incrementGenerated(int n = 1) { getInstance()->incrGenerated(n); }
    inline static void incrementEvaluated(int n = 1) { getInstance()->incrEvaluated(n); }
    inline static void reset() { getInstance()->resetCounters(); }
    inline static bool isLimitExceeded() { return getInstance()->limitExceeded(); }
    inline static bool isNodeLimitExceeded() {return getInstance()->nodeLimitExceeded(); }
    inline static bool isTimeExceeded() {return getInstance()->timeExceeded(); }
    inline static void startTimer() { getInstance()->beginTimer(); }
    inline static void stopTimer(){ getInstance() ->endTimer();}

    // Amiguis :-) <3
    friend class Parchis;
    friend class Heuristic;
    
public:
    // Evitar copia y asignación
    NodeCounter(const NodeCounter &) = delete;
    void operator=(const NodeCounter &) = delete;
    
    // Obtener instancia única
    static NodeCounter *getInstance();
    
    // Funciones públicas
    bool limitReached() const;
    bool nodeLimitReached() const;
    bool timeLimitReached() const;
    int generated() const;
    int evaluated() const;
    float time() const;
    void print_results(std::ostream & out) const;

    // Staticers
    inline static bool isLimitReached(){ return getInstance()->limitReached();}
    inline static bool isNodeLimitReached(){ return getInstance()->nodeLimitReached();}
    inline static bool isTimeLimitReached(){ return getInstance()->timeLimitReached();}
    inline static int getGenerated(){ return getInstance()->generated();}
    inline static int getEvaluated(){ return getInstance()->evaluated();}
    inline static float getTime(){ return getInstance()->time();}
    inline static void print(std::ostream &out) { getInstance()->print_results(out); }
};

#endif // NODE_COUNTER_H
