#include "Tank.hpp"

int Tank::getTheta() {
    return theta;
}

float Tank::getTurretTheta() {
    return turretTheta;
}

bool Tank::getFire() {
    return shotsFired;
}

bool Tank::setFire(bool fire) {
    shotsFired = fire;
}

bool Tank::rotate(float t){
    theta += t;

    if(theta < 0) {
        theta = 360 + theta;
    }
    theta %= 360;
    return true;
}

bool Tank::rotateTurret(float t){
    turretTheta = t;
}