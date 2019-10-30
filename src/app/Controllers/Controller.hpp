#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

class Controller {
    public:
        // Must override pollEvent function 
        virtual int pollEvent() = 0;
    private:


}

#endif