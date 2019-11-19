#ifndef MultiplayerMenu__H
#define MultiplayerMenu__H
#include "Args.hpp"
#include "Constants.hpp"
#include "Box.hpp"
#include "Render.hpp"
Args* MultiplayerMenu(Render* renderer);
class Menu{
    private:
        std::vector<Box*>* boxes;
        int hostX;
        int hostY;
        int connectX;
        int connectY;
        int exitX;
        int exitY;
        int ipX;
        int ipY;
        int portHX;
        int portHY;
        int portCX;
        int portCY;
        int okayX;
        int okayY;
        int cancelX;
        int cancelY;
        int saveX;
        int saveY;
        int savedX;
        int savedY;
        int WIDTH;
        int HEIGHT;
        int port[5];
        int ip[12];
        int PORT_MAX = 5;
        int IP_MAX = 12;
    public:
        Menu(){
            boxes = new std::vector<Box*>();
            //create all the menu structs
            SDL_Rect hostBox = { hostX, hostY , WIDTH, HEIGHT};
            boxes->push_back(new Box(hostBox, HOST, 1, BUTTON));
            SDL_Rect connectBox = { connectX, connectY , WIDTH, HEIGHT};
            boxes->push_back(new Box(connectBox, CONNECT, 2, BUTTON));
            SDL_Rect exitBox = { exitX, exitY , WIDTH, HEIGHT};
            boxes->push_back(new Box(exitBox, EXIT, -1, BUTTON));
            SDL_Rect ipBox = { ipX, ipY , WIDTH, HEIGHT};
            boxes->push_back(new Box(ipBox, IPBOX, 2, TEXT));
            SDL_Rect portBoxH = { portHX, portHY , WIDTH, HEIGHT};
            boxes->push_back(new Box(portBoxH , PORTBOXH, 2, TEXT));
            SDL_Rect portBoxC = { portCX, portCY , WIDTH, HEIGHT};
            boxes->push_back(new Box(portBoxC, PORTBOXC, 1, TEXT));
            SDL_Rect okayBox = { okayX, okayY , WIDTH, HEIGHT};
            boxes->push_back(new Box(okayBox, OKAY, 3, BUTTON));
            SDL_Rect cancelBox = { cancelX, cancelY , WIDTH, HEIGHT};
            boxes->push_back(new Box(cancelBox, CANCEL, 0, BUTTON));
            SDL_Rect saveBox = { saveX, saveY , WIDTH, HEIGHT};
            boxes->push_back(new Box(saveBox, SAVE, 0, BUTTON));
            SDL_Rect savedBox = { savedX, savedY , WIDTH, HEIGHT};
            boxes->push_back(new Box(savedBox, SAVED, 0, BUTTON));
            clearIP();
            clearPort();
        }
        Box getBox(int id){
            return *boxes->at(id);
        }
        void setInvisible(){
            for(int i = 0; i < NUM_BOXES; i++){
                boxes->at(i)->setVisible(false);
            }
        }
        void setVisible(int id){
            boxes->at(id)->setVisible(true);
        }
        void clearIP(){
            for(int i = 0; i < IP_MAX; i++){
                this->ip[i] = 0;
            }
        }
        void clearPort(){
            for(int i = 0; i < PORT_MAX; i++){
                this->port[i] = 0;
            }
        }
};
#endif