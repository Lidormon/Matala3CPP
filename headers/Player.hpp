#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include<vector>
#include<string>
#include "enums.hpp"
#include "DevelopmentCard.hpp"

namespace ariel {

class Building;
class Road;

class Player {
   std::string name;
   std::vector<Resource> resources;
   std::vector<DevelopmentCard> cards;
   std::vector<Building*> towns;
   std::vector<Road*> roads;

public:
   static Player EMPTY_PLAYER;

public:
   Player();

   std::string getName();
   void setName(std::string name);

   void addResource(Resource resource);
   void removeResource(Resource resource);
   bool hasResource(Resource resource);
   int countResource(Resource resource);

   std::vector<Resource> getResources();
   int countResources();
   Resource getResource(int i);

   std::vector<Building*> getTowns();
   std::vector<Road*> getRoads();

   void addTown(Building *build);
   void addRoad(Road *road);

   int cardsCount();
   CardType cardType(int i);
   void addCard(DevelopmentCard card); 
   void removeCard(CardType type);
   
   friend class Catan;
};

};

#endif