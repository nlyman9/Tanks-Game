#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <chrono>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Network.hpp"
#include <SDL2/SDL_thread.h>

class Client {
  public:
    Client(std::string ip, int port) {
      server_ip = ip;
      server_port = std::to_string(port);
    };
    ~Client();
    bool init();
    Client initClient(Client c);
    std::string server_ip;
    std::string server_port;
    bool gameOn;
    SDL_Thread* rcThread;
    std::vector<int>* gameMap;
    bool pollMap();

  private:

};
#endif