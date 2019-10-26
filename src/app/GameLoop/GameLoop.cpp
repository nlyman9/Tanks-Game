#include <iostream>
#include <chrono>
#include "GameLoop.hpp"
#include "Constants.hpp"
#include "Sprite.hpp"
#include <stdio.h>
#include <unistd.h>
#include<iostream>

GameLoop::~GameLoop() {}

bool GameLoop::networkInit(Args *options) {
	if(fork() == 0){
		//child process
		//checking working dir for testing
		//char buff[FILENAME_MAX];
 	 	//getcwd( buff, FILENAME_MAX );
  		//std::string current_working_dir(buff);
  		//std::cout << current_working_dir << std::endl;
		char *args[]={NULL}; 
		execvp("build/bin/server", args);
		//this line should not run!
		std::cout << "execvp failed" << std::endl;
	}
	// Create host process
		// Wait for connections

	// Create client process
		// Wait for map

		// Download map

		// Unpack map 

	// Init 

	return true;
}

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
	enemies.push_back(new Enemy( SCREEN_WIDTH - TANK_WIDTH/2 - 75, SCREEN_HEIGHT - TANK_HEIGHT/2 - 60, player));
	render = new Render(player, enemies);
	render->init();

	Sprite *player_tank = new Sprite(render->getRenderer(), "src/res/images/red_tank.png");
	player_tank->init();

	Sprite *enemy_tank = new Sprite(render->getRenderer(), "src/res/images/blue_tank.png");
	enemy_tank->init();

	player->setSprite(player_tank);
	for (auto enemy : enemies) {
		enemy->setSprite(enemy_tank);
	}

	isGameOn = true;

	// Fill 2D tile array of tiles with all 0s
	// int** array = 0;
	tile_map = new int*[24];

	for(int j = 0; j < 24; j++)
	{
		tile_map[j] = new int[13];
		for(int h = 0; h < 13; h++)
		{
			tile_map[j][h] = 0;
		}
	}


	//small randomly generated thing
	MapGenerator* mapGen = new MapGenerator();

	// Select map generation technique
	enum map_types { destructible, holes, line, maze, mirror };
	srand(time(NULL));

	// switch(rand() % 4)
	switch(line)
	{
		case destructible:
			tile_map[4][4] = 2;
			break;
		case holes:
			tile_map[1][1] = 2;
			break;
		case line:
			tile_map = mapGen->generateLineMap();
			break;
		case maze:
			tile_map[6][10] = 2;
			break;
		case mirror:
			tile_map = mapGen->generateMirrorMap();
			tile_map[14][10] = 2;
			break;
	}

	render->setTileMap(tile_map);

	int count = 0;
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			if(tile_map[i][j] == 2){
				tileArray.push_back(cur_out);
			}
		}
	}

	player->setObstacleLocations(&tileArray);
	for (auto enemy : enemies) {
		enemy->setObstacleLocations(&tileArray);
		enemy->setTileMap(tile_map);
	}

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