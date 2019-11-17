#include <iostream>
#include <chrono>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <SDL2/SDL_thread.h>
#include <signal.h>
#include <assert.h>

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
	
	// Assuming there are two players, create a second player 
	Player* player2 = new Player(SCREEN_WIDTH/2 + 100, 400, false);
	playerEnemies.push_back(player2);

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	player_tank->init();
	player_tank->sheetSetup(30, 30, 3);
	
	Sprite *player_turret = new Sprite(render->getRenderer(), "src/res/images/blue_turret.png");
	player_turret->init();
	
	player2->setSprite(player_tank);
	player2->setTurretSprite(player_turret);

	render->setPlayer(players);
	render->setPlayerEnemies(playerEnemies);
	// render->setPlayerEnemies(playerEnemies);
	// Create client process
	client = new Client(options->ip, options->port);
	// Init
	client->init();
	// player2->setClient(client);
	return true;
}

void GameLoop::initMapMultiPlayer() {
	while(!client->pollMap()) {}

	std::cout << "GAME: Setting Tile Map..." << std::endl;
	fflush(stdout);

	// std::vector<int> tile_map = client->gameMap;
	std::vector<std::vector<int>> map2D;
	// init the first row
	map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE - 1));
	int row = 0;
	int col = 0;
	for (auto tile : client->gameMap) {
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
				projectileObstacles.push_back(cur_out);
			}
		}
	}

	render->setTileMap(&map2D);

	std::cout << "GAME: Set Tile Map" << std::endl;
	fflush(stdout);

	for (auto player : players) {
		player->setObstacleLocations(&tileArray);
	}

	// for (auto enemy : enemies) {
	// 	enemy->setObstacleLocations(&tileArray);
	// 	enemy->setTileMap(&map2D);
	// }

	std::cout << "GAME: Set Player Obstacles" << std::endl;
	fflush(stdout);
}

int GameLoop::networkRun() {
	SDL_Event e;
	previous_time = std::chrono::system_clock::now(); // get current time of system
	lag_time = 0.0;	// Set duration of time to 0

	// Create bullet sprite
	Sprite *bullet = new Sprite(render->getRenderer(), "src/res/images/bullet.png");
	bullet->init();

	// Create shell sprite
	Sprite *shell = new Sprite(render->getRenderer(), "src/res/images/shell.png");
	shell->init();
	
	// Initialize cursor sprite
	SDL_Texture* cursor = loadImage("src/res/images/cursor.png", render->getRenderer());
	
	// Setup bullet explosion animation
	Sprite *pinksplosion =  new Sprite(render->getRenderer(), "src/res/images/pinksplosion.png");
	pinksplosion->init();
	pinksplosion->sheetSetup(42, 42, 6);

	//wait for both players to connect
	while(!client->startGame) {
		sleep(1); 
	} 

	const Uint8 *keystate;
	const Uint8 *keyStatePacket;
	int temp = 0;
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

		assert(players.size() == 1);
		for(auto player : players) {
			// Send same keystate to player object and to the client to send
			// Lets just support 10 keys at the same time
			// TODO find a good rate to send player keystates
			keystate = SDL_GetKeyboardState(nullptr);
			player->getEvent(elapsed_time, &e, keystate);

			//network version of player firing bullet
			if (player->getFire() == true) {
				//Projectile *newlyFired = new Projectile(player->getX(), player->getY());
				//projectiles.push_back(newlyFired);
				projectiles.push_back(new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta()));

				std::cout << projectiles.back()->getX() << ", " << projectiles.back()->getY() << "; " << projectiles.back()->getTheta() << std::endl;

				render->setProjectiles(projectiles);
				projectiles.back()->setSprite(shell);
				projectiles.back()->setObstacleLocations(&projectileObstacles);
				player->setFire(false);
			}
			fflush(stdout);
		}

		// Set inputs of enemy players over network
		for(auto playerEnemy : playerEnemies) {
			// Get the keystates from network
			// std::cout << "Getting network player's keystates... " << std::endl;
			// fflush(stdout);
			keyStatePacket = client->getKeyState(0);

			// std::cout << "Applying keystates from player..." << std::endl;
			// fflush(stdout);

			// std::cout << "Keystate " << keyStatePacket << std::endl;

			// Apply keysates to the network player
			playerEnemy->getEvent(elapsed_time, &e, keyStatePacket);
		}
		// std::cout << "update" << std::endl;
		// 2. Update
		// Update if time since last update is >= MS_PER_UPDATE
		while(lag_time >= MS_PER_UPDATE) {
			for(auto player : players) {
				player->update();
			}

			// Basically add a keyframe every ~2 updates -> 30 times a second
			temp += 1;
			// TODO Consolidate tickrates
			if (temp > 2 && keystate != nullptr) {
				printf("Keystate of w = %d \n", keyStatePacket[SDL_SCANCODE_W]);
				fflush(stdout);
				client->addLocalKeyState(keystate);
				keystate = nullptr; //only need to send one per update loop
				temp = 0;
			}

			for (auto playerEnemy : playerEnemies) {
				playerEnemy->update();
			}

			// for (auto enemy: enemies) {
			// 	enemy->update();
			// }
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

		// std::cout << "cursor" << std::endl;
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
		// client->getGameBufferReady(true);

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
			SDL_Rect hole_tile = { x+5, y+5, TILE_SIZE-5, TILE_SIZE-5 }; //does not work, enemy AI needs update
			if(mapVectors[i][j] == 2){
				tileArray.push_back(cur_out);
				enemyTileArray.push_back(cur_out);
				projectileObstacles.push_back(cur_out);
			}
			else if(mapVectors[i][j] == 1){
				tileArray.push_back(hole_tile);
				enemyTileArray.push_back(cur_out);
			}
		}
	}

	for (auto player : players) {
		player->setObstacleLocations(&tileArray);
	}

	std::vector<int> enemySpawn = GameLoop::spawnEnemies(map, 1);
	enemies.push_back(new Enemy( enemySpawn.at(0), enemySpawn.at(1), players.at(0))); // single player means player vector is size 1

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
 * @brief Create an unoccupied spawn location for enemy tanks
 * @param map   Vector representing current map state
 * @param count Total number of enemies spawnpoints to generate
 * @return      Vector of ints with enemy spawnpoints as pixel coordinates, (0=x1, 1=y1, etc)
 * TODO:        Allow to be used for multiple tanks
 */
