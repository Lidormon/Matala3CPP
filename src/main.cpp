#include<cstdio>
#include<map>
#include<string>
#include<cstring>
#include "ui/UI.hpp"
#include "ui/Board.hpp"
#include "Catan.hpp"
#include "Buildings.hpp"
#include "Road.hpp"
#include "Player.hpp"

using namespace ariel;

int main(int argc, char *argv[]) {
   ui::UI *gui = new ui::Board();
   gui->init();

   if (argc == 2 && std::strcmp(argv[1], "cheat") == 0) {
      printf("Cheat mode enabled!\n");
      for (int res = 0; res < 5; res++) {
         for (int i = 0; i < 1000; i++) {
            ((ui::Board*)(gui))->getGame().getPlayer(0).addResource((Resource)res);
         }
      }
   }

   try {
      gui->startGame();
   } catch (std::string str) {
      printf("Error: %s\n", str.c_str());
   }

   delete gui;
   return EXIT_SUCCESS;
}