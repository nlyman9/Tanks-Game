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
    
    public:
        ServerConnection(std::string ip, int port) {
            // Create tcp socket
            listener_tcp = new Socket(Protocol::TCP, ip, port);
            listener_udp = new Socket(Protocol::UDP, ip, port);
            // Set base socket to tcp socket
            listener = listener_tcp;
        }

        void recv() {
            // Recieve data from a client
        }

        void sendTo() {
            // Send to specific client
        }

        void broadcast() {
            // Send to all clients 
        }
    
};



class ClientConnection {
    private:
        Socket *server, *server_tcp, *server_udp;

        std::vector<Packet> recvBuffer;
        std::vector<Packet> sendBuffer;
        
        struct timeval* timeout;
    public:
        ClientConnection(std::string ip, int port) {
            // Create tcp socket
            server_tcp = new Socket(Protocol::TCP, ip, port);
            server_udp = new Socket(Protocol::UDP, ip, port);
            // Set base server to tcp socket
            server = server_tcp;

            (timeval*) calloc(1, sizeof(struct timeval));
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

        void addPacket(Packet p) {
            sendBuffer.push_back(p);
        }


};

#endif