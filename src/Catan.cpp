#include<algorithm>
#include<random>
#include<ctime>
#include<string>
#include<map>
#include "Catan.hpp"
#include "enums.hpp"
#include "DevelopmentCard.hpp"
#include "Buildings.hpp"
#include "Road.hpp"
#include "Exceptions.hpp"

using namespace ariel;
using std::array;
using std::to_string;

// ========================= Defines =========================

Building BEMPTY = Building(Tile::TILE_SEA, Tile::TILE_SEA, Tile::TILE_SEA);

const array<Terrain, TILES_N> terrains = {
   Terrain::DESERT,
   Terrain::FIELD, Terrain::FIELD, Terrain::FIELD, Terrain::FIELD, 
   Terrain::FOREST, Terrain::FOREST, Terrain::FOREST, Terrain::FOREST, 
   Terrain::HILLS, Terrain::HILLS, Terrain::HILLS, 
   Terrain::MOUNTAIN, Terrain::MOUNTAIN, Terrain::MOUNTAIN, 
   Terrain::PASTURE, Terrain::PASTURE, Terrain::PASTURE, Terrain::PASTURE, 
};

const array<TileDiceNum, TILES_N> tileNums = {
   TWO, THREE, THREE, FOUR, FOUR, FIVE, FIVE, SIX, SIX, SEVEN,
   EIGHT, EIGHT, NINE, NINE, TEN, TEN, ELEVEN, ELEVEN, TWELVE
};

const array<DevelopmentCard, TOTAL_CARDS> cards = {
   DevelopmentCard
   {CardType::CARD_MONOPOLY}, {CardType::CARD_MONOPOLY},
   {CardType::CARD_ROADS}, {CardType::CARD_ROADS}, {CardType::CARD_ROADS},
   {CardType::CARD_PLENTY_YEAR}, {CardType::CARD_PLENTY_YEAR},
   {CardType::CARD_PLENTY_YEAR}, {CardType::CARD_PLENTY_YEAR},
   {CardType::CARD_KNIGHT}, {CardType::CARD_KNIGHT}, {CardType::CARD_KNIGHT},
   {CardType::CARD_VICTORY}, {CardType::CARD_VICTORY},
   {CardType::CARD_VICTORY}, {CardType::CARD_VICTORY},
};

// ========================= Catan =========================

Catan::Catan(ui::UI *gui)
: gui(gui), die(), players{Player(), Player(), Player()}, tiles{}
, cardsDeck(cards.begin(), cards.end())
, edges(new Road[TOTAL_ROADS]), nodes(new Building[TOTAL_TOWNS]) {}

Catan::~Catan() {
   delete[] edges;
   delete[] nodes;
}

void Catan::init() {
   auto terrainArr = terrains;
   auto nums = tileNums;

   // Shuffle the tiles:
   std::default_random_engine engine (std::time(NULL));
   std::shuffle(terrainArr.begin(), terrainArr.end(), engine);
   std::shuffle(nums.begin(), nums.end(), engine);
   
   // Shuffle the development cards
   std::shuffle(cardsDeck.begin(), cardsDeck.end(), engine);

   // Put SEVEN and DESERT together:
   int desert_i, seven_i;
   for (int i = 0; i < TILES_N; i++) {
      if (terrainArr[i] == Terrain::DESERT) {
         desert_i = i;
      }
      if (nums[i] == TileDiceNum::SEVEN) {
         seven_i = i;
      }
   }
   // Swap:
   TileDiceNum temp = nums[seven_i];
   nums[seven_i] = nums[desert_i];
   nums[desert_i] = temp;

   // Put tiles into Board:
   for(int i = 0; i < TILES_N; i++) {
      tiles[i] = Tile(terrainArr[i], nums[i], i);
   }

   initTiles();
   initNodes();
   initEdges();
}

