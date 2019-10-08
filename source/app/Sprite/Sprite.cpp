#include "Sprite.hpp"

bool Sprite::loadImage(SDL_Renderer *gRenderer, std::string fname) {
    SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
			std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
			return 0;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
			std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
            return 0;
	}

	SDL_FreeSurface(startSurf);

	texture = newText;
    return 1;
}

SDL_Texture* Sprite::getTexture() {
    return texture;
}

void Sprite::setTexture(SDL_Texture *new_texture) {
    texture = new_texture;
}