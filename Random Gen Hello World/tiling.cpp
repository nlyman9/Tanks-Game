#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TILE_SIZE = 48;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTileSheet;
SDL_Rect gTileRects[3];

bool init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	gWindow = SDL_CreateWindow("Tiling", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Adding VSync to avoid absurd framerates
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	return true;
}

SDL_Texture* loadImage(std::string fname) {
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


void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_DestroyTexture(gTileSheet);
	gTileSheet = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	
	gTileSheet = loadImage("tiles.png");
	
	for (int i = 0; i < 3; i++) {
		gTileRects[i].x = i * TILE_SIZE;
		gTileRects[i].y = 0;
		gTileRects[i].w = TILE_SIZE;
		gTileRects[i].h = TILE_SIZE;
	}

	SDL_Event e;
	bool gameon = true;
	int c;
	SDL_Rect cur_out;
	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
		}
		
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);

		for (int x = 1; x * TILE_SIZE < SCREEN_WIDTH - TILE_SIZE; x++) {
			for (int y = 1; y * TILE_SIZE < SCREEN_HEIGHT - TILE_SIZE; y++) {
				cur_out = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
				SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[0], &cur_out);
			}
		}
		
		c = 0;
		while (c * TILE_SIZE < SCREEN_WIDTH) {
			cur_out = {c * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += 1;
		}
		c = 0;
		while (c * TILE_SIZE < SCREEN_HEIGHT) {
			cur_out = {0, c * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += 1;
		}
		c = 0;
		while (c * TILE_SIZE < SCREEN_WIDTH) {
			cur_out = {c * TILE_SIZE, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += 1;
		}
		c = 0;
		while (c * TILE_SIZE < SCREEN_HEIGHT) {
			cur_out = {SCREEN_WIDTH - TILE_SIZE, c * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += 1;
		}

		SDL_RenderPresent(gRenderer);
	}

	close();
}
