#include<iostream>
#include<string>
#include<array>
#include<vector>
#include<set>
#include<map>
#include<cstdio>
#include<cstdlib>
#include "ui/Board.hpp"
#include "ui/Die.hpp"
#include "Buildings.hpp"
#include "Road.hpp"

#define TRADE_BANK_RATE 4

using namespace ariel;
using namespace ariel::ui;
using std::set;
using std::map;
using std::vector;
using std::array;
using std::string;

const map<PlayerAction, string> actionStrings = {
   {PlayerAction::ACTION_BUILD_CITY, "build_city"},
   {PlayerAction::ACTION_BUILD_ROAD, "build_road"},
   {PlayerAction::ACTION_BUILD_SETTLMENT, "build_settlment"},
   {PlayerAction::ACTION_BUY_CARD, "buy_card"},
   {PlayerAction::ACTION_FINISH_TURN, "finish_turn"},
   {PlayerAction::ACTION_ROLL, "roll"},
   {PlayerAction::ACTION_TRADE, "trade"},
   {PlayerAction::ACTION_USE_CARD, "use_card"}
};

string trim(string &str);
string &tolower(string &str);

Building *parseBuilding(string &line, Player *player, Catan &game);
Road *parseRoad(string &line, Player *player, Catan &game);

void findTilesFromRoad(Road *road, Tile *&outTile1, Tile *&outTile2);

bool canBuildSettlmentHere(Player *player, Building *build, bool checkRoad = true);
bool canBuildRoadHere(Player *player, Road *road);
bool canAffordSettlment(Player *player);
bool canAffordRoad(Player *player);

bool ableBuildCity(Player *player);
bool ableBuildRoad(Player *player);
bool ableBuildSettlment(Player *player);
bool ableUseCard(Player *player);

Board::Board() : canvas(), game(this) {}

// ========================= Start =========================

void Board::init() {
   printf("==================== Catan ====================\n");
   printf("Welcome to the Catan game!\n");
   //printf("Each hexagon tile on the map is \n");

   game.init();
}

void Board::startGame() {
   game.buildRound();
   bool end = false;
   while (!end) {
      end = game.doRound();
   }
}

void Board::askPlayerNames(string &outName1, string &outName2,
string &outName3) {
   printf("Please enter your names:\n");
   printf("Player 1: ");
   std::getline(std::cin, outName1);
   printf("Player 2: ");
   std::getline(std::cin, outName2);
   printf("Player 3: ");
   std::getline(std::cin, outName3);
}

void Board::rollPlayerTurnOrder(int &outP1, int &outP2, int &outP3) {
   Die die;
   int rolls[6] = {0};
   do {
      for (size_t i = 0; i < 6; i++) {
         rolls[i] = die.roll();
      }
      outP1 = rolls[0] + rolls[1];
      outP2 = rolls[2] + rolls[3];
      outP3 = rolls[4] + rolls[5];
   } while(outP1 == outP2 || outP2 == outP3 || outP3 == outP1);

   printf("Player 1 (%s) rolled:\n", game.getPlayer(0).getName().c_str());
   displayDiceRolls(rolls, 2);
   printf("Player 2 (%s) rolled:\n", game.getPlayer(1).getName().c_str());
   displayDiceRolls(rolls + 2, 2);
   printf("Player 3 (%s) rolled:\n", game.getPlayer(2).getName().c_str());
   displayDiceRolls(rolls + 4, 2);
}

// ========================== Player Input ========================

Building* Board::askPlayerForSettlmentStart(Player *player) {
   display();
   printf("Player %s:\n", player->getName().c_str());
   printf("Please choose a starting Settlment:\n");
   printf("(For example, for the settlment between tiles A, D, and B, put ADB)\n");
   printf("(For sea tile, put Z)\n");

   // Parse input
   string line;
   bool valid = false;
   while(!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));
      Building *build = parseBuilding(line, player, game);
      if (build == nullptr) {
         continue;
      }
      if (build->getType() != BuildingType::NONE) {
         printf("Cannot build settlment %s, location already taken.\n", line.c_str());
         continue;
      }

      // Check that it isn't close to another settlment:
      if (!canBuildSettlmentHere(player, build, false)) {
         printf("Cannot build settlment %s, too close to another settlment.\n"
            , line.c_str());
         continue;
      }
      return build;
   }
   // We never get here
   return nullptr;
}

