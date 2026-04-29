#include "AIPlayer.h"
#include "../../include/model/Parchis.h"

// Para leer pesos
#include <fstream>
#include <sstream>

const float masinf = 9999999999.0, menosinf = -9999999999.0;
const float gana = masinf / 10.f, pierde = menosinf / 10.f;
const int num_pieces = 2;
const int PROFUNDIDAD_MINIMAX = 5;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

// Funciones auxiliares

std::vector<float> leerPesos(const std::string &nombre_archivo)
{
   std::ifstream archivo(nombre_archivo);
   std::vector<float> pesos;
   std::string linea;

   if (archivo.is_open())
   {
      if (std::getline(archivo, linea))
      {
         std::stringstream ss(linea);
         std::string valor;
         while (std::getline(ss, valor, ','))
         {
            pesos.push_back(std::stof(valor));
         }
      }
      archivo.close();
   }
   else
   {
      std::cerr << "No se pudo abrir el archivo de pesos.\n";
   }

   return pesos;
}

std::string toString(const ParchisSis &v)
{
   string res = "(";

   res += to_string(v.getMovedColor()) + "," + to_string(v.getMovedDiceValue()) + "," + to_string(v.getMovedPieceId());

   return res + ")";
}

template <typename T>
std::string toString(const std::vector<T> &v)
{
   string res = "{";
   for (auto it = v.begin(); it != v.end(); it++)
   {
      res += toString(*it) + ",";
   }

   return res + "}";
}

bool AIPlayer::move()
{
   cout << COUT_ORANGE_BOLD << "Realizo un movimiento automatico" << COUT_NOCOLOR << endl;

   color c_piece;
   int id_piece;
   int dice;
   think(c_piece, id_piece, dice);

   cout << COUT_ORANGE_BOLD << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << COUT_NOCOLOR << endl;

   actual->movePiece(c_piece, id_piece, dice);
   return true;
}

// OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
// respectivamente, de:
// - color de ficha a mover
// - identificador de la ficha que se va a mover
// - valor del dado con el que se va a mover la ficha.

/**
 * @brief Función que se encarga de decidir el mejor movimiento posible a
 * partir del estado actual del tablero. Asigna a las variables pasadas por
 * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
 *
 * @param c_piece Color de la ficha
 * @param id_piece Id de la ficha
 * @param dice Número de dado
 */
void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{
   float valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda

   // Defino las heurísticas que quiera usar.
   ValoracionTest valoracionTest;

   Valoracion0 valoracion0;
   Valoracion1 valoracion1;
   Valoracion2 valoracion2;
   Valoracion3 valoracion3;
   Valoracion4 valoracion4;
   Valoracion5 valoracion5;

   switch (id)
   {
   case 0:
      // Implementación básica del alpha-beta con valoracionTest
      valor = AlphaBeta_Limitado(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracionTest, menosinf, masinf);
      break;
   case 1:
      // Mejor heurística y versión de poda
      // Gana a ninja2
      valor = AlphaBeta_OrdenacionMovimientos(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion1, menosinf, masinf);
      break;
   case 2:
      // Gana a ninja1
      valor = AlphaBeta_OrdenacionMovimientos(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion0, menosinf, masinf);
      break;
   case 3:
      // Gana a ninja2
      valor = AlphaBeta_OrdenacionMovimientos(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion1, menosinf, masinf);
      break;
   case 4:
      // Coge los pesos de pesos.csv (para algoritmo genético)
      valor = AlphaBeta_OrdenacionMovimientos(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion3, menosinf, masinf);
      break;
   case 5:
      // Coge los pesos de pesos2.csv (para algoritmo genético)
      valor = AlphaBeta_OrdenacionMovimientos(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion4, menosinf, masinf);
      break;
   case 6:
      // Pueba de alpha-beta con poda probabilística
      valor = AlphaBeta_PodaProbabilistica(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion2, menosinf, masinf);
      break;
   case 7:
      // Resultado algoritmo genético
      valor = AlphaBeta_PodaProbabilistica(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracion5, menosinf, masinf);
      break;
   }
   cout << "Valor Elegido: " << valor << " Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
}

