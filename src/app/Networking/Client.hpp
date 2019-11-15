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
#include "Multiplayer.hpp"

static bool gameBufferReady = false; 
//buffer received
// static std::vector<char>* rcBuffer;
// //to send buffer
// static std::vector<char>* tsBuffer;
// //buffer to fill in
// static std::vector<char>* fBuffer;
//keystate double buffer
static std::vector<Uint8*>* keystates;

class Client {
  public:
    // Network
    ClientConnection *server;

    // Game map
    std::vector<int> gameMap;
    bool mapReceived;

    // Game 
    bool gameOn;
    bool startGame;

    SDL_Thread* rcThread;

    Client(std::string ip, int port) {
      server = new ClientConnection(ip, port);
    };

    ~Client() {
      delete server;
    }

    bool init();
    Client initClient(Client c);

    bool connect() {
      return server->connect();
    }

    bool isConnected() {
      return server->isConnected();
    }

    Packet* receive() {
      server->receive();
      return server->getPacket();
    }

    bool pollMap() {
      return mapReceived;
    }

    static int clientThread(void *data);
    // void getGameBufferReady(bool flag);

    // std::vector<char>* getFillBuffer();
    // Uint8* pollKeystate();

  private:
};
#endif