Road* Board::askPlayerForRoadStart(Player *player) {
   printf("Please build a road from the settlment:\n");
   printf("Options: ");

   auto towns = player->getTowns();
   Building *town = towns[towns.size() - 1];
   Road *roads[3] = {town->getRoad1(), town->getRoad2(), town->getRoad3()};
   // Print the two letters of each road
   for (size_t i = 0; i < 3; i++) {
      if (roads[i] == nullptr){
         continue;
      }
      Tile *t1, *t2;
      findTilesFromRoad(roads[i], t1, t2);
      printf("%c%c ", t1->getLetter(), t2->getLetter());
   }
   printf("\n");

   // Parse player input:
   string line;
   bool valid = false;
   while(!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(line);

      Road *road = parseRoad(line, player, game);
      if (road == nullptr) {
         continue;
      }
      if (road != roads[0] && road != roads[1] && road != roads[2]) {
         printf("Road %s is not one of the possible roads.\n", line.c_str());
         continue;
      }

      return road;
   }
   // We never get here
   return nullptr;
}

PlayerAction Board::askPlayerActionPreRoll(Player *player) {
   printf("What do you want to do?\n");
   printf("Actions: ");

   set<string> actions;
   if (ableUseCard(player)){
      string action = actionStrings.at(PlayerAction::ACTION_USE_CARD);
      printf("%s, ", action.c_str());
      actions.insert(action);
   }
   actions.insert(actionStrings.at(PlayerAction::ACTION_ROLL));
   printf("%s\n", actionStrings.at(PlayerAction::ACTION_ROLL).c_str());
   printf("What do you want to do?\n");

   bool valid = false;
   while (!valid) {
      string line;
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (actions.count(line) == 0) {
         printf("'%s' is not a valid action.\n", line.c_str());
      } else if (line == actionStrings.at(PlayerAction::ACTION_USE_CARD)) {
         return PlayerAction::ACTION_USE_CARD;
      } else if (line == actionStrings.at(PlayerAction::ACTION_ROLL)) {
         return PlayerAction::ACTION_ROLL;
      }
   }

   // We never get here.
   return ACTION_ROLL;
}

PlayerAction Board::askPlayerAction(Player *player) {
   printf("What do you want to do?\n");
   printf("Actions: ");
   set<string> actions;

   // Check which actions are available to the player:
   if (ableBuildCity(player)) {
      string action = actionStrings.at(PlayerAction::ACTION_BUILD_CITY);
      printf("%s, ", action.c_str());
      actions.insert(action);
   } if (ableBuildRoad(player)) {
      string action = actionStrings.at(PlayerAction::ACTION_BUILD_ROAD);
      printf("%s, ", action.c_str());
      actions.insert(action);
   } if (ableBuildSettlment(player)) {
      string action = actionStrings.at(PlayerAction::ACTION_BUILD_SETTLMENT);
      printf("%s, ", action.c_str());
      actions.insert(action);
   } if (game.countCards() > 0 && game.getCard(0).canAfford(*player)) {
      string action = actionStrings.at(PlayerAction::ACTION_BUY_CARD);
      printf("%s, ", action.c_str());
      actions.insert(action);
   } if (player->countResources() > 0) {
      string action = actionStrings.at(PlayerAction::ACTION_TRADE);
      printf("%s, ", action.c_str());
      actions.insert(action);
   } if (ableUseCard(player)){
      string action = actionStrings.at(PlayerAction::ACTION_USE_CARD);
      printf("%s, ", action.c_str());
      actions.insert(action);
   }
   actions.insert(actionStrings.at(PlayerAction::ACTION_FINISH_TURN));
   printf("%s\n", actionStrings.at(PlayerAction::ACTION_FINISH_TURN).c_str());
   printf("What do you want to do?\n");

   bool valid = false;
   while (!valid) {
      string line;
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (actions.count(line) == 0) {
         printf("'%s' is not a valid action.\n", line.c_str());
      } else if (line == actionStrings.at(PlayerAction::ACTION_BUILD_CITY)) {
         return PlayerAction::ACTION_BUILD_CITY;
      } else if (line == actionStrings.at(PlayerAction::ACTION_BUILD_ROAD)) {
         return PlayerAction::ACTION_BUILD_ROAD;
      } else if (line == actionStrings.at(PlayerAction::ACTION_BUILD_SETTLMENT)) {
         return PlayerAction::ACTION_BUILD_SETTLMENT;
      } else if (line == actionStrings.at(PlayerAction::ACTION_BUY_CARD)) {
         return PlayerAction::ACTION_BUY_CARD;
      } else if (line == actionStrings.at(PlayerAction::ACTION_FINISH_TURN)) {
         return PlayerAction::ACTION_FINISH_TURN;
      } else if (line == actionStrings.at(PlayerAction::ACTION_TRADE)) {
         return PlayerAction::ACTION_TRADE;
      } else if (line == actionStrings.at(PlayerAction::ACTION_USE_CARD)) {
         return PlayerAction::ACTION_USE_CARD;
      }
   }

   // We never get here.
   return ACTION_FINISH_TURN;
}

