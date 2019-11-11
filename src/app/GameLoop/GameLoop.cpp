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
#include "ImageLoader.hpp"
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

	Player* player2 = new Player(SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, false);
	Sprite* player_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	Sprite* player_turrent = new Sprite(render->getRenderer(), "src/res/images/red_turret.png");
	player_tank->init();
	player_turrent->init();
	player2->setSprite(player_tank);
	player2->setTurretSprite(player_turrent);
	players.push_back(player2);
	render->setPlayer(players);
	// Create client process
	client = new Client(options->ip, options->port);
	// Init
	client->init();
	player2->setClient(client);
	return true;
}

void GameLoop::initMapMultiPlayer() {
	while(!client->pollMap()) {}

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

	for (auto player : players) {
		player->setObstacleLocations(&tileArray);
	}

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

	SDL_Texture* cursor = loadImage("src/res/images/cursor.png", render->getRenderer());


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

		std::cout << "Player len: " << players.size() << std::endl;
		for(auto player : players) {
			player->getEvent(elapsed_time, &e);

			//std::cout << "check fire " << player->getFire() << std::endl;

			//network version of player firing bullet
			if (player->getFire() == true) {
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
			fflush(stdout);
			//std::cout << "finish player check fire" << std::endl;
			fflush(stdout);
		}

		std::cout << "update" << std::endl;
		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			for(auto player : players) {
				player->update();
			}

			for (auto enemy: enemies) {
				enemy->update();
			}
			for (auto projectile: projectiles) {
				projectile->update();
			}
			lag_time -= MS_PER_UPDATE;
		}

		std::cout << "cursor" << std::endl;
		// quick and dirty ;)
		int cursorX = 0, cursorY = 0;

		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&cursorX, &cursorY);
		}

		SDL_Rect cursorRect = {cursorX, cursorY, CROSSHAIR_SIZE, CROSSHAIR_SIZE};

		std::cout << "render" << std::endl;
		// 3. Render
		// Render everything
		render->draw(lag_time / MS_PER_UPDATE);
		SDL_RenderCopy(render->getRenderer(), cursor, NULL, &cursorRect);
		client->getGameBufferReady(true);

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

	Player* player = new Player(SCREEN_WIDTH/2 + 100, 50, true);
	render = renderer;
	players.push_back(player);

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	Sprite *pinksplosion =  new Sprite(render->getRenderer(), "src/res/images/pinksplosion.png");
	player_tank->init();
	player_tank->sheetSetup(30, 30, 3);
	
	
	Sprite *player_turrent = new Sprite(render->getRenderer(), "src/res/images/red_turret.png");
	player_turrent->init();
	
	player->setSprite(player_tank);
	player->setTurretSprite(player_turrent);

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

	for (auto player : players) {
		player->setObstacleLocations(&tileArray);
	}

	enemies.push_back(new Enemy( SCREEN_WIDTH/2 + 100, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, players.at(0))); // single player means player vector is size 1
	render->setEnemies(enemies);
	Sprite *enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();
	enemy_tank->sheetSetup(30, 30, 3);
	
	Sprite *enemy_turrent = new Sprite(render-> getRenderer(), "src/res/images/blue_turret.png");
	enemy_turrent->init();

	for (auto enemy : enemies) {
		enemy->setSprite(enemy_tank);
		enemy->setTurretSprite(enemy_turrent);
	}

	for (auto enemy : enemies) {
		enemy->setObstacleLocations(&tileArray);
		enemy->setTileMap(map);
		//enemy->setPathway(*map, *players.at(0), *enemy); // single player means player vector is size 1
	}
}

/**
 * @brief The actual GameLoop
 *
 */
int GameLoop::runSinglePlayer()
{
	std::cout << "single player" << std::endl;
	// Init single player only settngs
	fflush(stdout);
	render->setPlayer(players);

	SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0
	//Create bullet sprite
	Sprite *bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
	bullet->init();
	//Create shell sprite
	Sprite *shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
	shell->init();
	SDL_Texture* cursor = loadImage("src/res/images/cursor.png", render->getRenderer());
	Sprite *pinksplosion =  new Sprite(render->getRenderer(), "src/res/images/pinksplosion.png");
	pinksplosion->init();
	pinksplosion->sheetSetup(42, 42, 6);
	
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

		//checkEscape();
		for(auto player : players) {

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
		}

 		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			for(auto player : players) {
				player->update();
			}

			for (auto enemy: enemies) {
				enemy->update();
			}

			for (int i = 0; i < projectiles.size(); i++) {
				//update projectile
				projectiles.at(i)->update();
				//check if the projectile needs to be deleted
				if(projectiles.at(i)->isExploding()){
					//replace the projectile's image with the explosion
					projectiles.at(i)->setSprite(pinksplosion);
				}
				else if(projectiles.at(i)->isFinished()){
					projectiles.at(i)->~Projectile();
					//remove the projectile from the render array so the image does not stay
					render->gProjectiles.erase(render->gProjectiles.begin()+i);

					//remove projectile from projectiles array
					projectiles.erase(projectiles.begin()+i);
					i--; //since we removed an element, the next increment will skip an element so decrement
				}
			}
			lag_time -= MS_PER_UPDATE;
		}

		// quick and dirty ;)
		int cursorX = 0, cursorY = 0;
		if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&cursorX, &cursorY);
		}

		SDL_Rect cursorRect = {cursorX, cursorY, CROSSHAIR_SIZE, CROSSHAIR_SIZE};

		// 3. Render
		// Render everything
		render->draw(lag_time / MS_PER_UPDATE);
		SDL_RenderCopy(render->getRenderer(), cursor, NULL, &cursorRect);
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
