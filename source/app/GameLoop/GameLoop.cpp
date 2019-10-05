#include <iostream>
#include "GameLoop.hpp"
#include "Constants.hpp"


GameLoop::~GameLoop() {}

/**
 * @brief Initialize properties for Gameloop
 * 
 * @return true - Initialized successfully
 * @return false - Failed to initialize
 */
bool GameLoop::init() {
	player = new Player(0, 0);
	enemies.push_back(new Enemy(SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2, player));
	enemies.push_back(new Enemy(SCREEN_WIDTH - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2, player));
	enemies.push_back(new Enemy(SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT - BOX_HEIGHT / 2, player));
	render = new Render(player, enemies);
	render->init();

	isGameOn = true;
}

int GameLoop::run()
{
	SDL_Event e;

	while (isGameOn)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isGameOn = false;
			}
			else 
			{
				player->getEvent(e);
			}
		}
		
		for (auto enemy: enemies) {
			enemy->updatePos();
		}

		render->run();
	}
}