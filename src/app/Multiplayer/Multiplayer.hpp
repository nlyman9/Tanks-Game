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

/**
 * @brief A class to represent each client connected to a server
 *  - Each ClientConnection has a socket to communicate (currently only supports TCP)
 *  - A receive and send buffer that follow FIFO order
 *  - clientID is used to index clients for the server
 * 
 * @see Sockets for idea of clientSockets work
 */
class ClientConnection {
    private:
        Socket* clientSocket;
        // TODO add a udp socket for the client
        std::list<Packet *> recvBuffer;
        std::list<Packet *> sendBuffer;
        int clientID; // Index in the server's vector of clients 
    public:
        /**
         * @brief Construct a new Client Connection object
         * 
         * @param sock The socket to communicate through
         * @param id The ID for the server to index through its connections
         */
        ClientConnection(Socket * sock, int id) {
            clientSocket = sock;
            clientID = id;
        }

        /**
         * @brief Receive a packet from the socket and appends the packet to the receive buffer.
         *  - If the packet is null, or timeouts, then nothing is added
         * @warning BLOCKING function
         */
        void receive() {
            Packet *mail = clientSocket->receive();

            if (mail != nullptr) {
                recvBuffer.push_back(mail);
            } else {
#ifdef VERBOSE
                // Packet was probably invalid
                std::cerr << "SERVER: Packet from client was invalid!" << std::endl;
#endif
            }
        }

        /**
         * @brief Get the first Packet from the recvBuffer (FIFO)
         * 
         * @return Packet* a packet of data it has received
         * @warning if the recvbuffer is empty it will return nullptr
         */
        Packet* getPacket() {
            if (recvBuffer.size() == 0) {
                return nullptr;
            }

            Packet *mail = recvBuffer.front();
            recvBuffer.pop_front();
#ifdef VERBOSE            
            std::cout << "Size of recv buffer is === " << recvBuffer.size() << std::endl;
#endif
            return mail;
        }
        
        /**
         * @brief Add a packet to the send buffer to be sent at some point
         * 
         * @param mail The packet to send 
         * @return int - The size of send buffer (the position in the buffer)
         */
        int addPacketToSend(Packet* mail) {
            assert(mail != nullptr);

            sendBuffer.push_back(mail);
            return sendBuffer.size();
        }

        /**
         * @brief Send a packet from the receive buffer through the socket connection 
         * 
         * @return true - We sent a packet!
         * @return false - There was nothing to send (send buffer was empty)
         */
        bool sendFromBuffer() {
            if (sendBuffer.size() == 0) {
                // Nothing to send
                return false;
            } else {
                Packet *mail = sendBuffer.front();
                sendBuffer.pop_front();

                clientSocket->sendPacket(mail);

                delete mail;
                return true;
            }
        }


        /**
         * @brief Sends the given packet 
         *   - Usually used when the server needs to broadcast a packet
         * 
         * @warning - This overrides/skips the send buffer
         * @warning - Does not delete/free the packet after sending
         * 
         * @param mail - The packet want to send 
         */
        void sendPacket(Packet *mail) {
            assert(mail != nullptr);
            clientSocket->sendPacket(mail);
        }

        /**
         * @brief Set the id for the client connection
         * 
         * @param id 
         */
        void setID(int id) {
            this->clientID = id;
        }
        
        /**
         * @brief Get the ID of the client
         * 
         * @return int - The ID of the client connection
         */
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
/**
 * @brief Handles most server operations that a server would need
 *  - Abstracts having to use tcp or udp connections seperately
 *      + // TODO implement udp send/receive 
 *  - A vector of clients that the server needs to handle
 *  - A broadcast buffer that is used when we want to send something to all the clients
 * 
 * @see ClientConnection above to see how a server's clients are abstracted
 */
class ServerController {
    private:
        Socket *listener, *listener_tcp, *listener_udp;
        std::vector<ClientConnection *> clients;
        std::list<Packet *> broadcastBuffer;

        // TODO actually use this 
        const int MAX_PLAYERS = 10;

        // Select FDs
        fd_set client_fds;
        int fdmax;

        // Timeout / Rate the server will run at
        const int TICKS_PER_SECOND = 30;
        const long NANO_PER_SECOND = 1e+9;
        const long MICRO_PER_SECOND = 1e+6;

        // Set timeout based on constants
        // TODO Consolidate tickrates
        const struct timespec poll_timeout {
            0,                                 // tv_sec
            MICRO_PER_SECOND / TICKS_PER_SECOND // tv_nsec 
        };
    
