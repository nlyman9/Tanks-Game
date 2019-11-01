#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "Controller.hpp"

class KeyboardController : public Controller {
    public:
        const Uint8* pollEvent() override;
};

#endif