void Board::askPlayerRobberResources(Player *player) {
   printf("Player %s has more than 7 resources!\n", player->getName().c_str());

   int bricks = player->countResource(Resource::BRICKS);
   int ore = player->countResource(Resource::ORE);
   int wheat = player->countResource(Resource::WHEAT);
   int wood = player->countResource(Resource::WOOD);
   int wool = player->countResource(Resource::WOOL);
   int half = player->countResources()/2;

   printf("Inventory: "
      "Bricks x %d, Ore x %d, Wheat x %d, Wood x %d, Wool x %d\n",
      bricks, ore, wheat, wood, wool
   );
   printf("Please choose %d resources to throw away "
      "(Input resources one by one):\n", half
   );
   
   string line;
   for (int i = 1; i <= half; i++) {
      printf("%d) ", i);
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "bricks" || line == "brick") {
         if (bricks == 0) {
            printf("Out of Bricks\n");
            i--;
         } else {
            bricks--;
            player->removeResource(Resource::BRICKS);
         }
      } else if (line == "ore") {
         if (ore == 0) {
            printf("Out of Ore\n");
            i--;
         } else {
            ore--;
            player->removeResource(Resource::ORE);
         }
      } else if (line == "wheat") {
         if (wheat == 0) {
            printf("Out of Wheat\n");
            i--;
         } else {
            wheat--;
            player->removeResource(Resource::WHEAT);
         }
      } else if (line == "wood") {
         if (wood == 0) {
            printf("Out of Wood\n");
            i--;
         } else {
            wood--;
            player->removeResource(Resource::WOOD);
         }
      } else if (line == "wool") {
         if (wool == 0) {
            printf("Out of Wool\n");
            i--;
         } else {
            wool--;
            player->removeResource(Resource::WOOL);
         }
      } else {
         printf("No such resource.\n");
         i--;
      }
   }
}

// ========================= Actions =========================

Building* Board::askBuildableSettlmentCoords(Player *player) {
   printf("Please input the settlment location as a triplet of tiles:\n");
   printf("Options: ");

   //Find out all buildable locations:
   set<Building*> builds;
   for (Road *road : player->getRoads()) {
      if (canBuildSettlmentHere(player, &road->getBuild1())) {
         builds.insert(&road->getBuild1());
      }
      if (canBuildSettlmentHere(player, &road->getBuild2())) {
         builds.insert(&road->getBuild2());
      }
   }

   // Print the builds:
   for (Building *build : builds) {
      printf("%c%c%c ", build->getTile1().getLetter(),
         build->getTile2().getLetter(), build->getTile3().getLetter());
   }
   printf("Cancel\n");

   // Parse input
   string line;
   bool valid = false;
   while(!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));
      if (line == "cancel") {
         return nullptr;
      }

      Building *build = parseBuilding(line, player, game);
      if (build == nullptr) {
         continue;
      }

      if (builds.count(build) == 0) {
         printf("Cannot build settlment %s: not on road.\n", line.c_str());
         continue;
      }

      if (build->getType() != BuildingType::NONE) {
         printf("Cannot build settlment %s, location already taken.\n", line.c_str());
         continue;
      }

      // Check that it isn't close to another settlment:
      if (!canBuildSettlmentHere(player, build, false)) {
         printf("Cannot build settlment %s, too close to another settlment.\n"
            , line.c_str());
         continue;
      }
      return build;
   }
   // We never get here
   return nullptr;
}

Road* Board::askBuildableRoadCoords(Player *player) {
   printf("Please input the road location as a pair of tiles:\n");
   printf("Options: ");

   // Find all buildable roads:
   set<Road*> roads;
   for (Road *road : player->getRoads()) {
      Building &b1 = road->getBuild1(), &b2 = road->getBuild2();
      array<Road*, 6> connected = {
         b1.getRoad1(), b1.getRoad2(), b1.getRoad3(), 
         b2.getRoad1(), b2.getRoad2(), b2.getRoad3()
      };
      for (Road *neighbour : connected) {
         if (neighbour == nullptr || neighbour == road) {
            continue;
         }
         if (neighbour->getOwner() == &Player::EMPTY_PLAYER) {
            roads.insert(neighbour);
         }
      }
   }

   // Print them to the player:
   for (Road *road : roads) {
      Tile *t1, *t2;
      findTilesFromRoad(road, t1, t2);
      printf("%c%c ", t1->getLetter(), t2->getLetter());
   }
   printf("Cancel\n");

   // Parse player input:
   string line;
   bool valid = false;
   while(!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));
      if (line == "cancel") {
         return nullptr;
      }
      
      Road *road = parseRoad(line, player, game);
      if (road == nullptr) {
         continue;
      }
      if (roads.count(road) == 0) {
         printf("Road %s is not one of the possible roads.\n", line.c_str());
         continue;
      }

      return road;
   }
   // We never get here
   return nullptr;
}

