#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Constants.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class Render {
    public:
        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];
        SDL_Rect cur_out;
        Player* gPlayer;
        std::vector<Enemy *> gEnemies;

        Render(Player* player, std::vector<Enemy*> enemies): gPlayer{player}, gEnemies{enemies} {}
        ~Render();
        int run();
        bool init();
        void close();
    private:
       SDL_Texture* loadImage(std::string fname);
};
#endif