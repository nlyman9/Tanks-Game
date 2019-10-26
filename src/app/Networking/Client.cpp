/*
    Originally created by Jakob - Modified by Adam 
    Modifications:
    test to create a map on a server then pack it and send it over as bit data
    and unpack render on client
*/
#include "Client.hpp"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL.h>
bool mapRecieved = false;
int receiveThread( void* data){
    char* rcBuffer = (char*) data; 
    data = static_cast<char*>(data) + 1;
    Client* crClient = (Client*) data;
    std::cout << "Client info: " << std::endl;
    std::cout << crClient->status << std::endl;
    std::cout << "Receive thread created!" << std::endl;
    while(crClient->gameOn) {
        std::cout << "looping in thread" << std::endl;
        sleep(1);
        /*crClient->read_fds = crClient->master;
        // Check for any response from serrver
        std::cout << "select" << std::endl;
        if (select(crClient->fdmax+1, &crClient->read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }
        // If from server
        std::cout << "FD IS SET" << std::endl;
        if (FD_ISSET(crClient->sockfd, &crClient->read_fds)) {
            crClient->nbytes = recv(crClient->sockfd, rcBuffer, 151, 0);
            std::cout << "nbytes" << std::endl;
            if (crClient->nbytes <= 0) {
                std::cout << "Connection closing" << std::endl;
                close(crClient->sockfd);
                exit(10);
            } else {
                //recieved data
                std::cout << "receiving" << std::endl;
                std::vector<char> test(rcBuffer, rcBuffer + (sizeof(rcBuffer)/sizeof(rcBuffer[0])));
                std::cout << test.size() << std::endl;
                std::vector<int> map;
               // if(!mapRecieved){
                //    crClient->network->unpack(&test, &map, 2);
                //}
            }
        }*/
    }
}
bool Client::init() {
    std::cout << "initting client" << std::endl;
    // Innitialize sets to zero
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    network = new Network();
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP

    // Get address info of server
    if ((status = getaddrinfo("127.0.0.1", "8123", &hints, &serverInfo)) != 0) {
        perror("failed to get address info");
        exit(4);
    }

    // Create a socket based on server info 
    sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    // Connect to server
    std::cout << "connecting" << std::endl;
    while (connect(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        //perror("failed to connect");

    }
    std::cout << "connected" << std::endl;

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
    void* pointers = malloc(sizeof(void)*2);
    pointers = (void*) rcBuffer;
    pointers = static_cast<char*>(pointers) + 1;
    pointers = (void*) this;
    pointers = static_cast<char*>(pointers) - 1;
    gameOn = true;
    rcThread = SDL_CreateThread(receiveThread, "myThread", (void*) pointers);
    //SDL_Thread* sThread =  SDL_CreateThread(sendThread, (void*) this);
    return true;
}
Client Client::initClient(Client c){
    
}

