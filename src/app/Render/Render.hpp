#ifndef RENDER_HPP
#define RENDER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Constants.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "MapGenerator.hpp"
#include "ImageLoader.hpp"

class Render {
    public:
        std::vector<Enemy *> gEnemies;

        Render(Player* player, std::vector<Enemy*> enemies): gPlayer{player}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        bool init();
        void close();

        SDL_Renderer* getRenderer();
    private:

        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];
        Player* gPlayer;
        int** tile_map;

        std::vector<SDL_Rect> tileArray;
};
#endif
