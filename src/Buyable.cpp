#include<vector>
#include "Buyable.hpp"
#include "Player.hpp"

#define RESOURCES_N 5

using namespace ariel;
using std::vector;

Buyable::Buyable(vector<Resource> _cost) : cost(_cost){}

bool Buyable::canAfford(Player &player) {
   // Count how many of each resource.
   int counter[RESOURCES_N] = {0};
   for(Resource &res : cost) {
      // Each resource is an enum between [0-4]
      counter[(int)res]++;
   }

   // Check that player has enough of each one.
   for (int i = 0; i < RESOURCES_N; i++) {
      if (player.countResource((Resource)i) < counter[i]) {
         return false;
      }
   }

   return true;
}

void Buyable::buy(Player &player) {
   if (!canAfford(player)) {
      throw "Player " + player.getName() + " doesn't have enough resources";
      return;
   }
   
   for (Resource res : cost) {
      player.removeResource(res);
   }
}

vector<Resource> Buyable::getCost() { return cost; }