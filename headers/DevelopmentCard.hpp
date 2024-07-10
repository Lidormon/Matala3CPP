#ifndef __CARD_HPP__
#define __CARD_HPP__

#include "enums.hpp"
#include "Buyable.hpp"

#define CARD_KNIGHT_N 3
#define CARD_VICTORY_N 4

namespace ariel {

class DevelopmentCard : public Buyable {
   CardType type;
public:
   DevelopmentCard(CardType type);

   void buy(Player &player) override;

   CardType getType();

   std::string toStr();
};

};

#endif