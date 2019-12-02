#ifndef TANK_HPP
#define TANK_HPP

#include "Object.hpp"
#include "Sprite.hpp"
#include <chrono>

class Tank {
    private: 
      std::chrono::system_clock::time_point prev_fire_time = std::chrono::system_clock::now() - std::chrono::seconds{3};
      std::chrono::system_clock::time_point prev_bomb_time = std::chrono::system_clock::now() - std::chrono::seconds{5};
    public:
        // Member Functions
        int getTheta();
        float getTurretTheta();
        bool getFire();
        bool setFire(bool fire){
            if (fire == false) {
                shotsFired = false;
                return true;
            } else {
                std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
                if (current_time - prev_fire_time >= std::chrono::seconds{3}) {
                    prev_fire_time = std::chrono::system_clock::now();
                    shotsFired = true;
                    return true;
                }
                return false;
            }
        }
        bool getBomb();
        bool setBomb(bool bomb){
            if (bomb == false) {
                bombDeployed = false;
                return true;
            } else {
                std::chrono::system_clock::time_point current_time = std::chrono::system_clock::now();
                if (current_time - prev_bomb_time >= std::chrono::seconds{5}) {
                    prev_bomb_time = std::chrono::system_clock::now();
                    bombDeployed = true;
                    return true;
                }
                return false;
            }
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
        bool bombDeployed = false;
        Uint32 fire_last_time_bullet = 0;
        Uint32 fire_last_time_bomb = 0;
        int frame = 0;
        Sprite turretSprite;
		bool hit = false;
        bool purpleHit = false;
		bool destroyed = false;
};

#endif
