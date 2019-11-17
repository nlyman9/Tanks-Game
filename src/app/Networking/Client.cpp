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
    rcThread = SDL_CreateThread(this->clientThread, "myThread", (void*) clientInfo);
    return true;
}

int Client::clientThread(void* data) {
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

    // const struct timespec timeout = {1, 0};//((long) 1e+9 / 30)};
    client->setSocketTickrate(10);
    while (true) {
        // std::cout << "Client-Network: Loop" << std::endl;
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

        // if (nanosleep(&timeout, NULL) != 0) {
        //     std::cerr << "Error sleeping " << errno << std::endl;
        //     fflush(stdout);
        // }

    }

    return -1;

    // //set the socket to non-blocking
    // fcntl(sockfd, F_SETFL, O_NONBLOCK);

    // Game loop
    // while(crClient->gameOn) {
    //     if(gameBufferReady) {
    //         SDL_AtomicLock(&slock);
    //         for(auto item : *fBuffer) {
    //             tsBuffer->push_back(item);
    //         }
    //         gameBufferReady = false;
    //         tsReady = true; //make sure to set the flag within the lock
    //         SDL_AtomicUnlock(&slock);
    //         fBuffer->clear();
    //     }
    //     read_fds = master;
    //     // Check for any response from serrver
    //     if (select(fdmax+1, &read_fds, NULL, NULL, timeout) == -1) {
    //         std::cout << "CLIENT: Select error: " << strerror(errno) <<  std::endl;
    //         continue;
    //     }
    //     // If from server
    //     if (FD_ISSET(sockfd, &read_fds)) {
    //             nbytes = recv(sockfd, rcBuffer->data(), R_BUFFER_SIZE, 0);
    //         if (nbytes < 0) {
    //             fprintf(stderr, "CLIENT: recv: %s (%d)\n", strerror(errno), errno);
    //             std::cout << "CLIENT: Connection closing.  Error Number: " << errno << std::endl;
    //             close(sockfd);
    //             exit(10);
    //         } else if(nbytes > 0) {
    //             /*
    //                 With the changes in the server
    //                 ->strip header once get the size of the packet
    //                 ->strip again get the type of the packet
    //             */
    //             //received data
    //             int size = stripHeader(rcBuffer);
    //             int header = stripHeader(rcBuffer);
    //             switch(header)
    //             {
    //                 // Recieve Map
    //                 case 0:
    //                 {   
    //                     std::cout << "CLIENT: receiving map data of size: " << size << std::endl;
    //                     std::vector<int>* map = new std::vector<int>();
    //                     unpack(rcBuffer, map, 3);
    //                     for(int i = 0; i < map->size();i++){
    //                         crClient->gameMap->push_back(map->at(i));                 
    //                     }
    //                     mapReceived = true;
    //                     delete map;
    //                     std::cout << "CLIENT: map data received!" << std::endl;
    //                     break;
    //                 }

    //                 // Receive Keystate
    //                 case 1:
    //                 {
    //                     for(auto keystate : *rcBuffer) {
    //                         keystates->push_back((Uint8*)&keystate);
    //                     }
    //                     std::cout << "CLIENT: keystate received!" << std::endl;
    //                     break;
    //                 }
                    
    //                 // Receive Game State
    //                 case 2:
    //                 {

    //                     break;
    //                 }
                    
    //                 // Receive Wall Destroy
    //                 case 3:
    //                 {

    //                    break;
    //                 }

    //                 default:
    //                 {
    //                     std::cout << "CLIENT: HELP ILLEGAL PACKET RECEIVED" << std::endl;
    //                     break;
    //                 }
    //                 //clear rcBuffer and set it back to correct size for receive
    //                 rcBuffer->clear();
    //                 rcBuffer->resize(R_BUFFER_SIZE);
    //             }

    //         } else {
    //             //std::cout << "CLIENT: NO data received! check if buffer size is set!" << std::endl;
    //         }
    //     } else if (tsReady) { 
    //         SDL_AtomicLock(&slock);
    //             send(sockfd, tsBuffer->data(), tsBuffer->size(), 0);
    //             tsBuffer->clear();
    //             tsReady = false;
    //         SDL_AtomicUnlock(&slock);
    //     }
    // }
}

// bool Client::pollMap() {
//     return mapReceived;
// }

// void Client::getGameBufferReady(bool flag) {
//     gameBufferReady = flag;
// }

// std::vector<char>* Client::getFillBuffer() {
//     return fBuffer;
// }

// Uint8* Client::pollKeystate() {
//     fflush(stdout);
//     if(keystates->size() != 0) {
//         auto state = keystates->back();
//         keystates->pop_back();  
//         return state;
//     } else {
//         Uint8* emptyKeystate;
//         *emptyKeystate = 0;
//         return emptyKeystate;
//     }
// }