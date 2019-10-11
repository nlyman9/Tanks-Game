
#include "Sprite.hpp"

Sprite::Sprite(SDL_Renderer *renderer, std::string fname) : gRenderer{renderer}, gFname{fname} {}

void Sprite::init() {
	if(!loadImage()) {
			exit(0);
	}
}

bool Sprite::loadImage() {
  SDL_Texture* newText = nullptr;
	SDL_Surface* startSurf = IMG_Load(gFname.c_str());
	if (startSurf == nullptr) {
			std::cout << "Unable to load image " << gFname << "! SDL Error: " << SDL_GetError() << std::endl;
			return 0;
	}
	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
			std::cout << "Unable to create texture from " << gFname << "! SDL Error: " << SDL_GetError() << std::endl;
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