Building* Board::askUpgradableCityCoords(Player *player) {
   printf("Please input the settlment location as a triplet of tiles:\n");
   printf("Options: ");

   // Print all upgradable settlments:
   for (Building *build : player->getTowns()) {
      if (build->getType() == BuildingType::SETTLMENT) {
         printf("%c%c%c ", build->getTile1().getLetter(),
            build->getTile2().getLetter(), build->getTile3().getLetter());
      }
   }
   printf("Cancel\n");

   // Parse input
   string line;
   bool valid = false;
   while(!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));
      if (line == "cancel") {
         return nullptr;
      }

      Building *build = parseBuilding(line, player, game);
      if (build == nullptr) {
         continue;
      }

      if (build->getType() == BuildingType::NONE) {
         printf("%s is not a settlment.\n", line.c_str());
         continue;
      }
      if (build->getOwner() != player) {
         printf("%s is not your settlment.\n", line.c_str());
         continue;
      }
      if (build->getType() == BuildingType::CITY) {
         printf("%s is already a city.\n", line.c_str());
         continue;
      }
      return build;
   }
   // We never get here
   return nullptr;
}

bool Board::askToUseDevelopmentCard(Player *player, CardType &out) { 
   printf("Which development card do you want to use?\n");
   printf("Cards: ");

   // Count how many of each card the player owns:
   int monopoly = 0, plenty = 0, roads = 0;
   for (int i = 0; i < player->cardsCount(); i++) {
      switch (player->cardType(i)) {
      case CardType::CARD_MONOPOLY: monopoly++; break;
      case CardType::CARD_PLENTY_YEAR: plenty++; break;
      case CardType::CARD_ROADS: roads++; break;
      // Calm down the compiler:
      case CardType::CARD_KNIGHT: case CardType::CARD_VICTORY: break;
      }
   }
   // Print card inventory:
   if (monopoly > 0) {
      printf("Monopoly x %d ", monopoly);
   } if (plenty > 0) {
      printf("Year of Plenty x %d ", plenty);
   } if (roads > 0) {
      printf("Roads x %d ", roads);
   }
   printf("\n");

   // Print options:
   printf("Options: ");
   if (monopoly > 0) {
      printf("monopoly ");
   } if (plenty > 0) {
      printf("plenty_year ");
   } if (roads > 0) {
      printf("roads ");
   }
   printf("cancel\n");

   // Parse player input:
   string line;
   bool valid = false;
   while (!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } if (line == "monopoly" && monopoly > 0) {
         out = CardType::CARD_MONOPOLY;
         valid = true;
      } else if (line == "plenty_year" && plenty > 0) {
         out = CardType::CARD_PLENTY_YEAR;
         valid = true;
      } else if (line == "roads" && roads > 0) {
         out = CardType::CARD_ROADS;
         valid = true;
      } else if (line == "knight" || line == "victory") {
         printf("'%s' is not a usable card type.\n", line.c_str());
         valid = true;
      } else {
         printf("'%s' is not a valid card type.\n", line.c_str());
      }
   }

   return true;
}

bool Board::askMonopolyResource(Resource &outRes) {
   printf("Which resources do you want to get?\n");
   printf("Options: Bricks, Ore, Wheat, Wood, Wool, Cancel\n");

   // Parse player input:
   string line;
   bool valid = false;
   while (!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "bricks" || line == "brick") {
         outRes = Resource::BRICKS;
         valid = true;
      } else if (line == "ore") {
         outRes = Resource::ORE;
         valid = true;
      } else if (line == "wheat") {
         outRes = Resource::WHEAT;
         valid = true;
      } else if (line == "wood") {
         outRes = Resource::WOOD;
         valid = true;
      } else if (line == "wool") {
         outRes = Resource::WOOL;
         valid = true;
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
      }
   }

   return true;
}

void Board::askYearPlentyResources(Resource &outRes1, Resource &outRes2) {
   printf("Which 2 resources do you want to get?\n");
   printf("Options: Bricks, Ore, Wheat, Wood, Wool\n");

   // Parse 1st resource:
   string line;
   bool valid = false;
   while (!valid) {
      valid = true;
      printf("1) ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "bricks" || line == "brick") {
         outRes1 = Resource::BRICKS;
      } else if (line == "ore") {
         outRes1 = Resource::ORE;
      } else if (line == "wheat") {
         outRes1 = Resource::WHEAT;
      } else if (line == "wood") {
         outRes1 = Resource::WOOD;
      } else if (line == "wool") {
         outRes1 = Resource::WOOL;
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
         valid = false;
      }
   }

   // Parse 2nd resource:
   valid = false;
   while (!valid) {
      printf("2) ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "bricks") {
         outRes2 = Resource::BRICKS;
         valid = true;
      } else if (line == "ore") {
         outRes2 = Resource::ORE;
         valid = true;
      } else if (line == "wheat") {
         outRes2 = Resource::WHEAT;
         valid = true;
      } else if (line == "wood") {
         outRes2 = Resource::WOOD;
         valid = true;
      } else if (line == "wool") {
         outRes2 = Resource::WOOL;
         valid = true;
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
      }
   }

}

