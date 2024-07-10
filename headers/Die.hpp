#ifndef __DIE_HPP__
#define __DIE_HPP__

#include<random>

namespace ariel {

class Die {
   std::random_device rndev;
   std::uniform_int_distribution<int> rnd;
public:
   Die();

   int roll();
};

}

#endif