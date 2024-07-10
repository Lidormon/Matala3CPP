#ifndef __BUILDINGS_HPP__
#define __BUILDINGS_HPP__

#include<vector>
#include "enums.hpp"
#include "Buyable.hpp"

namespace ariel {

class Tile;
class Road;

/**An abstract class that represents a buildable object in the game.*/
class Building : public Buyable {
protected:
   BuildingType type;
   Tile *t[3];
   Road *r[3];
   Player *owner;

public:
   Building(Tile &t1, Tile &t2, Tile &t3);

   void buy(Player &player) override;

   BuildingType getType();

   Tile &getTile1();
   Tile &getTile2();
   Tile &getTile3();

   Road *getRoad1();
   Road *getRoad2();
   Road *getRoad3();

   Player *getOwner();

   friend class Catan;
private:
   Building();
};

};

#endif