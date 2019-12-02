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
                                        SDL_SCANCODE_S, SDL_SCANCODE_D}; 
    const int PLAYER_STATE_VALUES = 6;

    long startTime = 0;

  public:
    int id;
    bool win = false;
    bool gameOver = false;

    // Network
    ClientController *server;

    // Game map
    std::vector<int> gameMap;
    std::vector<int> initData;
    bool initDataReceived = false;
    bool mapReceived = false;

    // Game Player
    std::vector<Uint8*> playerKeystates;
    std::vector<int> playerTurretThetas;
    std::vector<bool> playerShot;
    std::vector<bool> playerBomb;
    // Game state vector?
    std::vector<char>* gameState;
    std::vector<std::vector<int>>* playerStates;
    bool stateSet = false;
    // Game 
    bool gameOn = false;
    bool startGame = false;

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
      playerTurretThetas.push_back(0);
      playerShot.push_back(false);
      playerBomb.push_back(false);
      playerStates = new std::vector<std::vector<int>>();
      playerStates->resize(2, std::vector<int>(PLAYER_STATE_VALUES, 0));
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
#ifdef VERBOSE
        // It should be all zeroes or the last keystate, not Null
        std::cerr << "CLIENT-NET: No keystate for player " << id << "!!" << std::endl;
        fflush(stdout);
#endif
        return nullptr;
      }
      // std::cout << "Returning player " << id  << "'s keystates... " << std::endl;
      return (const Uint8*) playerKeystates.at(id);
    }

    /**
     * @brief Get the Turret angle of the networked player
     * 
     * @param id - Which online player
     * @return int - Their turret's angle
     */
    int getTurretTheta(int id) {
      assert(id < playerTurretThetas.size());

      return playerTurretThetas.at(id);
    }

    /**
     * @brief Get the boolean if the networked player shot
     * 
     * @param id - Which network player
     * @return true - They shot
     * @return false - They didn't shoot
     */
    bool getPlayerShot(int id) {
      assert(id < playerShot.size());

      return playerShot.at(id);
    }

    /**
     * @brief Get the boolean if the networked player dropped a bomb
     * 
     * @param id - Which network player
     * @return true - They dropped a bomb
     * @return false - They didn't drop a bomb
     */
    bool getPlayerBomb(int id) {
      assert(id < playerBomb.size());

      return playerBomb.at(id);
    }

    /**
     * @brief Add a keystate received from the server to keystates vector 
     * 
     * @param id - The ID of the client it was from
     * @param charKeyStates - The keystates 
     * @param turretTheta - The player's turret angle from the packet
     * @param hasShot - the boolean if the player shot or not
     */
    void addNetworkKeyState(int id, std::vector<char> *charKeyStates, int turretTheta, bool hasShot, bool placedBomb) {
      assert(id < playerKeystates.size());
      assert(id < playerTurretThetas.size());
#ifdef VERBOSE
      std::cout << "Adding keystate from network (client " << id << ") - ";
      fflush(stdout);
#endif
      // This is a little hacky, but I am trying simulate SDL_getKeyboardState
      Uint8 *keystate = playerKeystates.at(id);

      for (int i = 0; i < keysToCheck.size(); i++) {
        if(i >= keysToCheck.size()) {
          std::cout << "CLIENT VECTOR OVER RUN" << std::endl;
        } else {
          keystate[keysToCheck[i]] = (Uint8) charKeyStates->at(i); 
        }
      }

      playerTurretThetas[id] = turretTheta;
      playerShot[id] = hasShot;
      playerBomb[id] = placedBomb;

#ifdef VERBOSE
      printf("Keystate[W] = %d  -- ", keystate[SDL_SCANCODE_W]);
      printf("Turret = %d\n", playerTurretThetas.at(id));
			fflush(stdout);
#endif
    }

    /**
     * @brief Add a keystate from the local client we want to prepare to send
     *  - Adds the packet to the clients send buffer
     * 
     * @param keystates - Keystates from the local client
     */
    void addLocalKeyState(const Uint8 *keystates, int turretTheta, bool hasShot, bool placedBomb) {
      Packet *mail = new Packet(PackType::KEYSTATE);
      std::vector<char> charKeyStates;

      for (auto key : keysToCheck) {
        charKeyStates.push_back((char)keystates[key]);
      }
      charKeyStates.push_back(' '); // use space to mark end of keystates
      mail->appendData(charKeyStates);
      mail->appendData(turretTheta);
      mail->appendData(' ');
      mail->appendData(hasShot);
      mail->appendData(placedBomb);
#ifdef VERBOSE
      std::cout << "Sending keystate - ";
      mail->printData();
      fflush(stdout);
#endif      
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
     * @brief Poll if we have received the initalization data
     * 
     * @return true - We have!
     * @return false - We havent :(
     */
    bool pollInitData() {
      return initDataReceived;
    }


    /**
     * @brief The client network process 
     * This is invoked on a new thread
     * 
     * @param data - The client object
     * @return int 
     */
    static int clientProcess(void *data);

    void setGameState(std::vector<char>* state){
#ifdef VERBOSE
      std::cout << "Setting game state" << std::endl;
#endif
      int offset = 0;
      int player = 0;
      while(player < 2){ //while less than num players
        for(int i = 0; i < PLAYER_STATE_VALUES; i++){ //for each value in the gamestate (players have 5 lives, x, y, xvel, yvel)
          std::string value = "";
          while(state->at(offset) != 32){
            value += state->at(offset);
            offset++;
          }
          offset++;
          playerStates->at(player).at(i) = stoi(value);
        }
#ifdef VERBOSE
        std::cout << "Printing player state of player : " << player << " state : ";
        for(auto x: playerStates->at(player))
          std::cout << x  << " ";
        std::cout << std::endl;
#endif
        player++;
      }
      //projectile stuff here - maybe a for x in state loop?

      stateSet = true;
    }

    void setStartTime(std::string time) {
        startTime = stol(time);
    }

    long getStartTime() {
      return startTime;
    }

};
#endif