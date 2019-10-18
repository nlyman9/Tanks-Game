#include <iostream>
#include <chrono>
#include "GameLoop.hpp"
#include "Constants.hpp"
#include "Sprite.hpp"

GameLoop::~GameLoop() {}

/**
 * @brief Initialize properties for Gameloop
 * 		Initializes player and enemies
 * 		Initializes the Renderer to render the game screen (Wrapper for SDL_Render)
 * 
 * @return true - Initialized successfully
 * @return false - Failed to initialize
 */
bool GameLoop::init() {
	player = new Player(75, 50);
	// enemies.push_back(new Enemy(SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2, player));
	// enemies.push_back(new Enemy(SCREEN_WIDTH - BOX_WIDTH / 2, SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2, player));
	// enemies.push_back(new Enemy(SCREEN_WIDTH / 2 - BOX_WIDTH / 2, SCREEN_HEIGHT - BOX_HEIGHT / 2, player));
	render = new Render(player, enemies);
	render->init();

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	player_tank->init();

	Sprite *enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();

	player->setSprite(player_tank);
	// for (auto enemy : enemies) {
	// 	enemy->setSprite(enemy_tank);
	// }

	isGameOn = true;

	// Initialized successfully
	return true;
}

/**
 * @brief The actual GameLoop
 * 
 */
int GameLoop::run()
{
	SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0
	while (isGameOn)
	{
		current_time = std::chrono::system_clock::now();
		elapsed_time = current_time - previous_time;
		previous_time = current_time;
		lag_time += elapsed_time.count();
		

		// 1. Process input
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
		
		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			player->update();
			
			for (auto enemy: enemies) {
				enemy->update();
			}

			lag_time -= MS_PER_UPDATE;
		}

		// 3. Render
		// Render everything 
		render->draw(lag_time / MS_PER_UPDATE);
	}

	// Exit normally
	return 0;
}