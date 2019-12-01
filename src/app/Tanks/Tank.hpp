#ifndef TANK_HPP
#define TANK_HPP

#include "Object.hpp"
#include "Sprite.hpp"
#include <chrono>

class Tank {
    private: 
      std::chrono::system_clock::time_point prev_fire_time = std::chrono::system_clock::now();
      std::chrono::system_clock::time_point prev_bomb_time = std::chrono::system_clock::now();
    public:
        // Member Functions
        int getTheta();
        float getTurretTheta();
        bool getFire();
        bool setFire(bool fire){
            std::chrono::system_clock::time_point current_fire_time = std::chrono::system_clock::now();
            if((current_fire_time - prev_fire_time) > std::chrono::seconds{1}){
                prev_fire_time = std::chrono::system_clock::now();
                shotsFired = fire;
                return true;
            }
            shotsFired = false;
            return false;
        }
        bool getBomb();
        bool setBomb(bool bomb){
            std::chrono::system_clock::time_point current_bomb_time = std::chrono::system_clock::now();
            if((current_bomb_time - prev_bomb_time) > std::chrono::seconds{1}){
                prev_bomb_time = std::chrono::system_clock::now();
                bombDeloyed = bomb;
                return true;
            }
            bombDeloyed = false;
            return false;
        }
        bool rotate(float theta);
        void rotateTurret(float theta);
        void setTurretSprite(Sprite* new_sprite);
        Sprite* getTurretSprite();
		bool isHit();
		void setHit(bool val);
    bool purpHit();
    void setPurpHit(bool hiterino);
		void resetFrame();
		bool isDestroyed();

        // Member Variable
        int theta = 0;
        int turretTheta = 0;
        bool shotsFired = false;
        bool bombDeloyed = false;
        Uint32 fire_last_time_bullet = 0;
        Uint32 fire_last_time_bomb = 0;
        int frame = 0;
        Sprite turretSprite;
		bool hit = false;
    bool purpleHit = false;
		bool destroyed = false;
};

#endif
