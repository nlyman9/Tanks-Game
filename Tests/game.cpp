#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TILE_SIZE = 48;
constexpr int BORDER_GAP = 16;
constexpr int OBST_WIDTH = 70;
constexpr int OBST_HEIGHT = 40;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int MAX_VELOCITY = 1;

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
bool checkPos(int playX, int playY, int enemX, int enemY) {
       
	double  stepOne = (double) (pow((playX - enemX), 2)+pow((playY - enemY), 2));

 	double distanceAway =  (pow(stepOne, .5));	
	
	if(distanceAway < 200.0) {
		return true;
	}
	return false;
}

bool checkWall(int x, int y) {
	
	//left wall
	if(x <= 20) {
		return true;
	}
	//right wall
	else if(x >= SCREEN_WIDTH - 2*BOX_WIDTH) {
		return true;
	}
	//top wall
	else if(y <= 20) {
		return true;
	}
	//bottom wall
	else if(y >= SCREEN_HEIGHT - 2*BOX_HEIGHT) {
		return true;
	}
	else {
		return false;

	}
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

	
	// Current position to render the box
	// Start off with it in the middle
	int x_pos = 0;
	int y_pos = 0;
	

	//Start position of obstacle - middle
	int x_obst_pos = SCREEN_WIDTH/2 - OBST_WIDTH/2;
	int y_obst_pos = SCREEN_HEIGHT/2 - OBST_HEIGHT/2;


	//Enemy box
	int x_enemy_pos = SCREEN_WIDTH/2 - BOX_WIDTH/2;
	int y_enemy_pos = SCREEN_HEIGHT/2 - BOX_HEIGHT/2;

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;
	

	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
			
			else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_w:
						y_vel -= MAX_VELOCITY;
						break;

					case SDLK_a:
						x_vel -= MAX_VELOCITY;
						break;

					case SDLK_s:
						y_vel += MAX_VELOCITY;
						break;

					case SDLK_d:
						x_vel += MAX_VELOCITY;
						break;
				}
			}
			else if(e.type == SDL_KEYUP) {
				switch(e.key.keysym.sym) {
					case SDLK_w:
						y_vel = 0;
						break;
					case SDLK_a:
						x_vel = 0;
						break;
					case SDLK_s:
						y_vel = 0;
						break;
					case SDLK_d:
						x_vel = 0; 
						break;
				}
			}
		}
		
		// Move box
		if(x_vel > MAX_VELOCITY) {
			x_vel = MAX_VELOCITY;
		}
		if(x_vel < -MAX_VELOCITY) {
			x_vel = -MAX_VELOCITY;
		}
		if(y_vel > MAX_VELOCITY) {
			y_vel = MAX_VELOCITY;
		}
		if(y_vel < -MAX_VELOCITY) {
			y_vel = -MAX_VELOCITY;
		}
		x_pos += x_vel;
		y_pos += y_vel;
		if(x_pos > SCREEN_WIDTH - BOX_WIDTH) {
			x_pos = SCREEN_WIDTH - BOX_WIDTH;
		}
		if(x_pos < 0){
			x_pos = 0;
		}
		if(y_pos < 0){
			y_pos = 0;
		}
		if(y_pos > SCREEN_HEIGHT - BOX_HEIGHT) {
			y_pos = SCREEN_HEIGHT - BOX_HEIGHT;
		}

		//Checking if enemy should move away
		bool retreat;
		retreat = checkPos(x_pos, y_pos, x_enemy_pos, y_enemy_pos);
			
				
		bool nearWall;
		nearWall = checkWall(x_enemy_pos, y_enemy_pos);

		
		if(!retreat) {
			
			//go to center
			
			if(x_enemy_pos <= SCREEN_WIDTH/2) {
				x_enemy_pos += MAX_VELOCITY;
			}
			else{
				x_enemy_pos += -MAX_VELOCITY;
			}
			if(y_enemy_pos <= SCREEN_HEIGHT/2) {
				y_enemy_pos += MAX_VELOCITY;
			}
			else{
				y_enemy_pos += -MAX_VELOCITY;
			}
		
		}
		else{
			
			//run away but not near wall
			if(!nearWall) {
				if(x_pos >= x_enemy_pos) {
					x_enemy_pos += -MAX_VELOCITY;
				}
				else{
					x_enemy_pos += MAX_VELOCITY;
				}
				if(y_pos >= y_enemy_pos) {
					y_enemy_pos += -MAX_VELOCITY;
				}
				else{
					y_enemy_pos += MAX_VELOCITY;
				}
			}
			else {
			//run away and on wall
			
		
			if(x_enemy_pos == 20 || x_enemy_pos == SCREEN_WIDTH - 2*BOX_WIDTH){	
				if(y_pos >= y_enemy_pos) {
											y_enemy_pos += -MAX_VELOCITY;
									}
									else {
											y_enemy_pos += MAX_VELOCITY;
									}

		
			}
			if(y_enemy_pos == 20 || y_enemy_pos == SCREEN_HEIGHT - 2*BOX_HEIGHT) {

				if(x_pos >= x_enemy_pos) {
											x_enemy_pos += -MAX_VELOCITY;
									}
									else{
											x_enemy_pos += MAX_VELOCITY;
									}

			}	

		}

		}
		
		if(x_enemy_pos > SCREEN_WIDTH - 2*BOX_WIDTH) {
			x_enemy_pos = SCREEN_WIDTH - 2*BOX_WIDTH;
		}
		if(x_enemy_pos < 20){
			x_enemy_pos = 20;
		}
		if(y_enemy_pos < 20){
			y_enemy_pos = 20;
		}
		if(y_enemy_pos > SCREEN_HEIGHT - 2*BOX_HEIGHT) {
			y_enemy_pos = SCREEN_HEIGHT - 2*BOX_HEIGHT;
		}

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
		// Draw box
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gRenderer, &fillRect);

		SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0xff, 0xff);
        SDL_Rect fillRect_obst = {x_obst_pos, y_obst_pos, OBST_WIDTH, OBST_HEIGHT};
		SDL_RenderFillRect(gRenderer, &fillRect_obst);

		SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
		SDL_Rect enemyRect = {x_enemy_pos, y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gRenderer, &enemyRect);
		SDL_RenderPresent(gRenderer);
	}

	close();
}
