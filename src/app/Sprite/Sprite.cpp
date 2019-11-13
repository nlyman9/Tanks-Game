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

void Sprite::sheetSetup(int width, int height, int length) {
	textRect = new SDL_Rect[length];
			
	for (int i = 0; i < length; i++) {
		textRect[i].x = i * width;
		textRect[i].y = 0;
		textRect[i].w = width;
		textRect[i].h = height;
	}
}

SDL_Rect* Sprite::getFrame(int frame) {
	return &textRect[frame];
}

Sprite::~Sprite() {
}