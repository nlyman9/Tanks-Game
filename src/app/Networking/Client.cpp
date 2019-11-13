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

bool mapReceived = false;
bool tsReady = false;

//receive lock
SDL_SpinLock rlock = 0;
//send lock
SDL_SpinLock slock = 0;

int clientThread(void* data) {
    // Unpack data in the Client object
    Client *client = (Client*) data;
    std::cout << "Client thread created!" << std::endl;

    // Try to connect to server... Usually only loops is the server is offline.
    while (!client->isConnected() && !client->connect()) {
            std::cout << "Client-Network: trying to connect to server..." << std::endl;
            sleep(1);
    }

    while(true) {
        std::cout << "Client-Network: looping" << std::endl;
        Packet *mail = client->receive();
        if (mail != nullptr) {
            // Do stuff 
            std::cout << "You got mail!" << std::endl;
            std::cout << "Mail: " << mail->data() << std::endl;
        }
        //else just sleep

        sleep(1);
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

bool Client::init() {
    // gameMap = new std::vector<int>();
    // //initialize all buffers
    // //receive buffer
    // rcBuffer = new std::vector<char>(R_BUFFER_SIZE);
    // //to send buffer
    // //buffer to fill in
    // fBuffer = new std::vector<char>();
    // //to send buffer
    // tsBuffer = new std::vector<char>();
    // // keystate buffer
    // keystates = new std::vector<Uint8*>();
    // Pack Client into void pointer for thread
    
    std::cout << "Client init()" << std::endl;
    void* clientInfo = (void *) malloc(sizeof(Client));
    clientInfo = (void*) this;
    gameOn = true;
    rcThread = SDL_CreateThread(clientThread, "myThread", (void*) clientInfo);
    return true;
}

// Client Client::initClient(Client c) {}