void Catan::buildRound() {
   std::string pname1, pname2, pname3;
   gui->askPlayerNames(pname1, pname2, pname3);
   players[0].setName(pname1);
   players[1].setName(pname2);
   players[2].setName(pname3);

   // Decides which player goes first:
   int p1, p2, p3;

   gui->rollPlayerTurnOrder(p1, p2, p3);

   // Sort the players according to their rolls.
   if (p1 >= p2) {
      if (p2 >= p3) {
         // p1 >= p2 >= p3
         // No need to shuffle players
      } else if (p1 >= p3) {
         // p1 >= p3 > p2
         std::swap(players[1], players[2]);
      } else {
         // p3 > p1 >= p2
         std::swap(players[2], players[1]);
         std::swap(players[1], players[0]);
      }
   } else {
      if (p1 >= p3) {
         // p2 > p1 >= p3
         std::swap(players[0], players[1]);
      } else if (p2 >= p3) {
         // p2 >= p3 > p1
         std::swap(players[0], players[1]);
         std::swap(players[1], players[2]);
      } else {
         // p3 > p2 > p1
         std::swap(players[0], players[2]);
      }
   }

   // Add the resources to buy the the first 2 Settlments and Roads:
   for(Player &p : players) {
      Building tempb;
      for (Resource &resource : tempb.cost) {
         p.addResource(resource);
         p.addResource(resource);
      }
      Road tempr;
      for (Resource &resource : tempr.cost) {
         p.addResource(resource);
         p.addResource(resource);
      }
   }

   // Start building:
   Building *build;
   Road *road;

   // Player 1:
   build = gui->askPlayerForSettlmentStart(&players[0]);
   build->buy(players[0]);
   players[0].addResource(build->getTile1().getResource());
   players[0].addResource(build->getTile2().getResource());
   players[0].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[0]);
   road->buy(players[0]);


   // Player 2:
   build = gui->askPlayerForSettlmentStart(&players[1]);
   build->buy(players[1]);
   players[1].addResource(build->getTile1().getResource());
   players[1].addResource(build->getTile2().getResource());
   players[1].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[1]);
   road->buy(players[1]);


   // Player 3:
   build = gui->askPlayerForSettlmentStart(&players[2]);
   build->buy(players[2]);
   players[2].addResource(build->getTile1().getResource());
   players[2].addResource(build->getTile2().getResource());
   players[2].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[2]);
   road->buy(players[2]);

   build = gui->askPlayerForSettlmentStart(&players[2]);
   build->buy(players[2]);
   players[2].addResource(build->getTile1().getResource());
   players[2].addResource(build->getTile2().getResource());
   players[2].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[2]);
   road->buy(players[2]);


   // Player 2:
   build = gui->askPlayerForSettlmentStart(&players[1]);
   build->buy(players[1]);
   players[1].addResource(build->getTile1().getResource());
   players[1].addResource(build->getTile2().getResource());
   players[1].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[1]);
   road->buy(players[1]);


   // Player 1:
   build = gui->askPlayerForSettlmentStart(&players[0]);
   build->buy(players[0]);
   players[0].addResource(build->getTile1().getResource());
   players[0].addResource(build->getTile2().getResource());
   players[0].addResource(build->getTile3().getResource());

   road = gui->askPlayerForRoadStart(&players[0]);
   road->buy(players[0]);
}

bool Catan::doRound() { 
   for (int i = 0; i < MAX_PLAYERS; i++) {
      if (doTurn(i)) {
         gui->displayWin(&players[i]);
         return true;
      }
   }
   return false;
}

