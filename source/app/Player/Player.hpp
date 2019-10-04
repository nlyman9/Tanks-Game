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
#include "NetworkController.hpp"
#include "KeyboardController.hpp"
#include "AIController.hpp"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
// TODO figure out Weapon object
using Weapon = UNDEFINED;

#ifndef PLAYER_HPP
#define PLAYER_HPP  
class Player : public OBJECT {
    private:
    	// Current velocity of the box
        // Start off at reset
        x_vel = 0;
        y_vel = 0;
        // Current position to render the box
        // Start off with it in the middle
        x_pos = 0;
        y_pos = 0;
        /* data */
        Sprite sprite;
        Sprite turret;
        Weapon weapon; //equipped weapon/or projectile to fire? Projectile projectile maybe-
        int x, y, lives; //x and y coordinates, health points left
        KeyboardController controller; 
    public:
        Player(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        Player(int x, int y); //constructor, initialize the x, y, and sprite
        Player(SDL_Window* window, SDL_Renderer* renderer) : gWindow{window}, gRenderer{renderer} {};

        void draw(float update_lag) override;
        void update(SDL_Event e) override;
        bool move(int x, int y) override; //move x offset from current x and y offset from current y
        bool place(int x, int y) override; //place/teleport to an x and y
        
        bool fire(); //return true if the player fired successfully
        bool rotatePlayer(float theta); //rotate the object
        bool rotateTurret(float theta); //rotate the turret

        int getX(); //return the current X coordinate
        int gety(); //return the current Y coordinate

        ~Player();
};
#endif
