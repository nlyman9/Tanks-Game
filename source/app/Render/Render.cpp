#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Render.hpp"

SDL_Texture* gTileSheet;
SDL_Rect gTileRects[3];
std::vector<SDL_Texture*> gTex;
SDL_Rect cur_out;
SDL_Texture* loadImage(std::string fname);

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

	gWindow = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// SEt up rendered with out vsync
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	gTileSheet = loadImage("source/res/images/tiles.png");
	for (int i = 0; i < 3; i++) {
			gTileRects[i].x = i * TILE_SIZE;
			gTileRects[i].y = 0;
			gTileRects[i].w = TILE_SIZE;
			gTileRects[i].h = TILE_SIZE;
	}

	return true;
}

SDL_Texture* Render::loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
			std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
			return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
			std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void Render::close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int Render::draw(double update_lag) {
		
	int c;
	SDL_Rect cur_out;

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);

	//FROM BORDERGAP + TILE SIZE TO GET INTERIOR OF MAP
	//x represents the pixels of the screen, not the tile index anymore
	for (int x = BORDER_GAP + TILE_SIZE; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE) {
		for (int y = TILE_SIZE; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE) {
			cur_out = { x, y, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[0], &cur_out);
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