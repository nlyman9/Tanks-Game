#ifndef MultiplayerMenu__H
#define MultiplayerMenu__H
#include "Args.hpp"
#include "Constants.hpp"
#include "Box.hpp"
#include "Render.hpp"
#include <string>
//if doing saved box add SAVE, SAVED
enum boxid {HOST, CONNECT, EXIT, IPBOX, PORTBOXC, PORTBOXH, OKAY, CANCEL, NUM_BOXES};
Args* MultiplayerMenu(Render* renderer);
class Menu{
    private:
        std::vector<Box>* boxes;
        /*
            x and y of every box

        */
        const int col[13] = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200};
        const int WIDTH = 100;
        const int HEIGHT = 80;
        const int row[8] = {0, 100, 200, 300, 400, 500, 600, 700};
        std::vector<char>* port;
        std::vector<char>* ip;
        const int PORT_MAX = 5;
        const int IP_MAX = 12;
        void initRect(SDL_Rect* box,int x,int y,int w,int h){
            box->x = x;
            box->y = y;
            box->h = h;
            box->w = w;
        }
    public:
        void printEnum(int i){
            std::string s;
            switch(i){
                case HOST:
                        s = std::string("Host");
                        break;
                    case CONNECT:
                        s = std::string("Connect");
                        break;
                    case EXIT:
                        s = std::string("Main Menu");
                        break;
                    case IPBOX:
                        s = std::string("Ip Box");
                        break;
                    case PORTBOXC:
                        s = std::string("Port box");
                        break;
                    case PORTBOXH:
                        s = std::string("Port box");
                        break;
                    case OKAY:
                        s = std::string("Okay");
                        break;
                    case CANCEL:
                        s = std::string("Cancel");
                        break;
            }
            std::cout << s << std::endl;
        }
        Menu(){
            std::cout << "Creating boxes" << std::endl;
            this->boxes = new std::vector<Box>(); 
            port = new std::vector<char>();
            ip = new std::vector<char>();
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
            initRect(rect, col[0], row[6], WIDTH, HEIGHT);
            Box* box = new Box(rect, HOST, 1, BUTTON); //screen 1
            box->setVisible(true);
            boxes->push_back(*box);
            //CONNECT BOX
            initRect(rect, col[4], row[6] , WIDTH, HEIGHT);
            box = new Box(rect, CONNECT, 2, BUTTON); //screen 2
            box->setVisible(true);
            boxes->push_back(*box);
            //EXIT BOX
            initRect(rect, col[9], row[6] , WIDTH, HEIGHT);
            box = new Box(rect, EXIT, -1, BUTTON); //return to menu
            box->setVisible(true);
            boxes->push_back(*box);

            std::cout << "Middle row" << std::endl;

            //top row
            initRect(rect, col[1], row[1] , WIDTH, HEIGHT);
            box = new Box(rect, IPBOX, 2, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[1], row[3] , WIDTH, HEIGHT);
            box = new Box(rect, PORTBOXC, 1, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[1], row[3] , WIDTH, HEIGHT);
            box = new Box(rect, PORTBOXH, 2, TEXT);
            box->setVisible(false);
            boxes->push_back(*box);
            
            //middle row
            initRect(rect, col[2], row[4] , WIDTH, HEIGHT);
            box = new Box(rect, OKAY, 3, BUTTON);
            box->setVisible(false);
            boxes->push_back(*box);

            initRect(rect, col[4], row[4] , WIDTH, HEIGHT);
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
            ip->clear();
            ip->resize(IP_MAX + 1);
        }
        void clearPort(){
            port->clear();
            port->resize(PORT_MAX + 1);
        }
        void typeCharacter(int buffer_name, char character){
            int pos = 0;
            switch(buffer_name){
                case 0:
                    //writing to ip
                    pos = ip->at(IP_MAX);
                    std::cout << "Position : " << pos << std::endl;
                    if(pos < IP_MAX){
                        ip->at(pos) = character;
                        ip->at(IP_MAX)++;
                    }
                    break;
                case 1:
                    //writing to port
                    pos = port->at(PORT_MAX);
                    std::cout << "Position : " << pos << std::endl;
                    if(pos < PORT_MAX){
                        port->at(pos) = character;
                        port->at(PORT_MAX)++;
                    }
                    break;
                default:
                    std::cout << "Buffer does not exist" << std::endl;
                    break;
            }
            std::cout << "IP : " << std::endl;
            for (auto i = ip->begin(); i != ip->end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl << "PORT : " << std::endl;
            for (auto i = port->begin(); i != port->end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl;
        }
        void deleteCharacter(int buffer_identifier){
            int pos = 0;
            switch(buffer_identifier){
                case 0:
                    //delete from ip
                    pos = ip->at(IP_MAX);
                    if(pos > 0){
                        ip->at(pos-1) = '\0';
                        ip->at(IP_MAX)--;
                    }
                    std::cout << "Position : " << pos << std::endl;
                    break;
                case 1:
                    //delete from port
                    pos = port->at(PORT_MAX);
                    if(pos > 0){
                        port->at(pos-1) = '\0';
                        port->at(PORT_MAX)--;
                    }
                    std::cout << "Position : " << pos << std::endl;
                    break;
                default:
                    std::cout << "Buffer does not exist" << std::endl;
                    break;
            }
            std::cout << "IP : " << std::endl;
            for (auto i = ip->begin(); i != ip->end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl << "PORT : " << std::endl;
            for (auto i = port->begin(); i != port->end(); ++i)
                std::cout << *i << ' ';
            std::cout << std::endl;
        }
};
#endif