std::vector<int> GameLoop::spawnEnemies(std::vector<std::vector<int>> *map, int count)
{
	std::vector<std::vector<int>> tileMap = *map;
	std::vector<int> coords;
	int enemy_x, enemy_y;
	int i = 0;

	while(true)
	{
		enemy_x = (rand() % 16) + 4;
		enemy_y = (rand() % 3) + 10;

		if(tileMap[enemy_y][enemy_x] == 0)
		{
			if(i < count)
			{
				i++;
			}
			else
			{	
				break;
			}
		}
	}

	coords.push_back(enemy_x * 48 + 100);
	coords.push_back(enemy_y * 48 + 48);

	return coords;
}

// Returns a vector with two int values, x at 0 and y at 1
// Values represent pixel coordinates of enemy spawn point
std::vector<int> GameLoop::spawnEnemy(std::vector<std::vector<int>> *map)
{
	std::vector<std::vector<int>> tileMap = *map;
	std::vector<int> coords;
	int enemy_x, enemy_y;

	while(true)
	{
		enemy_x = (rand() % 16) + 4;
		enemy_y = (rand() % 4) + 9;

		if(tileMap[enemy_y][enemy_x] == 0)
		{
			break;
		}
	}

	coords.push_back(enemy_x * 48 + 100);
	coords.push_back(enemy_y * 48 + 48);

	return coords;
}

/**
 * @brief The actual GameLoop
 *
 */
int GameLoop::runSinglePlayer()
{
	std::cout << "Running single player..." << std::endl;
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

		checkEscape();
		for(auto player : players) {
			const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
			player->getEvent(elapsed_time, &e, keystate);

			//The player fired a bullet
			if (player->getFire() == true) {

				projectiles.push_back(new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta()));

				std::cout << projectiles.back()->getX() << ", " << projectiles.back()->getY() << "; " << projectiles.back()->getTheta() << std::endl;

				// render->gProjectiles.push_back(projectiles.back());
				render->setProjectiles(projectiles);
				projectiles.back()->setSprite(shell);
				//newlyFired->setSprite(bullet);
				projectiles.back()->setObstacleLocations(&projectileObstacles);
				player->setFire(false);
			}
		}

		for(auto enemy : enemies) {
			if(enemy->getFire() == true){
				projectiles.push_back(new Projectile(enemy->getX() + TANK_WIDTH/4, enemy->getY() + TANK_HEIGHT/4, enemy->getTurretTheta()));

				// render->gProjectiles.push_back(projectiles.back());
				render->setProjectiles(projectiles);
				projectiles.back()->setSprite(shell);
				projectiles.back()->setObstacleLocations(&projectileObstacles);
				enemy->setFire(false);
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
