# include "../../include/model/Player.h"
# include "../../include/model/Parchis.h"

void Player::perceive(Parchis& p){
   actual = &p;
   jugador = actual->getCurrentPlayerId();
}