bool Catan::doTurn(int playerNum) {
   THROW_INVALID_RANGE(playerNum, 0, MAX_PLAYERS)

   Player &player = players[playerNum];
   gui->playerTurnStart(&player);

   PlayerAction action = gui->askPlayerActionPreRoll(&player);
   // If development card used
   if (action == PlayerAction::ACTION_USE_CARD) {
      useDevelopmentCard(player);
      return countPoints(&player) == POINT_TO_WIN;
   }

   int dice[2] = {die.roll(), die.roll()};
   int roll = dice[0] + dice[1];
   gui->displayDiceRolls(dice, 2);
   

   // Robber:
   if (roll == 7) {
      for (Player &pl : players) {
         if (pl.countResources() > 7) {
            gui->askPlayerRobberResources(&pl);
         }
      }
   } else {
      // Find all the tiles with the rolled number:
      turnGetResources((TileDiceNum)roll);
   }

   do {
      action = gui->askPlayerAction(&player);
      performAction(action, player);
   } while (action != PlayerAction::ACTION_FINISH_TURN);

   return countPoints(&player) == POINT_TO_WIN;
}

int Catan::countPoints(Player *player) {
   int victory_points = 0;

   // Count Victory Point and Knight cards:
   int knights = 0;
   for (DevelopmentCard &card : player->cards) {
      if (card.getType() == CardType::CARD_VICTORY) {
         victory_points++;
      } else if (card.getType() == CardType::CARD_KNIGHT) {
         knights++;
      }
   }
   if (knights == 3) {
      victory_points += 2;
   }

   // Count Buildings:
   for (Building *build = nodes; build < nodes + TOTAL_TOWNS; build++) {
      if (build->getOwner() == player) {
         if (build->getType() == BuildingType::SETTLMENT) {
            victory_points++;
         } else {
            victory_points += 2;
         }
      }
   }

   return victory_points;
}

// ========================= Actions =========================

// Assumption:
// User being able to afford a Buyable is checked in the UI class
// UI will not call tryAction functions if player can't afford to buy.

void Catan::performAction(PlayerAction action, Player &player) {
   // Initialize here because `switch` doesn't like constructions inside cases.
   // Knight doesn't mean anything, just a placeholder.
   DevelopmentCard card = DevelopmentCard(CardType::CARD_KNIGHT);
   Building *city, *set;
   Road *road;

   switch (action) {
   case PlayerAction::ACTION_BUILD_CITY:
      city = gui->askUpgradableCityCoords(&player);
      if (city != nullptr) {
         city->buy(player);
      }
      break;
   case PlayerAction::ACTION_BUILD_ROAD:
      road = gui->askBuildableRoadCoords(&player);
      if (road != nullptr) {
         road->buy(player);
      }
      break;
   case PlayerAction::ACTION_BUILD_SETTLMENT:
      set = gui->askBuildableSettlmentCoords(&player);
      if (set != nullptr) {
         set->buy(player);
      }
      break;
   case PlayerAction::ACTION_BUY_CARD:
      card = cardsDeck[cardsDeck.size() - 1];
      cardsDeck.pop_back();
      card.buy(player);
      gui->message("You got a '" + card.toStr() + "' card!\n");
      break;
   case PlayerAction::ACTION_TRADE:
      // Is long, so split to function.
      trade(player);
      break;
   case PlayerAction::ACTION_USE_CARD:
      // Is long, so split to function.
      useDevelopmentCard(player);
      break;
   // To calm down the compiler:
   case PlayerAction::ACTION_FINISH_TURN: case PlayerAction::ACTION_ROLL:
      break;
   }
}

