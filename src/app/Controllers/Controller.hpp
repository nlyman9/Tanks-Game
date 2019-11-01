#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SDL2/SDL.h>

class Controller {
    public:
        ~Controller();
        // Must override pollEvent function 
        virtual const Uint8* pollEvent() = 0;
    private:


};

#endif