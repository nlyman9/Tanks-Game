#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Network.hpp"
#include <SDL2/SDL_thread.h>

class Client {
  public:
    Client() {};
    ~Client();
    bool init();
    Client initClient(Client c);
    //will have to write getters and setters but just testing for now
    Network* network;
    //to send buffer
    char* tsBuffer;
    //buffer to fill in
    std::vector<char>* fBuffer;
    //receive buffer
    char* rcBuffer;
    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int sockfd;
    fd_set master;      // Master of file descriptors
    fd_set read_fds;    // Read fd's returned from select
    int fdmax;          // maximym file descriptor number
    int nbytes;
    bool gameOn;
    SDL_Thread* rcThread;
  private:

};
#endif