bool Board::askCardRoads(Player *player, Road *&outRoad1,
Road *&outRoad2) {
   printf("1st Road:");
   outRoad1 = askBuildableRoadCoords(player);
   if (outRoad1 == nullptr) {
      return false;
   }
   
   printf("2nd Road:");
   outRoad2 = askBuildableRoadCoords(player);
   if (outRoad2 == nullptr) {
      return false;
   }

   return true;
}

bool tradeBank(Player *player, vector<Resource> &buy, vector<Resource> &sell);
bool tradePlayer(Player *player, vector<Resource> &buy, vector<Resource> &sell);

bool Board::askTradeResources(Player *player, vector<Resource> &buy,
vector<Resource> &sell, bool &outBank) {
   printf("Do you want to trade with the bank or players?\n");
   printf("Options: players, bank, cancel\n");

   // Check if trading with bank or players.
   string line;
   bool valid = false;
   while (!valid) {
      valid = true;
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "players" || line == "player") {
         outBank = false;
      } else if (line == "bank") {
         outBank = true;
      } else {
         printf("'%s' is not an option.\n", line.c_str());
         valid = false;
      }
   }

   // Bank
   if (outBank) {
      return tradeBank(player, buy, sell);
   // Players:
   } else {
      return tradePlayer(player, buy, sell);
   }
}

bool tradeBank(Player *player, vector<Resource> &buy, vector<Resource> &sell) {
   printf("Which resource do you want to sell?\n");
   printf("Options: ");

   // Display which resources the player can trade with the bank
   set<Resource> resourses;
   if (player->countResource(Resource::BRICKS) >= TRADE_BANK_RATE) {
      printf("Bricks ");
      resourses.insert(Resource::BRICKS);
   }
   if (player->countResource(Resource::ORE) >= TRADE_BANK_RATE) {
      printf("Ore ");
      resourses.insert(Resource::ORE);
   }
   if (player->countResource(Resource::WHEAT) >= TRADE_BANK_RATE) {
      printf("Wheat ");
      resourses.insert(Resource::WHEAT);
   }
   if (player->countResource(Resource::WOOD) >= TRADE_BANK_RATE) {
      printf("Wood ");
      resourses.insert(Resource::WOOD);
   }
   if (player->countResource(Resource::WOOL) >= TRADE_BANK_RATE) {
      printf("Wool ");
      resourses.insert(Resource::WOOL);
   }
   printf("Cancel\n");

   // Parse resource:
   string line;
   bool valid = false;
   Resource res;
   while (!valid) {
      valid = true;
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "bricks" || line == "brick") {
         res = Resource::BRICKS;
      } else if (line == "ore") {
         res = Resource::ORE;
      } else if (line == "wheat") {
         res = Resource::WHEAT;
      } else if (line == "wood") {
         res = Resource::WOOD;
      } else if (line == "wool") {
         res = Resource::WOOL;
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
         valid = false;
      }
      if (resourses.count(res) == 0) {
         printf("You do not have at least 4 of this resource.\n");
         valid = false;
      }
   }

   // Add 4:
   sell.push_back(res); sell.push_back(res);
   sell.push_back(res); sell.push_back(res);

   printf("Which resource do you want to buy?\n");
   printf("Options: Bricks Ore Wheat Wood Wool Cancel\n");

   // Parse resource:
   valid = false;
   while (!valid) {
      valid = true;
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "bricks" || line == "brick") {
         res = Resource::BRICKS;
      } else if (line == "ore") {
         res = Resource::ORE;
      } else if (line == "wheat") {
         res = Resource::WHEAT;
      } else if (line == "wood") {
         res = Resource::WOOD;
      } else if (line == "wool") {
         res = Resource::WOOL;
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
         valid = false;
      }
   }

   buy.push_back(res);
   return true;
}