    public:
        /**
         * @brief Construct a new Server Controller from a IP and a PORT
         * 
         * @param ip - remote IP to create a socket to 
         * @param port - remote Port to create a socket over
         */
        ServerController(std::string ip, int port) {
            // Create tcp socket
            listener_tcp = new Socket(Protocol::TCP, ip, port);
            listener_udp = new Socket(Protocol::UDP, ip, port+1);
            // Set base socket to tcp socket
            listener = listener_tcp;
            fdmax = -1;
        }
        
        /**
         * @brief Bind the tcp and udp sockets
         * 
         * @return true - It binded
         * @return false - Failed to bind
         */
        bool bind() {
            // Set up sockets to connect online
            if (listener_tcp->bindSocket() && listener_udp->bindSocket()) {
                std::cout << "SUCCESS: Binded TCP and UDP connections" << std::endl;
                return true;
            } else {
                return false;
            }
        }

        /**
         * @brief Get the TCP socket to start listening for connections
         * 
         * @return true - It is listening!
         * @return false - It failed to listen on the TCP socket
         */
        bool listen() {
            if (listener_tcp->listenSocket(MAX_PLAYERS)) {
                std::cout << "SUCCESS: Listening on TCP socket" << std::endl;
                return true;
            } else {
                std::cerr << "FAIL: Server can't listen on TCP socket" << std::endl;
                return false;
            }
        }

        /**
         * @brief Accept the incoming tcp connection
         * 
         * @return true 
         * @return false 
         */
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

        /**
         * @brief Polls the clients using pselect()
         * @warning It may timeout - timeout is defined by the class' poll_timeout variable
         * 
         * @return std::vector<ClientConnection*>* A vector of client connections that have pending packets to receive 
         */
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
#ifdef VERBOSE
            std::cout << "Number of clients from pselect -- " << numberOfPendingClients << std::endl;
            fflush(stdout);
#endif
            // fd_set's will be modified with the clients that have pending messages, return that list
            if (numberOfPendingClients == 0) {
                return nullptr;
            } else {
                std::vector<ClientConnection*> *pendingClients = new std::vector<ClientConnection*>();
                for (auto& client : clients) {
                    if (FD_ISSET(client->fd(), &read_fds)) {
                        pendingClients->push_back(client);
                    }
                }

                return pendingClients;
            }
        }

        /**
         * @brief Get a packet from a client's reveive buffer using their ID
         * 
         * @param id - The ID of the client you want to get the packet from 
         * @return Packet* The packet from their receive buffer
         */
        Packet* getPacket(int id) {
            assert(id <  numClients());

            Packet* mail = clients[id]->getPacket();

            return mail;
        }

        /**
         * @brief Add a packet to the broadcast buffer 
         * 
         * @param mail - The packe to add
         * @return int - The size of the broadcast buffer after adding the packet
         */
        int addPacketToBroadcast(Packet *mail) {
            broadcastBuffer.push_back(mail);
            return broadcastBuffer.size();
        }

        /**
         * @brief Add a packet to a client's send buffer to prepare to send 
         * 
         * @param id - The ID of the client you want to add the packet 
         * @param mail - The packet to add
         * @return int - The size of the client's send buffer
         */
        int addPacketToSend(int id, Packet *mail) {
            assert(id < numClients());
            return clients[id]->addPacketToSend(mail);
        }
        
        /**
         * @brief Receive data from a client using an ID
         * @warning BLOCKING function
         * 
         * @param id - The ID of the client
         */
        void receiveFromClient(int id) {
            assert(listener->isOnline());
            assert(id < numClients());

            // Recieve data from a client
            clients[id]->receive();
        }

        /**
         * @brief Send a packet to a client using their send buffer
         * 
         * @param id - The ID of the client
         * @return true - We sent a packet!
         * @return false - There was nothing to send
         */
        bool sendTo(int id) {
            assert(id < numClients());
            // Send to specific client
            return clients[id]->sendFromBuffer();
        }

        /**
         * @brief Broadcast the first packet from the broadcast buffer to all cliennts
         * @warning this overrides every clients' send buffer. It immediately sends a packet
         * 
         * @return true - We broadcasted a packet to all clients!
         * @return false - There was nothing to send
         */
        bool broadcast() {
            if (broadcastBuffer.size() == 0) {
                // Nothing to broadcast
                return false;
            }

            // Send to all clients 
#ifdef VERBOSE
            std::cout << "Broadcasting to clients..." << std::endl;
#endif
            Packet *mail =  broadcastBuffer.front();
            broadcastBuffer.pop_front();

            for (auto& client : clients) {
                client->sendPacket(mail);
            }
#ifdef VERBOSE
            std::cout << "Broadcasted to clients!" << std::endl;
#endif
            // Free packet we are done with it!
            delete mail;
            return true;
        }

