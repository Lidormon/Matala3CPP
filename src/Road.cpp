#include "Player.hpp"
#include "Road.hpp"

using namespace ariel;

Road::Road(Building *b1, Building *b2) : Buyable({WOOD, BRICKS}),
owner(&Player::EMPTY_PLAYER), b1(b1), b2(b2) {}

Road::Road() : Road(nullptr, nullptr) {}

void Road::buy(Player &player) {
   if (owner == &Player::EMPTY_PLAYER && canAfford(player)) {
      Buyable::buy(player);
      owner = &player;
      player.addRoad(this);
   } else {
      throw "Can't buy this road!";
   }
}

Building &Road::getBuild1() { return *b1; }
Building &Road::getBuild2() { return *b2; }

Player *Road::getOwner() { return owner; }