bool tradePlayer(Player *player, vector<Resource> &buy, vector<Resource> &sell) {
   printf("Which resource do you want to sell?\n");
   printf("Options: ");

   if (player->countResource(Resource::BRICKS) > 0) {
      printf("Bricks ");
   }
   if (player->countResource(Resource::ORE) > 0) {
      printf("Ore ");
   }
   if (player->countResource(Resource::WHEAT) > 0) {
      printf("Wheat ");
   }
   if (player->countResource(Resource::WOOD) > 0) {
      printf("Wood ");
   }
   if (player->countResource(Resource::WOOL) > 0) {
      printf("Wool ");
   }
   printf("Finish Cancel\n");

   int bricks = 0, ore = 0, wheat = 0, wood = 0, wool = 0;

   // Parse resource:
   string line;
   bool valid = false;
   while (!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "finish") {
         if (sell.empty()) {
            printf("No resources chosen, cancelling.\n");
            return false;
         }
         valid = true;
      } else if (line == "bricks" || line == "brick") {
         if (player->countResource(Resource::BRICKS) <= bricks) {
            printf("You do not have %d of those\n", bricks+1);
         } else {
            bricks++;
            sell.push_back(Resource::BRICKS);
         }
      } else if (line == "ore") {
         if (player->countResource(Resource::ORE) <= ore) {
            printf("You do not have %d of those\n", ore+1);
         } else {
            ore++;
            sell.push_back(Resource::ORE);
         }
      } else if (line == "wheat") {
         if (player->countResource(Resource::WHEAT) <= wheat) {
            printf("You do not have %d of those\n", wheat+1);
         } else {
            wheat++;
            sell.push_back(Resource::WHEAT);
         }
      } else if (line == "wood") {
         if (player->countResource(Resource::WOOD) <= wood) {
            printf("You do not have %d of those\n", wood+1);
         } else {
            wood++;
            sell.push_back(Resource::WOOD);
         }
      } else if (line == "wool") {
         if (player->countResource(Resource::WOOL) <= wool) {
            printf("You do not have %d of those\n", wool+1);
         } else {
            wool++;
            sell.push_back(Resource::WOOL);
         }
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
      }
   }

   printf("Which resource do you want to buy?\n");
   printf("Options: Bricks Ore Wheat Wood Wool Finish Cancel\n");

   valid = false;
   while (!valid) {
      printf("> ");
      std::getline(std::cin, line);
      line = trim(tolower(line));

      if (line == "cancel") {
         return false;
      } else if (line == "finish") {
         if (buy.empty()) {
            printf("No resources chosen, cancelling.\n");
            return false;
         }
         valid = true;
      } else if (line == "bricks" || line == "brick") {
         buy.push_back(Resource::BRICKS);
      } else if (line == "ore") {
         buy.push_back(Resource::ORE);
      } else if (line == "wheat") {
         buy.push_back(Resource::WHEAT);
      } else if (line == "wood") {
         buy.push_back(Resource::WOOD);
      } else if (line == "wool") {
         buy.push_back(Resource::WOOL);
      } else {
         printf("'%s' is not a resource.\n", line.c_str());
      }
   }

   return true;
}

Player& Board::askTradePartner(Player *player, vector<Resource> &buy,
vector<Resource> &sell) {
   printf("Player %s wants to trade:", player->getName().c_str());
   int bricks, ore, wheat, wood, wool;

   // Count sell offer:
   bricks = ore = wheat = wood = wool = 0;
   for (Resource res : sell) {
      switch(res) {
         case Resource::BRICKS: bricks++; break;
         case Resource::ORE: ore++; break;
         case Resource::WHEAT: wheat++; break;
         case Resource::WOOD: wood++; break;
         case Resource::WOOL: wool++; break;
      }
   }

   // Print sell offer:
   if (bricks > 0) {
      printf("Bricks x %d ", bricks);
   } else if (ore > 0) {
      printf("Ore x %d ", ore);
   } else if (wheat > 0) {
      printf("Wheat x %d ", wheat);
   } else if (wood > 0) {
      printf("Wood x %d ", wood);
   } else if (wool > 0) {
      printf("Wool x %d ", wool);
   }
   printf("\n");

   // Count buy offer:
   printf("For:\n");
   bricks = ore = wheat = wood = wool = 0;
   for (Resource res : buy) {
      switch(res) {
         case Resource::BRICKS: bricks++; break;
         case Resource::ORE: ore++; break;
         case Resource::WHEAT: wheat++; break;
         case Resource::WOOD: wood++; break;
         case Resource::WOOL: wool++; break;
      }
   }

   // Print sell offer:
   if (bricks > 0) {
      printf("Bricks x %d ", bricks);
   } else if (ore > 0) {
      printf("Ore x %d ", ore);
   } else if (wheat > 0) {
      printf("Wheat x %d ", wheat);
   } else if (wood > 0) {
      printf("Wood x %d ", wood);
   } else if (wool > 0) {
      printf("Wool x %d ", wool);
   }
   printf("\n");

   // Ask if anybody wants to trade:
   for (int i = 0; i < MAX_PLAYERS; i++) {
      Player &target = game.getPlayer(i);
      if (&target == player) {
         continue;
      }
      printf("Player %s: Interested? [Yes/No]\n", target.getName().c_str());

      // Parse input:
      string line;
      bool valid = false;
      while (!valid) {
         printf("> ");
         std::getline(std::cin, line);
         line = trim(tolower(line));

         if (line == "yes") {
            return target;
         } else if (line == "no") {
            valid = true;
         } else {
            printf("'%s' is not a Yes/No answer.\n", line.c_str());
         }
      }
   }

   // No player wanted to trade:
   return Player::EMPTY_PLAYER;
}


// ========================= Display =========================

void Board::message(string msg) {
   printf("%s", msg.c_str());
}

