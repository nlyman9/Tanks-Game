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
#include "Multiplayer.hpp"

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
                std::cout << "SERVER: No pending clients" << std::endl;
                fflush(stdout);
                return nullptr;
            }

            // if there are clients pending, get their data
            for (auto client : *pendingClients) {
                std::cout << "SERVER: Going to receive from " << client->id() << std::endl;
                fflush(stdout);
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
};

#endif
