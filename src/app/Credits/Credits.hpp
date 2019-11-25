#ifndef CREDITS_HPP
#define CREDITS_HPP  

#include <vector>
#include "Render.hpp"

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif // __APPLE__


class Credits {
  public:
    Credits(Render* renderer) : render{renderer} {};
    int playCredits();

  private:
    // Function declarations
    Render* render;
    
    void jakobCredits(SDL_Texture* picture, SDL_Texture* hitmarker);

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
};

#endif