/******************************************
 *   _____      _             _       _   *
 *  |_   _|   _| |_ ___  _ __(_) __ _| |  *
 *    | || | | | __/ _ \| '__| |/ _` | |  *
 *    | || |_| | || (_) | |  | | (_| | |  *
 *    |_| \__,_|\__\___/|_|  |_|\__,_|_|  *
 *                                        *
 *****************************************/

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
   // El id de mi jugador actual.
   int player = actual->getCurrentPlayerId();

   // Vector que almacenará los dados que se pueden usar para el movimiento
   vector<int> current_dices;
   // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
   vector<tuple<color, int>> current_pieces;

   // Se obtiene el vector de dados que se pueden usar para el movimiento
   current_dices = actual->getAvailableNormalDices(player);

   // Elijo un dado de forma aleatoria.
   dice = current_dices[rand() % current_dices.size()];

   // Se obtiene el vector de fichas que se pueden mover para el dado elegido
   current_pieces = actual->getAvailablePieces(player, dice);

   // Si tengo fichas para el dado elegido muevo una al azar.
   if (current_pieces.size() > 0)
   {
      int random_id = rand() % current_pieces.size();
      id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
      c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
   }
   else
   {
      // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }

   // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.
   // float valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
   // float alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
   // // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
   // ValoracionTest valoracionTest;
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{

   // Elijo el dado haciendo lo mismo que el jugador aleatorio
   thinkAleatorio(c_piece, id_piece, dice);

   // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.

   // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
   // el dado especial a) la que esté más adelantada
   // (equivalentemente, la más cercana a la meta).

   int player = actual->getCurrentPlayerId();
   vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);

   int id_ficha_mas_adelantada = -1;
   color col_ficha_mas_adelantada = none;
   int min_distancia_meta = 9999;

   for (int i = 0; i < current_pieces.size(); i++)
   {
      // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
      color col = get<0>(current_pieces[i]);
      int id = get<1>(current_pieces[i]);
      int distancia_meta = actual->distanceToGoal(col, id);
      if (distancia_meta < min_distancia_meta)
      {
         min_distancia_meta = distancia_meta;
         id_ficha_mas_adelantada = id;
         col_ficha_mas_adelantada = col;
      }
   }
   // Si no he encontrado ninguna ficha, paso turno.
   if (id_ficha_mas_adelantada == -1)
   {
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }
   // En caso contrario, moveré la ficha más adelantada.
   else
   {
      id_piece = id_ficha_mas_adelantada;
      c_piece = col_ficha_mas_adelantada;
   }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{
   // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.

   // Cuando ya he recorrido todos los hijos, la función devuelve el estado actual. De esta forma puedo saber
   // cuándo paro de iterar

   // Para ello vamos a iterar sobre los hijos con la función de Parchis getChildren().
   // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
   // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.

   ParchisBros hijos = actual->getChildren();

   bool me_quedo_con_esta_accion = false;

   // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
   // Al hacer begin() accedemos al primer hijo de la rama,
   // y cada vez que hagamos ++it saltaremos al siguiente hijo.
   // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.

   for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end() and !me_quedo_con_esta_accion; ++it)
   {
      Parchis siguiente_hijo = *it;

      // cout<<"---------------------------------------------------"<<endl;
      // cout<<"Color: "<<it.getMovedColor()<<endl;
      // cout<<"Pieza: "<<it.getMovedPieceId()<<endl;
      // cout<<"Dado: "<<it.getMovedDiceValue()<<endl;

      // Si en el movimiento elegido comiera ficha, llegara a la meta o ganara, me quedo con esa accion.

      // Termino el bucle en este caso
      if (siguiente_hijo.isEatingMove() or
          siguiente_hijo.isGoalMove() or
          (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador))
      {
         me_quedo_con_esta_accion = true;
         c_piece = it.getMovedColor();
         id_piece = it.getMovedPieceId();
         dice = it.getMovedDiceValue();
      }
   }

   // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
   // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
   // Si no he encontrado ninguna acción que me interese, voy a mover la ficha más adelantada como en el caso anterior.

   if (!me_quedo_con_esta_accion)
   {
      thinkFichaMasAdelantada(c_piece, id_piece, dice);
   }
}

float ValoracionTest::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }
   else if (ganador == oponente)
   {
      return pierde;
   }
   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;

      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++)
      {
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_jugador++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_jugador += 5;
            }
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;

      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++)
      {
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            if (estado.isSafePiece(c, j))
            {
               // Valoro negativamente que la ficha esté en casilla segura o meta.
               puntuacion_oponente++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
            {
               puntuacion_oponente += 5;
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
}

float Minimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
              color &c_piece, int &id_piece, int &dice, Heuristic *heuristic)
{
   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;
         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                 heuristic);
         if (new_val > valor)
         {
            // Me voy quedando con el máximo
            valor = new_val;
            if (profundidad == 0)
            {
               // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;
         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                 heuristic);
         // Me voy quedando con el mínimo
         if (new_val < valor)
         {
            valor = new_val;
         }
      }
      return valor;
   }
}

