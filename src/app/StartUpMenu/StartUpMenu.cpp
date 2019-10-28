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
    gLoop.initMapSinglePlayer();
    gLoop.runSinglePlayer();
  } else if(gameMode == MENU_MULTI) {
    gLoop.networkInit(options);
    gLoop.initMapMultiPlayer();
    //run the game loop
	  gLoop.networkRun();
  } else {
    std::cout << "ROLL CREDITS" << std::endl;
  }
}