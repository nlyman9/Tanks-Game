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
        Packet HEAD = Packet(PackType::INIT);
    
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

        int addPacket(Packet p) {
            int index = sendBuffer.size();
            sendBuffer.push_back(p);
            return index;
        }

        void sendTo(int index) {
            // Send to specific client
            size_t num_bytes_sent;
            Packet mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());
            
            std::cout << "Sending to client " << index << "..." << std::endl;
            fflush(stdout);
            num_bytes_sent = send(clients.at(index)->fd(), mail.data(), mail.size(), 0);
            std::cout << "Sent packet of size: " << mail.size() << " with data = " << mail.data() << std::endl;
        }

        void broadcast() {
            // Send to all clients 
            std::cout << "Broadcasting to clients..." << std::endl;

            size_t num_bytes_sent;
            Packet mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());

            for (auto client : clients) {
                num_bytes_sent = send(client->fd(), mail.data(), mail.size(), 0);
            }

            std::cout << "Broadcasted to clients!" << std::endl;

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
        Packet HEAD = Packet(PackType::INIT);
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

        void receive() {
            assert(server->isOnline());

            // Recieve from connection, add to buffer.
            char headBuffer[HEAD.size()];
            int num_bytes;

            // Recieve initial header
            if (server == server_tcp) {
                // Use tcp
                num_bytes = recv(server->fd(), headBuffer, HEAD.size(), 0);
                if (num_bytes == -1) {
                    std::cerr << "CLIENT: read header error: " << strerror(errno) <<  std::endl;
                }
            } else {
                // Use udp
            }

            if (num_bytes == 0) {
                return;
            }
            // else 

            // Two ways of creating header from data
            // Casting works as so
            // Header head = (Header) rawbuffer; 
            // Or using constructor
            Header size_header = Header(headBuffer); 

            std::cout << "THE HEADER IS " << size_header.data() << std::endl;
            std::cout << "THE TYPE IS " << size_header.getValue() << std::endl;
            std::cout << "THE SIZE IS " << size_header.size() << std::endl;
            fflush(stdout);

            Header type_header = Header(&headBuffer[size_header.size()+1]);
            std::cout << "HOLY SHIT THE HEADER IS " << type_header.data() << std::endl;
            std::cout << "HOLY SHIT THE TYPE IS " << type_header.getValue() << std::endl;

            // Create packet and add to recvBuffer
            int packet_size = std::stoi(size_header.getValue()) - HEAD.size();
            std::cout << "Packet size remaining is " << packet_size << std::endl;

            char dataBuffer[packet_size];
            num_bytes = 0;
            if (packet_size > 0) {
                // Receive rest of data
                if (server == server_tcp) {
                    // Use tcp
                    num_bytes = recv(server->fd(), dataBuffer, packet_size, 0);
                    if (num_bytes == -1) {
                        std::cerr << "CLIENT: read packet error: " << strerror(errno) <<  std::endl;
                    }
                } else {
                    // Use udp
                }
            }

            std::cout << "Bytes received  " << num_bytes << std::endl;
            // This buffer should just be the data segments of the packet
            Packet mail = Packet(size_header, type_header, dataBuffer, packet_size);
            recvBuffer.push_back(mail);
        }

        Packet* getPacket() {
            if (recvBuffer.size() == 0) {
                return nullptr;
            } else {
                Packet *p = new Packet(recvBuffer.at(0));
                recvBuffer.erase(recvBuffer.begin());
                return p;
            }
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