void Catan::useDevelopmentCard(Player &player) {
   CardType card;

   // Cancel
   if (!gui->askToUseDevelopmentCard(&player, card)) {
      return;
   }


   Resource res1, res2;
   Road *r1, *r2;
   int count = 0, total = 0;
   
   switch(card) {
   case CardType::CARD_MONOPOLY:
      // Cancel
      if (!gui->askMonopolyResource(res1)) {
         return;
      }
      for (Player &p : players) {
         count = 0;
         if (&p == &player) {
            continue;
         }
         while (p.hasResource(res1)) {
            player.addResource(res1);
            p.removeResource(res1);
            count++; total++;
         }
         std::string str = "Stole " + std::to_string(count)
            + " from " + p.getName() + "\n";
         gui->message(str);
      }
      break;
   case CardType::CARD_PLENTY_YEAR:
      gui->askYearPlentyResources(res1, res2);
      player.addResource(res1);
      player.addResource(res2);
      break;
   case CardType::CARD_ROADS:
      // Cancel
      if (!gui->askCardRoads(&player, r1, r2)) {
         return;
      }
      
      // Add the resources for a road.
      player.addResource(Resource::WOOD);
      player.addResource(Resource::WOOD);
      player.addResource(Resource::BRICKS);
      player.addResource(Resource::BRICKS);
      r1->buy(player);
      r2->buy(player);
      break;
   // Calm down compiler:
   case CardType::CARD_KNIGHT: case CardType::CARD_VICTORY: break;
   }

   player.removeCard(card);
   // Return the card to the bottom of the deck.
   cardsDeck.insert(cardsDeck.begin(), DevelopmentCard(card));
}

void Catan::trade(Player &player) {
   std::vector<Resource> sell, buy;
   bool bank;

   // Cancel
   if (!gui->askTradeResources(&player, buy, sell, bank)) {
      return;
   }

   // Bank
   if (bank) {
      for (Resource &res : sell) {
         player.removeResource(res);
      }
      for (Resource &res : buy) {
         player.addResource(res);
      }
      return;
   }

   // Player
   Player &target = gui->askTradePartner(&player, buy, sell);
   // All reject
   if (&target == &Player::EMPTY_PLAYER) {
      return;
   }

   for (Resource &res : sell) {
      player.removeResource(res);
      target.addResource(res);
   }
   for (Resource &res : buy) {
      player.addResource(res);
      target.removeResource(res);
   }
}

// ========================= Get/Set =========================

Tile& Catan::getTile(int i) {
   if (i == Tile::TILE_SEA.index)
      return Tile::TILE_SEA;
   THROW_INVALID_RANGE(i, 0, TILES_N-1)
   return tiles[i];
}

Player& Catan::getPlayer(int i) {
   THROW_INVALID_RANGE(i, 0, MAX_PLAYERS-1)
   return players[i];
}

DevelopmentCard& Catan::getCard(int i) {
   THROW_INVALID_RANGE(i, 0, long(cardsDeck.size()-1))
   return cardsDeck[i];
}

int Catan::countCards() { return cardsDeck.size(); }

Building* Catan::getBuildingBetween(int i, int j, int k) {
   Tile *ti, *tj, *tk;
   if (i == Tile::TILE_SEA.index) {
      ti = &Tile::TILE_SEA;
   } else {
      THROW_INVALID_RANGE(i, 0, TILES_N-1)
      ti = &tiles[i];
   }
   if (j == Tile::TILE_SEA.index) {
      tj = &Tile::TILE_SEA;
   } else {
      THROW_INVALID_RANGE(j, 0, TILES_N-1)
      tj = &tiles[j];
   }
   if (k == Tile::TILE_SEA.index) {
      tk = &Tile::TILE_SEA;
   } else {
      THROW_INVALID_RANGE(k, 0, TILES_N-1)
      tk = &tiles[k];
   }

   return getBuildingBetween(ti, tj, tk);
}

Building* Catan::getBuildingBetween(Tile *t1, Tile *t2, Tile *t3) {
   THROW_NULLPTR(t1)
   THROW_NULLPTR(t2)
   THROW_NULLPTR(t3)

   // Make sure t1 is not the SEA tile
   if(t1 == &Tile::TILE_SEA) {
      if (t2 != &Tile::TILE_SEA) {
         t1 = t2;
         t2 = &Tile::TILE_SEA;
      } else if (t3 != &Tile::TILE_SEA) {
         t1 = t3;
         t3 = &Tile::TILE_SEA;
      } else {
         throw "All tiles are Sea tiles!";
      }
   }

   for (int i = 0; i < TILE_BUILDS_N; i++) {
      Building *build = t1->getBuilding(i);
      Tile *bt1 = &build->getTile1(), *bt2 = &build->getTile2();
      Tile *bt3 = &build->getTile3();
      // Check if the Building `build`'s tiles are t1 t2 and t3:
      if (t1 != bt1 && t1 != bt2 && t1 != bt3) {
         continue;
      } if (t2 != bt1 && t2 != bt2 && t2 != bt3) {
         continue;
      } if (t3 != bt1 && t3 != bt2 && t3 != bt3) {
         continue;
      }
      return build;
   }

   // The three tiles are not adjasent.
   return nullptr;
}

