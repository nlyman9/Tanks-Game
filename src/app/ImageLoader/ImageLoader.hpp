#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include<SDL2/SDL_image.h>

class ImageLoader {
    public:
        SDL_Texture* loadImage(std::string filename, SDL_Renderer* renderer);
};

#endif