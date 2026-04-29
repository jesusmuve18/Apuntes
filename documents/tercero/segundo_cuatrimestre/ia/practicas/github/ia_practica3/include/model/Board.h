#ifndef __BOARD_H__
#define __BOARD_H__

# include <vector>
# include <map>
# include <iostream>
# include "Attributes.h"
# include "Piece.h"
# include "BoardTrap.h"

using namespace std;

/**
 * @brief Enumerado para definir distintas configuraciones de tableros.
 *
 */
enum BoardConfig{
   ALL_AT_HOME, // Todas las fichas comienzan en su casa.
   ALL_AT_GOAL, // todas las fichas en meta
   CORRIDORS_ONE_PIECE, // una pieza en cada celda de los pasillos finales
   CORRIDORS_TWO_PIECES, // dos piezas en cada celda de los pasillos finales
   GROUPED, // Grouped legacy con los objetos.
   GROUPED2, // Solo dos fichas ya fuera.
   GROUPED_LEGACY, // Una ficha comienza en la casa, las otras tres ocupan
   // los tres primeros seguros de su color.
   ALTERNED, // Una ficha de cada color comienza en la casa, el resto se van
   // colocando en los seguros alternando los colores.
   ALMOST_GOAL, // Todas las fichas comienzan en el pasillo de la meta (sin utilidad
   // real, solo para facilitar depuración).
   PLAYGROUND, // Tablero para el modo creativo.
   DEBUG, // Para depurar determinadas situaciones (ir modificando según necesidad)
   DRAW_ONE_PIECE, // pinta una pieza en todas las casillas
   DRAW_TWO_PIECES, // pinta dos piezas en todas las casillas
   DRAW_ALL_PIECES, // pinta todas las fichas en todas las posiciones
   TEST_BOO, // Depurar boo
   TEST_BOOM, // BOOM
   TEST_MUSHROOM, // Mushroom
   TEST_SIZES, // Test de tamaños
   CHANGE_SIZE, //Test de cambio de tamaño
};


class Board{
private:
   //Conjunto de todas las piezas del tablero
   //Se usa como identificador el color, y después un vector con las casillas en las que
   //está cada una de las 4 piezas.
   map<color, vector<Piece>> pieces;
   vector<SpecialItem> special_items;
   vector<BoardTrap> traps;

   /**
    * metodo privado de depuracion para probar el pintado de piezas en las celdas
    * normales
    * asocia una pieza a cada casilla de los pasillos normales
    * @param one indica si solo se pinta una o dos
    */
   void drawOnNormalBoxes(bool one);

   /**
    * metodo de depuracion para probar el pintado de las piezas en casa
    * o en meta
    * @param atHome para indicar si se pintan en casa o en la meta
    */
   void drawOnHomeOrGoal(bool atHome);

   /**
    * metodo de depuracion para probar el pintado de las piezas en meta
    */
   void drawOnGoal();

   /**
    * metodo de depuracion para comprobar el pintado de lsa piezas en los
    * corredores finales
    * @param one indica si debe pintarse solo una o dos
    */
   void drawOnCorridors(bool one);

public:
   /**
    * @brief Constructor por defecto
    *
    */
   Board();

   /**
    * @brief Constructor a partir de un objeto Board.
    *
    * @param d
    */
   Board(const map<color, vector<Piece>>& b);

   /**
    * @brief Construye un nuevo tablero a partir de la configuración indicada.
    *
    */
   Board(const BoardConfig& config);

   /**
    * @brief Sobrecarga del operador de igualdad.
    *
    * @param board
    * @return true
    * @return false
    */
   bool operator==(const Board& board) const;

   /**
    * @brief Función que devuelve el Box correspondiente a la ficha
    * en la posición "idx" del vector de fichas de color "c".
    *
    * @param c
    * @param idx
    * @return Box
    */
   const Piece& getPiece(const color c, const int idx) const;

   void setPieceType(const color c, const int idx, const special_type type);

   void setPieceTurnsLeft(const color c, const int idx, const int turns_left);

   void decreasePieceTurnsLeft(const color c, const int idx);

   /**
    * @brief Función que devuelve el vector de Box del color "c".
    *
    * @param c
    * @return Box
    */
   const vector<Piece>& getPieces(const color c) const;

   const vector<SpecialItem>& getSpecialItems() const;

   //Elimina el special item con posicion "pos" en el vector
   void deleteSpecialItem(const int pos);

   const vector<BoardTrap>& getTraps() const;

   //Elimina la trampa con posicion "pos" en el vector
   void deleteTrap(const Box box);

   void addTrap(trap_type type, Box box);

   /**
    * @brief Función que mueve la ficha de la posición "idx" del vector
    * de fichas de color "c" al box "final_box".
    *
    * @param c
    * @param idx
    * @param final_box
    */
   void movePiece(const color c, const int idx, const Box& final_box);

   /**
    * @brief Actualiza el tablero según la configuración especificada.
    *
    * @param config
    */
   void setFromConfig(const BoardConfig& config);
};

#endif
