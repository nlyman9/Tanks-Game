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
        std::vector<Packet*> recvBuffer;
        std::vector<Packet*> sendBuffer;
        const int MAX_PLAYERS = 10;
    
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

        // TODO get client ID from packet
        Packet* getPacket() {
            Packet *mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());

            return mail;
        }

        int addPacket(Packet *p) {
            int index = sendBuffer.size();
            sendBuffer.push_back(p);
            return index;
        }

        // TODO Find a way to identify the client
        void receiveFrom(int id) {
            assert(id < clients.size());
            // Recieve data from a client
            Packet *mail = clients.at(id)->receive();
        }

        bool sendTo(int id) {
            // Send to specific client
            size_t num_bytes_sent;

            if (sendBuffer.size() == 0) {
                return false;
            } else {
                Packet *mail = sendBuffer.at(0);
                sendBuffer.erase(sendBuffer.begin());
                
                num_bytes_sent = clients.at(id)->sendSocket(mail);
            }
        }

        bool broadcast() {
            // Send to all clients 
            std::cout << "Broadcasting to clients..." << std::endl;

            if (sendBuffer.size() == 0) {
                return false;
            }

            size_t num_bytes_sent;
            Packet *mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());

            for (auto client : clients) {
                num_bytes_sent = client->sendSocket(mail);
            }

            std::cout << "Broadcasted to clients!" << std::endl;

            // Free packet we are done with it!
            free(mail);
            return true;
        }

        int numClients() {
            return clients.size();
        }
    
};



class ClientConnection {
    private:
        Socket *server, *server_tcp, *server_udp;

        std::vector<Packet*> recvBuffer;
        std::vector<Packet*> sendBuffer;
        
        struct timeval* timeout;
        // Packet HEAD = Packet(PackType::INIT);
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

        Packet* getPacket() {
            if (recvBuffer.size() == 0) {
                return nullptr;
            } else {
                Packet *p = recvBuffer.at(0);
                recvBuffer.erase(recvBuffer.begin());
                return p;
            }
        }

        void addPacket(Packet *p) {
            sendBuffer.push_back(p);
        }

        void receive() {
            Packet *mail = server->receive();
            recvBuffer.push_back(mail);
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

        bool isConnected() {
            return server->isOnline();
        }
};

#endif