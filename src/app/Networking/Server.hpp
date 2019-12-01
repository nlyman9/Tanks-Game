#ifndef SERVER_HPP
#define SERVER_HPP

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

#include "Bomb.hpp"
#include "Projectile.hpp"
#include "Multiplayer.hpp"
#include "Player.hpp"
#include "Constants.hpp"


// #include <SDL2/SDL_thread.h>

// Packed (compresed) map to send to clients
std::vector<char> packedMap;

// A server's main process
int serverProcess();

// Function to create a server
std::vector<int>* serverMapGen();

/**
 * @brief The whole wrapper for the server process
 * - handles all of the network functionality
 */
class Server {
    public:
        ServerController *host;
        bool gameOn;

        /**
         * @brief Construct a new Server object
         * 
         * @param ip - ip to bind to 
         * @param port - port to bind to
         */
        Server(std::string ip, int port) {
            host = new ServerController(ip, port);
            std::cout << " Created Server Object " << std::endl;
            gamestate = new std::vector<char>();
        }
        
        /**
         * @brief Bind the server's sockets
         * 
         * @return true - bind
         * @return false - bind
         */
        bool bind() {
            return host->bind();
        }

        /**
         * @brief Set the tcp socket to listen
         * 
         * @return true - Successful
         * @return false - Failed
         */
        bool listen() {
            return host->listen();
        }

        /**
         * @brief Accept an incoming client connection
         * 
         * @return true - Server accepted a connection
         * @return false - Server did not accept an incoming connection
         */
        bool accept() {
            if (host->accept()) {
                std::cout << "Server: accepted incoming connection" << std::endl;
                return true;
            } else {
                return false;
            }
        }
        
        /**
         * @brief Polls the clients using pselect and then
         *        Receives the packets from the pending clients
         * @warning BLOCKING Function
         * 
         * @return std::vector<ClientConnection*>* The connections that had sent data that was received
         */
        std::vector<ClientConnection*>* pollClientsAndReceive() {
            // Poll clients for messages
            std::vector<ClientConnection*> *pendingClients = host->pollClients();

            if (pendingClients == nullptr) {
                // No messages from clients!
#ifdef VERBOSE
                std::cout << "SERVER: No pending clients" << std::endl;
                fflush(stdout);
#endif
                return nullptr;
            }

            // if there are clients pending, get their data
            for (auto client : *pendingClients) {
#ifdef VERBOSE
                std::cout << "SERVER: Going to receive from " << client->id() << std::endl;
                fflush(stdout);
#endif
                host->receiveFromClient(client->id());
            }

            return pendingClients;
        }

        /**
         * @brief Get the Packet from a specific client
         * 
         * @param id - ID of the client we want to get a packet from
         * @return Packet* - The first packet from the receive buffer of the client's conenction
         */
        Packet* getPacket(int id) {
            return host->getPacket(id);
        }

        /**
         * @brief Receive a packet and return the first packet from an ID
         * @warning BLOCKING function
         * @warning Does not gaurantee to return the packe that was received
         * 
         * @param id - The client we want to receive from
         * @return Packet* The first packet from the client's receive buffer
         * @warning will return nullptr if receive buffer is empty
         */
        Packet* receiveFromID(int id) {
            host->receiveFromClient(id);
            return host->getPacket(id);
        }

        /**
         * @brief Add a packet to the broadcast buffer
         * 
         * @param mail - The packet we want to broadcast
         * @return int - The size of the broadcast buffer after appending the packet
         */
        int addPacketToBroadcast(Packet *mail) {
            return host->addPacketToBroadcast(mail);
        }

        /**
         * @brief Add a packet from one client to all the other clients' send buffers
         * 
         * @param fromClientID - The client the packet originated from  (we do not want to add to this client's send buffer - dont send back the sender)
         * @param mail - The packet we want to send to the other clients
         */
        void addPacketFromClientToClients(int fromClientID, Packet *mail) {
            for (auto client : host->getClients()) {
                if (client->id() != fromClientID) { // Dont send client packet to itself
                    client->addPacketToSend(mail);
                }
            }
        }

        /**
         * @brief Add a packet to a client's send buffer
         * 
         * @param id - The ID of the client we want to add the packet to 
         * @param mail - The packet we want to add
         */
        void addPacketToClient(int id, Packet *mail) {
            int index = host->addPacketToSend(id, mail);
        }

