#ifndef RENDER_HPP
#define RENDER_HPP
#if __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif
#include <vector>
#include "Constants.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "ImageLoader.hpp"

enum {
    MENU_SINGLE,
    MENU_MULTI,
    MENU_CREDITS,
    DEFAULT,
    MENU_NONE
};

class Render {
    public:
        std::vector<Player*> gPlayers;
		std::vector<Enemy *> gEnemies;
        std::vector<Projectile *> gProjectiles;

        Render() {}
        Render(std::vector<Player*> players, std::vector<Enemy*> enemies): gPlayers{players}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        int drawMenu();
        bool init();
        void close();

        SDL_Renderer* getRenderer();

        void setTileMap(std::vector<std::vector<int>>* tileMap);
        void addProjectile(Projectile* newProjectile);

        void setPlayer(std::vector<Player*> players);
        void setEnemies(std::vector<Enemy *> enemies);
        void setProjectiles(std::vector<Projectile *> projectiles);
    private:

        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        SDL_Surface* gScreenSurface;


        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];

       std::vector<std::vector<int>> tile_map;
};
#endif
