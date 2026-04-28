# include "../../include/model/Board.h"

Board::Board(){
   this->setFromConfig(ALL_AT_HOME);
}


Board::Board(const map<color, vector<Piece>>& b){
   this->pieces = map<color, vector<Piece>>(b);
}

Board::Board(const BoardConfig& config){
   this->setFromConfig(config);
}

bool Board::operator==(const Board& board) const{
   return this->pieces == board.pieces;
}

const Piece& Board::getPiece(const color c, const int idx) const{
   return pieces.at(c)[idx];
}

void Board::setPieceType(const color c, const int idx, const special_type type){
   pieces.at(c)[idx].set_type(type);
}

void Board::setPieceTurnsLeft(const color c, const int idx, const int turns_left){
   pieces.at(c)[idx].set_turns_left(turns_left);
}

void Board::decreasePieceTurnsLeft(const color c, const int idx){
   pieces.at(c)[idx].set_turns_left(pieces.at(c)[idx].get_turns_left() - 1);
   if (pieces.at(c)[idx].get_turns_left() < 0)
      pieces.at(c)[idx].set_turns_left(0);
}


const vector<Piece>& Board::getPieces(const color c) const{
   return pieces.at(c);
}

const vector<SpecialItem>& Board::getSpecialItems() const{
   return special_items;
}

void Board::deleteSpecialItem(const int pos){
   special_items.erase(special_items.begin() + pos);
}

const vector<BoardTrap>& Board::getTraps() const{
   return traps;
}

//Elimina la trampa con posicion "pos" en el vector
void Board::deleteTrap(const Box box){
   //Elimina la trampa de la box "box"
   int pos = -1;
   for (int i = 0; i < traps.size(); i++){
      if (traps[i].getBox() == box){
         pos = i;
         break;
      }
   }
   traps.erase(traps.begin() + pos);
}

void Board::addTrap(trap_type type, Box box){
   traps.push_back(BoardTrap(type, box));
}

void Board::movePiece(const color c, const int idx, const Box& final_box){
   pieces[c][idx].set_box(final_box);
}

