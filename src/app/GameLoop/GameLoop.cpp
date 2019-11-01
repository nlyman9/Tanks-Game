#include <iostream>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <SDL2/SDL_thread.h>
#include <signal.h>

#include "GameLoop.hpp"
#include "Constants.hpp"
#include "Sprite.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>


GameLoop::~GameLoop() {}

bool GameLoop::networkInit(Args *options) {
	// Create host process
	std::cout << options->isHost << std::endl;
	if(options->isHost == true){
		int pid = fork();
		if(pid == 0) {
			//child process
			std::vector<char *> args;

			args.push_back("build/bin/ServerProcess");
			args.push_back((char *)options->ip.c_str());
			args.push_back((char *)std::to_string(options->port).c_str());
			args.push_back(NULL);

			std::cout << args[0] << " | " << args[1] << " | " << args[2] << std::endl;

			if (execvp(args[0], args.data()) == -1) {
				//this line should not run!
				std::cout << "execvp failed: " << strerror(errno) << std::endl;
				exit(-1);
			} else {
				// exit normally
				exit(0);
			} 
		} else { // Parent
			server_pid = pid;
			std::cout << "Created server process " << server_pid << std::endl;
		}
	}
	// Create client process
	client = new Client(options->ip, options->port);
	// Init 
	client->init();
	return true;
}

void GameLoop::initMapMultiPlayer() {
	while(!client->pollMap()) {
	}

	std::vector<int> tile_map = *client->gameMap;
	std::vector<std::vector<int>> map2D;
	// init the first row
	map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE - 1));
	int row = 0;
	int col = 0;
	for (auto tile : tile_map) {
		if(col == (SCREEN_HEIGHT - TILE_SIZE) / TILE_SIZE - 1) {
			row++;
			col = 0;
			map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE - 1));
		}
		map2D[row][col] = tile;
		col++;
	}

	int count = 0;
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			if(map2D[i][j] == 2){
				tileArray.push_back(cur_out);
			}
		}
	}

	render->setTileMap(&map2D);

	player->setObstacleLocations(&tileArray);
	for (auto enemy : enemies) {
		enemy->setObstacleLocations(&tileArray);
		enemy->setTileMap(&map2D);
	}
}

int GameLoop::networkRun() {
	SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0

	//Create bullet sprite
	Sprite *bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
		bullet->init();
	//Create shell sprite
	Sprite *shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
		shell->init();

	while (client->gameOn)
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
				client->gameOn = false;
				// Kill server/client thread

				std::cout << "Killing server process " << server_pid << std::endl;
				kill(server_pid, SIGTERM);
			}
		}
		
		player->getEvent(elapsed_time, &e);

		//network version of player firing bullet
		if (player->getFire() == true) {

			//std::cout << "pew\n";

			//Projectile *newlyFired = new Projectile(player->getX(), player->getY());
			//projectiles.push_back(newlyFired);
			projectiles.push_back(new Projectile(player->getX(), player->getY(), player->getTurretTheta()));

			std::cout << projectiles.back()->getX() << ", " << projectiles.back()->getY() << "; " << projectiles.back()->getTheta() << std::endl;

			render->gProjectiles.push_back(projectiles.back());
			projectiles.back()->setSprite(shell);
			//newlyFired->setSprite(bullet);
			projectiles.back()->setObstacleLocations(&tileArray);
			player->setFire(false);
		}
		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			player->update();

			for (auto enemy: enemies) {
				enemy->update();
			}
			for (auto projectile: projectiles) {
				projectile->update();
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

/**
 * @brief Initialize properties for Gameloop
 * 		Initializes player and enemies
 * 		Initializes the Renderer to render the game screen (Wrapper for SDL_Render)
 *
 * @return true - Initialized successfully
 * @return false - Failed to initialize
 */
bool GameLoop::init(Render* renderer) {
	KeyboardController* keyController = new KeyboardController();
	player = new Player(SCREEN_WIDTH/2 + 100, 50, keyController);
	enemies.clear();
	tileArray.clear();
	enemies.push_back(new Enemy( SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, player));
	render = renderer;
	render->setPlayer(player);
	render->setEnemies(enemies);

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	Sprite *player_turrent = new Sprite(render->getRenderer(), "src/res/images/red_turret.png");
	player_tank->init();
	player_turrent->init();	
	player->setSprite(player_tank);
	player->setTurretSprite(player_turrent);

	// Init the enemy
	enemies.push_back(new Enemy( SCREEN_WIDTH - TANK_WIDTH/2 - 75, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, player));
	render->setEnemies(enemies);
	Sprite *enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();
	for (auto enemy : enemies) {
		enemy->setSprite(enemy_tank);
	}

	isGameOn = true;

	// Initialized successfully
	return true;
}

void GameLoop::initMapSinglePlayer() {
	// Generate map
	MapGenerator* mapGen = new MapGenerator();
	std::vector<std::vector<int>>* map = mapGen->generateMap();
	render->setTileMap(map);
	std::vector<std::vector<int>>& mapVectors = *map;

	int count = 0;
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			if(mapVectors[i][j] == 2){
				tileArray.push_back(cur_out);
			}
		}
	}

	player->setObstacleLocations(&tileArray);
	for (auto enemy : enemies) {
		enemy->setObstacleLocations(&tileArray);
		enemy->setTileMap(map);
		enemy->setPathway(*map, *player, *enemy);
	}
}

/**
 * @brief The actual GameLoop
 *
 */
int GameLoop::runSinglePlayer()
{
	SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0
	//Create bullet sprite
	Sprite *bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
		bullet->init();
	//Create shell sprite
	Sprite *shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
		shell->init();

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
		}

		checkEscape();
		player->getEvent(elapsed_time, &e);

		//The player fired a bullet
		if (player->getFire() == true) {

			projectiles.push_back(new Projectile(player->getX(), player->getY(), player->getTurretTheta()));

			std::cout << projectiles.back()->getX() << ", " << projectiles.back()->getY() << "; " << projectiles.back()->getTheta() << std::endl;

			render->gProjectiles.push_back(projectiles.back());
			projectiles.back()->setSprite(shell);
			//newlyFired->setSprite(bullet);
			projectiles.back()->setObstacleLocations(&tileArray);
			player->setFire(false);
		}
		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			player->update();

			for (auto enemy: enemies) {
				enemy->update();
			}

			for (auto projectile: projectiles) {
				projectile->update();
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
// press escape to return to main menu
void GameLoop::checkEscape()
{
	const Uint8* keystate = SDL_GetKeyboardState(nullptr);

	if (keystate[SDL_SCANCODE_ESCAPE]) {
		this->init(render);
		int gameMode = render->drawMenu();
		if(gameMode == MENU_SINGLE) {
			this->initMapSinglePlayer();
			this->runSinglePlayer();
		} else if(gameMode == MENU_MULTI) {
			this->networkInit(new Args());
			this->initMapMultiPlayer();
			//run the game loop
			this->networkRun();
		} else if(gameMode == MENU_CREDITS) {
			std::cout << "ROLL CREDITS" << std::endl;
		} else {
			render->close();
		}
	}
}
