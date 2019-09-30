#ifndef CREDITS_HPP
#define CREDITS_HPP  

#include <vector>

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif // __APPLE__


class Credits {
  public:
    Credits(SDL_Window* window, SDL_Renderer* renderer, std::vector<SDL_Texture*> tex, Mix_Chunk* sound) : gWindow{window}, gRenderer{renderer}, gTex{tex}, gSound{sound} {};
    int playCredits();

  private:
    // Function declarations
    void close();

    // Globals
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    std::vector<SDL_Texture*> gTex;
    Mix_Chunk* gSound;
    
    SDL_Texture* loadImage(std::string fname);
    void jakobCredits(SDL_Texture* picture, SDL_Texture* hitmarker);

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
};

#endif