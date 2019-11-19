#include "StartUpMenu.hpp"
#include "Render.hpp"
#include "LocalGameLoop.hpp"
#include "OnlineGameLoop.hpp"

void launch(Args *options) 
{
	Render* renderer = new Render();
  	renderer->init();

	int ret = 0;
	while(ret == 0) {
		// Draw the menu
		int gameMode = renderer->drawMenu();

		// Select game mode
		if(gameMode == MENU_SINGLE) {
			LocalGameLoop localGameLoop(renderer);
			localGameLoop.init();
			ret = localGameLoop.run();
		} else if(gameMode == MENU_MULTI) {
			OnlineGameLoop onlineGameLoop(renderer);
			onlineGameLoop.init(options);
			ret = onlineGameLoop.run();
		} else if(gameMode == MENU_CREDITS) {
			std::cout << "ROLL CREDITS" << std::endl;
		} else {
			renderer->close();
		}
	}
}