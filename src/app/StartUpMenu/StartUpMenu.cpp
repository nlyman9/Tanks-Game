#include "StartUpMenu.hpp"
#include "GameLoop.hpp"

void StartUpMenu::launch(Args *options) 
{
  drawMenu(options);
}

int StartUpMenu::drawMenu(Args *options) 
{    
  GameLoop gLoop;
  if (options->isOnline) {
    gLoop.networkInit(options);
  }

  gLoop.init();
  gLoop.run();
}