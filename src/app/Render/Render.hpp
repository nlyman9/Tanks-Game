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

enum {
    MENU_SINGLE,
    MENU_MULTI,
    MENU_CREDITS
};

class Render {
    public:
        std::vector<Enemy *> gEnemies;

        Render() {}
        Render(Player* player, std::vector<Enemy*> enemies): gPlayer{player}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        int drawMenu();
        bool init();
        void close();

        SDL_Renderer* getRenderer();

        void setTileMap(int** tileMap);

        void setPlayer(Player* player);
        void setEnemies(std::vector<Enemy *> enemies);
    private:

        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        SDL_Surface* gScreenSurface;

        Player* gPlayer;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];

        int** tile_map;
};
#endif