float Minimax_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                       color &c_piece, int &id_piece, int &dice, Heuristic *heuristic)
{
   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }
   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;

         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

            if (profundidad == 0)
            {
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }

            return valor;
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                          heuristic);
         if (new_val > valor)
         {
            // Me voy quedando con el máximo
            valor = new_val;
            if (profundidad == 0)
            {
               // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
               c_piece = it.getMovedColor();
               id_piece = it.getMovedPieceId();
               dice = it.getMovedDiceValue();
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         Parchis nuevo_hijo = *it;

         if (NodeCounter::isLimitReached())
         {
            cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

            return heuristic->evaluate(actual, jugador);
         }

         // Búsqueda en profundidad (llamada recursiva)
         float new_val = Minimax_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                          heuristic);
         // Me voy quedando con el mínimo
         if (new_val < valor)
         {
            valor = new_val;
         }
      }
      return valor;
   }
}

/********************************************************************
 *    ____  __      _ _               ____                  _        *
 *   / ___|/_/   __| (_) __ _  ___   |  _ \ _ __ ___  _ __ (_) ___   *
 *  | |   / _ \ / _` | |/ _` |/ _ \  | |_) | '__/ _ \| '_ \| |/ _ \  *
 *  | |__| (_) | (_| | | (_| | (_) | |  __/| | | (_) | |_) | | (_) | *
 *   \____\___/ \__,_|_|\__, |\___/  |_|   |_|  \___/| .__/|_|\___/  *
 *                      |___/                        |_|             *
 ********************************************************************/

// Ordenación por heurística
void Ordenar(std::vector<ParchisSis> &nodos, int jugador, Heuristic *heuristic)
{
   std::sort(nodos.begin(), nodos.end(),
             [=](const ParchisSis &a, const ParchisSis &b)
             {
                return heuristic->evaluate(*a, jugador) > heuristic->evaluate(*b, jugador);
             });
}

// Alpha Beta
float AlphaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta)
{

   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }

   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {
            Parchis nuevo_hijo = *it;
            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                      heuristic, alpha, beta);
            if (new_val > valor)
            {
               // Me voy quedando con el máximo
               valor = new_val;
               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }
            }

            // Actualizo el alpha
            if (valor > alpha)
            {
               alpha = valor;
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo

      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {

            Parchis nuevo_hijo = *it;

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                      heuristic, alpha, beta);

            // Me voy quedando con el mínimo
            if (new_val < valor)
            {
               valor = new_val;
            }

            // Actualizo beta
            if (valor < beta)
            {
               beta = valor;
            }
         }
      }

      return valor;
   }
}

float AlphaBeta_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                         color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta)
{

   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }

   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();
      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {
            Parchis nuevo_hijo = *it;

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                               heuristic, alpha, beta);
            if (new_val > valor)
            {
               // Me voy quedando con el máximo
               valor = new_val;
               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }
            }

            // Actualizo el alpha
            if (valor > alpha)
            {
               alpha = valor;
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo

      // Obtengo los hijos del nodo actual y los recorro
      ParchisBros rama = actual.getChildren();

      for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {

            Parchis nuevo_hijo = *it;

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                               heuristic, alpha, beta);

            // Me voy quedando con el mínimo
            if (new_val < valor)
            {
               valor = new_val;
            }

            // Actualizo beta
            if (valor < beta)
            {
               beta = valor;
            }
         }
      }

      return valor;
   }
}

float AlphaBeta_OrdenacionMovimientos(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                                      color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta)
{

   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }

   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo

      // Obtengo todos los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      // cout<<"Rama sin ordenar: " << toString(rama) << endl;

      Ordenar(rama, jugador, heuristic);

      // cout<<"Rama ordenada: " << toString(rama) << endl;

      for (auto it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {
            Parchis nuevo_hijo = *(*it);

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_OrdenacionMovimientos(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                                            heuristic, alpha, beta);
            if (new_val > valor)
            {
               // Me voy quedando con el máximo
               valor = new_val;
               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }
            }

            // Actualizo el alpha
            if (valor > alpha)
            {
               alpha = valor;
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo

      // Obtengo todos los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      Ordenar(rama, jugador, heuristic);

      for (auto it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta)
         {

            Parchis nuevo_hijo = *(*it);

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_OrdenacionMovimientos(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                                            heuristic, alpha, beta);

            // Me voy quedando con el mínimo
            if (new_val < valor)
            {
               valor = new_val;
            }

            // Actualizo beta
            if (valor < beta)
            {
               beta = valor;
            }
         }
      }

      return valor;
   }
}

