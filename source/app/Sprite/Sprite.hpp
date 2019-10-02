/**
 * @file object.hpp
 * @author Dylan Miao
 * @brief Header for abstract sprite class
 * @version 0.1
 * @date 2019-09-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OBJECT_HPP
#define OBJECT_HPP  
#include <iostream>

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif // __APPLE__

class Sprite {
    private:
        SDL_Texture* texture;
        // TODO?:
        // png size
    public:

        /**
         * @brief load image of sprite
         * 
         * @param fname - file to load sprite from
         * 
         * @return true  - image load success 
         * @return false - image load fail 
         */
        virtual bool loadImage(std::string fname);

        /**
         * @brief return sprite texture
         * 
         * @return SDL_Texture* 
         */
        virtual SDL_Texture* getImage();

        virtual ~Sprite() = 0; //destructor
};

Sprite::~Sprite() {
    std::cout << "Sprite destructor";
}
#endif