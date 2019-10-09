#ifndef STARTUPMENU_HPP
#define STARTUPMENU_HPP

#include <SDL2/SDL.h>
#include "GameLoop.hpp"

class StartUpMenu {
  public:
    void launch();
  private:
    int drawMenu();
};

#endif