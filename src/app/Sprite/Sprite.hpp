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
#if __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif


#include "ImageLoader.hpp"

class Sprite {
    private:
        SDL_Texture* texture;
        SDL_Renderer *gRenderer;
        std::string gFname;
        // TODO?
        // png size
		
		//animation
		SDL_Rect* textRect;
		
    public:
        Sprite(){};
        Sprite(SDL_Renderer *gRenderer, std::string fname);
        void init();

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

        ~Sprite(); //destructor
		
		void sheetSetup(int width, int length, int height);
		
		SDL_Rect* getFrame(int frame);
};
#endif