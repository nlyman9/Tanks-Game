#include "StartUpMenu.hpp"
#include "GameLoop.hpp"

void StartUpMenu::launch(Args *options) 
{
  renderer = new Render();
  renderer->init();
  drawMenu(options);
}

int StartUpMenu::drawMenu(Args *options) 
{    
  GameLoop gLoop;
  if (options->isOnline) {
    gLoop.networkInit(options);
  }
  if(renderer->drawMenu()) {
    gLoop.init(renderer);
    gLoop.run();
  }
}