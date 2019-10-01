#include "GameLoop.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "Credits.hpp"

GameLoop::~GameLoop() {
	close();
}

bool GameLoop::init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	gWindow = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void GameLoop::close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int GameLoop::run() {
  if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	// Current position to render the box
	// Start off with it in the middle
	int x_pos = SCREEN_WIDTH/2 - BOX_WIDTH/2;
	int y_pos = SCREEN_HEIGHT/2 - BOX_HEIGHT/2;

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
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
					Credits c(gWindow, gRenderer, gTex, gSound);
					c.playCredits();
			}		
		}

        //check key states
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        //y movement
        if( currentKeyStates[SDL_SCANCODE_W]){
            y_vel--;
        }else if( currentKeyStates[ SDL_SCANCODE_S ] ){
            y_vel++;
        }//if the player is not move in the y axis reduce speed
        else if(y_vel > 0){
            y_vel--;
        }else if(y_vel < 0){
            y_vel++;
        }
        //x movement
        if( currentKeyStates[ SDL_SCANCODE_A ] ){
            x_vel--;
        }else if( currentKeyStates[ SDL_SCANCODE_D ] ){
            x_vel++;
        }
        //if no key is pressed then reduce speed
        else if(x_vel > 0){
            x_vel--;
        }else if(x_vel < 0){
            x_vel++;
        }

        //cap movement speed
        if(x_vel > MAX_SPEED){
            x_vel = MAX_SPEED;
        }else if(x_vel < -MAX_SPEED){
            x_vel = -MAX_SPEED;
        }
        if(y_vel > MAX_SPEED){
            y_vel = MAX_SPEED;
        }else if(y_vel < -MAX_SPEED){
            y_vel = -MAX_SPEED;
        }

		// Move box
		x_pos += x_vel;
		y_pos += y_vel;

        //check boundaries
        if(x_pos < 0){
            x_pos = 0;
            x_vel = 0;
        }else if(x_pos > (SCREEN_WIDTH - BOX_WIDTH)){
            x_pos = SCREEN_WIDTH - BOX_WIDTH;
            x_vel = 0;
        }
        if(y_pos < 0){
            y_pos = 0;
            y_vel = 0;
        }else if(y_pos > (SCREEN_HEIGHT - BOX_HEIGHT)){
            y_pos = SCREEN_HEIGHT - BOX_HEIGHT;
            y_vel = 0;
        }
		
		// Draw box
		// Clear black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);
		
		// Cyan box
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gRenderer, &fillRect);
		
        SDL_RenderPresent(gRenderer);

	}

	// Out of game loop, clean up
	close();
}