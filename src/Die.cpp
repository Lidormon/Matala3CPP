#include "Die.hpp"

using namespace ariel;

Die::Die() : rndev(), rnd(1, 6) {}

int Die::roll() { return rnd(rndev); }