/**
 * @file object.hpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Header for abstract object class for general entities in the game
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

/**
 * @brief UNDEFINED means the object/type has not been defined/implemented yet
 * 
 * UNDEFINED = bool -> Essentially should be void but it doesn't work.
 */
//using UNDEFINED = bool;
//using Sprite = UNDEFINED;

#include "Object.hpp"
#include "Constants.hpp"

SDL_Rect* OBJECT::get_box()
{
    box = {getX(), getY(), BOX_HEIGHT, BOX_WIDTH};
    return &box;
}

SDL_Rect* OBJECT::check_collision(OBJECT *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(&box, B->get_box(), overlap)) {
        return overlap;
    }
    return nullptr;
}
SDL_Rect* OBJECT::check_collision(SDL_Rect* A, SDL_Rect *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(A, B, overlap)) {
        return overlap;
    }
    return nullptr;
}

SDL_Rect* OBJECT::check_collision(SDL_Rect *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(&box, B, overlap)) {
        return overlap;
    }
    return nullptr;
}

bool OBJECT::check_bounds()
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

Sprite* OBJECT::getSprite(){
    return &sprite;
}

void OBJECT::setSprite(Sprite *new_sprite)
{
    sprite = *new_sprite;
}

void OBJECT::setPos(int newx, int newy)
{
    x = newx;
    y = newy;
}
void OBJECT::setX(int newx)
{
    x = newx;
}
void OBJECT::setY(int newy)
{
    y = newy;
}
int OBJECT::getX()
{
    return x;
}
int OBJECT::getY()
{
    return y;
}
OBJECT::~OBJECT(){
    //std::cout << "OBJ Deleted";
}

void OBJECT::setObstacleLocations(std::vector<SDL_Rect>* obstacleLocs) {
    obstacles = *obstacleLocs;
}