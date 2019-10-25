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

typedef struct {
    SDL_Rect draw_rect;
    int r;
    int g;
    int b;
    int a;
    bool pressed;
} button_t;


class Render {
    public:
        std::vector<Enemy *> gEnemies;

        Render() {}
        Render(Player* player, std::vector<Enemy*> enemies): gPlayer{player}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        bool drawMenu();
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

        Player* gPlayer;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];

        int** tile_map;

        bool button(button_t *btn);
        void button_process_event(button_t *btn, const SDL_Event *e);
};
#endif
