#include "ImageLoader.hpp"

SDL_Texture* ImageLoader::loadImage(std::string filename, SDL_Renderer* gRenderer) {
    SDL_Surface* startSurf = IMG_Load(filename.c_str());
	if (startSurf == nullptr) {
        std::cout << "Unable to load image " << filename << "! SDL Error: " << SDL_GetError() << std::endl;
	}
	SDL_Texture* newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
        std::cout << "Unable to create texture from " << filename << "! SDL Error: " << SDL_GetError() << std::endl;
	}
	SDL_FreeSurface(startSurf);
    
    return newText;
}