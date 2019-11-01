#ifndef NETWORKCONTROLLER_HPP
#define NETWORKCONTROLLER_HPP

#include "Controller.hpp"

class NetworkController : public Controller {
    public:
        const Uint8* pollEvent() override;
};

#endif