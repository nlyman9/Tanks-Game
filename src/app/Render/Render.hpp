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
#include <memory>
#include "Constants.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "ImageLoader.hpp"
#include "Bomb.hpp"
#include "Box.hpp"

enum {
    MENU_SINGLE,
    MENU_MULTI,
    MENU_CREDITS,
    DEFAULT,
    MENU_NONE
};

class Render {
    public:
        Render() {}
        Render(std::vector<Player*> players, std::vector<Enemy*> enemies): gPlayers{players}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        int drawMenu();
        int drawMultiplayerMenu(char keypress, int selectedBox);
        bool init();
        void close();

        std::vector<Player*> gPlayers;
		std::vector<Enemy *> gEnemies;
        std::vector<Projectile *> gProjectiles;
        std::vector<Bomb *> gBombs;
        int cursorX; 
        int cursorY;

        SDL_Renderer* getRenderer();

        void setTileMap(std::vector<std::vector<int>>* tileMap);
        void addProjectile(Projectile* newProjectile);

        void setPlayer(std::vector<Player*> players);
        void setPlayerEnemies(std::vector<Player*> players);
        void setEnemies(std::vector<Enemy *> enemies);
        void setProjectiles(std::vector<Projectile *> projectiles);
        void setBombs(std::vector<Bomb *> bombs);
        void setTimer(unsigned int passed_timer); 

        void clear();

        std::vector<std::vector<int>> getTileMap();

        //menu stuff
        //draw a box
        int drawBox(Box toDraw);
        //clear the background
        int drawScreen(SDL_Texture* screen);
        //present the renderer
        int present();
        //draw text
        int drawText(Box* box, const std::string* toDraw, int XOFFSET, int YOFFSET, int WIDTH, int HEIGHT);

        int drawGameOver(SDL_Texture* screen);

    private:
        //for later to draw buttons differently from text fields
        int drawButton(Box toDraw);
        int drawTextField(Box toDraw);

        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        SDL_Surface* gScreenSurface;
        SDL_Color white;
        SDL_Surface* surfaceMessage;
        SDL_Texture* timer_display;
        SDL_Rect timer_box;
        unsigned int timer;
        SDL_Texture* crosshair;

        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[5];

        std::vector<std::vector<int>> tile_map;

        //The font that's going to be used
        TTF_Font *fontRoboto;
        TTF_Font *fontSansUntertale;
        //the texture to copy to the renderer
        SDL_Texture* Message;
        //The color of the font
        SDL_Color textColor = { 200, 200, 200 };
};
#endif
