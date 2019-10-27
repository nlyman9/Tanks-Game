#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "Render.hpp"

Render::~Render() {
  close();
}

bool Render::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	gWindow = SDL_CreateWindow("Tanks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	gScreenSurface = SDL_GetWindowSurface( gWindow );

	// Set up rendered with out vsync
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	ImageLoader imgLoad;
	gTileSheet = imgLoad.loadImage("src/res/images/tiles.png", gRenderer);
	for (int i = 0; i < 3; i++) {
			gTileRects[i].x = i * TILE_SIZE;
			gTileRects[i].y = 0;
			gTileRects[i].w = TILE_SIZE;
			gTileRects[i].h = TILE_SIZE;
	}

	return true;
}

void Render::close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int Render::drawMenu() {

	ImageLoader imgLoad;
	SDL_Texture* menuSinglePlayer = imgLoad.loadImage("src/res/images/menu_single_player.png", gRenderer);
	SDL_Texture* menuMultiPlayer = imgLoad.loadImage("src/res/images/menu_multi_player.png", gRenderer);
	SDL_Texture* menuCredits = imgLoad.loadImage("src/res/images/menu_credits.png", gRenderer);

	bool quit = false;
	int menuOption = MENU_SINGLE;
  	while(!quit) {
		SDL_Event e;

		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
				quit = true;
			} else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_s:
						menuOption++;
						break;
					case SDLK_w:
						menuOption--;
						break;
					case SDLK_RETURN:
						return menuOption;
						break;
				} 
			}
		}

		if(menuOption == -1) {
			menuOption = MENU_CREDITS;
		}

		if(menuOption == 3) {
			menuOption = MENU_SINGLE;
		}

		SDL_Rect fullscreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		if(menuOption == MENU_SINGLE) {
			SDL_RenderCopy(gRenderer, menuSinglePlayer, NULL, &fullscreen); 
		} else if(menuOption == MENU_MULTI) {
			SDL_RenderCopy(gRenderer, menuMultiPlayer, NULL, &fullscreen); 
		} else {
			SDL_RenderCopy(gRenderer, menuCredits, NULL, &fullscreen); 
		}

		SDL_RenderPresent(gRenderer);
	}

	return menuOption;
}

void Render::setTileMap(int** tileMap) {
	tile_map = tileMap;
}

int Render::draw(double update_lag) {
		
	int c;
	SDL_Rect cur_out;

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);

	// Render array of tiles
	for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
		for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
			cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[tile_map[i][j]], &cur_out);
		}
	}
	
	//GENERATES TOP BORDER
	c = BORDER_GAP;
	while (c < SCREEN_WIDTH - BORDER_GAP) {
		cur_out = {c, 0, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES LEFT BORDER
	c = 0;
	while (c < SCREEN_HEIGHT) {
		cur_out = {BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES BOTTOM BORDER
	c = BORDER_GAP;
	while (c < SCREEN_WIDTH - BORDER_GAP) {
		cur_out = {c, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}
	//GENERATES RIGHT BORDER
	c = 0;
	while (c < SCREEN_HEIGHT) {
		cur_out = {SCREEN_WIDTH - TILE_SIZE - BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
		SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
		c += TILE_SIZE;
	}

	// Render player
	gPlayer->draw(gRenderer, update_lag);

	// Render all the enemies
	for (auto enemy: gEnemies) {
		enemy->draw(gRenderer, update_lag);
	}

	// SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0xff, 0xff);
	// SDL_Rect fillRect_obst = {gloop->x_obst_pos, gloop->y_obst_pos, OBST_WIDTH, OBST_HEIGHT};
	// SDL_RenderFillRect(gloop->gRenderer, &fillRect_obst);

	SDL_RenderPresent(gRenderer);

	// Success?
	return 0;
}

SDL_Renderer* Render::getRenderer() {
	return gRenderer;
}

void Render::setPlayer(Player* player) {
	gPlayer = player;
}

void Render::setEnemies(std::vector<Enemy *> enemies) {
	gEnemies = enemies;
}