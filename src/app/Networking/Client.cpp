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

    // Wait for map
    while(true) {
        std::cout << "Client-Network: Waiting for map" << std::endl;
        Packet *mail = client->receiveAndGet();
        if (mail != nullptr) {
            if (mail->getType() == PackType::MAP) {
                std::cout << "Client: Loading map... " << mail->data() << std::endl;
                
                std::vector<int>* map = new std::vector<int>();
                map = unpack(mail->getBody(), map, 3);

                for (int i = 0; i < map->size();i++){
                    client->gameMap.push_back(map->at(i));                 
                }

                client->mapReceived = true;
                delete map;

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
                client->addNetworkKeyState(0, mail->getBody());
            }
        }

        // TODO wait if we finish early?
    }

    return -1;
}