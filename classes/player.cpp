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

// COMMENTED OUT BECAUSE SPRITE DNE yet
// /**
//  * @brief Construct a new Player:: Player object
//  * 
//  * @param sprite 
//  * @param turret 
//  * @param x 
//  * @param y 
//  */
// Player::Player(Sprite sprite, Sprite turret, int x, int y)
// {
// }
Player::Player(int x, int y) : x{x}, y{y} {}

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

}

/* Player Specific Functions */

bool Player::fire() {

}

bool Player::rotatePlayer(float theta) {

}

bool Player::rotateTurret(float theta) {

}