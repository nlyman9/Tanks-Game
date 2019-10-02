#ifndef RENDER_HPP
#define RENDER_HPP
#include "GameLoop.hpp"
class Render {
    public:
        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];
        SDL_Rect cur_out;
        mainLoop::GameLoop* gloop;

        Render(mainLoop::GameLoop* gl):gloop{gl} {
            gTileSheet = loadImage("../source/res/images/tiles.png");
            for (int i = 0; i < 3; i++) {
                gTileRects[i].x = i * TILE_SIZE;
                gTileRects[i].y = 0;
                gTileRects[i].w = TILE_SIZE;
                gTileRects[i].h = TILE_SIZE;
            }
        };
        SDL_Texture* loadImage(std::string fname) {
            SDL_Texture* newText = nullptr;

            SDL_Surface* startSurf = IMG_Load(fname.c_str());
            if (startSurf == nullptr) {
                std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
                return nullptr;
            }

            newText = SDL_CreateTextureFromSurface(gloop->gRenderer, startSurf);
            if (newText == nullptr) {
                std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
            }

            SDL_FreeSurface(startSurf);

            return newText;
        }
        ~Render();
        int run();
};
#endif