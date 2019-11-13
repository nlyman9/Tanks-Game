#ifndef MULTIPLAYER_HPP
#define MULTIPLAYER_HPP

#include <SDL2/SDL_thread.h>
#include <assert.h> 

#include "Header.hpp"
#include "Packet.hpp"
#include "Socket.hpp"

class ServerConnection {
    private:
        Socket *listener, *listener_tcp, *listener_udp;
        std::vector<Socket*> clients;
        std::vector<Packet> recvBuffer;
        std::vector<Packet> sendBuffer;
        const int MAX_PLAYERS = 10;
        Header HEAD = Header("TYPE", std::to_string((int)5));
    
    public:
        ServerConnection(std::string ip, int port) {
            // Create tcp socket
            listener_tcp = new Socket(Protocol::TCP, ip, port);
            listener_udp = new Socket(Protocol::UDP, ip, port+1);
            // Set base socket to tcp socket
            listener = listener_tcp;
        }

        bool bind() {
            // Set up sockets to connect online
            if (listener_tcp->bindSocket() && listener_udp->bindSocket()) {
                std::cout << "SUCCESS: Binded TCP and UDP connections" << std::endl;
                return true;
            } else {
                return false;
            }
        }

        bool listen() {
            if (listener_tcp->listenSocket(MAX_PLAYERS)) {
                std::cout << "SUCCESS: Listening on TCP socket" << std::endl;
                return true;
            } else {
                std::cerr << "FAIL: Server can't listen on TCP socket" << std::endl;
                return false;
            }
        }

        bool accept() {
            // Accept client connection on TCP and create approriate client connection
            std::cout << "Server: Waiting to accept incoming connections... " << std::endl;
            auto client = listener_tcp->acceptSocket();
            if (client == nullptr) {
                std::cerr << "Failed to accept client connection" << std::endl;
                return false;
            } else {
                clients.push_back(client);
                std::cout << "New client: " << client->ip() << " on port " << client->port() << std::endl;
                return true;
            }
        }

        void recv() {
            // Recieve data from a client
        }

        void addPacket(Packet p) {
            sendBuffer.push_back(p);
        }

        void sendTo(int fd) {
            // Send to specific client
            size_t num_bytes_sent;
            Packet mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());

            for (auto client : clients) {
                if (client->fd() == fd) {
                    num_bytes_sent = send(client->fd(), mail.data(), mail.size(), 0);
                    break;
                }
            }
        }

        void broadcast() {
            // Send to all clients 
        }

        int numClients() {
            return clients.size();
        }
    
};



class ClientConnection {
    private:
        Socket *server, *server_tcp, *server_udp;

        std::vector<Packet> recvBuffer;
        std::vector<Packet> sendBuffer;
        
        struct timeval* timeout;
        Header HEAD = Header("TYPE", std::to_string((int)5));
    public:
        ClientConnection(std::string ip, int port) {
            // Create tcp socket
            server_tcp = new Socket(Protocol::TCP, ip, port);
            server_udp = new Socket(Protocol::UDP, ip, port+1);
            // Set base server to tcp socket
            server = server_tcp;

            timeout = (timeval*) calloc(1, sizeof(struct timeval));
            timeout->tv_sec = 0;
            timeout->tv_usec = 0;
        };

        Socket* currSocket() {
            return server;
        }

        void swapSocket() {
            if (server == server_tcp)
                server = server_udp;
            else
                server = server_tcp;
        }

        bool connect() {
            if (server_tcp->connectSocket()) {
                std::cout << "CLIENT SUCCESS: Connected to server over TCP" << std::endl;
                return true;
            } else {
                std::cout << "CLIENT ERROR: Failed to connect to server over TCP" << std::endl;
                return false;
            }
        }

        void recieve() {
            assert(server->isOnline());

            // Recieve from connection, add to buffer.
            char rawbuffer[sizeof(HEAD)];

            // Recieve initial header
            if (server == server_tcp) {
                // Use tcp
                int numbytes = recv(server->fd(), rawbuffer, sizeof(HEAD), 0);
                if (numbytes == -1) {
                    std::cerr << "CLIENT: read error: " << strerror(errno) <<  std::endl;
                }
            } else {
                // Use udp
            }

            Header head = (Header) rawbuffer; 
            std::cout << "HOLY SHIT THE TYPE IS " << (int) PackType(5) << std::endl;
        }

        
        bool send() {
            assert(server->isOnline());

            // Check if we have something to send.
            if (sendBuffer.size() == 0) {
                return false; // NO DATA TO SEND!?
            }

            // Send first packet in vector to server
            auto mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());



            return true;
        }

        void addPacketToSend(Packet p) {
            sendBuffer.push_back(p);
        }

        bool isConnected() {
            return server->isOnline();
        }
};

#endif