#include "Enemy.hpp"

void Enemy::update(int playerX, int playerY) 
{

    //Checking if enemy should move away
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
}