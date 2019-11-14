/**
 * @file Object.hpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Header for abstract Object class for general entities in the game
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

/**
 * @brief UNDEFINED means the Object/type has not been defined/implemented yet
 * 
 * UNDEFINED = bool -> Essentially should be void but it doesn't work.
 */
//using UNDEFINED = bool;
//using Sprite = UNDEFINED;

#include <math.h>

#include "Object.hpp"
#include "Constants.hpp"

SDL_Rect* Object::get_box()
{
    box = {(int)nearbyint(getX()), (int)nearbyint(getY()), TANK_WIDTH, TANK_HEIGHT};
    return &box;
}


// TODO make a custom collision detection that detects a box based off of 4 points. 
SDL_Rect* Object::check_collision(Object *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(&box, B->get_box(), overlap)) {
        return overlap;
    }
    return nullptr;
}

SDL_Rect* Object::check_collision(SDL_Rect* A, SDL_Rect *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(A, B, overlap)) {
        return overlap;
    }
    return nullptr;
}

SDL_Rect* Object::check_collision(SDL_Rect *B)
{
    SDL_Rect* overlap = new SDL_Rect;
    if(SDL_IntersectRect(&box, B, overlap)) {
        return overlap;
    }
    return nullptr;
}

bool Object::check_bounds()
{
    //left wall

    if (x <= 20)
    {
        return true;
    }
    //right wall
    else if (x >= SCREEN_WIDTH - 2 * TANK_WIDTH)
    {
        return true;
    }
    //top wall
    else if (y <= 20)
    {
        return true;
    }
    //bottom wall
    else if (y >= SCREEN_HEIGHT - 2 * TANK_HEIGHT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Sprite* Object::getSprite(){
    return &sprite;
}


void Object::setSprite(Sprite *new_sprite)
{
    sprite = *new_sprite;
}

void Object::setPos(float newx, float newy)
{
    x = newx;
    y = newy;
}
void Object::setX(float newx)
{
    x = newx;
}
void Object::setY(float newy)
{
    y = newy;
}
float Object::getX()
{
    return x;
}
float Object::getY()
{
    return y;
}
Object::~Object(){
    //std::cout << "OBJ Deleted";
}

void Object::setObstacleLocations(std::vector<SDL_Rect>* obstacleLocs) {
    obstacles = *obstacleLocs;
}