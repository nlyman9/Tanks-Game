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

        Render(Player* player): gPlayer{player} {}
        ~Render();
        int run();
        bool init();
        void close();
    private:
       SDL_Texture* loadImage(std::string fname);
};
#endif