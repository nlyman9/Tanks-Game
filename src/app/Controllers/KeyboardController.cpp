#include "KeyboardController.hpp"
#include <SDL2/SDL.h>

const Uint8* KeyboardController::pollEvent() {
    return SDL_GetKeyboardState(nullptr);
}