#include "NetworkController.hpp"
#include <SDL2/SDL.h>

const Uint8* NetworkController::pollEvent() {
    return keystate;
}

void NetworkController::setKeystate(uint8_t* state) {
    keystate = state;
}