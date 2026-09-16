# include "../../include/model/Dice.h"

#include <iostream>
#include <algorithm>

Dice::Dice(){
   //Asigna por defecto los 5 valores del dado a cada jugador.
   this->dice = map<color, vector<vector<int>>>{
      {color::blue, {{1, 2, 4, 5, 6, yinyang}}},
      {color::yellow, {{1, 2, 4, 5, 6, yinyang}}}
   };
};

Dice::Dice(const map<color, vector<vector<int>>>& d){
   this->dice = map<color, vector<vector<int>>>(d);
};

const vector<int>& Dice::getDice(color player) const{
   // Si hay 3 capas se devuelve la última.
   if (dice.at(player).size() == 2){
      return dice.at(player).at(dice.at(player).size() - 1);
   }
   // Si hay solo dos capas se devuelve la primera.
   else{
      return dice.at(player).at(0);
   }
}

const vector<vector<int>>& Dice::getAllDiceLayers(color player) const{
   return dice.at(player);
}

const int Dice::getLayersSize(color player) const{
   return dice.at(player).size();
}

void Dice::removeNumber(color player, int n){
   if (player != yellow and player != blue){
      player = partner_color(player);
   }
   // Si la tercera capa existe se elimina el elemento de ahí.
   if (dice[player].size() == 2){
      dice[player][dice[player].size() - 1].erase(
         remove(dice[player][dice[player].size() - 1].begin(), dice[player][dice[player].size() - 1].end(), n),
         dice[player][dice[player].size() - 1].end());
      // Si la capa se  vacía (se va a vaciar siempre) se elimina.
      if (dice[player][dice[player].size() - 1].empty()){
         dice[player].pop_back();
      }
   }
   else{
      dice[player][0].erase(remove(dice[player][0].begin(), dice[player][0].end(), n), dice[player][0].end());
      // Si se han gastado todos los números, se regenera de nuevo el dado.
      if (dice[player][0].empty()){
         resetDice(player);
      }
   }
}

void Dice::resetDice(color player, const vector<int>& new_dice){
   dice[player][0] = new_dice;
}

bool Dice::isAvailable(color player, int n) const{
   if (player != yellow && player != blue){
      player = partner_color(player);
   }

   // Si la tercera capa existe se revisa si n está en esa capa.
   if (dice.at(player).size() == 2){
      return (find(dice.at(player).at(dice.at(player).size() - 1).begin(),
                   dice.at(player).at(dice.at(player).size() - 1).end(), n) != dice.at(player).at(
         dice.at(player).size() - 1).end());
   }
   else{
      return (find(dice.at(player).at(0).begin(), dice.at(player).at(0).end(), n) != dice.at(player).at(0).end());
   }
   //Gracias copilot ;)
}

void Dice::addNumber(color player, int n){
   if (player != yellow and player != blue){
      player = partner_color(player);
   }
   dice[player][0].push_back(n);
}

void Dice::forceNumber(color player, int n){
   if (player != yellow and player != blue){
      player = partner_color(player);
   }
   dice[player].push_back({n});
}
