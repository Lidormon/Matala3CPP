#ifndef __BUYABLE_HPP__
#define __BUYABLE_HPP__

#include<vector>
#include "enums.hpp"

namespace ariel {

class Player;

/**An abstract class that represents a buyable object in the game.*/
class Buyable {
protected:
   std::vector<Resource> cost;

public:
   Buyable(std::vector<Resource> cost);

   bool canAfford(Player &player);
   virtual void buy(Player &player);

   std::vector<Resource> getCost();

   friend class Catan;
};

};

#endif