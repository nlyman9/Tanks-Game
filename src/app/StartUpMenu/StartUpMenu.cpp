#include "StartUpMenu.hpp"
#include "GameLoop.hpp"

void StartUpMenu::launch(Args *options) 
{
  renderer = new Render();
  renderer->init();

  GameLoop gLoop;
  gLoop.init(renderer);

  int gameMode = renderer->drawMenu();
  if(gameMode == MENU_SINGLE) {
    gLoop.runSinglePlayer();
  } else if(gameMode == MENU_MULTI) {
    gLoop.networkInit(options);
    gLoop.runMultiPlayer();
  } else {
    std::cout << "ROLL CREDITS" << std::endl;
  }
}