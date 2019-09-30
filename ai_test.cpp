#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>

constexpr int SCREEN_WIDTH = 1080;
constexpr int SCREEN_HEIGHT = 720;
constexpr int OBST_WIDTH = 70;
constexpr int OBST_HEIGHT = 40;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;
constexpr int MAX_VELOCITY = 1;

// Function declarations
bool init();
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

bool init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	gWindow = SDL_CreateWindow("TANKS!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	return true;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	
	// Current position to render the box
	// Start off with it in the middle
	int x_pos = 0;
	int y_pos = 0;
	

	//Start position of obstacle - middle
	int x_obst_pos = SCREEN_WIDTH/2 - OBST_WIDTH/2;
	int y_obst_pos = SCREEN_HEIGHT/2 - OBST_HEIGHT/2;


	//Enemy box
	int x_enemy_pos = SCREEN_WIDTH - BOX_WIDTH;
	int y_enemy_pos = SCREEN_HEIGHT - BOX_HEIGHT;

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;

	SDL_Event e;
	bool gameon = true;
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

		x_enemy_pos += -x_vel;
		y_enemy_pos += -y_vel;

		if(x_enemy_pos > SCREEN_WIDTH - BOX_WIDTH) {
                        x_enemy_pos = SCREEN_WIDTH - BOX_WIDTH;
                }
                if(x_enemy_pos < 0){
                        x_enemy_pos = 0;
                }
                if(y_enemy_pos < 0){
                        y_enemy_pos = 0;
                }
                if(y_enemy_pos > SCREEN_HEIGHT - BOX_HEIGHT) {
                        y_enemy_pos = SCREEN_HEIGHT - BOX_HEIGHT;
                }

		
		
		// Draw box
		// Clear black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);
		// Cyan box
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
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

	// Out of game loop, clean up
	close();
}
