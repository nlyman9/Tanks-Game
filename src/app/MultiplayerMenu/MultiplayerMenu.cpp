#include "MultiplayerMenu.hpp"

Args* MultiplayerMenu(Render* renderer) 
{
    int crBox, currMenu;
    int screen = 0;
    int prevScreen = 0;
    std::cout << "creating menu" << std::endl;
    Menu* menu = new Menu();
    std::cout << "menu created" << std::endl;
    Args* options = new Args();
    std::cout << "In menu" << std::endl;
    while(true){
        SDL_Event e;
        int keyPress = 0;
        //check for key presses and mouse presses, relay that information to the renderer, 
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
                return nullptr;
            }
            else if(e.type == SDL_KEYDOWN){
                //get the int of the keypress
                //with this we will use it later to check if we render a character to the screen or not
                keyPress = e.key.keysym.sym; 
                std::cout << "key press is : " << keyPress << std::endl;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				SDL_Rect clickBox = {x, y, 1, 1};
				SDL_Rect intersection;
				for(int i = 0; i < NUM_BOXES; i++){
                    Box currBox = menu->getBox(i);
                    if(currBox.isVisible()){
                        SDL_Rect temp = *(currBox.getPosition());
                        if(currBox.getType() == TEXT){
                            temp.w += TEXT_FIELD_OFFSET + TEXT_FIELD_WIDTH;
                        }
                        if(SDL_IntersectRect(&clickBox, &temp, &intersection)){
                            std::cout << "Current box is ";
                            menu->printEnum(i);
                            if(screen != currBox.getScreen())
                                screen = currBox.getScreen();
                            crBox = i;
                            break;
                        }
                    }
                }
			}
        }
        if(prevScreen != screen){
            prevScreen = screen;
            //we've changed screens clear ip and port
            menu->clearIP();
            menu->clearPort();
            menu->setInvisible();
            switch(screen){
                //set buttons visible based on screen
                case -1:
                    //exit
                    delete menu;
                    delete options;
                    return nullptr;
                case 0:
                    //default screen
                    /*
                        visible boxes
                        SAVED, HOST, CONNECT, EXIT
                    */
                    //menu->setVisible(SAVED);
                    std::cout << "DEFAULT" << std::endl;
                    menu->setVisible(HOST);
                    menu->setVisible(CONNECT);
                    menu->setVisible(EXIT);
                    break;
                case 1:
                    //HOST screen
                    /*
                        visible boxes
                        SAVE, HOST, IPBOX, PORTBOXC CONNECT, OKAY, CANCEL, EXIT
                    */
                    std::cout << "CONNECT" << std::endl;
                    menu->setVisible(HOST);
                    menu->setVisible(CONNECT);
                    menu->setVisible(EXIT);
                    menu->setVisible(PORTBOXC);
                    //menu->setVisible(SAVE);
                    menu->setVisible(OKAY);
                    menu->setVisible(CANCEL);
                    break;
                case 2:
                    //host screen
                    /*
                        visible boxes
                        HOST, CONNECT, EXIT, PORTBOXH
                    */
                   std::cout << "HOST" << std::endl;
                    menu->setVisible(HOST);
                    menu->setVisible(CONNECT);
                    menu->setVisible(EXIT);
                    menu->setVisible(PORTBOXH);
                    menu->setVisible(IPBOX);
                    menu->setVisible(OKAY);
                    menu->setVisible(CANCEL);
                    break;
                case 3:
                    //we clicked ok so check to see if ip and port are set and create the args array   
                default:
                    break;    
            }
        }
        //draw background
        renderer->drawBackground();
        //draw box
        for(int i = 0; i < NUM_BOXES; i++){
            Box currBox = menu->getBox(i);
            if(currBox.isVisible()){
                renderer->drawBox(currBox);
                std::string* string = nullptr;
                switch(currBox.getID()){
                    case HOST:
                        string = new std::string("Host");
                        break;
                    case CONNECT:
                        string = new std::string("Connect");
                        break;
                    case EXIT:
                        string = new std::string("Main Menu");
                        break;
                    case IPBOX:
                        string = new std::string("Ip Box");
                        break;
                    case PORTBOXC:
                        string = new std::string("Port box");
                        break;
                    case PORTBOXH:
                        string = new std::string("Port box");
                        break;
                    case OKAY:
                        string = new std::string("Okay");
                        break;
                    case CANCEL:
                        string = new std::string("Cancel");
                        break;
                }
                if(string != nullptr){
                    renderer->drawText(&currBox, string, 0, 0, 0, 0);
                    delete string;
                }
            }
        }
        renderer->present();
        //delete a character
        if(keyPress == 8){
            switch(crBox){
                case IPBOX:
                    //delete from ip
                    menu->deleteCharacter(0);
                    break;
                case PORTBOXC:
                    //delete from port
                    menu->deleteCharacter(1);
                    break;
                case PORTBOXH:
                    //delete from port
                    menu->deleteCharacter(1);
                    break;
                default:
                    break;
            }
        }
        //make sure keyboard has an int and write it to a box
        if(keyPress >= 48 && keyPress <= 57){
            keyPress = keyPress - 48; //set it from 0 to 9
            std::string typed = std::to_string(keyPress);
            std::cout << "Typed char = " << typed.at(0);
            switch(crBox){
                case IPBOX:
                    menu->typeCharacter(0, typed.at(0));
                    break;
                case PORTBOXC:
                    //write to port box connect and port array
                    menu->typeCharacter(1, typed.at(0));
                    break;
                case PORTBOXH:
                    //write to port box host and port array
                    menu->typeCharacter(1, typed.at(0));
                    break;
                default:
                    break;
            }
        }
    }
}