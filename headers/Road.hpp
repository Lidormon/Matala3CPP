#ifndef __ROAD_HPP__
#define __ROAD_HPP__

#include<vector>
#include "Buyable.hpp"

namespace ariel {

class Building;
class Player;

/**Represents a road between two tiles.*/
class Road : public Buyable {
   Player *owner;
   Building *b1, *b2;
public:
   Road(Building *b1, Building *b2);

   void buy(Player &player) override;

   Building &getBuild1();
   Building &getBuild2();
   
   Player *getOwner();

   friend class Catan;
private:
   Road();
};

};

#endif