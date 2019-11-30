#ifndef STARTUPMENU_HPP
#define STARTUPMENU_HPP

#include "Args.hpp"

enum {
    CLOSE = -1,
    MENU,
    WIN,
    LOSE    
};

void launch(Args *options);

#endif