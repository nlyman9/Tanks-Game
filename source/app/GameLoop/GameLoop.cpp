#include "GameLoop.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "Credits.hpp"
#include "Render.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
using namespace mainLoop;

GameLoop::~GameLoop()
{
	close();
}

bool GameLoop::init()
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

	// Adding VSync to avoid absurd framerates
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	return true;
}

void GameLoop::close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}
bool checkPos(int playX, int playY, int enemX, int enemY)
{

	double stepOne = (double)(pow((playX - enemX), 2) + pow((playY - enemY), 2));

	double distanceAway = (pow(stepOne, .5));

	if (distanceAway < 200.0)
	{
		return true;
	}
	return false;
}
bool checkWall(int x, int y)
{

	//left wall
	if (x <= 20)
	{
		return true;
	}
	//right wall
	else if (x >= SCREEN_WIDTH - 2 * BOX_WIDTH)
	{
		return true;
	}
	//top wall
	else if (y <= 20)
	{
		return true;
	}
	//bottom wall
	else if (y >= SCREEN_HEIGHT - 2 * BOX_HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int GameLoop::run()
{
	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
		close();
		return 1;
	}
	Render *render = new Render(this);
	SDL_Event e;
	bool gameon = true;

	//Start position of obstacle - middle
	x_obst_pos = SCREEN_WIDTH / 2 - OBST_WIDTH / 2;
	y_obst_pos = SCREEN_HEIGHT / 2 - OBST_HEIGHT / 2;

	Player player(gWindow, gRenderer);
	Enemy enemy(gWindow, gRenderer);

	while (gameon)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				gameon = false;
			}
			else 
			{
				player.update(e);
			}
			
			enemy.update(player.getX(), player.getY());

			render->run();
		}
	}
}