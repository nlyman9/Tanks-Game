#ifndef BOX__H
#define BOX__H
#include <SDL2/SDL.h>
#include <vector>
#include <string>

enum boxtype {TEXT, BUTTON};
class Box{
    private:
        SDL_Rect* position;
        int id;
        int screen;
        bool visible;
        int type;
        int w;
        int o;
        std::string src;
    public:
    Box(SDL_Rect* box, int id, int screen, int type){
        this->id = id;
        this->screen = screen;
        this->type = type;
        this->position = new SDL_Rect();
        setRectangle(box);
        src = "";
    }
    void setSRC(std::string newSRC){
        src = newSRC;
    }
    std::string getIMGPath(){
        return src;
    }
    int TEXT_FIELD_WIDTH(){
        return w;
    }
    int TEXT_FIELD_OFFSET(){
        return o;
    }
    void setText_Field_Width(int width){
        w = width;
    }
    void setText_Field_Offset(int offset){
        o = offset;
    }
    void setVisible(bool visibility){
        this->visible = visibility;
    }
    bool isVisible(){
        return this->visible;
    }
    void setRectangle(SDL_Rect* newPosition){
        this->position->x = newPosition->x;
        this->position->y = newPosition->y;
        this->position->w = newPosition->w;
        this->position->h = newPosition->h;
    }
    SDL_Rect* getRectangle(){
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