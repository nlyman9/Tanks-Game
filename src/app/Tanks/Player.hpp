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
#include "Tank.hpp"

const int THETA_WINDOW = 5;

class Player : public Object, public Tank {
    private:
        /* data */
        Sprite *turret = nullptr;
        float x_vel, y_vel; // x and y velocity
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
        int id;
        Uint32 anim_last_time = 0;
        std::vector<SDL_Rect*> enemies;
        int lives;

        std::vector<char>* gamestate;
    public:

        Player(Sprite *sprite, Sprite *turret, float x, float y, bool local); //constructor, initialize the x, y, and sprite
        Player(float x, float y, bool local); //constructor, initialize the x, snd y

        void draw(SDL_Renderer *gRenderer, double update_lag) override;
        void update() override;
        bool move(float x, float y) override; //move x offset from current x and y offset from current y
        bool place(float x, float y) override; //place/teleport to an x and y

        void getEvent(std::chrono::duration<double, std::ratio<1, 1000>> time,
                      SDL_Event* e,
                      const Uint8 *keystate);
        void setId(int i) {
            id = i;
        }
        void setTurretTheta();
        void setTurretTheta(int theta);
        void setEnemies(std::vector<SDL_Rect*> enemyList);

        BoundingBox* getBoundingBox() override;

        std::vector<char>* getState(){
            gamestate->clear();
            for(auto x : std::to_string((int)lives))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            for(auto x : std::to_string((int)this->getX()))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            for(auto x : std::to_string((int)this->getY()))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            for(auto x : std::to_string((int)x_vel))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            for(auto x : std::to_string((int)y_vel))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            for(auto x : std::to_string((int)getTheta()))
                gamestate->push_back(x);
            gamestate->push_back(' ');
            std::cout << "Gamestate is : ";
            for(auto x : *gamestate)
                std::cout << x << " ";
            std::cout << std::endl;
            return gamestate;
        }
        void applyState(std::vector<int> state){
            lives = state.at(0);
            this->setX(state.at(1));
            this->setY(state.at(2));
            this->x_vel = state.at(3);
            this->y_vel = state.at(4);
            this->theta = state.at(5);
        }
        ~Player();
};
#endif
