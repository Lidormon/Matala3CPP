#include<map>
#include "ui/Board.hpp"
#include "ui/Pixel.hpp"
#include "Tile.hpp"
#include "Buildings.hpp"
#include "Road.hpp"

using namespace ariel;
using namespace ariel::ui;
using std::map;

#define ROAD_TOP 0
#define ROAD_TOP_RIGHT 1
#define ROAD_BOTTOM_RIGHT 2
#define ROAD_BOTTOM 3
#define ROAD_BOTTOM_LEFT 4
#define ROAD_TOP_LEFT 5

#define BUILD_TOP_RIGHT 0
#define BUILD_RIGHT 1
// Can't draw the bottom ones
// #define BUILD_BOTTOM_RIGHT 2
// #define BUILD_BOTTOM_LEFT 3
#define BUILD_LEFT 4
#define BUILD_TOP_LEFT 5

constexpr color_t PLAYER1COLOR = {255, 0, 0};
constexpr color_t PLAYER2COLOR = {0, 255, 0};
constexpr color_t PLAYER3COLOR = {255, 0, 255};

struct terain_meta_t {
   color_t bg, fg;
   char name[6];
};

map<Terrain, terain_meta_t> teraMetas = {
   {Terrain::DESERT, {{194, 178, 128}, {0, 0, 0}, "Deser"} },
   {Terrain::FIELD, {{255, 195, 0}, {0, 0, 0}, "Field"} },
   {Terrain::FOREST, {{6, 28, 6}, {34, 139, 34}, "Fores"} },
   {Terrain::HILLS, {{186, 127, 80}, {53, 23, 12}, "Hills"} },
   {Terrain::MOUNTAIN, {{200, 200, 255}, {20, 20, 80}, "Mount"} },
   {Terrain::PASTURE, {{34, 139, 34}, {255, 255, 255}, "Pastu"} }
};

color_t roadColor(Catan &game, Tile &t, int road);

void putBuilding(Canvas &canvas, Catan &game, Tile &t, int build);
void putRoad(Canvas &canvas, Catan &game, Tile &t, int roadIndex);

void Board::drawTile(int r, int c, int tileIndex) {
   Tile &tile = game.getTile(tileIndex);
   // Tile &tile = game.getTile(0);
   terain_meta_t info = teraMetas[tile.getTerrain()];

   canvas.setCursor(r, c+2);
   putRoad(canvas, game, tile, ROAD_TOP);

   canvas.setCursor(r+1, c+1);
   putRoad(canvas, game, tile, ROAD_TOP_LEFT);
   putBuilding(canvas, game, tile, BUILD_TOP_LEFT);
   canvas.setColor(info.bg, info.fg);
   canvas.putChar(' ');
   canvas.putChar(tile.getLetter());
   canvas.putChar(' ');
   canvas.setColor(DEFAULT_BG, DEFAULT_FG);
   putBuilding(canvas, game, tile, BUILD_TOP_RIGHT);
   putRoad(canvas, game, tile, ROAD_TOP_RIGHT);

   canvas.setCursor(r+2, c);
   putRoad(canvas, game, tile, ROAD_TOP_LEFT);
   putBuilding(canvas, game, tile, BUILD_LEFT);
   canvas.setColor(info.bg, info.fg);
   canvas.putString(info.name);
   putBuilding(canvas, game, tile, BUILD_RIGHT);
   putRoad(canvas, game, tile, ROAD_TOP_RIGHT);

   canvas.setCursor(r+3, c);
   putRoad(canvas, game, tile, ROAD_BOTTOM_LEFT);
   putBuilding(canvas, game, tile, BUILD_LEFT);
   canvas.putChar(' ');
   canvas.setColor(info.bg, info.fg);
   canvas.putChar(' ');
   canvas.putInt((int)tile.getNum());
   if ((int)tile.getNum() < 10) {
      canvas.putChar(' ');
   }
   canvas.setColor(DEFAULT_BG, DEFAULT_FG);
   canvas.putChar(' ');
   putBuilding(canvas, game, tile, BUILD_RIGHT);
   putRoad(canvas, game, tile, ROAD_BOTTOM_RIGHT);

   canvas.setCursor(r+4, c+1);
   putRoad(canvas, game, tile, ROAD_BOTTOM_LEFT);
   putRoad(canvas, game, tile, ROAD_BOTTOM);
   putRoad(canvas, game, tile, ROAD_BOTTOM_RIGHT);
}

void putBuilding(Canvas &canvas, Catan &game, Tile &t, int buildIndex) {
   Building *build = t.getBuilding(buildIndex);
   Player *owner = build->getOwner();
   if (owner == &game.getPlayer(0)) {
      // canvas.setColor(DEFAULT_BG, PLAYER1COLOR);
      canvas.setColor(PLAYER1COLOR, DEFAULT_BG);
   } else if (owner == &game.getPlayer(1)) {
      // canvas.setColor(DEFAULT_BG, PLAYER2COLOR);
      canvas.setColor(PLAYER2COLOR, DEFAULT_BG);
   } else if (owner == &game.getPlayer(2)) {
      // canvas.setColor(DEFAULT_BG, PLAYER3COLOR);
      canvas.setColor(PLAYER3COLOR, DEFAULT_BG);
   } else {
      canvas.setColor(DEFAULT_BG, DEFAULT_FG);
   }

   if (build->getType() == BuildingType::NONE) {
      canvas.putChar(' ');
   } else if (build->getType() == BuildingType::SETTLMENT) {
      canvas.putChar('*');
   } else if (build->getType() == BuildingType::CITY) {
      canvas.putChar('#');
   }

   canvas.setColor(DEFAULT_BG, DEFAULT_FG);
}

void putRoad(Canvas &canvas, Catan &game, Tile &t, int roadIndex) {
   Road *road = t.getRoad(roadIndex);
   Player *owner = road->getOwner();

   // Set road color:
   if (owner == &game.getPlayer(0)) {
      canvas.setColor(DEFAULT_BG, PLAYER1COLOR);
   } else if (owner == &game.getPlayer(1)) {
      canvas.setColor(DEFAULT_BG, PLAYER2COLOR);
   } else if (owner == &game.getPlayer(2)) {
      canvas.setColor(DEFAULT_BG, PLAYER3COLOR);
   } else {
      canvas.setColor(DEFAULT_BG, DEFAULT_FG);
   }

   // Draw road:
   switch (roadIndex) {
   case ROAD_TOP: case ROAD_BOTTOM:
      canvas.putString("_____");
      break;
   case ROAD_TOP_RIGHT: case ROAD_BOTTOM_LEFT:
      canvas.putChar('\\');
      break;
   case ROAD_BOTTOM_RIGHT: case ROAD_TOP_LEFT:
      canvas.putChar('/');
      break;
   }
   canvas.setColor(DEFAULT_BG, DEFAULT_FG);
}

