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

#ifndef SPRITE_HPP
#define SPRITE_HPP  
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Sprite {
    private:
        SDL_Texture* texture;
        // TODO?:
        // png size
    public:
        Sprite(){};
        Sprite(SDL_Renderer *gRenderer, std::string fname) {
            if(!loadImage(gRenderer, fname)) {
                exit(0);
            }
        };

        /**
         * @brief load image of sprite
         * 
         * @param fname - file to load sprite from
         * 
         * @return true  - image load success 
         * @return false - image load fail 
         */
        bool loadImage(SDL_Renderer *gRenderer, std::string fname);


        /**
         * @brief return sprite texture
         * 
         * @return SDL_Texture* 
         */
        SDL_Texture* getTexture();

        /**
         * @brief set sprite texture to argument
         * 
         * @return void
         */
        void setTexture(SDL_Texture *new_texture);

        ~Sprite(){}; //destructor
};
#endif