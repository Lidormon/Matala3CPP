#include<cstdio>
#include "ui/Die.hpp"

using namespace ariel;

void ui::printDie(int num) {
   for(size_t i = 0; i < DIE_LINES; i++) {
      printf("%s\n", DICE[num-1][i]);
   }
}

void ui::printDice(int *num, size_t n) {
   for(size_t i = 0; i < DIE_LINES; i++) {
      for(size_t d = 0; d < n; d++) {
         printf("%s  ", DICE[num[d]-1][i]);
      }
      printf("\n");
   }
}