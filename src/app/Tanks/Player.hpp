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
#include "Client.hpp"
#include "Tank.hpp"

const int THETA_WINDOW = 5;

class Player : public Object, public Tank {
    private:
        /* data */
        Sprite *turret = nullptr;
        Client *client = nullptr;
        float x_vel, y_vel; // x and y velocity
        int lives; //health points left
        int theta_v = 0;
        int mouseTheta = 0;
        int mouseX = 0;
        int mouseY = 0;
        int turretTheta_v = 0;
        float x_deltav = 0;
        float y_deltav = 0;
        float velocity = 0;
        float delta_velocity = 0;
        bool localPlayer;
        bool connected;
        Uint32 anim_last_time = 0;

    public:
        
        Player(Sprite *sprite, Sprite *turret, float x, float y, bool local); //constructor, initialize the x, y, and sprite
        Player(float x, float y, bool local); //constructor, initialize the x, snd y

        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y

        bool isConnected(); //check if the player has entered the game

        void getEvent(std::chrono::duration<double, std::ratio<1, 1000>> time, SDL_Event* e, const Uint8 *keystate);
        void setClient(Client* client);

        BoundingBox* getBoundingBox() override;

        ~Player();
};
#endif