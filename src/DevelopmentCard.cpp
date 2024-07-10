#include<string>
#include "DevelopmentCard.hpp"
#include "Player.hpp"

using namespace ariel;
using std::string;

DevelopmentCard::DevelopmentCard(CardType type)
: Buyable({Resource::ORE, Resource::WOOL, Resource::WHEAT}), type(type) {}

CardType DevelopmentCard::getType() { return type; }

string DevelopmentCard::toStr() {
   switch (type) {
      case CardType::CARD_KNIGHT: return "Knight";
      case CardType::CARD_MONOPOLY: return "Monopoly";
      case CardType::CARD_PLENTY_YEAR: return "Year of Plenty";
      case CardType::CARD_ROADS: return "Roads";
      case CardType::CARD_VICTORY: return "Victory Point";
   }
   return "???";
}

void DevelopmentCard::buy(Player &player) {
   Buyable::buy(player);
   player.addCard(*this);
}