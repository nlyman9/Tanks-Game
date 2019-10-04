/**
 * @file player.cpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Player class implementation
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "Player.hpp"
#include "Constants.hpp"

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param sprite 
 * @param turret 
 * @param x 
 * @param y 
 */
Player::Player(Sprite sprite, Sprite turret, int x, int y) : sprite{sprite}, turret{turret}, x_pos{x}, y_pos{y} {}

Player::Player(int x, int y) :  x_pos{x}, y_pos{y} {}

/**
 * @brief Destroy the Player:: Player object
 * 
 */
Player::~Player() {
    
}

/**
 * @brief draws the player object 
 *  Overrides base class OBJECT
 * 
 * @param update_lag - the value to extrapolate by
 */
void Player::draw(float update_lag) {

}

/**
 * @brief update the player object
 *  Overrides base class OBJECT
 * 
 */
void Player::update() {

}

/**
 * @brief move the player an offset from its current x-y position
 *  Overrides base class OBJECT
 * 
 * @param x - how much to move player's current x position by
 * @param y - how much to move player's current y position by
 * 
 * @return true  - moved player succesfully 
 * @return false - failed to move player 
 */
bool Player::move(int x, int y) {
    return false;
}

/**
 * @brief place/teleport player to a specified x-y location
 * 
 * @param x - set x position
 * @param y - set y position
 * 
 * @return true  - placed player succesfully 
 * @return false - failed to place player 
 */
bool Player::place(int x, int y) {
    return false;
}

/* Player Specific Functions */

bool Player::fire() {
    return false;
}

bool Player::rotatePlayer(float theta) {
    return false;
}

bool Player::rotateTurret(float theta) {
    return false;
}

void Player::getEvent(SDL_Event e) {
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
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
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
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

    // Move box
    if (x_vel > MAX_VELOCITY)
    {
        x_vel = MAX_VELOCITY;
    }
    if (x_vel < -MAX_VELOCITY)
    {
        x_vel = -MAX_VELOCITY;
    }
    if (y_vel > MAX_VELOCITY)
    {
        y_vel = MAX_VELOCITY;
    }
    if (y_vel < -MAX_VELOCITY)
    {
        y_vel = -MAX_VELOCITY;
    }
    x_pos += x_vel;
    y_pos += y_vel;
    if (x_pos > SCREEN_WIDTH - BOX_WIDTH)
    {
        x_pos = SCREEN_WIDTH - BOX_WIDTH;
    }
    if (x_pos < 0)
    {
        x_pos = 0;
    }
    if (y_pos < 0)
    {
        y_pos = 0;
    }
    if (y_pos > SCREEN_HEIGHT - BOX_HEIGHT)
    {
        y_pos = SCREEN_HEIGHT - BOX_HEIGHT;
    }
}

int Player::getX() {
    return x_pos;
}

int Player::getY() {
    return y_pos;
}