void Board::playerTurnStart(Player *player) {
   display();
   printf("Player %s's turn:\n", player->getName().c_str());
   printf("Inventory:\n");

   printf("Resources: ");
   printf("Bricks x %d ", player->countResource(Resource::BRICKS));
   printf("Ore x %d ", player->countResource(Resource::ORE));
   printf("Wheat x %d ", player->countResource(Resource::WHEAT));
   printf("Wood x %d ", player->countResource(Resource::WOOD));
   printf("Wool x %d", player->countResource(Resource::WOOL));
   printf("\n");

   int knights, monopoly, plenty, roads, victory;
   knights = monopoly = plenty = roads = victory = 0;
   printf("Cards: ");
   if (player->cardsCount() == 0) {
      printf("None\n");
   }
   for (int i = 0; i < player->cardsCount(); i++) {
      switch(player->cardType(i)) {
         case CardType::CARD_KNIGHT: knights++; break;
         case CardType::CARD_MONOPOLY: monopoly++; break;
         case CardType::CARD_PLENTY_YEAR: plenty++; break;
         case CardType::CARD_ROADS: roads++; break;
         case CardType::CARD_VICTORY: victory++; break;
      }
   }

   if (knights > 0) {
      printf("Knight x %d ", knights);
   } else if (monopoly > 0) {
      printf("Monopoly x %d ", monopoly);
   } else if (plenty > 0) {
      printf("Year of Plenty x %d ", plenty);
   } else if (roads > 0) {
      printf("Roads x %d ", roads);
   } else if (victory > 0) {
      printf("Victory x %d ", victory);
   }
   printf("\n");

   printf("Victory Points: %d\n", game.countPoints(player));
}

void Board::informRobber() {
   printf("Oh no! The dice rolled a 7, all players with more than 7 resources "
      "must discard half of their hand.\n");
}

void Board::displayWin(Player *winner) {
   printf("======== GAME OVER ========\n");
   printf("Congratulations Player %s for winning!\n", winner->getName().c_str());
}

void Board::displayResourceGain(Player *player, Resource res) {
   printf("Player %s gained a ", player->getName().c_str());
   switch(res) {
      case Resource::BRICKS: printf("Bricks.\n"); break;
      case Resource::ORE: printf("Ore.\n"); break;
      case Resource::WHEAT: printf("Wheat.\n"); break;
      case Resource::WOOD: printf("Wood.\n"); break;
      case Resource::WOOL: printf("Wool.\n"); break;
   }
}

void Board::display() {
   canvas.clear();
   for (int i = 0; i < TILES_N; i++) {
      const size_t *pos = TILE_POSITIONS[i];
      drawTile(pos[0], pos[1], i);
   }
   canvas.print();
}

void Board::displayDieRoll(int roll) {
   printDie(roll);
}

void Board::displayDiceRolls(int *dice, size_t n) {
   printDice(dice, n);
}

// ========================= Helpers =========================

string trim(string &str) {
   size_t start = 0;
   while (start < str.size() && std::isspace(str[start])) {
      start++;
   }
   if (start == str.size()) {
      return "";
   }
   
   size_t end = str.size() - 1;
   while (end > start && std::isspace(str[end])) {
      end--;
   }
   
   return str.substr(start, end-start+1);
}

string &tolower(string &str) {
   for (size_t i = 0; i < str.length(); i++) {
      str[i] = std::tolower(str[i]);
   }
   return str;
}

Building *parseBuilding(string &line, Player *player, Catan &game) {
   // Check for 3 coordinates:
   if (line.length() != 3) {
      printf("Incorrect format(length: %zu), please put 3 tiles.\n",
         line.size());
      return nullptr;
   }

   // Check for correct format(Tile letters):
   int coords[3] = {0};
   for (size_t i = 0; i < 3; i++) {
      coords[i] = std::toupper(line[i]) - 'A';
      if (coords[i] < 0 || coords[i] > 18) {
         printf("Invalid tile '%c'.\n", line[i]);
         return nullptr;
      }
   }

   Building *build = game.getBuildingBetween(coords[0], coords[1], coords[2]);
   if (build == nullptr) {
      printf("Tiles %s are not adjacent.\n", line.c_str());
      return nullptr;
   }

   return build;
}

Road *parseRoad(string &line, Player *player, Catan &game) {
   // Check for 2 coordinates:
   if (line.length() != 2) {
      printf("Incorrect format(length: %zu), please put 2 tiles.\n",
         line.size());
      return nullptr;
   }

   // Check for correct format(Tile letters):
   int coords[2] = {0};
   for (size_t i = 0; i < 2; i++) {
      coords[i] = std::toupper(line[i]) - 'A';
      if (coords[i] < 0 || coords[i] > 18) {
         printf("Invalid tile '%c'.\n", line[i]);
         return nullptr;
      }
   }

   Road *road = game.getRoadBetween(coords[0], coords[1]);
   if (road == nullptr) {
      printf("Tiles %s are not adjacent.\n", line.c_str());
      return nullptr;
   }

   return road;
}

