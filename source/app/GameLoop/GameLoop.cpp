#include <iostream>
#include "GameLoop.hpp"
#include "Constants.hpp"

GameLoop::~GameLoop() = default;

int GameLoop::run()
{
	SDL_Event e;
	bool gameon = true;

	Player* player = new Player(50, 50);
	Enemy* enemy = new Enemy(SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2, player);
	Render *render = new Render(player, enemy);
	render->init();

	//Start position of obstacle - middle
	x_obst_pos = SCREEN_WIDTH / 2 - OBST_WIDTH / 2;
	y_obst_pos = SCREEN_HEIGHT / 2 - OBST_HEIGHT / 2;

	while (gameon)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameon = false;
			}
			else 
			{
				player->getEvent(e);
			}
		}
		
		enemy->updatePos();

		render->run();
	}
}