void Board::setFromConfig(const BoardConfig& config){

   //Definimos las siguientes configuraciones especificando en qué casillas empiezan cada
   //una de las piezas.
   switch (config){
   case DRAW_TWO_PIECES:
      drawOnNormalBoxes(false);
      break;

   case DRAW_ONE_PIECE:
      drawOnNormalBoxes(true);
      break;

   case ALL_AT_HOME:
      drawOnHomeOrGoal(true);
      break;

   case ALL_AT_GOAL:
      drawOnHomeOrGoal(false);
      break;

   case CORRIDORS_ONE_PIECE:
      drawOnCorridors(true);
      break;

   case CORRIDORS_TWO_PIECES:
      drawOnCorridors(false);
      break;

   case GROUPED:
      this->pieces = map<color, vector<Piece>>{
         {
            color::green, {
               Piece(color::green, {55, box_type::normal, color::none}),
               Piece(color::green, {64, box_type::normal, color::none}),
               Piece(color::green, {68, box_type::normal, color::none})
            }
         },
         {
            color::red, {
               Piece(color::red, {38, box_type::normal, color::none}),
               Piece(color::red, {47, box_type::normal, color::none}),
               Piece(color::red, {51, box_type::normal, color::none})
            }
         },
         {
            color::blue, {
               Piece(color::blue, {21, box_type::normal, color::none}),
               Piece(color::blue, {30, box_type::normal, color::none}),
               Piece(color::blue, {34, box_type::normal, color::none})
            }
         },
         {
            color::yellow, {
               Piece(color::yellow, {4, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {17, box_type::normal, color::none})
            }
         }
      };
      break;
   
   case GROUPED2:
      this->pieces = map<color, vector<Piece>>{
         {
            color::green, {
               Piece(color::green, {55, box_type::normal, color::none}),
               Piece(color::green, {64, box_type::normal, color::none}),
            }
         },
         {
            color::red, {
               Piece(color::red, {38, box_type::normal, color::none}),
               Piece(color::red, {47, box_type::normal, color::none}),
            }
         },
         {
            color::blue, {
               Piece(color::blue, {21, box_type::normal, color::none}),
               Piece(color::blue, {30, box_type::normal, color::none}),
            }
         },
         {
            color::yellow, {
               Piece(color::yellow, {4, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
            }
         }
      };
      break;

   case GROUPED_LEGACY:
      this->pieces = map<color, vector<Piece>>{
         {
            color::green,
            {
               Piece(color::green, {0, box_type::home, color::green}),
               Piece(color::green, {55, box_type::normal, color::none}),
               Piece(color::green, {64, box_type::normal, color::none}),
               Piece(color::green, {68, box_type::normal, color::none})
            }
         },
         {
            color::red,
            {
               Piece(color::red, {0, box_type::home, color::red}),
               Piece(color::red, {38, box_type::normal, color::none}),
               Piece(color::red, {47, box_type::normal, color::none}),
               Piece(color::red, {51, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {0, box_type::home, color::blue}),
               Piece(color::blue, {21, box_type::normal, color::none}),
               Piece(color::blue, {30, box_type::normal, color::none}),
               Piece(color::blue, {34, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {0, box_type::home, color::yellow}),
               Piece(color::yellow, {4, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {17, box_type::normal, color::none})
            }
         }
      };
      break;

   case TEST_BOO:
      this->pieces = map<color, vector<Piece>>{
         {
            color::green,
            {
               Piece(color::green, {0, box_type::home, color::green}),
               Piece(color::green, {16, box_type::normal, color::none}),
               Piece(color::green, {15, box_type::normal, color::none}),
               Piece(color::green, {68, box_type::normal, color::none})
            }
         },
         {
            color::red,
            {
               Piece(color::red, {0, box_type::home, color::red}),
               Piece(color::red, {18, box_type::normal, color::none}),
               Piece(color::red, {47, box_type::normal, color::none}),
               Piece(color::red, {51, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {0, box_type::home, color::blue}),
               Piece(color::blue, {19, box_type::normal, color::none}),
               Piece(color::blue, {21, box_type::normal, color::none}),
               Piece(color::blue, {34, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {0, box_type::home, color::yellow}),
               Piece(color::yellow, {20, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {17, box_type::normal, color::none})
            }
         }
      };
      break;

   case TEST_BOOM:
      this->pieces = map<color, vector<Piece>>{
         {
            color::red,
            {
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none})
            }
         },
         {
            color::green,
            {
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {22, box_type::normal, color::none}),
               Piece(color::green, {23, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {18, box_type::normal, color::none}),
               Piece(color::blue, {19, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {0, box_type::home, color::yellow}),
               Piece(color::yellow, {4, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {16, box_type::normal, color::none})
            }
         }
      };
      // this->special_items = vector<SpecialItem>{
      //    {star, {17, box_type::normal, color::none}}
      // };
      break;

   case TEST_MUSHROOM:
      this->pieces = map<color, vector<Piece>>{
         {
            color::red,
            {
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none})
            }
         },
         {
            color::green,
            {
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {22, box_type::normal, color::none}),
               Piece(color::green, {23, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {18, box_type::normal, color::none}),
               Piece(color::blue, {19, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {0, box_type::home, color::yellow}),
               Piece(color::yellow, {4, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {16, box_type::normal, color::none})
            }
         }
      };
      break;

   case TEST_SIZES:
      this->pieces = map<color, vector<Piece>>{
         {
            color::red,
            {
               Piece(color::red, {1, box_type::normal, color::none}, normal_piece, 3),
               Piece(color::red, {4, box_type::normal, color::none}, small_piece, 3),
               Piece(color::red, {4, box_type::normal, color::none}, small_piece, 3),
               Piece(color::red, {7, box_type::normal, color::none}, mega_piece, 3)
            }
         },
         {
            color::green,
            {
               Piece(color::green, {9, box_type::normal, color::none}, mega_piece, 3),
               Piece(color::green, {11, box_type::normal, color::none}, small_piece, 3),
               Piece(color::green, {13, box_type::normal, color::none}, small_piece, 3),
               Piece(color::green, {15, box_type::normal, color::none}, mega_piece, 3)
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {17, box_type::normal, color::none}, mega_piece, 3),
               Piece(color::blue, {19, box_type::normal, color::none}, small_piece, 3),
               Piece(color::blue, {21, box_type::normal, color::none}, small_piece, 3),
               Piece(color::blue, {23, box_type::normal, color::none}, mega_piece, 3)
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {25, box_type::normal, color::none}, mega_piece, 3),
               Piece(color::yellow, {27, box_type::normal, color::none}, small_piece, 3),
               Piece(color::yellow, {29, box_type::normal, color::none}, small_piece, 3),
               Piece(color::yellow, {31, box_type::normal, color::none}, mega_piece, 3)
            }
         }
      };
      // this->special_items = vector<SpecialItem>{
      //    {star, {17, box_type::normal, color::none}}
      // };
      break;

   case CHANGE_SIZE:
      this->pieces = map<color, vector<Piece>>{
         {
            color::green,
            {
               Piece(color::green, {0, box_type::home, color::green}),
               Piece(color::green, {16, box_type::normal, color::none}),
               Piece(color::green, {15, box_type::normal, color::none}),
               Piece(color::green, {68, box_type::normal, color::none})
            }
         },
         {
            color::red,
            {
               Piece(color::red, {0, box_type::home, color::red}),
               Piece(color::red, {18, box_type::normal, color::none}),
               Piece(color::red, {47, box_type::normal, color::none}),
               Piece(color::red, {51, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {0, box_type::home, color::blue}),
               Piece(color::blue, {19, box_type::normal, color::none}),
               Piece(color::blue, {21, box_type::normal, color::none}),
               Piece(color::blue, {34, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {0, box_type::home, color::yellow}),
               Piece(color::yellow, {20, box_type::normal, color::none}),
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {17, box_type::normal, color::none})
            }
         }
      };
      break;

   case PLAYGROUND:
      this->pieces = map<color, vector<Piece>>{
         {
            color::red, {
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {24, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none}),
               Piece(color::red, {25, box_type::normal, color::none}),
            }
         },
         {
            color::green,
            {
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {21, box_type::normal, color::none}),
               Piece(color::green, {22, box_type::normal, color::none}),
               Piece(color::green, {23, box_type::normal, color::none})
            }
         },
         {
            color::blue,
            {
               Piece(color::blue, {18, box_type::normal, color::none}),
               Piece(color::blue, {19, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none}),
               Piece(color::blue, {20, box_type::normal, color::none})
            }
         },
         {
            color::yellow,
            {
               Piece(color::yellow, {13, box_type::normal, color::none}),
               Piece(color::yellow, {14, box_type::normal, color::none}),
               Piece(color::yellow, {15, box_type::normal, color::none}),
               Piece(color::yellow, {16, box_type::normal, color::none})
            }
         }
      };
      break;
   /*
   case ALTERNED:
       this->pieces = map<color, vector<Box>>{
           {color::green, {{0, box_type::home, color::green}, {55, box_type::normal, color::none}, {13, box_type::normal, color::none}, {34, box_type::normal, color::none}}},
           {color::red, {{0, box_type::home, color::red}, {38, box_type::normal, color::none}, {64, box_type::normal, color::none}, {17, box_type::normal, color::none}}},
           {color::blue, {{0, box_type::home, color::blue}, {21, box_type::normal, color::none}, {47, box_type::normal, color::none}, {68, box_type::normal, color::none}}},
           {color::yellow, {{0, box_type::home, color::yellow}, {4, box_type::normal, color::none}, {30, box_type::normal, color::none}, {51, box_type::normal, color::none}}}};
       break;
   case ALMOST_GOAL:
       this->pieces = map<color, vector<Box>>{
           {color::green, {{51, box_type::normal, color::none}, {1, box_type::final_queue, color::green}, {2, box_type::final_queue, color::green}, {3, box_type::final_queue, color::green}}},
           {color::red, {{34, box_type::normal, color::none}, {1, box_type::final_queue, color::red}, {2, box_type::final_queue, color::red}, {3, box_type::final_queue, color::red}}},
           {color::blue, {{17, box_type::normal, color::none}, {1, box_type::final_queue, color::blue}, {2, box_type::final_queue, color::blue}, {3, box_type::final_queue, color::blue}}},
           {color::yellow, {{68, box_type::normal, color::none}, {1, box_type::final_queue, color::yellow}, {2, box_type::final_queue, color::yellow}, {3, box_type::final_queue, color::yellow}}}};
       break;
   case DEBUG:
       this->pieces = map<color, vector<Box>>{
           {color::green, {{56, box_type::normal, color::none}, {1, box_type::final_queue, color::green}, {2, box_type::final_queue, color::green}, {3, box_type::final_queue, color::green}}},
           {color::red, {{34, box_type::normal, color::none}, {1, box_type::final_queue, color::red}, {2, box_type::final_queue, color::red}, {3, box_type::final_queue, color::red}}},
           {color::blue, {{17, box_type::normal, color::none}, {1, box_type::final_queue, color::blue}, {2, box_type::final_queue, color::blue}, {3, box_type::final_queue, color::blue}}},
           {color::yellow, {{55, box_type::normal, color::none}, {1, box_type::final_queue, color::yellow}, {2, box_type::final_queue, color::yellow}, {3, box_type::final_queue, color::yellow}}}};
       break;*/
   }
}

/**
 * asocia una pieza a cada casilla
 * @param one indica si solo se pinta una pieza
 */
void Board::drawOnNormalBoxes(bool one){
   vector<color> colors = {color::yellow, color::blue, color::red, color::green};
   vector<int> starts = {1, 18, 35, 52};
   vector<int> ends = {17, 34, 51, 68};

   // se considera cada color
   color colorPiece;
   int start, end;
   vector<Piece> newPieces;
   for (int i = 0; i < colors.size(); i++){
      colorPiece = colors[i];
      start = starts[i];
      end = ends[i];
      for (int j = start; j <= end; j++){
         // se crean las piezas: una o dos
         newPieces.push_back(Piece(colorPiece, {j, box_type::normal, none}));
         if (one == false){
            newPieces.push_back(Piece(colorPiece, {j, box_type::normal, none}));
         }
      }

      // se agregan al mapa de piezas
      this->pieces[colorPiece] = newPieces;

      // se limpia el vector de piezas
      newPieces.clear();
   }
}

/**
 * metodo de depuracion para probar el pintado de las piezas en casa
 * o en meta
 * @param atHome para indicar si se pintan en casa o en la meta
 */
void Board::drawOnHomeOrGoal(bool atHome){
   vector<color> colors = {color::yellow, color::blue, color::red, color::green};
   box_type type;

   // se asigna el tipo
   if (atHome){
      type = home;
   }
   else{
      type = goal;
   }

   // se considera cada color
   color colorPiece;
   vector<Piece> newPieces;
   for (int i = 0; i < colors.size(); i++){
      // se asigna el color
      colorPiece = colors[i];

      // se ubican las 4 piezas
      for (int j = 0; j < 4; j++){
         // se crean las 4 piezas
         newPieces.push_back(Piece(colorPiece, {0, type, colorPiece}));
      }

      // se agregan las piezas al mapa
      this->pieces[colorPiece] = newPieces;

      // se limpia el vector de piezas
      newPieces.clear();
   }
}

/**
 * metodo de depuracion para comprobar el pintado de lsa piezas en los
 * corredores finales
 * @param one indica si debe pintarse solo una o dos
 */
void Board::drawOnCorridors(bool one){
   vector<color> colors = {color::yellow, color::blue, color::red, color::green};

   // se considera cada color
   color colorPiece;
   vector<Piece> newPieces;
   for (int i = 0; i < colors.size(); i++){
      colorPiece = colors[i];

      // se ubican las 7 piezas
      for (int j = 1; j <= 7; j++){
         // se crean las piezas: una o dos
         newPieces.push_back(Piece(colorPiece, {j, box_type::final_queue, colorPiece}));
         if (one == false){
            newPieces.push_back(Piece(colorPiece, {j, box_type::final_queue, colorPiece}));
         }
      }

      // se agregan al mapa de piezas
      this->pieces[colorPiece] = newPieces;

      // se limpia el vector de piezas
      newPieces.clear();
   }
}
