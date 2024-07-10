#include "Tile.hpp"
#include "Exceptions.hpp"

using namespace ariel;

Tile Tile::TILE_EMPTY = Tile(Terrain::EMPTY, TileDiceNum::EMPTY_NUM, -1);
Tile Tile::TILE_SEA = Tile(Terrain::SEA, TileDiceNum::SEA_NUM, 'Z' - 'A');

Tile::Tile(Terrain terrain, TileDiceNum num, int index)
: index(index), terrain(terrain), num(num) {}

Tile::Tile() : Tile(EMPTY, EMPTY_NUM, -1) {}

Terrain Tile::getTerrain() { return terrain; }
Resource Tile::getResource() { return (Resource)terrain; }
TileDiceNum Tile::getNum() { return num; }

int Tile::getIndex() { return index; }
char Tile::getLetter() { return 'A' + index; }

Building* Tile::getBuilding(int i) {
   THROW_INVALID_RANGE(i, 0, TILE_BUILDS_N-1)
   return buildings[i];
}

Road* Tile::getRoad(int i) {
   THROW_INVALID_RANGE(i, 0, TILE_ROADS_N-1)
   return roads[i];
}

Tile* Tile::getNeigbour(int i) {
   THROW_INVALID_RANGE(i, 0, TILES_AROUND_TILE-1)
   return neighbors[i];
}
