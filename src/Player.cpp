#include "Player.hpp"
#include "Buildings.hpp"
#include "Exceptions.hpp"
#include "Road.hpp"

using namespace ariel;

using std::string;
using std::vector;

Player Player::EMPTY_PLAYER;

Player::Player() : name("") {}

string Player::getName() { return name; }

void Player::setName(string name) { this->name = name; }

void Player::addResource(Resource resource) { resources.push_back(resource); }

void Player::removeResource(Resource resource) {
   for (size_t i = 0; i < resources.size(); i++) {
      if (resources[i] == resource) {
         resources.erase(resources.begin() + i);
         return;
      }
   }
}

bool Player::hasResource(Resource resource) {
   for (size_t i = 0; i < resources.size(); i++) {
      if (resources[i] == resource) {
         return true;
      }
   }
   return false;
}

int Player::countResource(Resource resource) {
   int count = 0;
   for (size_t i = 0; i < resources.size(); i++) {
      if (resources[i] == resource) {
         count++;
      }
   }
   return count;
}

vector<Resource> Player::getResources() { return resources; }

int Player::countResources() { return resources.size(); }

Resource Player::getResource(int i) {
   THROW_INVALID_RANGE(i, 0, long(resources.size()-1))
   return resources[i];
}

vector<Building*> Player::getTowns() { return towns; }

vector<Road*> Player::getRoads() { return roads; }

void Player::addTown(Building *build) {
   if (build->getOwner() != this) {
      throw "Can't add town to a player that doesn't own it!";
   }
   towns.push_back(build);
}

void Player::addRoad(Road *road) {
   if (road->getOwner() != this) {
      throw "Can't add road to a player that doesn't own it!";
   }
   roads.push_back(road);
}

int Player::cardsCount() { return cards.size(); }

CardType Player::cardType(int i) {
   THROW_INVALID_RANGE(i, 0, long(cards.size()-1))
   
   return cards[i].getType();
}

void Player::addCard(DevelopmentCard card) { cards.push_back(card); }

void Player::removeCard(CardType type) {
   for (size_t i = 0; i < cards.size(); i++) {
      if (cards[i].getType() == type) {
         cards.erase(cards.begin() + i);
         return;
      }
   }
}
