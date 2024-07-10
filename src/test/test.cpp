#include<cstdio>
#include<vector>
#include "Buildings.hpp"
#include "Catan.hpp"
#include "Road.hpp"
#include "doctest.h"

using namespace ariel;
using std::vector;

void addResourcesFor(Player &player, Buyable *buyable);

// 92 Tests Currently:

TEST_CASE("Test Catan Class") {
   ui::UI *ui = nullptr;
   Catan game (ui);
   game.init();
   int sea = Tile::TILE_SEA.getIndex();

   SUBCASE("Test Neighbours") {
      // Test Tile0 is neighbor of Tile4
      CHECK(game.getTile(0).getNeigbour(4) == &game.getTile(4));
      CHECK(game.getTile(4).getNeigbour(1) == &game.getTile(0));
      // Test Tile15 is neighbor of tile16
      CHECK(game.getTile(15).getNeigbour(0) == &game.getTile(16));
      CHECK(game.getTile(16).getNeigbour(3) == &game.getTile(15));
      // Test Tile9 is neighbor of Tile2
      CHECK(game.getTile(9).getNeigbour(4) == &game.getTile(2));
      CHECK(game.getTile(2).getNeigbour(1) == &game.getTile(9));
      // Test Tile14 has sea neighbours
      CHECK(game.getTile(14).getNeigbour(0) != &Tile::TILE_SEA);
      CHECK(game.getTile(14).getNeigbour(1) != &Tile::TILE_SEA);
      CHECK(game.getTile(14).getNeigbour(2) != &Tile::TILE_SEA);
      CHECK(game.getTile(14).getNeigbour(3) == &Tile::TILE_SEA);
      CHECK(game.getTile(14).getNeigbour(4) == &Tile::TILE_SEA);
      CHECK(game.getTile(14).getNeigbour(5) == &Tile::TILE_SEA);
      // Test Tile5 is neighbor of Tile0
      CHECK(game.getTile(5).getNeigbour(2) == &game.getTile(0));
      CHECK(game.getTile(0).getNeigbour(5) == &game.getTile(5));
   }

   SUBCASE("Test Buldings") {
      Building *build061 = game.getBuildingBetween(0,6,1);
      // Test Building Between Tiles 0_6_1
      CHECK(build061 == game.getTile(0).getBuilding(0));
      CHECK(build061 == game.getTile(6).getBuilding(2));
      CHECK(build061 == game.getTile(1).getBuilding(4));

      bool build061_correctTile1 =
         &build061->getTile1() == &game.getTile(0) ||
         &build061->getTile1() == &game.getTile(6) ||
         &build061->getTile1() == &game.getTile(1);
      bool build061_correctTile2 =
         &build061->getTile2() == &game.getTile(0) ||
         &build061->getTile2() == &game.getTile(6) ||
         &build061->getTile2() == &game.getTile(1);
      bool build061_correctTile3 =
         &build061->getTile3() == &game.getTile(0) ||
         &build061->getTile3() == &game.getTile(6) ||
         &build061->getTile3() == &game.getTile(1);
      bool build061_allDiff =
         &build061->getTile1() != &build061->getTile2() ||
         &build061->getTile1() != &build061->getTile3() ||
         &build061->getTile2() != &build061->getTile3();
      CHECK(build061_correctTile1);
      CHECK(build061_correctTile2);
      CHECK(build061_correctTile3);
      CHECK(build061_allDiff);


      // Test Building Between Tiles 3_13_4
      Building *build3134 = game.getBuildingBetween(3,13,4);
      CHECK(build3134 == game.getTile( 3).getBuilding(4));
      CHECK(build3134 == game.getTile(13).getBuilding(0));
      CHECK(build3134 == game.getTile( 4).getBuilding(2));
      bool build3134_correctTile1 = 
         &build3134->getTile1() == &game.getTile( 3) ||
         &build3134->getTile1() == &game.getTile(13) ||
         &build3134->getTile1() == &game.getTile( 4);
      bool build3134_correctTile2 = 
         &build3134->getTile2() == &game.getTile( 3) ||
         &build3134->getTile2() == &game.getTile(13) ||
         &build3134->getTile2() == &game.getTile( 4);
      bool build3134_correctTile3 = 
         &build3134->getTile3() == &game.getTile( 3) ||
         &build3134->getTile3() == &game.getTile(13) ||
         &build3134->getTile3() == &game.getTile( 4);
      bool build3134_allDiff =
         &build3134->getTile1() != &build3134->getTile2() ||
         &build3134->getTile1() != &build3134->getTile3() ||
         &build3134->getTile2() != &build3134->getTile3();
      CHECK(build3134_correctTile1);
      CHECK(build3134_correctTile2);
      CHECK(build3134_correctTile3);
      CHECK(build3134_allDiff);
      

      // Test Building Between Tiles 7_8_SEA
      Building *build78s = game.getBuildingBetween(7,8,sea);
      CHECK(build78s == game.getTile(  7).getBuilding(1));
      CHECK(build78s == game.getTile(  8).getBuilding(5));
      bool build78s_correctTile1 = 
         &build78s->getTile1() == &game.getTile( 7) ||
         &build78s->getTile1() == &game.getTile( 8) ||
         &build78s->getTile1() == &Tile::TILE_SEA;
      bool build78s_correctTile2 = 
         &build78s->getTile2() == &game.getTile(  7) ||
         &build78s->getTile2() == &game.getTile(  8) ||
         &build78s->getTile2() == &Tile::TILE_SEA;
      bool build78s_correctTile3 = 
         &build78s->getTile3() == &game.getTile(  7) ||
         &build78s->getTile3() == &game.getTile(  8) ||
         &build78s->getTile3() == &Tile::TILE_SEA;
      bool build78s_allDiff =
         &build78s->getTile1() != &build78s->getTile2() ||
         &build78s->getTile1() != &build78s->getTile3() ||
         &build78s->getTile2() != &build78s->getTile3();
      CHECK(build78s_correctTile1 == true);
      CHECK(build78s_correctTile2 == true);
      CHECK(build78s_correctTile3 == true);
      CHECK(build78s_allDiff);
   }

   SUBCASE("Test Roads") {
      Road *road06 = game.getRoadBetween(0, 6);
      Building *build0 = game.getTile(0).getBuilding(0);
      Building *build5 = game.getTile(0).getBuilding(5);

      CHECK(road06 == game.getTile(0).getRoad(0));
      CHECK(road06 == game.getTile(6).getRoad(3));

      bool road06_correctBuild1 = 
         &road06->getBuild1() == build0 || &road06->getBuild1() == build5;
      bool road06_correctBuild2 = 
         &road06->getBuild2() == build0 || &road06->getBuild2() == build5;
      CHECK(road06_correctBuild1);
      CHECK(road06_correctBuild2);
      CHECK(&road06->getBuild1() != &road06->getBuild2());
   }

   SUBCASE("Test Points") {
      // Victory Card(+1) + 3xKnights(+2) + Settlment(+1) + City(+2)
      const int expected = 1 + 2 + 1 + 2;
      Player &player = game.getPlayer(0);
      Building *settlment = game.getBuildingBetween(0, 6, 1);
      Building *city = game.getBuildingBetween(5, 17, 16);
      player.addCard(CardType::CARD_VICTORY); // +1 point
      player.addCard(CardType::CARD_KNIGHT);
      player.addCard(CardType::CARD_KNIGHT);
      player.addCard(CardType::CARD_KNIGHT); // +2 points

      addResourcesFor(player, settlment);
      addResourcesFor(player, settlment);
      settlment->buy(player); // +1 point
      city->buy(player); // +1 point (still a settlment)

      addResourcesFor(player, city);
      city->buy(player); // +1 point (now a city)

      CHECK(game.countPoints(&player) == expected);
   }
}

