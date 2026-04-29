#ifndef __POWEBAR_H__
#define __POWEBAR_H__

# include <vector>
# include <map>
# include "Attributes.h"


using namespace std;

class PowerBar{
private:
   int power;
   static const int MAX_POWER;

   /**
    * @brief Set power
    *
    * @param e
   */
   void setPower(int e);

public:
   /**
    * @brief Constructor por defecto de un nuevo objeto PowerBar
    *
   */
   PowerBar();

   /**
    * @brief Constructor de un nuevo objeto PowerBar
    *
    * @param e
   */
   PowerBar(int e);

   /**
    * @brief Get power
    *
    * @return int
   */
   int getPower() const;

   /**
    * @brief Increase power by e units
    *
    * @param e
   */
   void increasePower(int e);

   /**
    * @brief Empty power bar
    *
    */
   void emptyPowerBar();
};

# endif
