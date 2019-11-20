#include "MultiplayerMenu.hpp"

Args* MultiplayerMenu(Render* renderer) 
{
    int crBox, currMenu;
    int screen = 0;
    int prevScreen = 0;
    bool host = false;
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
                        SDL_Rect temp = *(currBox.getRectangle());
                        if(currBox.getType() == TEXT){
                            temp.w += currBox.TEXT_FIELD_OFFSET() + currBox.TEXT_FIELD_WIDTH();
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
        //first we check to see if we changed screens, if we did clear ip and port numbers
        if(prevScreen != screen){
            prevScreen = screen;
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
                    //Connect
                    /*
                        visible boxes
                        SAVE, HOST, IPBOX, PORTBOXC CONNECT, OKAY, CANCEL, EXIT
                    */
                    std::cout << "HOST" << std::endl;
                    menu->setVisible(HOST);
                    menu->setVisible(CONNECT);
                    menu->setVisible(EXIT);
                    menu->setVisible(PORTBOXC);
                    host = true;
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
                    std::cout << "CONNECT" << std::endl;
                    host = false;
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
                    if(!host && menu->ipDigits() < 4){
                        std::cout << "Not enough digits in ip, setting to local area connection" << std::endl;
                        options->ip = "0.0.0.0";
                    }else if(host){
                        std::cout << "we are host dun dun dun dun dun" << std::endl;
                        options->ip = "0.0.0.0";
                    }
                    else{
                        options->ip = *(menu->getIPwithPeriods());
                    }
                    if(menu->portDigits() == 0){
                        std::cout << "Not enough digits in port, setting port to 7777" << std::endl;
                        options->port = 7777;
                    }else{
                        options->port = stoi(*(menu->getPort()), nullptr, 10);
                    }
                    options->isOnline = true;
                    options->isHost = host;
                    std::cout << "Options ip : " << options->ip << std::endl;
                    std::cout << "Options port : " <<  options->port << std::endl;
                    delete menu;
                    return options;
                    break;
                default:
                    break;    
            }
            //we've changed screens clear ip and port
            menu->clearIP();
            menu->clearPort();
        }
        //next we deal with typing into the port or ip box
        //first we check if we hit backspace
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
        //next we check if a number was pressed
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
        //now we draw
        //draw background
        renderer->drawBackground();
        //draw box
        for(int i = 0; i < NUM_BOXES; i++){
            Box currBox = menu->getBox(i);
            if(currBox.isVisible()){
                renderer->drawBox(currBox);
                std::string* string = nullptr;
                std::string* textField = nullptr;
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
                        textField = menu->getIPwithPeriods();
                        renderer->drawText(&currBox, textField, currBox.getRectangle()->w + currBox.TEXT_FIELD_OFFSET() + 5, 0, menu->intSize * (menu->ipDigits() + menu->numPeriods()), currBox.getRectangle()->h);
                        delete textField;
                        break;
                    case PORTBOXC:
                        string = new std::string("Port box");
                        textField = menu->getPort();
                        renderer->drawText(&currBox, textField, currBox.getRectangle()->w + currBox.TEXT_FIELD_OFFSET() + 5, 0, menu->intSize * menu->portDigits(), currBox.getRectangle()->h);
                        break;
                    case PORTBOXH:
                        string = new std::string("Port box");
                        textField = menu->getPort();
                        renderer->drawText(&currBox, textField, currBox.getRectangle()->w + currBox.TEXT_FIELD_OFFSET() + 5, 0, menu->intSize * menu->portDigits(), currBox.getRectangle()->h);
                        delete textField;
                        break;
                    case OKAY:
                        string = new std::string("Okay");
                        break;
                    case CANCEL:
                        string = new std::string("Cancel");
                        break;
                }
                if(string != nullptr){
                    renderer->drawText(&currBox, string, 0, 0, currBox.getRectangle()->w, currBox.getRectangle()->h);
                    delete string;
                }
            }
        }
        renderer->present();
    }
}