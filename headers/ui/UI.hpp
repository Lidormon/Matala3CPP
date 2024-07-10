#ifndef __UI_HPP__
#define __UI_HPP__

#include "enums.hpp"
#include<string>
#include<vector>

namespace ariel {
class Building;
class Road;
class Player;
}

namespace ariel::ui {

/**Represents an UI interface class.*/
class UI {
public:
   virtual ~UI() {};
   // ========================= Start =========================

   /**Initialize the UI.*/
   virtual void init() = 0;

   /**Start the game, begin drawing it and start handling user I/O.*/
   virtual void startGame() = 0;

   /**Asks the user for the player names of the three players.*/
   virtual void askPlayerNames(std::string &outName1, std::string &outName2,
      std::string &outName3) = 0;
   
   /**
    * Roll the dice to decide the turn order for the players, the resulting
    * rolls for player 1, 2, and 3 will be written into outP1, outP2, and outP3
    * respectively.
    * In the case two players roll the same number, rerolls are made until all
    * three players score different numbers.
   */
   virtual void rollPlayerTurnOrder(int &outP1, int &outP2, int &outP3) = 0;

   // ========================= Player Input =========================

   /**Ask the player to choose a settlment at the build round(The 1st round).*/
   virtual Building *askPlayerForSettlmentStart(Player *player) = 0;

   /**Ask the player to choose a road at the build round(The 1st round).*/
   virtual Road *askPlayerForRoadStart(Player *player) = 0;

   /**
    * Ask the player what action they want to take before rolling the dice.
    * @return Either `ACTION_USE_CARD`, or `ACTION_ROLL`.
    * @note If the user has no development cards, only option is
    * rolling the dice.
    */
   virtual PlayerAction askPlayerActionPreRoll(Player *player) = 0;

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
   virtual PlayerAction askPlayerAction(Player *player) = 0;

   /**
    * Inform the player that the player has more than 7 cards and
    * ask the player which cards to drop.
    * @note The function also removes the resources from the player.
    */
   virtual void askPlayerRobberResources(Player *player) = 0;

   // ========================= Actions =========================

   /**
    * Ask the player to choose coords for a settlments that the player
    * is able to build(within road range and unuccupied).
    * @return A pointer to a building on the map where the player wants
    * to build a settlment, or a null pointer if the player cancels.
    */
   virtual Building *askBuildableSettlmentCoords(Player *player) = 0;

   /**
    * Ask the player to choose coords for a road that the player
    * is able to build(within range of other roads and unuccupied).
    * @return A pointer to a road on the map where the player wants
    * to build a road, or a null pointer if the player cancels.
    */
   virtual Road *askBuildableRoadCoords(Player *player) = 0;

   /**
    * Ask the player to choose coords for a settlment that can be
    * upgraded into a city by the player.
    * @return A pointer to a settlment on the map that the player wants
    * to upgrade to a city, or a null pointer if the player cancels.
    */
   virtual Building *askUpgradableCityCoords(Player *player) = 0;

   /**
    * Ask the player to choose a development card that the player
    * can use and owns. The player's choice will be written to `out` param.
    * Non usable cards(Victory Point, and Knights) are excluded.
    * @return True if the player choses, false if the player cancels.
    */
   virtual bool askToUseDevelopmentCard(Player *player, CardType &out) = 0;

   /**
    * Ask the player to choose a resource for the 'Monopoly' card and
    * write it to `res` param.
    * @return True if the player choses, false if the player cancels.
    */
   virtual bool askMonopolyResource(Resource &outRes) = 0;

   /**
    * Ask the player to choose two resources for the 'Year of Plenty' card and
    * write them into `outRes1` and `outRes2` params.
    */
   virtual void askYearPlentyResources(Resource &outRes1, Resource &outRes2) = 0;

   /**
    * Ask the player to choose two roads for the 'Roads' card and
    * write them into `outRoad1` and `outRoad2` params.
    * @return True if the player choses, false if the player cancels.
    */
   virtual bool askCardRoads(Player *player, Road *&outRoad1,
      Road *&outRoad2) = 0;
   
   /**
    * Ask the player what resources the player wants to buy,
    * and what resources he's willing to give(sell) for them.
    * Afformentioned resources would be inserted into the `buy` and
    * `sell` vectors.
    * Asks the player if the player wants to trade with the bank or other
    * players, the answer is written into `outBank`.
    * @return True if the player choses, false if the player cancels.
    */
   virtual bool askTradeResources(Player *player, std::vector<Resource> &buy,
   std::vector<Resource> &sell, bool &outBank) = 0;

   /**
    * Ask the other players if anybody's willing to accept the player's trade.
    * @return The player that accepted the offer, or the empty player if all
    * players have rejected the offer.
    */
   virtual Player& askTradePartner(Player *player, std::vector<Resource> &buy,
   std::vector<Resource> &sell) = 0;

   // ========================= Display =========================
   
   /**Output a message to the user.*/
   virtual void message(std::string msg) = 0;

   /**Handles any UI updates that need to happen on a new player's turn.*/
   virtual void playerTurnStart(Player *player) = 0;

   /**Display that the dice rolled a 7*/
   virtual void informRobber() = 0;

   /**Display a game over message and congradulate winner.*/
   virtual void displayWin(Player *winner) = 0;

   /**Inform the user of gaining a resource.*/
   virtual void displayResourceGain(Player *player, Resource res) = 0;
   
   /**Display the game state.*/
   virtual void display() = 0;

   /**Display a die roll.*/
   virtual void displayDieRoll(int roll) = 0;
   /**Display n dice rolls.*/
   virtual void displayDiceRolls(int *dice, size_t n) = 0;
};

}

#endif