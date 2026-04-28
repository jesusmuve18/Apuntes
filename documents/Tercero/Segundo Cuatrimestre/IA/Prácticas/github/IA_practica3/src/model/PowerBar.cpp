# include "../../include/model/PowerBar.h"

const int PowerBar::MAX_POWER = 100;

PowerBar::PowerBar(){
   this->power = 0;
}

PowerBar::PowerBar(int e){
   this->power = e;
}

int PowerBar::getPower() const{
   return this->power;
}

void PowerBar::increasePower(int e){
   this->power = min(this->power + e, MAX_POWER);
}

void PowerBar::emptyPowerBar(){
   this->power = 0;
}

void PowerBar::setPower(int e){
   this->power = e;
}