        /**
         * @brief 
         * @warning - Does not gaurantee to send the packet given (due to send buffer)
         * 
         * @param id - The ID of the client we want to add the packet to 
         * @param mail 
         * @return true 
         * @return false 
         */
        bool sendPacketToClient(int id, Packet *mail) {
            int index = host->addPacketToSend(id, mail);
            host->sendTo(id);

            // Did we send that packet or was buffer already filled?
            if (index != 1) {
                return false;
            }
            return true;
        }

        /**
         * @brief Broadcast a packet to all the clients
         * 
         * @param mail - The packet to broadcast
         * @return true - The packet given (argument) was sent
         * @return false - If the buffer sent a different packet (this like shouldnt happen but thats what everyone says so...)
         */
        bool broadcast(Packet *mail) {
            int index = host->addPacketToBroadcast(mail);
            host->broadcast();

            // Did we send that packet or was buffer already filled?
            if (index != 1) {
                return false;
            }
            return true;
        }

        /**
         * @brief Get the vector of clients connected in the form of Client Connections
         * 
         * @return std::vector<ClientConnection*> 
         */
        std::vector<ClientConnection*> clients() {
            return host->getClients();
        }

        /**
         * @brief Get the number of clients connected
         * 
         * @return int - The number of clients connected
         */
        int numClients() {
            return host->numClients();
        }
        void reset_lag_time(){
            lag_time = prev_lag_time;
        }
        //simulate game
        bool simulate(){
            current_time = std::chrono::system_clock::now();
            elapsed_time = current_time - previous_time;
            previous_time = current_time;
            prev_lag_time = lag_time;
            lag_time += elapsed_time.count();
            // Check that the last states of the players is not null
            //  If the server didnt receive packets on the first tick, the server would segfault
            for (auto& mail : *lastMail) {
                if (mail == nullptr)
                    return false;
            }

            //for each player
            //apply latest mail
            for(int i = 0; i < numClients(); i++){
#ifdef VERBOSE
                std::cout << "SERVER: Applying keystates to gamestate" << std::endl;
                lastMail->at(i)->printData();
                std::cout << std::endl;
#endif
                if(!applyKeyStatePacket(lastMail->at(i), players->at(i), lag_time)){
#ifdef VERBOSE
                    printf("SERVER Error: could not apply keystate packet\n");
#endif
                    return false;
                }

                //Check for collisions with projectiles and bombs here
                int count = 0;
                for (auto& projectile : projectiles) {
                    projectile->update();
                    if(projectile->isFinished()) {
                        projectiles.erase(projectiles.begin() + count);
                        count--;
                    }
                    count++;
                }

			    // Update every bomb in the game
                int bombCount = 0;
                for(auto& bomb : bombs) {
                    // Update bombs
				    bomb->update();

                    // Check if bomb is done exploding
                    if(bomb->getFinished()) {
                        bombs.erase(bombs.begin() + bombCount);
                        bombCount--;
                    }
                    bombCount++;
                }
            }
            //update everything
            if(lag_time >= MS_PER_UPDATE) {
                for(auto player: *players)
                    player->update();
                lag_time -= MS_PER_UPDATE;
            }

            return true;
        }

        bool applyKeyStatePacket(Packet* packet, Player* player, double lag_time){
            SDL_Event e;
            if(packet == nullptr)
                return false;
            if(!packet->getBody())
                return false;
            if(!(packet->getType() == PackType::KEYSTATE))
                return false;
#ifdef VERBOSE
            packet->printData();
#endif
            Uint8 *keystate = keystateify(packet->getBody());
            bool hasShot = getHasShot(packet->getBody());
            bool droppedBomb = getDroppedBomb(packet->getBody());
            if(keystate == nullptr)
                return false;
            try{
                player->getEvent(elapsed_time, &e, keystate);
                if(hasShot) {
                    Projectile* projectile = new Projectile(player->getX() + TANK_WIDTH/4, player->getY() + TANK_HEIGHT/4, player->getTurretTheta(), 1);
                    projectile->setObstacleLocations(&projectileObstacles);
                    projectiles.push_back(projectile);
                    hasShot = false;
                }                
                if(droppedBomb) {
                    Bomb* bomb = new Bomb(player->get_box(), player->getTheta());
                    bombs.push_back(bomb);
                    droppedBomb = false;
                }
            }catch (const std::exception &exc){
                // catch anything thrown within try block that derives from std::exception
                std::cout << "SERVER ERROR" << std::endl;
                std::cerr << exc.what() << std::endl;
                return false;
            }
            return true;
        }

