#include "Object.hpp"

Sprite* OBJECT::getSprite() {
    return sprite;
}

struct position OBJECT::getPos() {
    return pos;
}

int OBJECT::getX() {
    return pos.x;
}

int OBJECT::getY() {
    return pos.y;
}

void OBJECT::setSprite(Sprite *new_sprite) {
    sprite = new_sprite;
}

void OBJECT::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void OBJECT::setX(int x) {
    pos.x = x;
}

void OBJECT::setY(int y) {
    pos.y = y;
}