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
        bool setFire(bool fire);
        bool rotate(float theta);
        void rotateTurret(float theta);
        void setTurretSprite(Sprite* new_sprite);
        Sprite* getTurretSprite();
		bool isHit();
		void setHit(bool val);
		void resetFrame();
		bool isDestroyed();

        // Member Variable
        int theta = 0;
        int turretTheta = 0;
        bool shotsFired = false;
        Uint32 fire_last_time = 0;
        int frame = 0;
        Sprite turretSprite;
		bool hit = false;
		bool destroyed = false;
};

#endif