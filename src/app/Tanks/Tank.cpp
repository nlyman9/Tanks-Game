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

void Tank::rotateTurret(float t){
    turretTheta = t;
}

void Tank::setHit(bool val) {
	hit = val;
}

void Tank::resetFrame() {
	frame = 0;
}

bool Tank::isHit() {
    return hit;
}

bool Tank::isDestroyed() {
    return destroyed;
}

void Tank::setTurretSprite(Sprite *new_sprite) 
{
    turretSprite = *new_sprite;
}

Sprite* Tank::getTurretSprite(){
    return &turretSprite;
}