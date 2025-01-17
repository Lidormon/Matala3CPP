#ifndef __ENUMS_HPP__
#define __ENUMS_HPP__

namespace ariel {

enum Terrain {
   FIELD ,FOREST, HILLS, MOUNTAIN, PASTURE, DESERT, SEA, EMPTY
};

enum TileDiceNum {
   TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7,
   EIGHT = 8, NINE = 9, TEN = 10, ELEVEN = 11, TWELVE = 12, SEA_NUM, EMPTY_NUM
};

enum Resource {
   WHEAT, WOOD, BRICKS, ORE, WOOL
};

enum BuildingType {
   NONE, SETTLMENT, CITY
};

enum CardType {
   CARD_MONOPOLY, CARD_ROADS, CARD_PLENTY_YEAR,
   CARD_VICTORY, CARD_KNIGHT
};

enum PlayerAction {
   ACTION_ROLL, ACTION_USE_CARD,
   ACTION_BUILD_SETTLMENT, ACTION_BUILD_CITY, ACTION_BUILD_ROAD,
   ACTION_TRADE, ACTION_BUY_CARD,
   ACTION_FINISH_TURN
};

}

#endif