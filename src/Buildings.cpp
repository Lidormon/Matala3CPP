#include "Buildings.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Road.hpp"

using namespace ariel;

// ==================== Building ====================

Building::Building(Tile &t1, Tile &t2, Tile &t3)
: Buyable({BRICKS, WOOD, WOOD, WHEAT}), type(NONE),
t{&t1, &t2, &t3}, r{nullptr, nullptr, nullptr},
owner(&Player::EMPTY_PLAYER) {}

Building::Building() : Building(
   Tile::TILE_EMPTY, Tile::TILE_EMPTY, Tile::TILE_EMPTY
) {}

void Building::buy(Player &player) {
   if (type == BuildingType::NONE) {
      Buyable::buy(player);
      owner = &player;
      player.addTown(this);
      cost = {WHEAT, WHEAT, ORE, ORE, ORE};
      type = BuildingType::SETTLMENT;
   } else if (type == BuildingType::SETTLMENT) {
      if (owner != &player) {
         throw "Player " + player.getName() + " doesn't own this Settlment!";
         return;
      }
      Buyable::buy(player);
      type = BuildingType::CITY;
   } else {
      throw "Can't upgrade a City any more!";
   }
}

BuildingType Building::getType() { return type; }

Tile& Building::getTile1() { return *(t[0]); }
Tile& Building::getTile2() { return *(t[1]); }
Tile& Building::getTile3() { return *(t[2]); }

Road* Building::getRoad1() { return r[0]; }
Road* Building::getRoad2() { return r[1]; }
Road* Building::getRoad3() { return r[2]; }

Player* Building::getOwner() { return owner; }