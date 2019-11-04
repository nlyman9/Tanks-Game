#include "KeyboardController.hpp"
#include <SDL2/SDL.h>
#include <iostream>

const Uint8* KeyboardController::pollEvent() {
    int* numKeys;
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    return keystate;
}