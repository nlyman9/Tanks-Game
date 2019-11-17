#ifndef MULTIPLAYER_HPP
#define MULTIPLAYER_HPP

#include <SDL2/SDL_thread.h>
#include <assert.h> 
#include <unistd.h>
#include <signal.h>
#include <list>

#include "Header.hpp"
#include "Packet.hpp"
#include "Socket.hpp"

class ClientConnection {
    private:
        Socket* clientSocket;
        std::list<Packet *> recvBuffer;
        std::list<Packet *> sendBuffer;
        int clientID; // Index in the server's vector of clients 
    public:
        ClientConnection(Socket * sock, int id) {
            clientSocket = sock;
            clientID = id;
        }

        void receive() {
            Packet *mail = clientSocket->receive();

            if (mail != nullptr) {
                recvBuffer.push_back(mail);
            } else {
                // Packet was probably invalid
                std::cerr << "SERVER: Packet from client was invalid!" << std::endl;
            }
        }

        Packet* getPacket() {
            if (recvBuffer.size() == 0) {
                return nullptr;
            }

            Packet *mail = recvBuffer.front();
            recvBuffer.pop_front();
            
            std::cout << "Size of recv buffer is === " << recvBuffer.size() << std::endl;

            return mail;
        }

        int addPacketToSend(Packet* mail) {
            assert(mail != nullptr);

            sendBuffer.push_back(mail);
            return sendBuffer.size();
        }

        bool sendFromBuffer() {
            if (sendBuffer.size() == 0) {
                // Nothing to send
                return false;
            } else {
                Packet *mail = sendBuffer.front();
                sendBuffer.pop_front();

                clientSocket->sendSocket(mail);

                delete mail;
                return true;
            }
        }


        /**
         * @brief Sends the given packet 
         *   - Usually used when the server needs to broadcast keyframes
         * 
         * @warning - This overrides/skips the send buffer
         * @warning - Does not delete/free the packet after sending
         * 
         * @param mail - The packet want to send 
         */
        void sendPacket(Packet *mail) {
            assert(mail != nullptr);
            clientSocket->sendSocket(mail);
        }


        void setID(int id) {
            this->clientID = id;
        }

        int id() {
            return clientID;
        }

        /**
         * @brief Get the file descriptor of the socket 
         * 
         * @return int - the file descriptor
         */
        int fd() {
            return clientSocket->fd();
        }
};

// TODO - Properly handle when a client disconnects
//          + Poll for client, if number of bytes from receive == 0, remove client.
class ServerController {
    private:
        Socket *listener, *listener_tcp, *listener_udp;
        std::vector<ClientConnection *> clients;
        std::list<Packet *> broadcastBuffer;


        const int MAX_PLAYERS = 10;

        // Select FDs
        fd_set client_fds;
        int fdmax;

        // Timeout / Rate the server will run at
        const int TICKS_PER_SECOND = 30;
        const long NANO_PER_SECOND = 1e+9;

        // Set timeout based on constants
        // Nanoseconds per tick = NANO_PER_SECOND / TICKS_PER_SECOND
        const struct timespec poll_timeout {
            1,                                 // tv_sec
            0//NANO_PER_SECOND / TICKS_PER_SECOND // tv_nsec 
        };
    
    public:
        ServerController(std::string ip, int port) {
            // Create tcp socket
            listener_tcp = new Socket(Protocol::TCP, ip, port);
            listener_udp = new Socket(Protocol::UDP, ip, port+1);
            // Set base socket to tcp socket
            listener = listener_tcp;
            fdmax = -1;
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
                // Add client
                clients.push_back(new ClientConnection(client, clients.size()));

                // Add client's File descriptor to set
                FD_SET(client->fd(), &client_fds);
                if (client->fd() > fdmax) {
                    std::cout << "New fdmax!!! " << fdmax << " -> " << client->fd() << std::endl;
                    fdmax = client->fd();
                }

                std::cout << "New client: " << client->ip() << " on port " << client->port() << std::endl;
                return true;
            }
        }
    
        std::vector<ClientConnection*>* pollClients() {
            // Create a temp fd_set
            fd_set read_fds = client_fds;

            // wait for their info with the defined timeout (for now 1/30th of a second)
            int numberOfPendingClients = pselect(fdmax+1, &read_fds, nullptr, nullptr, &poll_timeout, nullptr);
            if (numberOfPendingClients < 0) {
                std::cout << "SELECT ERROR: " << std::endl;
                perror("Error: ");
            }
            if (numberOfPendingClients == EBADF) {
                std::cout << "SELECT ERROR: BAD FD " << std::endl;
                fflush(stdout);
                return nullptr;
            }

            std::cout << "Number of clients from pselect -- " << numberOfPendingClients << std::endl;
            fflush(stdout);

            // fd_set's will be modified with the clients that have pending messages, return that list
            if (numberOfPendingClients == 0) {
                return nullptr;
            } else {
                std::vector<ClientConnection*> *pendingClients = new std::vector<ClientConnection*>();
                for (auto client : clients) {
                    if (FD_ISSET(client->fd(), &read_fds)) {
                        pendingClients->push_back(client);
                    }
                }

                return pendingClients;
            }
        }

        Packet* getPacket(int id) {
            assert(id <  numClients());

            Packet* mail = clients[id]->getPacket();

            return mail;
        }

        int addPacketToBroadcast(Packet *p) {
            broadcastBuffer.push_back(p);
            return broadcastBuffer.size();
        }

        int addPacketToSend(int id, Packet *mail) {
            assert(id < numClients());
            clients[id]->addPacketToSend(mail);
        }
        
        void receiveFromClient(int id) {
            assert(listener->isOnline());
            assert(id < numClients());

            // Recieve data from a client
            clients[id]->receive();
        }

        bool sendTo(int id) {
            assert(id < numClients());
            // Send to specific client
            clients[id]->sendFromBuffer();
        }

        bool broadcast() {
            if (broadcastBuffer.size() == 0) {
                // Nothing to broadcast
                return false;
            }

            // Send to all clients 
            std::cout << "Broadcasting to clients..." << std::endl;
            Packet *mail =  broadcastBuffer.front();
            broadcastBuffer.pop_front();

            for (auto client : clients) {
                client->sendPacket(mail);
            }

            std::cout << "Broadcasted to clients!" << std::endl;

            // Free packet we are done with it!
            delete mail;
            return true;
        }

        std::vector<ClientConnection*> getClients() {
            return clients;
        }

        int getClientsFD(int id) {
            assert(id < clients.size());
            return clients.at(id)->fd();
        }

        int getClientsID(int fd) {
            assert(fd < clients.size());

            for (int i = 0; i < clients.size(); i++) {
                if (clients.at(i)->fd() == fd) {
                    return i;
                }
            }

            return -1;
        }
        int numClients() {
            return clients.size();
        }
    
};



class ClientController {
    private:
        Socket *server, *server_tcp, *server_udp;

        std::list<Packet*> recvBuffer;
        std::list<Packet*> sendBuffer;
        
        struct timeval* timeout;
    public:
        ClientController(std::string ip, int port) {
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
                Packet *p = recvBuffer.front();
                recvBuffer.pop_front();
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

        bool sendPacket() {
            assert(server->isOnline());

            // Check if we have something to send.
            if (sendBuffer.size() == 0) {
                return false; // NO DATA TO SEND!?
            }

            // Send first packet in vector to server
            auto mail = sendBuffer.front();
            sendBuffer.pop_front();
            server->sendSocket(mail);

            delete mail;
            return true;
        }

        bool isConnected() {
            return server->isOnline();
        }
};

#endif