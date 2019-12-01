#ifndef TANK_HPP
#define TANK_HPP

#include "Object.hpp"
#include "Sprite.hpp"

class Tank {
    public:
        // Member Functions
        int getTheta();
        float getTurretTheta();
        bool getFire();
        void setFire(bool fire);
        bool getBomb();
        void setBomb(bool bomb);
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
