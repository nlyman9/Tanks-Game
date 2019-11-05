#ifndef NETWORKCONTROLLER_HPP
#define NETWORKCONTROLLER_HPP

#include "Controller.hpp"

class NetworkController : public Controller {
    public:
        NetworkController();
        const Uint8* pollEvent() override;
        void setKeystate(uint8_t* state);
    private:
        uint8_t* keystate;
};

#endif