#ifndef TANK_HPP
#define TANK_HPP

#include "Object.hpp"

class Tank {
    public:
        // Member Functions
        int getTheta();
        float getTurretTheta();
        bool getFire();
        bool setFire(bool fire);
        bool rotate(float theta);
        bool rotateTurret(float theta);

        // Member Variable
        int theta = 0;
        float turretTheta = 0;
        bool shotsFired = false;
        Uint32 fire_last_time = 0;
        int frame = 0;
};

#endif