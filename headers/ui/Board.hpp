#ifndef __UI_BOARD_HPP__
#define __UI_BOARD_HPP__

#include "ui/Canvas.hpp"
#include "ui/UI.hpp"
#include "Catan.hpp"
#include "enums.hpp"

namespace ariel::ui {

/**The positions of all the tiles on the canvas.*/
constexpr size_t TILE_POSITIONS[TILES_N][2] = {
   // Centre
   {8, 15},
   // Radius 1, Clockwise:
   {6, 22}, {10, 22}, {12, 15}, {10, 8},  {6, 8}, {4, 15},
   // Radius 2, Clockwise:
   {2, 22}, {4, 29}, {8, 29}, {12, 29}, {14, 22}, {16, 15},
   {14, 8}, {12, 1}, {8, 1}, {4, 1},  {2, 8}, {0, 15}
};

class Board : public UI {
   Canvas canvas;
   Catan game;

public:
   Board();

   // ========================= Start =========================

   void init() override;

   /**Asks the user for the player names of the three players.*/
   void askPlayerNames(std::string &outName1, std::string &outName2,
      std::string &outName3) override;
   
   /**Start the game, begin drawing it and start handling user I/O.*/
   void startGame() override;

   /**
    * Roll the dice to decide the turn order for the players, the resulting
    * rolls for player 1, 2, and 3 will be written into outP1, outP2, and outP3
    * respectively.
    * In the case two players roll the same number, rerolls are made until all
    * three players score different numbers.
   */
   void rollPlayerTurnOrder(int &outP1, int &outP2, int &outP3) override;

   // ========================= Player Input =========================

   /**Ask the player to choose a settlment at the build round(The 1st round).*/
   Building *askPlayerForSettlmentStart(Player *player) override;

   /**Ask the player to choose a road at the build round(The 1st round).*/
   Road *askPlayerForRoadStart(Player *player) override;

   /**
    * Ask the player what action they want to take before rolling the dice.
    * @return Either `ACTION_USE_CARD`, or `ACTION_ROLL`.
    * @note If the user has no development cards, only option is
    * rolling the dice.
    */
   PlayerAction askPlayerActionPreRoll(Player *player) override;

   /**
    * Ask the player what action they want to take after rolling the dice.
    * @return One of: `ACTION_BUILD_SETTLMENT`, `ACTION_BUILD_CITY`,
    * `ACTION_BUILD_ROAD`, `ACTION_TRADE`, `ACTION_BUY_CARD`,
    * `ACTION_USE_CARD`, `ACTION_FINISH_TURN`.
    * @note Only available actions will be shown to the player.
    * I.e if the player has no usable development cards, the option to use a
    * card will not be given to the player, and will not be an possible return
    * value. The return value is guaranteed to be a possible action.
    */
   PlayerAction askPlayerAction(Player *player) override;

   /**
    * Inform the player that the player has more than 7 cards and
    * ask the player which cards to drop.
    * @note The function also removes the resources from the player.
    */
   void askPlayerRobberResources(Player *player) override;

   // ========================= Actions =========================

   /**
    * Ask the player to choose coords for a settlments that the player
    * is able to build(within road range and unuccupied).
    * @return A pointer to a building on the map where the player wants
    * to build a settlment, or a null pointer if the player cancels.
    */
   Building *askBuildableSettlmentCoords(Player *player) override;

   /**
    * Ask the player to choose coords for a road that the player
    * is able to build(within range of other roads and unuccupied).
    * @return A pointer to a road on the map where the player wants
    * to build a road, or a null pointer if the player cancels.
    */
   Road *askBuildableRoadCoords(Player *player) override;

   /**
    * Ask the player to choose coords for a settlment that can be
    * upgraded into a city by the player.
    * @return A pointer to a settlment on the map that the player wants
    * to upgrade to a city, or a null pointer if the player cancels.
    */
   Building *askUpgradableCityCoords(Player *player) override;

   /**
    * Ask the player to choose a development card that the player
    * can use and owns. The player's choice will be written to `out` param.
    * Non usable cards(Victory Point, and Knights) are excluded.
    * @return True if the player choses, false if the player cancels.
    */
   bool askToUseDevelopmentCard(Player *player, CardType &out) override;

   /**
    * Ask the player to choose a resource for the 'Monopoly' card and
    * write it to `res` param.
    * @return True if the player choses, false if the player cancels.
    */
   bool askMonopolyResource(Resource &outRes) override;

   /**
    * Ask the player to choose two resources for the 'Year of Plenty' card and
    * write them into `outRes1` and `outRes2` params.
    */
   void askYearPlentyResources(Resource &outRes1, Resource &outRes2) override;

   /**
    * Ask the player to choose two roads for the 'Roads' card and
    * write them into `outRoad1` and `outRoad2` params.
    * @return True if the player choses, false if the player cancels.
    */
   bool askCardRoads(Player *player, Road *&outRoad1,
      Road *&outRoad2) override;
   
   /**
    * Ask the player what resources the player wants to buy,
    * and what resources he's willing to give(sell) for them.
    * Afformentioned resources would be inserted into the `buy` and
    * `sell` vectors.
    * Asks the player if the player wants to trade with the bank or other
    * players, the answer is written into `outBank`.
    * @return True if the player choses, false if the player cancels.
    */
   bool askTradeResources(Player *player, std::vector<Resource> &buy,
   std::vector<Resource> &sell, bool &outBank) override;

   /**
    * Ask the other players if anybody's willing to accept the player's trade.
    * @return The player that accepted the offer, or the empty player if all
    * players have rejected the offer.
    */
   Player& askTradePartner(Player *player, std::vector<Resource> &buy,
   std::vector<Resource> &sell) override;

   // ========================= Display =========================
   
   /**Output a message to the user.*/
   void message(std::string msg) override;

   /**Handles any UI updates that need to happen on a new player's turn.*/
   void playerTurnStart(Player *player) override;

   /**Display that the dice rolled a 7*/
   void informRobber() override;

   /**Display a game over message and congradulate winner.*/
   void displayWin(Player *winner) override;

   /**Inform the user of gaining a resource.*/
   void displayResourceGain(Player *player, Resource res) override;
   
   /**Display the game state.*/
   void display() override;

   /**Display a die roll.*/
   void displayDieRoll(int roll) override;
   /**Display n dice rolls.*/
   void displayDiceRolls(int *dice, size_t n) override;

   Catan &getGame() {return game;}
   Canvas &getCanvas() {return canvas;}
public:
   void drawTile(int r, int c, int tile);
};

}

#endif