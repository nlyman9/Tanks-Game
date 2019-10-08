#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TILE_SIZE = 48;
constexpr int BORDER_GAP = 16;
constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;

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
	
	gTileSheet = loadImage("../source/res/images/tiles.png");
	
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

		// enum { floor = gTileRects[0], hole = gTileRects[1], wall = gTileRects[2], bomb = gTileRects[3] };

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
		
		/**		SIMPLE Random Generation
		 * 	- Pick a side to generate on (side one (0 - 11) or side two (12 - 24))
		 * 	- Pick a random x,y in that side
		 * 	- Generate a 3x3 box around that x,y
		 * 
		 * 
		 *
		 * 
		 */
		
		srand(time(NULL));

		int maze[X_WIDE][Y_HIGH];
		int side = rand() % 2; // 0 = (0-11), 1 = (12-24
		int x_coord = (side == 0) ? rand() % 11 : (rand() % 12) + 12;
		int y_coord = rand() % 13; 

		for (int x = 0; x < X_WIDE; x++) {
			for (int y = 0; y < Y_HIGH; y++) {
				maze[x][y] = rand() % 2;
				cur_out = {BORDER_GAP + TILE_SIZE + (x*TILE_SIZE), TILE_SIZE + (y*TILE_SIZE), TILE_SIZE, TILE_SIZE};
				if (maze[x][y] == 0) {
					SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[0], &cur_out);
				} else if (maze[x][y] == 1) {
					SDL_RenderCopy(gRenderer, gTileSheet, &gTileRects[2], &cur_out);
				}
			}
		}

		SDL_RenderPresent(gRenderer);
	}

	close();
}
