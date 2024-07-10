#ifndef __TILE_HPP__
#define __TILE_HPP__

#include<array>
#include "enums.hpp"

#define TILES_N 19
#define TILE_ROADS_N 6
#define TILE_BUILDS_N 6
#define TILES_AROUND_TILE 6

namespace ariel {

class Road;
class Building;

class Tile {
   int index;
   Terrain terrain;
   TileDiceNum num;

   /**
    * All the roads around the tile.
    * Road 0 is the road above the the tile
    * and the rest are Clockwise.
    */
   std::array<Road*, TILE_ROADS_N> roads;

   /**
    * All the buildings around the tile.
    * Bulding 0 is the building top-right of the the tile
    * and the rest are Clockwise.
    */
   std::array<Building*, TILE_BUILDS_N> buildings;

   /**
    * All the tiles around the tile.
    * Tile 0 is the tile above the the tile
    * and the rest are Clockwise.
    */
   std::array<Tile*, TILES_AROUND_TILE> neighbors;

public:
   static Tile TILE_SEA;
   static Tile TILE_EMPTY;

public:
   Tile(Terrain terrain, TileDiceNum num, int index);
   Terrain getTerrain();
   Resource getResource();
   TileDiceNum getNum();

   int getIndex();
   char getLetter();

   Building* getBuilding(int i);
   Road* getRoad(int i);
   Tile* getNeigbour(int i);

   friend class Catan;

private:
   Tile();
};

};

#endif