float AlphaBeta_PodaProbabilistica(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                                      color &c_piece, int &id_piece, int &dice, Heuristic *heuristic, float alpha, float beta)
{

   const int UMBRAL = 10;

   if (profundidad == profundidad_max || actual.gameOver())
   { // Nodo terminal o profundidad límite: llamo a la función heurística
      // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
      return heuristic->evaluate(actual, jugador);
   }

   // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
   else if (actual.getCurrentPlayerId() == jugador)
   {                          // Nodo MAX
      float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo

      // Obtengo todos los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      for (auto it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         // Le pongo un umbral
         if (alpha < beta + UMBRAL)
         {
            Parchis nuevo_hijo = *(*it);

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_PodaProbabilistica(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                                            heuristic, alpha, beta);
            if (new_val > valor)
            {
               // Me voy quedando con el máximo
               valor = new_val;
               if (profundidad == 0)
               {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }
            }

            // Actualizo el alpha
            if (valor > alpha)
            {
               alpha = valor;
            }
         }
      }
      return valor;
   }
   else
   {                        // Nodo MIN
      float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo

      // Obtengo todos los hijos del nodo actual
      vector<ParchisSis> rama = actual.getChildrenList();

      for (auto it = rama.begin(); it != rama.end(); ++it)
      {
         // Si no se cumple la condición de poda sigo con la rama
         if (alpha < beta + UMBRAL)
         {

            Parchis nuevo_hijo = *(*it);

            if (NodeCounter::isLimitReached())
            {
               cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << endl;

               if (profundidad == 0)
               {
                  c_piece = (*it).getMovedColor();
                  id_piece = (*it).getMovedPieceId();
                  dice = (*it).getMovedDiceValue();
               }

               return valor;
            }

            // Búsqueda en profundidad (llamada recursiva)
            float new_val = AlphaBeta_PodaProbabilistica(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice,
                                                            heuristic, alpha, beta);

            // Me voy quedando con el mínimo
            if (new_val < valor)
            {
               valor = new_val;
            }

            // Actualizo beta
            if (valor < beta)
            {
               beta = valor;
            }
         }
      }

      return valor;
   }
}


/*****************************************************
 *   _   _                 __    _   _                *
 *  | | | | ___ _   _ _ __/_/___| |_(_) ___ __ _ ___  *
 *  | |_| |/ _ \ | | | '__| / __| __| |/ __/ _` / __| *
 *  |  _  |  __/ |_| | |  | \__ \ |_| | (_| (_| \__ \ *
 *  |_| |_|\___|\__,_|_|  |_|___/\__|_|\___\__,_|___/ *
 *                                                    *
 *****************************************************/

// Vence ninja 1
float Valoracion0::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;

      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++)
      {
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_jugador += 10;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_jugador += 50;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_jugador -= 20;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_jugador += 15;
            }

            puntuacion_jugador += 70 - estado.distanceToGoal(c, j);
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;

      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++)
      {
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_oponente += 10;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_oponente += 50;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_oponente -= 20;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_oponente += 15;
            }

            puntuacion_oponente += 70 - estado.distanceToGoal(c, j);
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
   return 1;
}

// Vence ninja 2
float Valoracion1::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;

      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++)
      {
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_jugador += 10;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_jugador += 20; // Recompenso haber ganado pero no mucho más que estar en la recta final
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_jugador -= 17.5; // Un poco peor que avanzar 20 (es muy rápido)
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_jugador += 15; // Un poco más que casilla segura
            }

            if (estado.isEatingMove())
            {
               // se obtienen las piezas comidas con la ultima accion
               pair<color, int> piezaComida = estado.eatenPiece();

               // se determina la distancia a meta de la pieza comida
               int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

               // se determina si es de uno de los colores propios
               vector<color> colores = estado.getPlayerColors(jugador);

               // se comprueba si color de la pieza comida es de los propios
               bool propia = false;

               for (int i = 0; i < colores.size() && !propia; i++)
               {
                  if (colores.at(i) == piezaComida.first)
                  {
                     propia = true;
                  }
               }

               if (!propia)
               {
                  puntuacion_jugador += (70 - distancia) / 3.5; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
            }

            puntuacion_jugador += 70 - estado.distanceToGoal(c, j);
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;

      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++)
      {
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_oponente += 10;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_oponente += 20;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_oponente -= 17.5;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_oponente += 15;
            }

            if (estado.isEatingMove())
            {
               // se obtienen las piezas comidas con la ultima accion
               pair<color, int> piezaComida = estado.eatenPiece();

               // se determina la distancia a meta de la pieza comida
               int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

               // se determina si es de uno de los colores propios
               vector<color> colores = estado.getPlayerColors(jugador);

               // se comprueba si color de la pieza comida es de los propios
               bool propia = false;

               for (int i = 0; i < colores.size() && !propia; i++)
               {
                  if (colores.at(i) == piezaComida.first)
                  {
                     propia = true;
                  }
               }

               if (!propia)
               {
                  puntuacion_oponente += (70 - distancia) / 3.5; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
            }

            puntuacion_oponente += 70 - estado.distanceToGoal(c, j);
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
   return 1;
}