Road* Catan::getRoadBetween(int i, int j) {
   Tile *ti, *tj;
   if (i == Tile::TILE_SEA.index) {
      ti = &Tile::TILE_SEA;
   } else {
      THROW_INVALID_RANGE(i, 0, TILES_N-1)
      ti = &tiles[i];
   }
   if (j == Tile::TILE_SEA.index) {
      tj = &Tile::TILE_SEA;
   } else {
      THROW_INVALID_RANGE(j, 0, TILES_N-1)
      tj = &tiles[j];
   }
   return getRoadBetween(ti, tj);
}

Road* Catan::getRoadBetween(Tile *t1, Tile *t2) {
   THROW_NULLPTR(t1)
   THROW_NULLPTR(t2)

   // Make sure t1 is the non sea tile:
   if(t1 == &Tile::TILE_SEA) {
      if(t2 == &Tile::TILE_SEA) {
         // No roads between sea tiles:
         return nullptr;
      }
      t1 = t2;
      t2 = &Tile::TILE_SEA;
   }

   // Find which side the tiles neighbour on:
   for (int i = 0; i < TILES_AROUND_TILE; i++) {
      // Roads and tiles use the same indices for direction,
      // So the road between two tiles is in the same index
      // as the index those tiles neighbour on.
      if (t1->getNeigbour(i) == t2) {
         return t1->getRoad(i);
      }      
   }

   // The three tiles are not adjasent.
   return nullptr;
}

// ========================= Validity =========================

bool Catan::isValidBuildingCoords(int i, int j, int k) {
   if (i < 0 || j < 0 || k < 0 || i > 18 || j > 18 || k > 18) {
      return false;
   }

   for (Building *build : tiles[i].buildings) {
      if (build->t[0] == &tiles[i] 
      && build->t[1] == &tiles[j]
      && build->t[2] == &tiles[k]) {
         return true;
      }
      if (build->t[0] == &tiles[i] 
      && build->t[1] == &tiles[k]
      && build->t[2] == &tiles[j]) {
         return true;
      }
      if (build->t[0] == &tiles[j]
      && build->t[1] == &tiles[i]
      && build->t[2] == &tiles[k]) {
         return true;
      }
      if (build->t[0] == &tiles[j]
      && build->t[1] == &tiles[k]
      && build->t[2] == &tiles[i]) {
         return true;
      }
      if (build->t[0] == &tiles[k]
      && build->t[1] == &tiles[j]
      && build->t[2] == &tiles[i]) {
         return true;
      }
      if (build->t[0] == &tiles[k]
      && build->t[1] == &tiles[i]
      && build->t[2] == &tiles[j]) {
         return true;
      }
   }

   return false;
}

// ========================= Boilerplate =========================

void Catan::turnGetResources(TileDiceNum num) {
   for (Tile &tile : tiles) {
      if (tile.num == num) {
         for (Building *build : tile.buildings) {
            if (build->type == BuildingType::SETTLMENT) {
               build->owner->addResource(tile.getResource());
               gui->displayResourceGain(build->owner, tile.getResource());
            } else if (build->type == BuildingType::CITY) {
               build->owner->addResource(tile.getResource());
               build->owner->addResource(tile.getResource());
               gui->displayResourceGain(build->owner, tile.getResource());
               gui->displayResourceGain(build->owner, tile.getResource());
            }
         }
      }
   }
}

