#include "StartUpMenu.hpp"
#include "Render.hpp"
#include "LocalGameLoop.hpp"
#include "OnlineGameLoop.hpp"
#include "MultiplayerMenu.hpp"
#include "Credits.hpp"

void StartUpMenu::launch(Args *options)
{
  renderer = new Render();
  renderer->init();

  GameLoop gLoop;
  gLoop.init(renderer);

		// Select game mode
		if(gameMode == MENU_SINGLE) {
			LocalGameLoop localGameLoop(renderer);
			localGameLoop.init();
			ret = localGameLoop.run();
		} else if(gameMode == MENU_MULTI) {
			OnlineGameLoop onlineGameLoop(renderer);
			std::cout << "Going into menu" << std::endl;
			options = MultiplayerMenu(renderer);
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