float Valoracion2::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Incrementos por cada métrica
   const float beneficio_distancia = 20;        // Penalización/Ganancia por distancia
   const float beneficio_distancia_comida = 15; // Penalización/Ganancia por distancia

   const float ficha_en_casa = -30;
   const float ficha_en_casilla_segura = 25;
   const float ficha_en_recta_final = 30;
   const float ficha_en_casilla_final = 50;
   const float rebote = -10;
   const float barrera = 20;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;

      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++)
      {
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_jugador += ficha_en_casilla_segura;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_jugador += ficha_en_casilla_final; // Recompenso haber ganado pero no mucho más que estar en la recta final
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_jugador += ficha_en_casa; // Un poco peor que avanzar 20 (es muy rápido)
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_jugador += ficha_en_recta_final; // Un poco más que casilla segura
            }

            // Si al mover me como una ficha
            if (estado.isEatingMove())
            {

               // se obtienen las piezas comidas con la ultima accion
               pair<color, int> piezaComida = estado.eatenPiece();

               // se determina la distancia a meta de la pieza comida
               int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

               // se comprueba si color de la pieza comida es de los propios
               bool propia = false;

               for (int i = 0; i < my_colors.size() && !propia; i++)
               {
                  if (my_colors.at(i) == piezaComida.first)
                  {
                     propia = true;
                  }
               }

               if (!propia)
               {
                  // La función distancia tenemos que es d : F -> [0,72]
                  // Lo interpolamos para tener d_2 : F -> [0,K]
                  //
                  // Para ello tenemos d(x) = ax + b para todo x\in F
                  // Queremos d(0) = K y d(72) = 0
                  // por tanto b=K y a(72)+K = 0 => a = -K/72
                  // Y esto nos da d(x) = (-K/72)x + K
                  puntuacion_jugador += (-beneficio_distancia_comida / 72) * distancia + beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
               else
               {
                  puntuacion_jugador -= (-beneficio_distancia_comida / 72) * distancia + beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
            }

            // La función distancia tenemos que es d : F -> [0,72]
            // Lo interpolamos para tener d_2 : F -> [-K, K]
            //
            // Para ello tenemos d(x) = ax + b para todo x\in F
            // Queremos d(0) = K y d(72) = -K
            // por tanto b=K y a(72)+K = -K => a = -2K/72 = -K/36
            // Y esto nos da d(x) = (-K/36)x + K

            puntuacion_jugador += (-beneficio_distancia / 36) * estado.distanceToGoal(c, j) + beneficio_distancia;

            if (estado.goalBounce() and estado.piecesAtGoal(c) == 0)
            {
               puntuacion_jugador += rebote;
            }

            if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
            {
               puntuacion_jugador += barrera;
            }
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;

      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++)
      {
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++)
         {
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)) // Casilla segura
            {
               puntuacion_oponente += ficha_en_casilla_segura;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
            {
               puntuacion_oponente += ficha_en_casilla_final;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
            {
               puntuacion_oponente -= ficha_en_casa;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
            {
               puntuacion_oponente += ficha_en_recta_final;
            }

            if (estado.isEatingMove())
            {
               // se obtienen las piezas comidas con la ultima accion
               pair<color, int> piezaComida = estado.eatenPiece();

               // se determina la distancia a meta de la pieza comida
               int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

               // se comprueba si color de la pieza comida es de los propios
               bool propia = false;

               for (int i = 0; i < op_colors.size() && !propia; i++)
               {
                  if (op_colors.at(i) == piezaComida.first)
                  {
                     propia = true;
                  }
               }

               if (!propia)
               {
                  puntuacion_oponente += (-beneficio_distancia_comida / 72) * distancia + beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
               else
               {
                  puntuacion_oponente -= (-beneficio_distancia_comida / 72) * distancia + beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
               }
            }

            puntuacion_oponente += (-beneficio_distancia / 36) * estado.distanceToGoal(c, j) + beneficio_distancia;

            if (estado.goalBounce() and estado.piecesAtGoal(c) == 0)
            {
               puntuacion_oponente += rebote;
            }

            if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
            {
               puntuacion_oponente += barrera;
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
   return 1;
}

float Valoracion3::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Incrementos por cada métrica
   // const float beneficio_distancia = 15;            // Penalización/Ganancia por distancia
   // const float max_beneficio_distancia_comida = 35; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   // const float min_beneficio_distancia_comida = 20; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)

   // const float ficha_en_casa = -25;
   // const float ficha_en_casilla_segura = 25;
   // const float ficha_en_recta_final = 30;
   // const float ficha_en_casilla_final = 50;
   // const float rebote = -10; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   // const float barrera = 25;

   // Leo los pesos del archivo
   std::vector<float> pesos = leerPesos("pesos.csv");

   const float max_beneficio_distancia = pesos[0]; // Penalización/Ganancia por distancia
   const float min_beneficio_distancia = pesos[1];
   const float max_beneficio_distancia_comida = pesos[2]; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   const float min_beneficio_distancia_comida = pesos[3]; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)
   const float comer_propia = (pesos[4] / 100);

   const float ficha_en_casa = pesos[5];
   const float ficha_en_casilla_segura = pesos[6];
   const float ficha_en_recta_final = pesos[7];
   const float ficha_en_casilla_final = pesos[8];
   const float rebote = pesos[9]; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   const float barrera = pesos[10];

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      vector<vector<color>> colors{my_colors, op_colors};

      
      vector<int> puntuaciones = {0, 0};

      // Recorro todas las fichas de ambos jugadores
      for (int k = 0; k < puntuaciones.size(); k++)
      {
         // Recorro colores de mi jugador.
         for (int i = 0; i < colors[k].size(); i++)
         {
            color c = colors[k][i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
               // Valoro positivamente que la ficha esté en casilla segura o meta.
               if (estado.isSafePiece(c, j)) // Casilla segura
               {
                  puntuaciones[k] += ficha_en_casilla_segura;
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
               {
                  puntuaciones[k] += ficha_en_casilla_final; // Recompenso haber ganado pero no mucho más que estar en la recta final
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
               {
                  puntuaciones[k] += ficha_en_casa; // Un poco peor que avanzar 20 (es muy rápido)
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
               {
                  puntuaciones[k] += ficha_en_recta_final; // Un poco más que casilla segura
               }

               // Si al mover me como una ficha
               if (estado.isEatingMove())
               {

                  // se obtienen las piezas comidas con la ultima accion
                  pair<color, int> piezaComida = estado.eatenPiece();

                  // se determina la distancia a meta de la pieza comida
                  int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

                  // se comprueba si color de la pieza comida es de los propios
                  bool propia = false;

                  for (int i = 0; i < colors[k].size() && !propia; i++)
                  {
                     if (colors[k].at(i) == piezaComida.first)
                     {
                        propia = true;
                     }
                  }

                  if (!propia)
                  {
                     // La función distancia tenemos que es d : F -> [0,62]
                     // Lo interpolamos para tener d_2 : F -> [L,K]
                     //
                     // Para ello tenemos d(x) = ax + b para todo x\in F
                     // Queremos d(0) = K y d(62) = L
                     // por tanto b=K y a(62)+K = 20 => a = (-K+L)/62
                     // Y esto nos da d(x) = ((-K+L)/62)x + K
                     puntuaciones[k] += ((min_beneficio_distancia_comida - max_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
                  else
                  {
                     puntuaciones[k] += comer_propia * ((-max_beneficio_distancia_comida + min_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
               }

               // La función distancia tenemos que es d : F -> [0,72]
               // Lo interpolamos para tener d_2 : F -> [0, K]
               //
               // Para ello tenemos d(x) = ax + b para todo x\in F
               // Queremos d(0) = K y d(72) = 0
               // por tanto b=K y a(72)+K = 0 => a = -K/72 = -K/72
               // Y esto nos da d(x) = (-K/72)x + K

               puntuaciones[k] += ((min_beneficio_distancia - max_beneficio_distancia) / 72) * (estado.distanceToGoal(c, j)) + max_beneficio_distancia;

               if (estado.goalBounce() and estado.piecesAtGoal(c) == 0)
               {
                  puntuaciones[k] += rebote * estado.distanceToGoal(c, j);
               }

               if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
               {
                  puntuaciones[k] += barrera;
               }
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuaciones[0]-puntuaciones[1];
   }
   return 1;
}

float Valoracion4::getHeuristic(const Parchis &estado, int jugador) const
{
  // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Incrementos por cada métrica
   // const float beneficio_distancia = 15;            // Penalización/Ganancia por distancia
   // const float max_beneficio_distancia_comida = 35; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   // const float min_beneficio_distancia_comida = 20; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)

   // const float ficha_en_casa = -25;
   // const float ficha_en_casilla_segura = 25;
   // const float ficha_en_recta_final = 30;
   // const float ficha_en_casilla_final = 50;
   // const float rebote = -10; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   // const float barrera = 25;

   // Leo los pesos del archivo
   std::vector<float> pesos = leerPesos("pesos2.csv");

   const float max_beneficio_distancia = pesos[0]; // Penalización/Ganancia por distancia
   const float min_beneficio_distancia = pesos[1];
   const float max_beneficio_distancia_comida = pesos[2]; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   const float min_beneficio_distancia_comida = pesos[3]; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)
   const float comer_propia = (pesos[4] / 100);

   const float ficha_en_casa = pesos[5];
   const float ficha_en_casilla_segura = pesos[6];
   const float ficha_en_recta_final = pesos[7];
   const float ficha_en_casilla_final = pesos[8];
   const float rebote = pesos[9]; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   const float barrera = pesos[10];

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      vector<vector<color>> colors{my_colors, op_colors};

      
      vector<int> puntuaciones = {0, 0};

      // Recorro todas las fichas de ambos jugadores
      for (int k = 0; k < puntuaciones.size(); k++)
      {
         // Recorro colores de mi jugador.
         for (int i = 0; i < colors[k].size(); i++)
         {
            color c = colors[k][i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
               // Valoro positivamente que la ficha esté en casilla segura o meta.
               if (estado.isSafePiece(c, j)) // Casilla segura
               {
                  puntuaciones[k] += ficha_en_casilla_segura;
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
               {
                  puntuaciones[k] += ficha_en_casilla_final; // Recompenso haber ganado pero no mucho más que estar en la recta final
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
               {
                  puntuaciones[k] += ficha_en_casa; // Un poco peor que avanzar 20 (es muy rápido)
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
               {
                  puntuaciones[k] += ficha_en_recta_final; // Un poco más que casilla segura
               }

               // Si al mover me como una ficha
               if (estado.isEatingMove())
               {

                  // se obtienen las piezas comidas con la ultima accion
                  pair<color, int> piezaComida = estado.eatenPiece();

                  // se determina la distancia a meta de la pieza comida
                  int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

                  // se comprueba si color de la pieza comida es de los propios
                  bool propia = false;

                  for (int i = 0; i < colors[k].size() && !propia; i++)
                  {
                     if (colors[k].at(i) == piezaComida.first)
                     {
                        propia = true;
                     }
                  }

                  if (!propia)
                  {
                     // La función distancia tenemos que es d : F -> [0,62]
                     // Lo interpolamos para tener d_2 : F -> [L,K]
                     //
                     // Para ello tenemos d(x) = ax + b para todo x\in F
                     // Queremos d(0) = K y d(62) = L
                     // por tanto b=K y a(62)+K = 20 => a = (-K+L)/62
                     // Y esto nos da d(x) = ((-K+L)/62)x + K
                     puntuaciones[k] += ((min_beneficio_distancia_comida - max_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
                  else
                  {
                     puntuaciones[k] += comer_propia * ((-max_beneficio_distancia_comida + min_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
               }

               // La función distancia tenemos que es d : F -> [0,72]
               // Lo interpolamos para tener d_2 : F -> [0, K]
               //
               // Para ello tenemos d(x) = ax + b para todo x\in F
               // Queremos d(0) = K y d(72) = 0
               // por tanto b=K y a(72)+K = 0 => a = -K/72 = -K/72
               // Y esto nos da d(x) = (-K/72)x + K

               puntuaciones[k] += ((min_beneficio_distancia - max_beneficio_distancia) / 72) * (estado.distanceToGoal(c, j)) + max_beneficio_distancia;

               if (estado.goalBounce() and estado.piecesAtGoal(c) == 0)
               {
                  puntuaciones[k] += rebote * estado.distanceToGoal(c, j);
               }

               if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
               {
                  puntuaciones[k] += barrera;
               }
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuaciones[0]-puntuaciones[1];
   }
   return 1;
}

// Resultado del algoritmo genético
float Valoracion5::getHeuristic(const Parchis &estado, int jugador) const
{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.

   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Incrementos por cada métrica
   // const float beneficio_distancia = 15;            // Penalización/Ganancia por distancia
   // const float max_beneficio_distancia_comida = 35; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   // const float min_beneficio_distancia_comida = 20; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)

   // const float ficha_en_casa = -25;
   // const float ficha_en_casilla_segura = 25;
   // const float ficha_en_recta_final = 30;
   // const float ficha_en_casilla_final = 50;
   // const float rebote = -10; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   // const float barrera = 25;

   // Leo los pesos del archivo
   std::vector<float> pesos = leerPesos("pesos.csv");

   // 46.42,46.31,15.98,19.72,-16.81,-93.31,26.39,0,83.44,-9.76,74.38

   const float max_beneficio_distancia = 46.42; // Penalización/Ganancia por distancia
   const float min_beneficio_distancia = 46.31;
   const float max_beneficio_distancia_comida = 15.98; // Ganancia máxima por comerse una ficha (cuanto más cerca de la meta mejor)
   const float min_beneficio_distancia_comida = 19.72; // Ganancia mínima por comerse una ficha (cuanto más lejos de la meta peor)
   const float comer_propia = (-16.81 / 100);

   const float ficha_en_casa = -93.31;
   const float ficha_en_casilla_segura = 26.39;
   const float ficha_en_recta_final = 0;
   const float ficha_en_casilla_final = 83.44;
   const float rebote = -9.76; // Factor de multiplicación de rebote con el número de movimientos hacia atrás (cuanto más rebote más movimientos se pierden)
   const float barrera = 74.38;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador)
   {
      return gana;
   }

   else if (ganador == oponente)
   {
      return pierde;
   }

   else
   {
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      vector<vector<color>> colors{my_colors, op_colors};

      
      vector<int> puntuaciones = {0, 0};

      // Recorro todas las fichas de ambos jugadores
      for (int k = 0; k < puntuaciones.size(); k++)
      {
         // Recorro colores de mi jugador.
         for (int i = 0; i < colors[k].size(); i++)
         {
            color c = colors[k][i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
               // Valoro positivamente que la ficha esté en casilla segura o meta.
               if (estado.isSafePiece(c, j)) // Casilla segura
               {
                  puntuaciones[k] += ficha_en_casilla_segura;
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == goal) // Meta
               {
                  puntuaciones[k] += ficha_en_casilla_final; // Recompenso haber ganado pero no mucho más que estar en la recta final
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == home) // Casa
               {
                  puntuaciones[k] += ficha_en_casa; // Un poco peor que avanzar 20 (es muy rápido)
               }
               else if (estado.getBoard().getPiece(c, j).get_box().type == final_queue) // Recta final
               {
                  puntuaciones[k] += ficha_en_recta_final; // Un poco más que casilla segura
               }

               // Si al mover me como una ficha
               if (estado.isEatingMove())
               {

                  // se obtienen las piezas comidas con la ultima accion
                  pair<color, int> piezaComida = estado.eatenPiece();

                  // se determina la distancia a meta de la pieza comida
                  int distancia = estado.distanceToGoal(piezaComida.first, piezaComida.second);

                  // se comprueba si color de la pieza comida es de los propios
                  bool propia = false;

                  for (int i = 0; i < colors[k].size() && !propia; i++)
                  {
                     if (colors[k].at(i) == piezaComida.first)
                     {
                        propia = true;
                     }
                  }

                  if (!propia)
                  {
                     // La función distancia tenemos que es d : F -> [0,62]
                     // Lo interpolamos para tener d_2 : F -> [L,K]
                     //
                     // Para ello tenemos d(x) = ax + b para todo x\in F
                     // Queremos d(0) = K y d(62) = L
                     // por tanto b=K y a(62)+K = 20 => a = (-K+L)/62
                     // Y esto nos da d(x) = ((-K+L)/62)x + K
                     puntuaciones[k] += ((min_beneficio_distancia_comida - max_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
                  else
                  {
                     puntuaciones[k] += comer_propia * ((-max_beneficio_distancia_comida + min_beneficio_distancia_comida) / 62) * distancia + max_beneficio_distancia_comida; // Le sumo una cantidad entre 0 y 20 (no incluídos)
                  }
               }

               // La función distancia tenemos que es d : F -> [0,72]
               // Lo interpolamos para tener d_2 : F -> [0, K]
               //
               // Para ello tenemos d(x) = ax + b para todo x\in F
               // Queremos d(0) = K y d(72) = 0
               // por tanto b=K y a(72)+K = 0 => a = -K/72 = -K/72
               // Y esto nos da d(x) = (-K/72)x + K

               puntuaciones[k] += ((min_beneficio_distancia - max_beneficio_distancia) / 72) * (estado.distanceToGoal(c, j)) + max_beneficio_distancia;

               if (estado.goalBounce() and estado.piecesAtGoal(c) == 0)
               {
                  puntuaciones[k] += rebote * estado.distanceToGoal(c, j);
               }

               if (estado.isWall(estado.getBoard().getPiece(c, j).get_box()))
               {
                  puntuaciones[k] += barrera;
               }
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuaciones[0]-puntuaciones[1];
   }
   return 1;
}