void Catan::initTiles() {
   // Connect center to all six neighbours.
   for (int i = 1; i <= 6; i++) {
      // Neighbours are on opposite sides of eachother so +3,
      // A tile's [1] neighbour is it's [4] neighbour. [5] <-> [2]
      tiles[0].neighbors[i%6] = &tiles[i];
      tiles[i].neighbors[(i+3)%6] = &tiles[0];
   }

   // Connect inner ring(1-6) to eachother
   for (int i = 1; i <= 6; i++) {
      tiles[i].neighbors[(i+2)%6] = &tiles[i%6+1];
      tiles[i%6+1].neighbors[(i+5)%6] = &tiles[i];
   }

   //Connect inner ring to outer ring(7-18)
   for (int i = 1, j=7; i <= 6; i++) {
      tiles[i].neighbors[i-1] = &tiles[j];
      tiles[j].neighbors[(i+2)%6] = &tiles[i];
      j++;
      tiles[i].neighbors[i%6] = &tiles[j];
      tiles[j].neighbors[(i+3)%6] = &tiles[i];
      // Edge case, last tile(6)'s last edge is to 7, not 18+1 (=19).
      j = (j<18) ? j+1 : 7;
      tiles[i].neighbors[(i+1)%6] = &tiles[j];
      tiles[j].neighbors[(i+4)%6] = &tiles[i];
   }

   // Connect outer ring(7-18) to eachother:
   for (int i = 7; i <= 18; i++) {
      // Derived from (i-6) / 2 + 2:
      // (i-6) -> normalize to (1,12), divide by 2 -> sea direction.
      // +2 -> The direction of the neighbour is "turning" twice from the sea.
      int n = (i-2) / 2;
      int next = (i == 18) ? 7 : i+1;
      tiles[i].neighbors[n%6] = &tiles[next];
      tiles[next].neighbors[(n+3)%6] = &tiles[i];
   }

   // Connect outer ring to the sea:
   for (int i = 7; i <= 18; i++) {
      int n = (i-6) / 2; // Sea direction.
      if (i%2 == 0) {
         tiles[i].neighbors[(n-1)%6] = &Tile::TILE_SEA;
      }
      tiles[i].neighbors[n%6] = &Tile::TILE_SEA;
      tiles[i].neighbors[(n+1)%6] = &Tile::TILE_SEA;
   }
}

void Catan::initNodes() {
   // ni is the index in the `nodes` array.
   int ni = 0;
   // Center ring (Tile i=0):
   for (int j = 1; j <= 6; j++) {
      int t2 = (j%6) + 1; //(j+1) or (1 if j = 6)
      nodes[ni] = Building(tiles[0], tiles[j], tiles[t2]);
      
      // The same village/building is on different thirds of the
      // neighbouring tiles, that's why +2,
      // So a village on side 1 of a tile, will be on side 3 of the
      // next(clockwise) tile, and 5 on the next next(clockwise) tile.
      tiles[0].buildings[j%6] = tiles[j].buildings[(j+2)%6]
         = tiles[t2].buildings[(j+4)%6] = &nodes[ni];
      ni++;
   }

   // Inner ring (Tiles 1-6)
   for (int i = 1; i <= 6; i++) {
      // (i-7) + 6 = i-1
      int prev = (i-7)%6 + 6; // prev=i-1 OR (6 if i==1)
      int t2 = 5+2*i;
      int t3 = 6+2*i;
      int t4 = 7+((2*i)%12);

      nodes[ni] = Building(tiles[i], tiles[prev], tiles[t2]);
      nodes[ni+1] = Building(tiles[i], tiles[t2], tiles[t3]);
      nodes[ni+2] = Building(tiles[i], tiles[t3], tiles[t4]);

      tiles[i].buildings[(i+4)%6] = tiles[prev].buildings[i%6] =
         tiles[t2].buildings[(i+2)%6] = &nodes[ni];
      
      tiles[i].buildings[(i+5)%6] = tiles[t2].buildings[(i+1)%6] =
         tiles[t3].buildings[(i+3)%6] = &nodes[ni+1];

      tiles[i].buildings[i%6] = tiles[t3].buildings[(i+2)%6] =
         tiles[t4].buildings[(i+4)%6] = &nodes[ni+2];
      
      ni += 3;
   }

   // Outer ring (Tiles 7-18):
   for (int i = 7; i <= 18; i++) {
      // (i-19) + 18 = i - 1
      int prev = (i-19)%12 + 18; // prev=i-1 OR (18 if i==7)
      // TODO: CHECK (might be i-6 instead)
      // Derived from (i-7)/2 + 5:
      // (i-7) => normalize i to (0,11), divide by 2 -> sea direction.
      // +5 is the previous direction(same as -1 by mod6 math).
      int vil = ((i+3)/2)%6;

      nodes[ni] = Building(tiles[i], tiles[prev], Tile::TILE_SEA);
      nodes[ni+1] = Building(tiles[i], Tile::TILE_SEA, Tile::TILE_SEA);
      
      tiles[i].buildings[vil%6] = tiles[prev].buildings[(vil+2)%6] = &nodes[ni];
      tiles[i].buildings[(vil+1)%6] = &nodes[ni+1];
      
      // The even tiles have one more sea adjacent tile than odd tiles.
      if (i%2 == 0) {
         nodes[ni+2] = Building(tiles[i], Tile::TILE_SEA, Tile::TILE_SEA);
         tiles[i].buildings[(vil+2)%6] = &nodes[ni+2];
         ni++;
      }
      ni += 2;
   }
}