        void closeClient(int idx) {
            assert(idx < clients.size());
            std::vector<ClientConnection *>::iterator it = clients.begin() + idx;
            ClientConnection *c = clients.at(idx);

            FD_CLR(c->fd(), &client_fds);   // Remove client from fds
            delete c;                       // Free allocated memory
            clients.erase(it);              // Remove pointer from vector
        }

        /**
         * @brief Get the vector of clients 
         * 
         * @return std::vector<ClientConnection*> 
         */
        std::vector<ClientConnection*> getClients() {
            return clients;
        }

        /**
         * @brief Get the Clients File descriptor using their ID
         * 
         * @param id - The ID of the client
         * @return int - The client's file descriptor
         */
        int getClientsFD(int id) {
            assert(id < clients.size());
            return clients.at(id)->fd();
        }

        /**
         * @brief Get the Clients ID object
         * @warning this has to iterate through the list of clients
         * 
         * @param fd - The file descriptor you are searching for 
         * @return int - Returns a positive number if they found the value
         * @warning returns -1 if the given file descriptor didnt match any client 
         */
        int getClientsID(int fd) {
            assert(fd < clients.size());

            int counter = 0;
            for (auto& client : clients) {
                if (client->fd() == fd) {
                    return counter;
                }
                counter++;
            }

            return -1;
        }

        /**
         * @brief Get the number of clients connected 
         * 
         * @return int 
         */
        int numClients() {
            return clients.size();
        }
    
};


/**
 * @brief Similar to the idea of the server controller, the client Controller handles the network functionality for a client 
 *  - One tcp and udp socket to comminucate to a server
 *  - A receive and send buffer
 */
class ClientController {
    private:
        Socket *server, *server_tcp, *server_udp;

        std::list<Packet*> recvBuffer;
        std::list<Packet*> sendBuffer;
        
        struct timeval* timeout;
    public:
        /**
         * @brief Construct a new Client Controller from a IP and a PORT
         * 
         * @param ip - remote IP to create a socket to 
         * @param port - remote Port to create a socket over
         */
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
        
        /**
         * @brief Returns the current socket being used
         * 
         * @return Socket* - could be either TCP or UDP
         */
        Socket* currSocket() {
            return server;
        }

        /**
         * @brief swap the socket to use either the tcp or udp connection 
         * 
         */
        void swapSocket() {
            if (server == server_tcp)
                server = server_udp;
            else
                server = server_tcp;
        }

        /**
         * @brief Connect the tcp socket to the server
         * 
         * @return true - We connected 
         * @return false - Faieled to connect 
         */
        bool connect() {
            if (server_tcp->connectSocket()) {
                std::cout << "CLIENT SUCCESS: Connected to server over TCP" << std::endl;
                return true;
            } else {
                std::cout << "CLIENT ERROR: Failed to connect to server over TCP" << std::endl;
                return false;
            }
        }

        /**
         * @brief Set the Socket Tickrate (timeout) of the sockets
         * 
         * @param tickrate - The rate in seconds -> 'tickrate' times a second
         */
        void setSocketTimeout(int tickrate) {
            server_tcp->setTimeout(tickrate);
            server_udp->setTimeout(tickrate);
        }

        /**
         * @brief Get the first Packet from the receive buffer
         * 
         * @return Packet* Packet of information
         */
        Packet* getPacket() {
            if (recvBuffer.size() == 0) {
                return nullptr;
            } else {
                Packet *p = recvBuffer.front();
                recvBuffer.pop_front();
                return p;
            }
        }

        /**
         * @brief Add a packet to the send buffer 
         * 
         * @param mail - packet to add
         */
        void addPacket(Packet *mail) {
            sendBuffer.push_back(mail);
        }

        /**
         * @brief Receive data from the server 
         * @warning BLOCKING Function (based on timeout value)
         * 
         */
        void receive() {
            Packet *mail = server->receive();

            if (mail != nullptr)
                recvBuffer.push_back(mail);
        }
        
        /**
         * @brief Send the first packet from the send buffer 
         * 
         * @return true - We sent a packet
         * @return false - There wa nothing to send
         */
        bool sendPacket() {
            assert(server->isOnline());

            // Check if we have something to send.
            if (sendBuffer.size() == 0) {
                return false; // NO DATA TO SEND!?
            }

            // Send first packet in vector to server
            auto mail = sendBuffer.front();
            sendBuffer.pop_front();
            server->sendPacket(mail);

            delete mail;
            return true;
        }

        /**
         * @brief Check the socket has been connected correctly 
         * 
         * @return true - Is conencted
         * @return false - Failed
         */
        bool isConnected() {
            return server->isOnline();
        }
};

#endif