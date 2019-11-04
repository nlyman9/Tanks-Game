/*
    Originally created by Jakob - Modified by Adam 
    Modifications:
    test to create a map on a server then pack it and send it over as bit data
    and unpack render on client
*/
#include "Client.hpp"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL.h>

bool mapReceived = false;
bool tsReady = false;

int receiveThread(void* data) {
    // Unpack data in the Client object
    Client* crClient = (Client*) data;
    std::cout << "Client thread created!" << std::endl;

    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int sockfd;
    fd_set master;      // Master of file descriptors
    fd_set read_fds;    // Read fd's returned from select
    int fdmax;          // maximym file descriptor number
    int nbytes;
    // Innitialize sets to zero
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&hints, 0, sizeof(hints));

    struct timeval* timeout = (timeval*) calloc(1, sizeof(struct timeval));
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;

    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP
    // Get address info of server
    std::cout << "Connecting to " << crClient->server_ip << " on port " << crClient->server_port << std::endl;
    if ((status = getaddrinfo(crClient->server_ip.c_str(), crClient->server_port.c_str(), &hints, &serverInfo)) != 0) {
        std::cout << "Failed to get address info" << std::endl;
        exit(4);
    }
    // Create a socket based on server info 
    sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
    sleep(2);
    // Connect to server
    while (connect(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        std::cout << "Waiting for connection..." << std::endl;
        sleep(1);
    }
    std::cout << "Connected ip:" << crClient->server_ip << " Port: " << crClient->server_port << std::endl;
    //set the socket to non-blocking
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // Add server fd to master
    FD_SET(sockfd, &master);
    // Add stdin fd to master
    FD_SET(STDIN_FILENO, &master);

    fdmax = sockfd;

    std::cout << "Receive thread created!" << std::endl;

    // Game loop
    while(crClient->gameOn) {
        if(gameBufferReady) {
            for(auto item : *fBuffer) {
                tsBuffer->push_back(item);
            }
            gameBufferReady = false;
            tsReady = true;
            fBuffer->clear();
        }

        read_fds = master;
        // Check for any response from serrver
        if (select(fdmax+1, &read_fds, NULL, NULL, timeout) == -1) {
            std::cout << "Select error" << std::endl;
            continue;
        }
        // If from server
        if (FD_ISSET(sockfd, &read_fds)) {
            nbytes = recv(sockfd, rcBuffer->data(), rcBuffer->size(), 0);
            if (nbytes < 0) {
                fprintf(stderr, "recv: %s (%d)\n", strerror(errno), errno);
                std::cout << "Connection closing" << std::endl;
                close(sockfd);
                exit(10);
            } else if(nbytes > 0) {
                //recieved data
                int header = stripHeader(rcBuffer);
                switch(header)
                {
                    // Recieve Map
                    case 0:
                    {   
                        std::cout << "receiving map data..." << std::endl;
                        std::vector<int>* map = new std::vector<int>();
                        if(!mapReceived) {
                            unpack(rcBuffer, map, 3);
                            for(int i = 0; i < map->size();i++){
                                crClient->gameMap->push_back(map->at(i));                 
                            }
                            mapReceived = true;
                        }
                        delete map;
                        std::cout << "map data received!" << std::endl;
                        break;
                    }

                    // Receive Keystate
                    case 1:
                    {
                        netController->setKeystate((uint8_t*)(rcBuffer->at(0)));
                        std::cout << "keystate received!" << std::endl;
                        break;
                    }
                    
                    // Receive Game State
                    case 2:
                    {

                        break;
                    }
                    
                    // Receive Wall Destroy
                    case 3:
                    {

                       break;
                    }

                    default:
                    {
                        std::cout << "HELP ILLEGAL PACKET RECEIVED" << std::endl;
                        break;
                    }
                }
            }else{
                std::cout << "NO data received! check if buffer size is set!" << std::endl;
            }
        } else if (tsReady) { 
            send(sockfd, tsBuffer->data(), tsBuffer->size(), 0);
            tsBuffer->clear();
            tsReady = false;
        }
    }
}

bool Client::pollMap() {
    return mapReceived;
}

void Client::getGameBufferReady(bool flag) {
    gameBufferReady = flag;
}

std::vector<char>* Client::getFillBuffer() {
    return fBuffer;
}

void Client::setController(NetworkController* controller) {
    netController = controller;
}

bool Client::init() {
    gameMap = new std::vector<int>();
    //initialize all buffers
    //receive buffer
    rcBuffer = new std::vector<char>(152);
    //to send buffer
    //buffer to fill in
    fBuffer = new std::vector<char>();
    //to send buffer
    tsBuffer = new std::vector<char>();;
    // Pack Client into void pointer for thread
    void* clientInfo = malloc(sizeof(long));
    clientInfo = (void*) this;
    gameOn = true;
    rcThread = SDL_CreateThread(receiveThread, "myThread", (void*) clientInfo);
    return true;
}

Client Client::initClient(Client c) {}