        Uint8 *keystateify(std::vector<char>* mail){
            Uint8 *keystate;
            try{
#ifdef VERBOSE
                std::cout << "calloc keystate" << std::endl;
                std::cout << "for loop on keys" << std::endl;
#endif
                keystate = (Uint8 *) calloc(27, sizeof(Uint8));
                for (int i = 0; i < keysToCheck.size(); i++) {
                    if(i >= keysToCheck.size()) {
                        std::cout << "SERVER VECTOR OVER RUN" << std::endl;
                    } else {
                        keystate[keysToCheck[i]] = (Uint8) mail->at(i); 
                    }
                }
            }
            catch (const std::exception &exc){
                // catch anything thrown within try block that derives from std::exception
                std::cerr << exc.what();
            }
            return keystate;
        }

        bool getHasShot(std::vector<char>* mail) {
            if(mail->at(10) == '1') {
                return true;
            }
            return false;
        }

        bool getDroppedBomb(std::vector<char>* mail) {
            if(mail->at(11) == '1') {
                return true;
            }
            return false;
        }

        //set the player in the player array - make a copy
        void addPlayer(Player* newPlayer){
            players->push_back(newPlayer);
        }

        Player* getPlayer(int idx){
            return players->at(idx);
        }

        void setMail(Packet* mail, int idx){
            lastMail->at(idx) = new Packet(PackType::KEYSTATE);
            lastMail->at(idx)->appendData(mail->getBodyString());
        }

        void initPlayerAndMailLists(){
            //init player list
            players = new std::vector<Player*>();
            //init mail list
            lastMail = new std::vector<Packet*>;
            for(int i = 0; i < numClients(); i++){
                lastMail->push_back(nullptr);
            }
        }

        void setStartTime(){
            start_time = std::chrono::system_clock::now();
        }

        std::string getStartTimeStr(){
            return std::to_string(start_time.time_since_epoch().count());
        }

        Packet* getGamestatePacket(){
            Packet* gamestatepacket = new Packet(PackType::KEYFRAME);
            for(auto x : *getGamestate())
                gamestatepacket->appendData(x);
#ifdef VERBOSE
            std::cout << "Game state" << std::endl;
            gamestatepacket->printData();
#endif
            return gamestatepacket;
        }

        std::vector<char>* getGamestate(){
            gamestate->clear();
            std::vector<char>* retVal = new std::vector<char>();
            for(auto player: *players){
                std::vector<char>* playerstate = player->getState();
                gamestate->insert(gamestate->end() , playerstate->begin(), playerstate->end());
            }
            for(auto x : *gamestate)
                retVal->push_back(x);
            return retVal;
        }

        std::vector<SDL_Rect> getProjectileObstacles() {
            return projectileObstacles;
        }
    
    
    std::chrono::system_clock::time_point time_since_last_keyframe;

    private:
        //player vector - parallel to clientConnection
        std::vector<Player*>* players;
        //latest mail vector - parallel to player
        std::vector<Packet*>* lastMail;
        // Vector of simulated bombs
        std::vector<Bomb*> bombs;
        // Vector of simulated projectiles
        std::vector<Projectile*> projectiles;
        // Vector of obstacles the projectiles see
        std::vector<SDL_Rect> projectileObstacles;
        // The keys we want to check for when we add a keystate packet
        std::vector<Uint8> keysToCheck =  { SDL_SCANCODE_W, SDL_SCANCODE_A, 
                                    SDL_SCANCODE_S, SDL_SCANCODE_D}; 
        std::chrono::duration<double, std::ratio<1, 1000>> elapsed_time;
        std::chrono::system_clock::time_point previous_time;
        std::chrono::system_clock::time_point current_time;
        double lag_time = 0;
        double prev_lag_time;
        bool ready_to_update = false;
        std::vector<char>* gamestate;

        std::chrono::system_clock::time_point start_time;
};

#endif