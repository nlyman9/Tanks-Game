
#include "Sprite.hpp"

Sprite::Sprite(SDL_Renderer *renderer, std::string fname) : gRenderer{renderer}, gFname{fname} {}

void Sprite::init() {
	texture = loadImage(gFname, gRenderer);
}

SDL_Texture* Sprite::getTexture() {
    return texture;
}

void Sprite::setTexture(SDL_Texture *new_texture) {
    texture = new_texture;
}