void findTilesFromRoad(Road *road, Tile *&outTile1, Tile *&outTile2) {
   Building *build = &road->getBuild1();
   for (size_t r = 0; r < TILE_ROADS_N; r++){
      if (build->getTile1().getRoad(r) == road) {
         outTile1 = &build->getTile1();
         outTile2 = build->getTile1().getNeigbour(r);
         break;
      }
      if (build->getTile2().getRoad(r) == road) {
         outTile1 = &build->getTile2();
         outTile2 = build->getTile2().getNeigbour(r);
         break;
      }
      if (build->getTile3().getRoad(r) == road) {
         outTile1 = &build->getTile3();
         outTile2 = build->getTile3().getNeigbour(r);
         break;
      }
   }
}

bool canBuildSettlmentHere(Player *player, Building *build, bool checkRoad) {
   // Somebody already built here:
   if (build->getType() != BuildingType::NONE) {
      return false;
   }

   // Check that the settlment isn't too close to another,
   // and that it's connected to a road.
   array<Road*, 3> roads = {
      build->getRoad1(), build->getRoad2(), build->getRoad3()
   };
   bool connectedByRoad = false;
   for (Road *road : roads) {
      if (road == nullptr) {
         continue;
      }

      if (road->getOwner() == player) {
         connectedByRoad = true;
      }
      if (road->getBuild1().getType() != BuildingType::NONE
       || road->getBuild2().getType() != BuildingType::NONE) {
         return false;
      }
   }

   // Check if it's connected by road only if checkRoad is true.
   return !checkRoad || connectedByRoad;
}

bool canBuildRoadHere(Player *player, Road *road) {
   // Somebody already owns this road.
   if (road->getOwner() != &Player::EMPTY_PLAYER) {
      return false;
   }

   // Check that it's connected to another road:
   Building &b1 = road->getBuild1(), &b2 = road->getBuild2();
   array<Road*, 6> roads = {
      b1.getRoad1(), b1.getRoad2(), b1.getRoad3(), 
      b2.getRoad1(), b2.getRoad2(), b2.getRoad3()
   };

   for (Road *r : roads) {
      if (r == nullptr || r == road) {
         continue;
      }

      if (r->getOwner() == player) {
         return true;
      }
   }

   return false;
}

bool canAffordSettlment(Player *player) {
   Building setl = Building(Tile::TILE_EMPTY, Tile::TILE_EMPTY, Tile::TILE_EMPTY);
   return setl.canAfford(*player);
}

bool canAffordRoad(Player *player) {
   Building b1 = Building(Tile::TILE_EMPTY, Tile::TILE_EMPTY, Tile::TILE_EMPTY);
   Building b2 = Building(Tile::TILE_EMPTY, Tile::TILE_EMPTY, Tile::TILE_EMPTY);
   Road road = Road(&b1, &b2);
   return road.canAfford(*player);
}

// Check player's turn actions:

bool ableBuildCity(Player *player) {
   // Check that player has resources:
   for (Building *build : player->getTowns()) {
      if (build->getType() == BuildingType::SETTLMENT) {
         return build->canAfford(*player);
      }
   }

   // Can't build a city if you don't have settlments.
   return false;
}

bool ableBuildRoad(Player *player) {
   // Check that player has resources:
   if (!canAffordRoad(player)) {
      return false;
   }

   // Check that there are roads available to be built:
   for (Road *road : player->getRoads()) {
      Building &b1 = road->getBuild1(), &b2 = road->getBuild2();
      array<Road*, 6> connected = {
         b1.getRoad1(), b1.getRoad2(), b1.getRoad3(), 
         b2.getRoad1(), b2.getRoad2(), b2.getRoad3()
      };
      for (Road *neighbour : connected) {
         if (neighbour != nullptr
         && neighbour->getOwner() == &Player::EMPTY_PLAYER) {
            return true;
         }
      }
   }

   return false;
}

bool ableBuildSettlment(Player *player) {
   // Check that player has resources:
   if (!canAffordSettlment(player)) {
      return false;
   }
   
   // Check that there are buildings the player can actually build:
   set<Building*> builds;

   // Find all Building nodes between the roads the player owns:
   for (Road *road : player->getRoads()) {
      builds.insert(&road->getBuild1());
      builds.insert(&road->getBuild2());
   }

   // Go over said nodes and check if any of them are available.
   for (Building *build : builds) {
      if (canBuildSettlmentHere(player, build)) {
         return true;
      }
   }

   return false;
}

bool ableUseCard(Player *player) {
   // Check that the user has cards that are usable:
   for (int i = 0; i < player->cardsCount(); i++) {
      CardType type = player->cardType(i);
      if (type == CardType::CARD_MONOPOLY || type == CardType::CARD_PLENTY_YEAR
      || type == CardType::CARD_ROADS) {
         return true;
      }
   }

   return false;
}


