#include "StartUpMenu.hpp"
#include "Render.hpp"
#include "LocalGameLoop.hpp"
#include "OnlineGameLoop.hpp"
#include "MultiplayerMenu.hpp"
#include "Credits.hpp"

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
			
			// Only load menu if no command line options were inputted
			if(!options->isOnline) {
				options = MultiplayerMenu(renderer);
			}

			if(options == nullptr){
				//Player exited menu instead of starting a game
				gameMode = 0;
				continue;
			}
			
			onlineGameLoop.init(options);
			ret = onlineGameLoop.run();
		} else if(gameMode == MENU_CREDITS) {
			Credits credits(renderer);
			credits.playCredits();
		} else {
			renderer->close();
			exit(0);
		}
	}
}