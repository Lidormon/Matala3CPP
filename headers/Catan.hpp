#ifndef __CATAN_HPP__
#define __CATAN_HPP__

#include<array>
#include "enums.hpp"
#include "Die.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include "ui/UI.hpp"

#define TOTAL_ROADS 72
#define TOTAL_TOWNS 54
#define MAX_PLAYERS 3
#define TOTAL_CARDS 16
#define POINT_TO_WIN 10

namespace ariel {

class Road;
class Building;

class Catan {
   ui::UI *gui;
   Die die;
   std::array<Player, MAX_PLAYERS> players;
   /**
    * The 19 tiles of the map, ordered in rings.
    * index 0 is center.
    * index 1-6 is the inner ring (all six tiles around center).
    * index 7-18 is the outer ring (all 12 tiles around the inner ring).
   */
   std::array<Tile, TILES_N> tiles;
   /**All the *owned* buildings(roads, buildings, towns) currently on the map.*/
   // std::vector<Building&> buildings;

   /**All the cards currently in the bank.*/
   std::vector<DevelopmentCard> cardsDeck;

//TODO: Change pointer array to array of pointers.

   /**An array of all the roads on the map.(Even empty ones)*/
   Road *edges;
   /**An array of all the "building nodes" on the map.(Even empty ones)*/
   Building *nodes;
   
public:
   // NOTE: Constructor and Destructor only handle memory!
   Catan(ui::UI *ui);
   ~Catan();

   /**
    * Initializes the game. Shuffles the tiles, shuffles the players,
    * Generates the nodes and edges, etc'
   */
   void init();

   /**
    * Do the first build round, where each player places two settlments
    * and two roads.
    */
   void buildRound();

   /**Do full round - all the player's turns.*/
   bool doRound();

   /**Do the turn of 1 player.*/
   bool doTurn(int player);

   int countPoints(Player *player);

   Tile &getTile(int i);

   Player &getPlayer(int i);

   DevelopmentCard &getCard(int i);
   int countCards();

   Building *getBuildingBetween(int i, int j, int k);
   Building *getBuildingBetween(Tile *t1, Tile *t2, Tile *t3);
   Road *getRoadBetween(int i, int j);
   Road *getRoadBetween(Tile *t1, Tile *t2);

   // Validity:
   bool isValidBuildingCoords(int i, int j, int k);

private:
   void turnGetResources(TileDiceNum num);
   void performAction(PlayerAction action, Player &player);
   void useDevelopmentCard(Player &player);
   void trade(Player &player);
   void initTiles();
   void initNodes();
   void initEdges();
};

};

#endif