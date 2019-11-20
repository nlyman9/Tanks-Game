#ifndef MultiplayerMenu__H
#define MultiplayerMenu__H
#include "Args.hpp"
#include "Constants.hpp"
#include "Box.hpp"
#include "Render.hpp"
Args* MultiplayerMenu(Render* renderer);
class Menu{
    private:
        std::vector<Box>* boxes;
        /*
            x and y of every box

        */
        const int col[6] = {0, 100, 200, 300, 400, 500};
        const int WIDTH = 50;
        const int HEIGHT = 100;
        const int row[3] = {600, 300, 100};
        int port[6];
        int ip[13];
        const int PORT_MAX = 6;
        const int IP_MAX = 13;
        void initRect(SDL_Rect* box,int x,int y,int w,int h){
            box->x = x;
            box->y = y;
            box->h = h;
            box->w = w;
        }
    public:
        Menu(){
            std::cout << "Creating boxes" << std::endl;
            this->boxes = new std::vector<Box>();      
            //for later?
            /*
            SDL_Rect saveBox = {col[ ], row[ ] , WIDTH, HEIGHT};
            boxes->push_back(new Box(saveBox, SAVE, 0, BUTTON));
            SDL_Rect savedBox = {col[ ], row[ ] , WIDTH, HEIGHT};
            boxes->push_back(new Box(savedBox, SAVED, 0, BUTTON));
            */
            std::cout << "Filling in boxes array" << std::endl;
            initMultiplayerMenu();
            std::cout << "Clearing ip and port" << std::endl;
            clearIP();
            clearPort();
        }
        void initMultiplayerMenu(){
            //create all the menu structs
            std::cout << "bottom row" << std::endl;
            //bottom row
            SDL_Rect* rect = new SDL_Rect();
            //HOST BOX
            initRect(rect, col[0], row[0], WIDTH, HEIGHT);
            Box* box = new Box(rect, HOST, 1, BUTTON); //scren 1
            box->setVisible(true);
            boxes->push_back(*box);
            //CONNECT BOX
            initRect(rect, col[1], row[0] , WIDTH, HEIGHT);
            box = new Box(rect, CONNECT, 2, BUTTON); //screen 2
            box->setVisible(true);
            boxes->push_back(*box);
            //EXIT BOX
            initRect(rect, col[2], row[0] , WIDTH, HEIGHT);
            box = new Box(rect, EXIT, -1, BUTTON);
            box->setVisible(true);
            boxes->push_back(*box);

            std::cout << "Middle row" << std::endl;

            //top row
            initRect(rect, col[1], row[2] , WIDTH, HEIGHT);
            box = new Box(rect, IPBOX, 2, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[2], row[2] , WIDTH, HEIGHT);
            box = new Box(rect, PORTBOXC, 1, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[2], row[2] , WIDTH, HEIGHT);
            box = new Box(rect, PORTBOXH, 2, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);
            
            //middle row
            initRect(rect, col[3], row[1] , WIDTH, HEIGHT);
            box = new Box(rect, OKAY, 3, BUTTON);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[5], row[1] , WIDTH, HEIGHT);
            box = new Box(rect, CANCEL, 0, BUTTON); //goes back to screen 0
            box->setVisible(false);
            boxes->push_back(*box);

            std::cout << "Top Row" << std::endl;
            
        }
        Box getBox(int id){
            return (boxes->at(id));
        }
        void setInvisible(){
            for(int i = 0; i < NUM_BOXES; i++){
                boxes->at(i).setVisible(false);
            }
        }
        void setVisible(int id){
            std::cout << id << std::endl;
            boxes->at(id).setVisible(true);
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