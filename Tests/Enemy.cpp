/**
 * @file Enemy.cpp
 * @author Danny Phan
 * @brief Enemy class implementation
 * @version 0.1
 * @date 2019-10-4 
 * @copyright Copyright (c) 2019
 * 
 */
#include "Enemy.hpp"

/**
 * @brief Construct a new Enemy:: Enemy object
 * 
 * @param sprite 
 * @param turret 
 * @param x 
 * @param y 
 */
Enemy::Enemy(Sprite sprite, Sprite turret, int x, int y) : sprite{sprite}, turret{turret}, x{x}, y{y} {}

/**
 * @brief Destroy the Enemy:: Enemy object
 * 
 */
Enemy::~Enemy() {
    
}

/**
 * @brief draws the Enemy object 
 *  Overrides base class Object
 * 
 * @param update_lag - the value to extrapolate by
 */
void Enemy::draw(float update_lag) {

}

/**
 * @brief update the Enemy object
 *  Overrides base class Object
 * 
 */
void Enemy::update() {

}

/**
 * @brief move the enemy an offset from its current x-y position
 *  Overrides base class Object
 * 
 * @param x - how much to move enemy's current x position by
 * @param y - how much to move enemy's current y position by
 * 
 * @return true  - moved enemy succesfully 
 * @return false - failed to move enemy 
 */
bool enemy::move(int x, int y) {
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
