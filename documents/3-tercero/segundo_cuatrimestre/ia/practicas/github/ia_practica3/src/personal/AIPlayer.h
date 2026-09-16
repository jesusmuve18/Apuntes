#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

# include "../../include/model/Attributes.h"
# include "../../include/model/Player.h"
# include "../../include/model/Heuristic.h"


class ValoracionTest: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion1: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion0: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion2: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion3: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion4: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class Valoracion5: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class AIPlayer : public Player{
protected:
   //Id identificativo del jugador
   const int id;

public:
   /**
    * @brief Constructor de un objeto AIPlayer
    *
    * @param name Nombre del jugador
    */
   inline AIPlayer(const string& name): Player(name), id(1){
   };

   /**
    * @brief Constructor de un objeto AIPlayer
    *
    * @param name Nombre  del jugador
    * @param id Id del jugador
    */
   inline AIPlayer(const string& name, const int id): Player(name), id(id){
   };

   /**
    * @brief Función que percibe el el parchís y al jugador actual.
    * Asigna el tablero en actual y el id del jugador.
    *
    * @param p Instancia Parchis que se quiere percibir
    */
   inline virtual void perceive(Parchis& p){ Player::perceive(p); }

   /**
    * @brief Función abstracta que define el movimiento devuelto por el jugador.
    * Llama a la función movePiece con el valor asignado a los parámetros pasados
    * por referencia.
    *
    * @return true
    * @return false
    */
   virtual bool move();

   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    *
    * @param c_piece Color de la ficha
    * @param id_piece Id de la ficha
    * @param dice Número de dado
    */
   virtual void think(color& c_piece, int& id_piece, int& dice) const;

   void thinkAleatorio(color& c_piece, int& id_piece, int& dice) const;

   void thinkFichaMasAdelantada(color& c_piece, int& id_piece, int& dice) const;

   void thinkMejorOpcion(color& c_piece, int& id_piece, int& dice) const;


   /**
    * @brief Método que determina si el player es inteligente (decide el mejor movimiento)
    * o no. True para AIPlayer.
    *
    * @return true
    * @return false
    */
   inline virtual bool canThink() const{ return true; }
   
};

/**
 * @brief Propuesta de declaración de la función poda alfa-beta.
 * La propuesta es solo sugerencia, los parámetros de la declaración podrían variar.
 */
// float Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, float alpha, float beta, Heuristic *heuristic);

float Minimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic);

float Minimax_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic);

float AlphaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta);

float AlphaBeta_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta);

float AlphaBeta_OrdenacionMovimientos(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta);

float AlphaBeta_PodaProbabilistica(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta);

#endif