TEST_CASE("Test Player Class") {
   Player player;
   
   SUBCASE("Test Name") {
      player.setName("Lidor");
      CHECK(player.getName() == "Lidor");
   }

   SUBCASE("Test Resources") {
      vector<Resource> resources = {
         Resource::BRICKS,
         Resource::ORE, Resource::ORE, Resource::ORE, 
         Resource::WHEAT, Resource::WHEAT
      };
      player.addResource(Resource::BRICKS);
      player.addResource(Resource::ORE);
      player.addResource(Resource::ORE);
      player.addResource(Resource::ORE);
      player.addResource(Resource::WHEAT);
      player.addResource(Resource::WHEAT);

      CHECK( player.hasResource(Resource::BRICKS));
      CHECK( player.hasResource(Resource::ORE));
      CHECK( player.hasResource(Resource::WHEAT));
      CHECK(!player.hasResource(Resource::WOOD));
      CHECK(!player.hasResource(Resource::WOOL));

      CHECK(player.countResources() == 6);
      CHECK(player.countResource(Resource::BRICKS) == 1);
      CHECK(player.countResource(Resource::ORE) == 3);
      CHECK(player.countResource(Resource::WHEAT) == 2);
      CHECK(player.countResource(Resource::WOOD) == 0);
      CHECK(player.countResource(Resource::WOOL) == 0);

      CHECK(player.getResources() == resources);

      player.removeResource(Resource::ORE);
      CHECK(player.countResources() == 5);
      CHECK(player.countResource(Resource::BRICKS) == 1);
      CHECK(player.countResource(Resource::ORE) == 2);
      CHECK(player.countResource(Resource::WHEAT) == 2);
      CHECK(player.countResource(Resource::WOOD) == 0);
      CHECK(player.countResource(Resource::WOOL) == 0);
   }

   SUBCASE("Test Cards") {
      vector<CardType> resources = {
         CardType::CARD_MONOPOLY,
         CardType::CARD_VICTORY, CardType::CARD_VICTORY,
         CardType::CARD_KNIGHT, CardType::CARD_KNIGHT, CardType::CARD_KNIGHT
      };
      player.addCard(CardType::CARD_MONOPOLY);
      player.addCard(CardType::CARD_VICTORY);
      player.addCard(CardType::CARD_VICTORY);
      player.addCard(CardType::CARD_KNIGHT);
      player.addCard(CardType::CARD_KNIGHT);
      player.addCard(CardType::CARD_KNIGHT);

      CHECK( player.cardsCount() == 6);
      CHECK(player.cardType(0) == CardType::CARD_MONOPOLY);
      CHECK(player.cardType(1) == CardType::CARD_VICTORY);
      CHECK(player.cardType(2) == CardType::CARD_VICTORY);
      CHECK(player.cardType(3) == CardType::CARD_KNIGHT);
      CHECK(player.cardType(4) == CardType::CARD_KNIGHT);
      CHECK(player.cardType(5) == CardType::CARD_KNIGHT);

      player.removeCard(CardType::CARD_KNIGHT);
      CHECK( player.cardsCount() == 5);
      CHECK(player.cardType(0) == CardType::CARD_MONOPOLY);
      CHECK(player.cardType(1) == CardType::CARD_VICTORY);
      CHECK(player.cardType(2) == CardType::CARD_VICTORY);
      CHECK(player.cardType(3) == CardType::CARD_KNIGHT);
      CHECK(player.cardType(4) == CardType::CARD_KNIGHT);
   }
}

