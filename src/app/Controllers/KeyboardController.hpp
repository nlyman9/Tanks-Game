#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

class KeyboardController : public Controller {
    public:
        int pollEvent() override;

};

#endif