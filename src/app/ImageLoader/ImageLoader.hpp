#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <iostream>

#if __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

class ImageLoader {
    public:
        SDL_Texture* loadImage(std::string filename, SDL_Renderer* renderer);
};

#endif