void Catan::initEdges() {
   // ni is the index in the `edges` array.
   int ei = 0;
   // Roads around the center.
   for (int j = 1; j <= 6; j++) {
      int prev = (j+5)%6;
      edges[ei] = Road(tiles[0].buildings[prev], tiles[0].buildings[j%6]);

      int road = j%3;
      tiles[0].buildings[prev]->r[road] =
         tiles[0].buildings[j%6]->r[road] = &edges[ei];
      tiles[0].roads[j%6] = &edges[ei];
      tiles[j].roads[(j+3)%6] = &edges[ei];
      ei++;
   }

   // Roads around inner ring (Tiles 1-6)
   for (int i = 1; i <= 6; i++) {
      int start = (i+4)%6; // i-2 in mod6
      for (int j = start; j < start+4; j++) {
         int prev = (j+5)%6;
         edges[ei] = Road(tiles[i].buildings[prev], tiles[i].buildings[j%6]);

         int road = j%3;
         tiles[i].buildings[prev]->r[road] =
            tiles[i].buildings[j%6]->r[road] = &edges[ei];
         tiles[i].roads[j%6] = &edges[ei];
         tiles[i].neighbors[j%6]->roads[(j+3)%6] = &edges[ei];
         ei++;
      }
   }

   // Roads around outer ring (Tiles 7-18)
   for (int i = 7; i <= 18; i++) {
      // Derived from (i-7) / 2 + 5:
      // (i-7) -> normalize to (0,11), divide by 2 -> road in sea direction.
      // +5(=-1 mod6) -> The road one turn "away" from the sea.
      int start = ((i+3)/2)%6;
      int even = (i%2 == 0); //if even, equals to 1.
      for (int j = start; j < start+3+even; j++) {
         int prev = (j+5)%6;
         edges[ei] = Road(tiles[i].buildings[prev], tiles[i].buildings[j%6]);

         int road = j%3;
         tiles[i].buildings[prev]->r[road] =
            tiles[i].buildings[j%6]->r[road] = &edges[ei];
         tiles[i].roads[j%6] = &edges[ei];
         
         if (j == start) {
            tiles[i].neighbors[j%6]->roads[(j+3)%6] = &edges[ei];
         } else if (j < start+2+even) {
            tiles[i].buildings[j%6]->r[(road+2)%3] = nullptr;
         }
         ei++;
      }
   }
}
