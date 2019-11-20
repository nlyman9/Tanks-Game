/*
    Originally created by Jakob - Modified by Adam 
    Modifications:
    test to create a map on a server then pack it and send it over as bit data
    and unpack render on client
*/
#include "Client.hpp"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL.h>

#define R_BUFFER_SIZE 152

//receive lock
SDL_SpinLock rlock = 0;
//send lock
SDL_SpinLock slock = 0;


bool Client::init() {
    std::cout << "Client init()" << std::endl;
    void* clientInfo = (void *) malloc(sizeof(Client));
    clientInfo = (void*) this;
    rcThread = SDL_CreateThread(this->clientProcess, "myThread", (void*) clientInfo);
    return true;
}

int Client::clientProcess(void* data) {
    // Unpack data in the Client object
    Client *client = (Client*) data;
    std::cout << "Client thread created!" << std::endl;

    // Try to connect to server... Usually only loops is the server is offline.
    while (!client->isConnected() && !client->connect()) {
        std::cout << "Client-Network: trying to connect to server..." << std::endl;
        sleep(1);
    }

    // Wait for initialization information
    bool recvedMap = false;
    bool recvedInit = false;
    while(true) {
        std::cout << "Client-Network: Waiting for initialization data" << std::endl;
        Packet *mail = client->receiveAndGet();
        if (mail != nullptr) {

            // Recv Init
            if(mail->getType() == PackType::INIT) {
                std::cout << "Client: Initializing game... " << mail->data() << std::endl;
                std::vector<char>* data = mail->getBody();

                // Get this player's id
                std::string id_str = "";
                id_str += data->at(0);
                int currId = atoi(id_str.c_str());

                // Get player 1's position
                std::string id_strP1 = "";
                id_strP1 += data->at(2);
                int p1ID = atoi(id_strP1.c_str());

                int i = 4;
                std::string x_strP1 = "";
                while(data->at(i) != ' ') {
                    x_strP1 += data->at(i);
                    i++;
                }
                int p1X_pos = atoi(x_strP1.c_str());
                
                i++;
                std::string y_strP1 = "";
                while(data->at(i) != ' ') {
                    y_strP1 += data->at(i);
                    i++;
                }
                int p1Y_pos = atoi(y_strP1.c_str());
                
                // Get player 2's position
                i++;
                std::string id_strP2 = "";
                id_strP2 += data->at(i);
                int p2ID = atoi(id_strP2.c_str());

                i += 2;
                std::string x_strP2 = "";
                while(data->at(i) != ' ') {
                    x_strP2 += data->at(i);
                    i++;
                }
                int p2X_pos = atoi(x_strP2.c_str());
                
                i++;
                std::string y_strP2 = "";
                while(data->at(i) != ' ') {
                    y_strP2 += data->at(i);
                    i++;
                }
                int p2Y_pos = atoi(y_strP2.c_str());

                if(currId == p1ID) {
                    std::cout << "Client: ID: " << currId << " Starting Position: (" << p1X_pos << ", " << p1Y_pos << ")" << std::endl;
                    client->initData.push_back(currId);
                    client->initData.push_back(p1X_pos);
                    client->initData.push_back(p1Y_pos);
                    client->initData.push_back(p2X_pos);
                    client->initData.push_back(p2Y_pos);
                } else if(currId == p2ID) {
                    std::cout << "Client: ID: " << currId << " Starting Position: (" << p2X_pos << ", " << p2Y_pos << ")" << std::endl;
                    client->initData.push_back(currId);
                    client->initData.push_back(p2X_pos);
                    client->initData.push_back(p2Y_pos);
                    client->initData.push_back(p1X_pos);
                    client->initData.push_back(p1Y_pos);
                } else {
                    std::cout << "Client: Received Illegal ID" << std::endl;
                }
                
                recvedInit = true;
            }

            // Recv Map
            if (mail->getType() == PackType::MAP) {
                std::cout << "Client: Loading map... " << mail->data() << std::endl;
                
                std::vector<int>* map = new std::vector<int>();
                map = unpack(mail->getBody(), map, 3);

                for (int i = 0; i < map->size();i++){
                    client->gameMap.push_back(map->at(i));                 
                }

                client->mapReceived = true;
                delete map;

                recvedMap = true;
            }
            
            if(recvedInit && recvedMap) {
                client->initDataReceived = true;
                client->gameOn = true;
                client->startGame = true;
                break;
            }
        }
        //else just sleep

        sleep(1);
    }

    // TODO Consolidate tickrates
    client->setSocketTickrate(30);
    while (true) {
        // Check if we have anything to send
        if (client->send()) {
            // std::cout << "CLIENT-NET: Sent packet!"<< std::endl;
            // fflush(stdout);
        } else {
            // std::cout << "CLIENT-NET: Nothing to send!" << std::endl;
            // fflush(stdout);
        }

        // Receive data from server 
        Packet *mail = client->receiveAndGet();
        if (mail != nullptr) {
            std::cout << "CLIENT-NET: Received packet type " << (int)mail->getType() << " -> ";
            mail->printData();
            fflush(stdout);

            // If keystate, unpack a load into formable keystate
            // TODO not hardcode id to 0 
            if (mail->getType() == PackType::KEYSTATE) {
                int turret_theta = mail->getInt(5); // 5 is the starting index of the integer for the turret theta
                bool hasShot = mail->getBody()->at(10); // 10 is the index of the boolean if the player has shot
                client->addNetworkKeyState(0, mail->getBody(), turret_theta, hasShot);
            }
        }

        // TODO wait if we finish early?
    }

    return -1;
}