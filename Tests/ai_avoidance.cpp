#include <iostream>
#include <complex>
#include <vector>
#include <string>
#include <SDL.h>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;

// Function declarations
bool init();
bool check_collision(SDL_Rect* a, SDL_Rect* b);
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
	
	gWindow = SDL_CreateWindow("AI Avoidance", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

bool check_collision(SDL_Rect* a, SDL_Rect* b) {
	// Check vertical overlap
	if (a->y + a->h <= b->y)
		return false;
	if (a->y >= b->y + b->h)
		return false;

	// Check horizontal overlap
	if (a->x >= b->x + b->w)
		return false;
	if (a->x + a->w <= b->x)
		return false;

	// Must overlap in both
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
	
	// Moving box
	SDL_Rect moveBox = {SCREEN_WIDTH/2  - BOX_WIDTH/2, SCREEN_HEIGHT/2 - BOX_HEIGHT/2, BOX_WIDTH, BOX_HEIGHT};	
	int x_vel = 0;
	int y_vel = 0;
	int x_deltav = 0;
	int y_deltav = 0;

	// Static box
	SDL_Rect staticBox = {SCREEN_WIDTH/2 + (2 * BOX_WIDTH), SCREEN_HEIGHT/2 - BOX_HEIGHT/2, (2 * BOX_WIDTH), (10 * BOX_HEIGHT)};


	// Enemy box
	SDL_Rect aiBox = {0, SCREEN_HEIGHT/2, BOX_WIDTH, BOX_HEIGHT};
	int x_ai_vel = 0;
	int y_ai_vel = 0;
	int x_ai_deltav = 0;
	int y_ai_deltav = 0;
	/*
	int x_ai_pos = 0;
	int y_ai_pos = SCREEN_HEIGHT/2;
	int x_ai
	*/
	// AI detection box
	SDL_Rect x_detectBox = {0, SCREEN_HEIGHT/2, BOX_WIDTH, BOX_HEIGHT};
	SDL_Rect y_detectBox = {0, SCREEN_HEIGHT/2, BOX_WIDTH, BOX_HEIGHT};
	//int x_ai_detect = 0;
	//int y_ai_detect = 0;
	
	// AI destination
	SDL_Rect destBox = {SCREEN_WIDTH - BOX_WIDTH, SCREEN_HEIGHT/2, BOX_WIDTH, BOX_HEIGHT};

	SDL_Event e;
	bool gameon = true;
	while(gameon) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
		}

		// Modify player box state
		x_deltav = 0;
		y_deltav = 0;
		const Uint8* keystate = SDL_GetKeyboardState(nullptr);
		if (keystate[SDL_SCANCODE_W])
			y_deltav -= 1;
		if (keystate[SDL_SCANCODE_A])
			x_deltav -= 1;
		if (keystate[SDL_SCANCODE_S])
			y_deltav += 1;
		if (keystate[SDL_SCANCODE_D])
			x_deltav += 1;

		if (x_deltav == 0) {
			if (x_vel > 0)
				x_deltav = -1;
			else if (x_vel < 0)
				x_deltav = 1;
		}
		if (y_deltav == 0) {
			if (y_vel > 0)
				y_deltav = -1;
			else if (y_vel < 0)
				y_deltav = 1;
		}
		
		x_vel += x_deltav;
		y_vel += y_deltav;

		if (x_vel < -5)
			x_vel = -5;
		else if (x_vel > 5)
			x_vel = 5;

		if (y_vel < -5)
			y_vel = -5;
		else if (y_vel > 5)
			y_vel = 5;

		// Try to move vertically
		moveBox.y += y_vel;
		if (moveBox.y < 0 || (moveBox.y + BOX_HEIGHT > SCREEN_HEIGHT) || check_collision(&staticBox, &moveBox))
			// Can't move, go back
			moveBox.y -= y_vel;

		// Try to move horizontally
		moveBox.x += x_vel;
		if (moveBox.x < 0 || (moveBox.x + BOX_WIDTH > SCREEN_WIDTH) || check_collision(&staticBox, &moveBox))
			moveBox.x -= x_vel;
		
		// Modify AI box state
		
		x_ai_deltav = 0;
		y_ai_deltav = 0;
		int x_dist = destBox.x - aiBox.x;
		int y_dist = destBox.y - aiBox.y;
		
		bool x_detect = check_collision(&x_detectBox, &staticBox);
		bool y_detect = check_collision(&y_detectBox, &staticBox);
		
		if (x_detect)
		{
			//slow down
			if(x_ai_vel > 0)
				x_ai_deltav = -1;
			else if (x_ai_vel < 0)
				x_ai_deltav = 1;
			
			if(aiBox.y + BOX_HEIGHT > staticBox.y + (staticBox.h / 2))
			{
				y_ai_deltav = 1;
			}
			else
				y_ai_deltav = -1;
			
		}
		if (y_detect)
		{
			if(y_ai_vel > 0)
				y_ai_deltav = -1;
			else if (y_ai_vel < 0)
				y_ai_deltav = 1;
			
			if(aiBox.x + BOX_WIDTH > staticBox.x + (staticBox.w / 2))
			{
				x_ai_deltav = 1;
			}
			else
				x_ai_deltav = -1;
			
		}
		
		if(x_detect == false && y_detect == false)
		{
			
			if (x_dist != 0 && y_dist == 0)
			{
				if (x_dist > 0)
					x_ai_deltav += 1;
				else
					x_ai_deltav -= 1;
			}
			else if (x_dist == 0 && y_dist != 0)
			{	
				if (y_dist > 0)
					y_ai_deltav += 1;
				else
					y_ai_deltav -= 1;
			}
			else if (x_dist > 0 && y_dist > 0)
			{
				if(x_dist >= y_dist)
					x_ai_deltav += 1;
				else
					y_ai_deltav += 1;
			}
			else if (x_dist > 0 && y_dist < 0)
			{
				if(x_dist >= -y_dist)
					x_ai_deltav += 1;
				else
					y_ai_deltav -= 1;
			}
			else if (x_dist < 0 && y_dist < 0)
			{
				if(-x_dist >= y_dist)
					x_ai_deltav -= 1;
				else
					y_ai_deltav -= 1;
				
			}
			else if (x_dist < 0 && y_dist > 0)
			{
				if(-x_dist >= -y_dist)
					x_ai_deltav -= 1;
				else
					y_ai_deltav += 1;
			}
		}
		
		if (x_ai_deltav == 0) {
			if (x_ai_vel > 0)
				x_ai_deltav = -1;
			else if (x_ai_vel < 0)
				x_ai_deltav = 1;
		}
		if (y_ai_deltav == 0) {
			if (y_ai_vel > 0)
				y_ai_deltav = -1;
			else if (y_ai_vel < 0)
				y_ai_deltav = 1;
		}
		
		x_ai_vel += x_ai_deltav;
		y_ai_vel += y_ai_deltav;

		if (x_ai_vel < -5)
			x_ai_vel = -5;
		else if (x_ai_vel > 5)
			x_ai_vel = 5;

		if (y_ai_vel < -5)
			y_ai_vel = -5;
		else if (y_ai_vel > 5)
			y_ai_vel = 5;
		
		// AI move vertically
		aiBox.y += y_ai_vel;
		if (aiBox.y < 0 || (aiBox.y + BOX_HEIGHT > SCREEN_HEIGHT) || check_collision(&staticBox, &aiBox))
			// Can't move, go back
			aiBox.y -= y_ai_vel;

		// AI move horizontally
		aiBox.x += x_ai_vel;
		if (aiBox.x < 0 || (aiBox.x + BOX_WIDTH > SCREEN_WIDTH) || check_collision(&staticBox, &aiBox))
			aiBox.x -= x_ai_vel;
		
		// Modify detection box
		if(x_ai_vel > 0)
		{
			x_detectBox.x = aiBox.x;
			x_detectBox.y = aiBox.y;
			x_detectBox.w = aiBox.w * 3;
		}
		else if (x_ai_vel < 0)
		{
			x_detectBox.x = aiBox.x - (aiBox.w * 3);
			x_detectBox.y = aiBox.y;
			x_detectBox.w = aiBox.w * 3;
		}
		else
		{
			x_detectBox.x = aiBox.x;
			x_detectBox.y = aiBox.y;
			x_detectBox.w = aiBox.w;
		}
		if(y_ai_vel > 0)
		{
			y_detectBox.x = aiBox.x;
			y_detectBox.y = aiBox.y;
			y_detectBox.h = aiBox.h * 3;
		}
		else if (y_ai_vel < 0)
		{
			y_detectBox.x = aiBox.x;
			y_detectBox.y = aiBox.y - (aiBox.h * 3);
			y_detectBox.h = aiBox.h * 3;
		}
		else
		{
			y_detectBox.x = aiBox.x;
			y_detectBox.y = aiBox.y;
			y_detectBox.h = aiBox.h;
		}
	
		// Clear black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);
		
		// Green detection box
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &destBox);
		
		// White detection box
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &x_detectBox);
		
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &y_detectBox);
		
		// Cyan move box
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &moveBox);
		
		// Red static box
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, &staticBox);
		
		// Magenta AI box
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderFillRect(gRenderer, &aiBox);

		SDL_RenderPresent(gRenderer);
	}

	// Out of game loop, clean up
	close();
}
