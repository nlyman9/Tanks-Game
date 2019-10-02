#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
constexpr int TILE_SIZE = 48;
  namespace mainLoop{
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;
    constexpr int BORDER_GAP = 16;
    constexpr int OBST_WIDTH = 70;
    constexpr int OBST_HEIGHT = 40;
    constexpr int BOX_WIDTH = 20;
    constexpr int BOX_HEIGHT = 20;
    constexpr int MAX_VELOCITY = 1;
    class GameLoop {
      public:
        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        //for first week -- want to remove when we switch to player object
        int x_pos;
        int y_pos;
        int x_vel;
        int y_vel;
        //change to enemy object
        int y_enemy_pos;
        int x_enemy_pos;
        //change to other/object?
        int y_obst_pos;
        int x_obst_pos;
        GameLoop() {};
        ~GameLoop();
        int run();
        // SDL_Window* getWindows();
        // SDL_Renderer* getRenderer();
        // std::vector<SDL_Texture*> getTextures();
        bool init();
        void close();
    };
  }
#endif