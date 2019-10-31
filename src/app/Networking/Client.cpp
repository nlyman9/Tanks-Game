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

int receiveThread(void* data) {

    // Unpack data in the recvBuffer and Client object
    char* recvBuffer = (char*) data; 
    data = static_cast<char*>(data) + 1;
    Client* crClient = (Client*) data;
    std::cout << "Client info: " << std::endl;
    std::cout << crClient->status << std::endl;
    std::cout << "Receive thread created!" << std::endl;

    // Game loop
    while(crClient->gameOn) {
        crClient->read_fds = crClient->master;
        // Check for any response from serrver
        if (select(crClient->fdmax+1, &crClient->read_fds, NULL, NULL, NULL) == -1) {
            std::cout << "Select error" << std::endl;
            exit(4);
        }
        // If from server
        if (FD_ISSET(crClient->sockfd, &crClient->read_fds)) {
            crClient->nbytes = recv(crClient->sockfd, recvBuffer, 152, 0);
            if (crClient->nbytes <= 0) {
                std::cout << "Connection closing" << std::endl;
                close(crClient->sockfd);
                exit(10);
            } else {
                //recieved data
                std::cout << "receiving map data..." << std::endl;
                std::vector<char> test(recvBuffer, recvBuffer + crClient->nbytes);
                std::vector<int> map;
                if(!mapReceived){
                   crClient->network->unpack(&test, &map, 3);
                   crClient->gameMap = map;
                   mapReceived = true;
                }
            }
        }
    }
}

bool Client::pollMap() {
    return mapReceived;
}

bool Client::init() {
    // Innitialize sets to zero
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    network = new Network();
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP

    // Get address info of server
    std::cout << "Connecting to " << server_ip << " on port " << server_port << std::endl;
    if ((status = getaddrinfo(server_ip.c_str(), server_port.c_str(), &hints, &serverInfo)) != 0) {
        std::cout << "Failed to get address info" << std::endl;
        exit(4);
    }

    // Create a socket based on server info 
    sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    // Connect to server
    while (connect(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        std::cout << "Waiting for connection..." << std::endl;
        sleep(1);
    }
    std::cout << "Connected" << std::endl;

    // Add server fd to master
    FD_SET(sockfd, &master);
    // Add stdin fd to master
    FD_SET(STDIN_FILENO, &master);

    fdmax = sockfd;
    //initialize all buffers
    //receive buffer
    rcBuffer = (char*) calloc(152, sizeof(char)); 
    //to send buffer
    tsBuffer = (char*) calloc(152, sizeof(char)); 
    //buffer to fill in
    fBuffer = new std::vector<char>();

    // Pack recvBuffer and Client into void pointer for thread
    void* clientInfo = malloc(sizeof(void)*2);
    clientInfo = (void*) rcBuffer;
    clientInfo = static_cast<char*>(clientInfo) + 1;
    clientInfo = (void*) this;
    clientInfo = static_cast<char*>(clientInfo) - 1;
    gameOn = true;
    rcThread = SDL_CreateThread(receiveThread, "myThread", (void*) clientInfo);
    return true;
}

Client Client::initClient(Client c) {}
