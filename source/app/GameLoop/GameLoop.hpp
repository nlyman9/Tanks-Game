#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

class GameLoop {
  public:
    GameLoop() {};
    ~GameLoop();
    int run();
    // SDL_Window* getWindows();
    // SDL_Renderer* getRenderer();
    // std::vector<SDL_Texture*> getTextures();

  private:
    bool init();
    void close();

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    std::vector<SDL_Texture*> gTex;

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int BOX_WIDTH = 20;
    const int BOX_HEIGHT = 20;

    const int MAX_SPEED = 5;
};

#endif