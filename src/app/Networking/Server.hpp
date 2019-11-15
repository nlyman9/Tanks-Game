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
        ServerConnection *host;
        bool gameOn;

        Server(std::string ip, int port) {
            host = new ServerConnection(ip, port);
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

        Packet* receiveFrom(int id) {
            host->receiveFrom(id);
            return host->getPacket(id);
        }

        int addPacket(Packet *p) {
            return host->addPacket(p);
        }

        bool sendTo(int client, Packet *p) {
            int index = host->addPacket(p);
            host->sendTo(client);
            if (index > 0) {
                return false;
            }
            return true;
        }

        bool broadcast(Packet *p) {
            int index = host->addPacket(p);
            host->broadcast();
            return true;
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