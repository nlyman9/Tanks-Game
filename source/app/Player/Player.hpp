/**
 * @file player.hpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Header file for player object
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "Object.hpp"
// TODO figure out Weapon object
using Weapon = UNDEFINED;

#ifndef PLAYER_HPP
#define PLAYER_HPP  
class Player : public OBJECT {
    private:
        /* data */
        Sprite sprite;
        Sprite turret;
        Weapon weapon; //equipped weapon/or projectile to fire? Projectile projectile maybe-
        int x, y, lives; //x and y coordinates, health points left
    public:
        Player(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        Player(int x, int y); //constructor, initialize the x, y, and sprite

        void draw(float update_lag) override;
        void update() override;
        bool move(int x, int y) override; //move x offset from current x and y offset from current y
        bool place(int x, int y) override; //place/teleport to an x and y
        
        bool fire(); //return true if the player fired successfully
        bool rotatePlayer(float theta); //rotate the object
        bool rotateTurret(float theta); //rotate the turret

        ~Player();
};
#endif