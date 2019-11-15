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

class ServerConnection {
    private:
        Socket *listener, *listener_tcp, *listener_udp;
        std::vector<Socket*> clients;
        std::vector<std::list<Packet *> *> recvBuffer;
        std::vector<Packet*> sendBuffer;
        const int MAX_PLAYERS = 10;

        // Select FDs
        fd_set client_fds;
        int fdmax;

        // Timeout / Rate the server will run at
        const int TICKS_PER_SECOND = 1;
        const long NANO_PER_SECOND = 1e+9;

        // Set timeout based on constants
        // Nanoseconds per tick = NANO_PER_SECOND / TICKS_PER_SECOND
        const struct timespec poll_timeout {
            0,                                 // tv_sec
            NANO_PER_SECOND / TICKS_PER_SECOND // tv_nsec 
        };
    
    public:
        ServerConnection(std::string ip, int port) {
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
                clients.push_back(client);
                
                // Add a recvBuffer for client
                recvBuffer.push_back(new std::list<Packet *>);

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

        std::vector<int>* pollClients() {
            // Create a temp fd_set
            fd_set read_fds = client_fds;

            std::cout << "Polling clients... -- ";
            for (auto client : clients) {
                if (FD_ISSET(client->fd(), &read_fds))
                    std::cout << client->fd() << " ";
            }
            std::cout << "Out of clients: ";
            for (auto client : clients) {
                std::cout << client->fd() << " ";
            }
            std::cout << std::endl;
            fflush(stdout);


            // wait for their info with the defined timeout (for now 1/30th of a second)
            int numberOfPendingClients = pselect(fdmax+1, &read_fds, nullptr, nullptr, &poll_timeout, nullptr);//&poll_timeout, nullptr);
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
                std::vector<int> *pendingClients = new std::vector<int>(clients.size());
                for (int i = 0; i < clients.size(); i++) {
                    if (FD_ISSET(clients.at(i)->fd(), &read_fds)) {
                        pendingClients->push_back(i);
                    }
                }

                return pendingClients;
            }
        }

        // TODO get client ID from packet
        Packet* getPacket(int id) {
            assert(id <  numClients());

            if (recvBuffer.at(id)->size() == 0) 
                return nullptr;

            Packet *mail = recvBuffer.at(id)->front();
            recvBuffer.at(id)->pop_front();
            std::cout << "Size of recv buffer is === " << recvBuffer.at(id)->size();

            return mail;
        }

        int addPacket(Packet *p) {
            int index = sendBuffer.size();
            sendBuffer.push_back(p);
            return index;
        }
        
        void receiveFromID(int id) {
            assert(listener->isOnline());
            assert(id < numClients());

            // Recieve data from a client
            Packet *mail = clients.at(id)->receive();

            std::cout << "SERVER: RECEIVED DATA" << mail << std::endl;
            mail->printData();
            fflush(stdout);

            if (mail != nullptr) {
                recvBuffer.at(id)->push_back(mail);
            }
            else {
                std::cout << "SERVER: NO VALID DATA" << std::endl;
            }
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
            delete mail;
            return true;
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

        bool sendPacket() {
            assert(server->isOnline());

            // Check if we have something to send.
            if (sendBuffer.size() == 0) {
                std::cout << "NO DATA?? " << std::endl;
                return false; // NO DATA TO SEND!?
            }

            // Send first packet in vector to server
            auto mail = sendBuffer.at(0);
            sendBuffer.erase(sendBuffer.begin());
            server->sendSocket(mail);

            delete mail;
            return true;
        }

        bool isConnected() {
            return server->isOnline();
        }
};

#endif