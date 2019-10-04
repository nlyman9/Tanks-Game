#include <iostream>
#include "GameLoop.hpp"
#include "Constants.hpp"

GameLoop::~GameLoop() = default;

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
	SDL_Event e;
	bool gameon = true;

	Player player(0,0);
	Render *render = new Render(&player);
	render->init();

	//Start position of obstacle - middle
	x_obst_pos = SCREEN_WIDTH / 2 - OBST_WIDTH / 2;
	y_obst_pos = SCREEN_HEIGHT / 2 - OBST_HEIGHT / 2;

	//Enemy box
	x_enemy_pos = SCREEN_WIDTH / 2 - BOX_WIDTH / 2;
	y_enemy_pos = SCREEN_HEIGHT / 2 - BOX_HEIGHT / 2;

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
				player.getEvent(e);
			}
		}
		//Checking if enemy should move away
		int x_pos = player.getX();
		int y_pos = player.getY();
		bool retreat;
		retreat = checkPos(x_pos, y_pos, x_enemy_pos, y_enemy_pos);

		bool nearWall;
		nearWall = checkWall(x_enemy_pos, y_enemy_pos);

		if (!retreat)
		{

			//go to center

			if (x_enemy_pos <= SCREEN_WIDTH / 2)
			{
				x_enemy_pos += MAX_VELOCITY;
			}
			else
			{
				x_enemy_pos += -MAX_VELOCITY;
			}
			if (y_enemy_pos <= SCREEN_HEIGHT / 2)
			{
				y_enemy_pos += MAX_VELOCITY;
			}
			else
			{
				y_enemy_pos += -MAX_VELOCITY;
			}
		}
		else
		{

			//run away but not near wall
			if (!nearWall)
			{
				if (x_pos >= x_enemy_pos)
				{
					x_enemy_pos += -MAX_VELOCITY;
				}
				else
				{
					x_enemy_pos += MAX_VELOCITY;
				}
				if (y_pos >= y_enemy_pos)
				{
					y_enemy_pos += -MAX_VELOCITY;
				}
				else
				{
					y_enemy_pos += MAX_VELOCITY;
				}
			}
			else
			{
				//run away and on wall

				if (x_enemy_pos == 20 || x_enemy_pos == SCREEN_WIDTH - 2 * BOX_WIDTH)
				{
					if (y_pos >= y_enemy_pos)
					{
						y_enemy_pos += -MAX_VELOCITY;
					}
					else
					{
						y_enemy_pos += MAX_VELOCITY;
					}
				}
				if (y_enemy_pos == 20 || y_enemy_pos == SCREEN_HEIGHT - 2 * BOX_HEIGHT)
				{

					if (x_pos >= x_enemy_pos)
					{
						x_enemy_pos += -MAX_VELOCITY;
					}
					else
					{
						x_enemy_pos += MAX_VELOCITY;
					}
				}
			}
		}

		if (x_enemy_pos > SCREEN_WIDTH - 2 * BOX_WIDTH)
		{
			x_enemy_pos = SCREEN_WIDTH - 2 * BOX_WIDTH;
		}
		if (x_enemy_pos < 20)
		{
			x_enemy_pos = 20;
		}
		if (y_enemy_pos < 20)
		{
			y_enemy_pos = 20;
		}
		if (y_enemy_pos > SCREEN_HEIGHT - 2 * BOX_HEIGHT)
		{
			y_enemy_pos = SCREEN_HEIGHT - 2 * BOX_HEIGHT;
		}

		render->run();
	}
}