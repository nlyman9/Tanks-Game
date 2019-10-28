#include <iostream>
#include <chrono>
#include "GameLoop.hpp"
#include "Constants.hpp"
#include "Sprite.hpp"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <SDL2/SDL_thread.h>

GameLoop::~GameLoop() {}

bool GameLoop::networkInit(Args *options) {
	// Create host process
	std::cout << options->isHost << std::endl;
	if(options->isHost){
		if(fork() == 0){
			//child process
			execvp("build/bin/ServerProcess", nullptr);
			//this line should not run!
			std::cout << "execvp failed" << std::endl;
			exit(-1);
		}
	}
	// Create client process
	client = new Client();
	// Init 
	client->init();
	return true;
}

void GameLoop::initMapMultiPlayer() {
	while(!client->pollMap()) {
	}

	std::vector<int> tile_map = client->gameMap;
	std::vector<std::vector<int>> map2D;
	// init the first row
	map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE));
	int row = 0;
	int col = 0;
	for (auto tile : tile_map) {
		if(col == ( SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE) {
			row++;
			col = 0;
			map2D.push_back(std::vector<int>((SCREEN_WIDTH - BORDER_GAP - TILE_SIZE) / TILE_SIZE));
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
			}
		}
		
		player->getEvent(elapsed_time);
		
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
		std::cout << "render" << std::endl;
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
	player = new Player(75, 50);
	enemies.push_back(new Enemy( SCREEN_WIDTH - TANK_WIDTH/2 - 75, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, player));
	render = renderer;
	render->setPlayer(player);
	render->setEnemies(enemies);

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	player_tank->init();

	Sprite *enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();

	player->setSprite(player_tank);
	for (auto enemy : enemies) {
		enemy->setSprite(enemy_tank);
	}

	isGameOn = true;

	// Initialized successfully
	return true;
}

void GameLoop::initMapSinglePlayer() {
	//small randomly generated thing
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
		
		player->getEvent(elapsed_time);
		
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