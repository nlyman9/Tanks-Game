#include <iostream>

class OBJECT{
    int x, y;
    Sprite sprite;
    public:
        virtual void draw() = 0; //draw the object to the screen
        virtual void move(int x, int y) = 0; //move the object an offset from its current position
        virtual void place(int x, int y) = 0; //place/teleport an object to another location
        virtual ~OBJECT() = 0; //destructor
};  
OBJECT::~OBJECT()
{
    std::cout << "Object destructor";
}

