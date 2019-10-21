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
#ifndef PLAYER_HPP
#define PLAYER_HPP 

#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include "Object.hpp"

class Player : public OBJECT {
    private:
        /* data */
        Sprite *turret;
        float x_vel, y_vel; // x and y velocity
        int lives; //health points left
        int theta = 0;
        float x_deltav = 0;
        float y_deltav = 0;
        int velocity = 0;
        int delta_velocity = 0;
    public:
        Player(Sprite *sprite, Sprite *turret, int x, int y); //constructor, initialize the x, y, and sprite
        Player(int x, int y); //constructor, initialize the x, snd y

        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(int x, int y) override; //move x offset from current x and y offset from current y
        bool place(int x, int y) override; //place/teleport to an x and y
        
        bool fire(); //return true if the player fired successfully
        bool rotatePlayer(float theta); //rotate the object
        bool rotateTurret(float theta); //rotate the turret

        void getEvent(SDL_Event e);

        ~Player();
};
#endif