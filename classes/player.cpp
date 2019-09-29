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
#include "../headers/player.hpp"

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param sprite 
 * @param turret 
 * @param x 
 * @param y 
 */
Player::Player(Sprite sprite, Sprite turret, int x, int y) : sprite{sprite}, turret{turret}, x{x}, y{y} {}

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