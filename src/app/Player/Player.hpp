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

#include <chrono>
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
        int theta_v = 0;
        float x_deltav = 0;
        float y_deltav = 0;
        float velocity = 0;
        float delta_velocity = 0;
    public:
        Player(Sprite *sprite, Sprite *turret, float x, float y); //constructor, initialize the x, y, and sprite
        Player(float x, float y); //constructor, initialize the x, snd y

        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y
        
        bool fire(); //return true if the player fired successfully
        bool rotatePlayer(float theta); //rotate the object
        bool rotateTurret(float theta); //rotate the turret

        void getEvent(std::chrono::duration<double, std::ratio<1, 1000>> time);

        ~Player();
};
#endif