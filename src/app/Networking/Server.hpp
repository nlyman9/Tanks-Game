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

std::vector<char> packedMap;
class Server {
    public:
        ServerController *host;
        bool gameOn;

        Server(std::string ip, int port) {
            host = new ServerController(ip, port);
            std::cout << " Created Server Object " << std::endl;
        }

        bool bind() {
            return host->bind();
        }

        bool listen() {
            return host->listen();
        }

        bool accept() {
            if (host->accept()) {
                std::cout << "Server: accepted incoming connection" << std::endl;
                return true;
            } else {
                return false;
            }
        }
        

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

        Packet* getPacket(int id) {
            return host->getPacket(id);
        }

        Packet* receiveFromID(int id) {
            host->receiveFromClient(id);
            return host->getPacket(id);
        }

        int addPacketToBroadcast(Packet *mail) {
            return host->addPacketToBroadcast(mail);
        }

        bool sendPacketToClient(int id, Packet *mail) {
            int index = host->addPacketToSend(id, mail);
            host->sendTo(id);

            // Did we send that packet or was buffer already filled?
            if (index != 1) {
                return false;
            }
            return true;
        }

        bool broadcast(Packet *mail) {
            int index = host->addPacketToBroadcast(mail);
            host->broadcast();

            // Did we send that packet or was buffer already filled?
            if (index != 1) {
                return false;
            }
            return true;
        }

        std::vector<ClientConnection*> clients() {
            return host->getClients();
        }

        int numClients() {
            return host->numClients();
        }
};

int serverProcess();
std::vector<int>* serverMapGen();

int status;
struct addrinfo hints;
struct addrinfo *serverInfo, *p;
struct sockaddr_storage remoteaddr;
char remoteIP[INET_ADDRSTRLEN];
int listenerfd;
int sockfd;
fd_set master;      // Master of file descriptors
fd_set read_fds;    // Read fd's returned from select
fd_set client_fds;
int fdmax;          // maximum file descriptor number
int nbytes;
socklen_t addr_len;
//server buffers
// //buffer received
// static std::vector<char>* rBuffer;
// //double buffered receive
// static std::vector<char>* rcBuffer;
// //to send buffer
// static std::vector<char>* sBuffer;
// //double buffered send
// static std::vector<char>* sfBuffer;
#endif