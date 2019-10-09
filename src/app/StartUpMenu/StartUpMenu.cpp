#include "StartUpMenu.hpp"

void StartUpMenu::launch() 
{
  drawMenu();
}

int StartUpMenu::drawMenu() 
{    
  GameLoop gLoop;
  gLoop.init();
  gLoop.run();
}