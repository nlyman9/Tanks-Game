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
#include <SDL2/SDL.h>

/**
 * @brief The whole wrapper for the local client to connect to a remote server
 * - This class is the bridge between the local game process the server network connection
 *  + More specifically, The local game process and the network client process that communicates with the remote server.
 */
class Client {
  private:
    // The keys we want to check for when we add a keystate packet
    std::vector<Uint8> keysToCheck =  { SDL_SCANCODE_W, SDL_SCANCODE_A, 
                                        SDL_SCANCODE_S, SDL_SCANCODE_D,
                                        (Uint8) SDL_MOUSEBUTTONDOWN }; 
  public:
    // Network
    ClientController *server;

    // Game map
    std::vector<int> gameMap;
    bool mapReceived;

    // Game Player
    std::vector<Uint8*> playerKeystates;
    // Game state vector?

    // Game 
    bool gameOn;
    bool startGame;

    SDL_Thread* rcThread;

    /**
     * @brief Construct a new Client wuth an IP and Port
     * 
     * @param ip - remote IP
     * @param port - Port to use
     */
    Client(std::string ip, int port) {
      server = new ClientController(ip, port);

      // This is a little hacky bbut im trying to create a pseudo SDL keystate so Player can use it 
      // It works thus far
      Uint8 *player2Keystats = (Uint8 *) calloc(27, sizeof(Uint8));
      playerKeystates.push_back(player2Keystats);
    };

    ~Client() {
      delete server;
    }

    /**
     * @brief Initialize the thread for the client 
     * 
     * @return true - Worked
     * @return false - Failed
     */
    bool init();

    /**
     * @brief Connect to the server with the client's clientController 
     * 
     * @return true - Successful
     * @return false - Failed
     */
    bool connect() {
      return server->connect();
    }

    /**
     * @brief Check is the clientController's sockets are connected to the server
     * 
     * @return true - Yes
     * @return false - No
     */
    bool isConnected() {
      return server->isConnected();
    }

    /**
     * @brief Set the Tickrate of the client's connection to the server
     * 
     * @param tickrate - The rate in seconds 
     */
    void setSocketTickrate(int tickrate) { 
      return server->setSocketTimeout(tickrate);
    }

    /**
     * @brief Receive a packet from the server and return a packet from the receive buffer 
     * @warning Does not gaurantee to return the packet it received from the server
     * @warning BLOCKING function
     * 
     * @return Packet* The first packet from the receive buffer
     */
    Packet* receiveAndGet() {
      server->receive();
      return server->getPacket();
    }

    /**
     * @brief Get the Key State from the vector of keystates.
     * 
     * @param id - The ID (index) of the network players. Use the same index for the same networked players
     * @return const Uint8* A pseudo SDL keystate scan code
     * @warning this is not a fully functional SDL keystate (The array you receive from SDL_getKeyboardState())
     * @warning This may retur nullptr
     */
    const Uint8* getKeyState(int id) {
      assert(id < playerKeystates.size());

      if (playerKeystates.at(id) == nullptr) {
        // It should be all zeroes or the last keystate, not Null
        std::cerr << "CLIENT-NET: No keystate for player " << id << "!!" << std::endl;
        fflush(stdout);
        return nullptr;
      }
      // std::cout << "Returning player " << id  << "'s keystates... " << std::endl;
      return (const Uint8*) playerKeystates.at(id);
    }

    /**
     * @brief Add a keystate received from the server to keystates vector 
     * 
     * @param id - The ID of the client it was from
     * @param charKeyStates - The keystates 
     */
    void addNetworkKeyState(int id, std::vector<char> *charKeyStates) {
      assert(id < playerKeystates.size());

      std::cout << "Adding keystate from network (client " << id << ") - ";
      fflush(stdout);

      // This is a little hacky, but I am trying simulate SDL_getKeyboardState
      Uint8 *keystate = playerKeystates.at(id);

      for (int i = 0; i < keysToCheck.size() - 1; i++) {
        keystate[keysToCheck[i]] = (Uint8) charKeyStates->at(i); 
      }

      printf("Keystate[W] = %d \n", keystate[SDL_SCANCODE_W]);
			fflush(stdout);
    }

    /**
     * @brief Add a keystate from the local client we want to prepare to send
     *  - Adds the packet to the clients send buffer
     * 
     * @param keystates - Keystates from the local client
     */
    void addLocalKeyState(const Uint8 *keystates) {
      Packet *mail = new Packet(PackType::KEYSTATE);
      std::vector<char> charKeyStates;

      for (auto key : keysToCheck) {
        charKeyStates.push_back((char)keystates[key]);
      }

      mail->appendData(charKeyStates);
      std::cout << "Sending keystate - ";
      mail->printData();
      fflush(stdout);
      
      server->addPacket(mail);
    }

    /**
     * @brief Send a packet from the send buffer to the server
     * 
     * @return true - Sent a packet!
     * @return false - Nothing to send
     */
    bool send() {
      return server->sendPacket();
    }

    /**
     * @brief Poll if we have received the map
     * 
     * @return true - We have!
     * @return false - We havent :(
     */
    bool pollMap() {
      return mapReceived;
    }

    /**
     * @brief The client network process 
     * This is invoked on a new thread
     * 
     * @param data - The client object
     * @return int 
     */
    static int clientProcess(void *data);

};
#endif