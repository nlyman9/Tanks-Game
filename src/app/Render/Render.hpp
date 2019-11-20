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
        std::vector<Player*> gPlayers;
		std::vector<Enemy *> gEnemies;
        std::vector<Projectile *> gProjectiles;

        Render() {}
        Render(std::vector<Player*> players, std::vector<Enemy*> enemies): gPlayers{players}, gEnemies{enemies} {}
        ~Render();

        int draw(double update_lag);
        int drawMenu();
        int drawMultiplayerMenu(char keypress, int selectedBox);
        bool init();
        void close();

        SDL_Renderer* getRenderer();

        void setTileMap(std::vector<std::vector<int>>* tileMap);
        void addProjectile(Projectile* newProjectile);

        void setPlayer(std::vector<Player*> players);
        void setPlayerEnemies(std::vector<Player*> players);
        void setEnemies(std::vector<Enemy *> enemies);
        void setProjectiles(std::vector<Projectile *> projectiles);

        //menu stuff
        //draw a box
        int drawBox(Box toDraw);
        //clear the background
        int drawBackground();
        //present the renderer
        int present();
        //draw text
        int drawText(Box* box, const std::string* toDraw, int XOFFSET, int YOFFSET, int WIDTHEXT, int HEIGHTEXT);
    private:
        //for later to draw buttons differently from text fields
        int drawButton(Box toDraw);
        int drawTextField(Box toDraw);

        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        SDL_Surface* gScreenSurface;


        SDL_Texture* gTileSheet;
        SDL_Rect gTileRects[3];

        std::vector<std::vector<int>> tile_map;

        //The font that's going to be used
        TTF_Font *font;
        //the texture to copy to the renderer
        SDL_Texture* Message;
        //The color of the font
        SDL_Color textColor = { 200, 200, 200 };
};
#endif
