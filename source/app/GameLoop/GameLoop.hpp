#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

constexpr int TILE_SIZE = 48;
  namespace mainLoop{
    class GameLoop {
      public:
        std::vector<SDL_Texture*> gTex;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;

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