TEST_CASE("Exception Testing") {
   Catan game (nullptr);
   game.init();

   SUBCASE("Invalid Range") {
      // Check negative indicies
      CHECK_THROWS(game.getTile(-10));
      CHECK_THROWS(game.getCard(-2));
      CHECK_THROWS(game.getPlayer(-3));
      CHECK_THROWS(game.getTile(0).getBuilding(-3));
      // Check too large indicies
      CHECK_THROWS(game.getTile(19));
      CHECK_THROWS(game.getPlayer(5));
      CHECK_THROWS(game.getCard(2500));
      CHECK_THROWS(game.getTile(0).getRoad(8));
      // Check Sea Tile
      CHECK_NOTHROW(game.getTile(Tile::TILE_SEA.getIndex()));
      CHECK(&game.getTile(Tile::TILE_SEA.getIndex()) == &Tile::TILE_SEA);
   }

   SUBCASE("Null Pointer") {
      CHECK_THROWS(game.getBuildingBetween(nullptr, &game.getTile(0), &game.getTile(1)));
      CHECK_THROWS(game.getRoadBetween(nullptr, &game.getTile(0)));
   }

   SUBCASE("Buyables") {
      Player &player1 = game.getPlayer(0);
      Player &player2 = game.getPlayer(1);
      Building &build = *game.getTile(5).getBuilding(0);
      Road &road = *game.getTile(5).getRoad(3);
      DevelopmentCard &card = game.getCard(0);
      // Check buying stuff you can't afford:
      CHECK_THROWS(card.buy(player1));
      CHECK_THROWS(build.buy(player1));
      CHECK_THROWS(road.buy(player1));

      // Check buying stuff that you can afford:
      addResourcesFor(player1, &card); addResourcesFor(player2, &card);
      addResourcesFor(player1, &build); addResourcesFor(player2, &build);
      addResourcesFor(player1, &road); addResourcesFor(player2, &road);
      CHECK_NOTHROW(card.buy(player1));
      CHECK_NOTHROW(build.buy(player1));
      CHECK_NOTHROW(road.buy(player1));

      // Check buying stuff you don't own:
      CHECK_THROWS(build.buy(player2));
      CHECK_THROWS(road.buy(player2));
   }
}


void test() {
   // CatanTest catanTest;
   // catanTest.testAll();
   exit(0);
}

void addResourcesFor(Player &player, Buyable *buyable) {
   for (Resource res : buyable->getCost()) {
      player.addResource(res);
   }
}