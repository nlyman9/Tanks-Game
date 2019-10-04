#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.hpp"
#include "Constants.hpp"
#include <vector>

class Render {
    public:
        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];
        SDL_Rect cur_out;
        Player* gPlayer;

        Render(Player* player): gPlayer{player} {
            gTileSheet = loadImage("res/images/tiles.png");
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

            newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
            if (newText == nullptr) {
                std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
            }

            SDL_FreeSurface(startSurf);

            return newText;
        }
        ~Render();
        int run();
        bool init();
        void close();
};
#endif