#ifndef __BOARDTRAPSPRITE_H_
#define __BOARDTRAPSPRITE_H_

#include "../model/Attributes.h"
#include "IncludesSFML.h"
#include "../model/BoardTrap.h"

using namespace std;

class BoardTrapSprite : public Sprite{
private:
   trap_type type;
   static const map<trap_type, vector<int>> trap2textrec;

public:
   BoardTrapSprite(const Texture& t, trap_type type);
   inline trap_type getType(){ return this->type; }
};


#endif
