#ifndef BOX__H
#define BOX__H
#include <SDL2/SDL.h>
#include <vector>
//if doing saved box add SAVE, SAVED
enum boxid {HOST, CONNECT, EXIT, IPBOX, PORTBOXC, PORTBOXH, OKAY, CANCEL, NUM_BOXES};
enum boxtype {TEXT, BUTTON};
class Box{
    private:
        SDL_Rect* position;
        int id;
        int screen;
        bool visible;
        int type;
    public:
    Box(SDL_Rect* box, int id, int screen, int type){
        this->id = id;
        this->screen = screen;
        this->type = type;
        this->position = new SDL_Rect();
        setPosition(box);
    }
    void setVisible(bool visibility){
        this->visible = visibility;
    }
    bool isVisible(){
        return this->visible;
    }
    void setPosition(SDL_Rect* newPosition){
        this->position->x = newPosition->x;
        this->position->y = newPosition->y;
        this->position->w = newPosition->w;
        this->position->h = newPosition->h;
    }
    SDL_Rect* getPosition(){
        return this->position;
    }
    int getScreen(){
        return this->screen;
    }
    int getID(){
        return this->id;
    }
    int getType(){
        return this->type;
    }
};
#endif