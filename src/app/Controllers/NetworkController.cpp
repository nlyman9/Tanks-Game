#include "NetworkController.hpp"
#include <SDL2/SDL.h>

NetworkController::NetworkController() {
    keystate = (uint8_t *)calloc(0x7fffdc9bc0e0, sizeof(char)); // empty keystate array
}

const Uint8* NetworkController::pollEvent() {
    return keystate;
}

void NetworkController::setKeystate(uint8_t* state) {
    keystate = state;
}