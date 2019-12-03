#ifndef SOCKET_HPP
#define SOCKET_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h> 
#include <fcntl.h>

#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>
#include <unistd.h>

/**
 * @brief Define a socket with these two types of protocols
 * 
 */
enum class Protocol {
    UDP,
    TCP
};

/**
 * @brief This is an abstraction of the TCP and UDP connections for client-server connectionns
 * Each socket has to have a type, a remoteIP and Port.
 * - Then they should have a filedescriptor added later to be useable
 * 
 * @see the HEAD Packet below, the Socket assumes this is hwo the data will be formatted for receiving
 * 
 * @see Multiplayer.hpp::ServerConnection
 *  Sockets are also used to keep track of the current client connections
 * 
 */
class Socket {
    private:
        Protocol type;
        std::string remoteIP;
        std::string remotePort;
        int socket_fd;

        // Used for assertions to prevent doing sends annd receive on a improperly set sockets
        // True after connecSocket() or bind()
        bool isConnected;
        bool isListening;
        bool isReceiving;

        /**
         * @brief Every packet should have this basic underlying structure.
         *  - Two headers:  A size-header and a type-header (in that order)
         * 
         * @see constructor of Packet
         */
        Packet HEAD = Packet(PackType::INIT);

    public:
        /**
         * @brief Construct a new Socket
         * 
         * @param prot  - The protocol type you want to use for the socket (UDP/TCP)
         * @param ip    - The IP you want to connnect to 
         * @param port  - The port you want to use to connect
         */
        Socket(Protocol prot, std::string ip, int port) {
            type = prot;
            remoteIP = ip;
            remotePort = std::to_string(port);
            socket_fd = -1;
            isConnected = false;
            isListening = false;

            std::cout << "Remote IP = " << remoteIP << std::endl;
        };

        /**
         * @brief Bind a socket based on the IP, Port, and protocol 
         *        type given at object creation
         * 
         * @return true binded successfully
         * @return false bnding failed
         */
        bool bindSocket() {
            // We should only connect each socket once.
            assert(!isConnected);

            struct addrinfo hints;
            struct addrinfo *serverInfo;
            int yes = 1;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET; // IPv4
            if (type == Protocol::TCP)
                hints.ai_socktype = SOCK_STREAM; // TCP
            else 
                hints.ai_socktype = SOCK_DGRAM; // UDP

            // Set address info for listening address '0.0.0.0'
            std::cout << "Socket: Binding to " << remoteIP << " on port " << remotePort << std::endl;
            if (getaddrinfo(remoteIP.c_str(), remotePort.c_str(), &hints, &serverInfo) != 0) {
                std::cerr << "Socket: Failed to get address info of " << remoteIP << " on port " << remotePort  << std::endl;
                exit(4);
            }

            // Create socket with remote destination
            socket_fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

            // lose the pesky "address already in use" error message
            setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

            if (bind(socket_fd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0)
            {
                std::cerr << "Socket: Failed to bind to " << remoteIP << " on port " << remotePort  << std::endl;
                close(socket_fd);
                exit(5);
            }
            freeaddrinfo(serverInfo);

            
            if (type == Protocol::TCP) {
                std::cout << "Binded TCP socket to " << remoteIP << " @ " << remotePort << std::endl;
            } else {
                std::cout << "Binded UDP socket to " << remoteIP << " @ " << remotePort << std::endl;
            }

            isConnected = true;
            return true;
        }

        /**
         * @brief - Sets the socket to start listening
         * 
         * @param queue_size - The max number of waiting connections that can wait in the listen queue
         * @return true - We CAN listen on the socket
         * @return false - We CANNOT listen on the socket 
         */
        bool listenSocket(int queue_size) {
            assert(type == Protocol::TCP);

            if (listen(socket_fd, queue_size) == -1) {
                std::cerr << "ERROR: failed to listen: " << errno << std::endl;
                return false;
            }
            isListening = true;
            isReceiving = true;
            return true;
        }

        /**
         * @brief Accept a connection from another socket
         * 
         * @return Socket* returns a socket object of the incoming connection if all goes well
         */
        Socket* acceptSocket() {
            assert(type == Protocol::TCP);
            assert(isListening);

            struct sockaddr_storage incomingConnection;
            socklen_t incomingSize = sizeof(incomingConnection);

            // Wait to accept a incoming connection
            //   Its okay to wait since we do this before the game starts
            std::cout << "Accepting... "<< std::endl;
            fflush(stdout);
            int clientFD = accept(socket_fd, (struct sockaddr *)&incomingConnection, &incomingSize);

            // Check for invalid fd
            if (clientFD < 0) {
                std::cerr << "ERROR: Unable to accept connection!" << std::endl;
                return nullptr;
            }

            // Get IP of client connection
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(incomingConnection.ss_family, Socket::get_in_addr((struct sockaddr *)&incomingConnection), clientIP, sizeof(clientIP));

            // Create socket connection of the accepted connection
            Socket *client = new Socket(Protocol::TCP, std::string(clientIP), std::atoi(remoteIP.data()));
            client->setFD(clientFD);
            client->setReceiving(true);

            return client;
        }

        /**
         * @brief Set that this function is ready to receive
         * 
         * @param b - The boolean state
         */
        void setReceiving(bool b) {
            this->isReceiving = b;
        }

        /**
         * @brief Connect to the remote destination defined in the socket
         * 
         * @return true - We connected Successfully
         * @return false - We failed to connnect
         */
        bool connectSocket() {
            // We should only connect each socket once.
            assert(!isConnected);

            struct addrinfo hints;
            struct addrinfo *serverInfo;
            std::string sock = ((int)type == 1 ? "TCP" : "UDP");


            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET; // IPv4
            if (type == Protocol::TCP)
                hints.ai_socktype = SOCK_STREAM; // TCP
            else 
                hints.ai_socktype = SOCK_DGRAM; // UDP
            
            // Get address info of destination
            std::cout << sock << " Socket: Connecting to " << remoteIP << " on port " << remotePort << std::endl;
            if (getaddrinfo(remoteIP.c_str(), remotePort.c_str(), &hints, &serverInfo) != 0) {
                std::cout << "Socket: Failed to get address info of " << remoteIP << " on port " << remotePort  << std::endl;
                exit(4);
            }

            // Create socket with remote destination
            socket_fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

            // Connect to remote destination
            if (connect(socket_fd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
                std::cout << "Socket: Failed to connect." << std::endl;
                return false;
            }
            freeaddrinfo(serverInfo);

            // Connected!
            this->isConnected = true;
            return true;
        }

        /**
         * @brief Set the tickrate (timeout) of the socket 
         * @warning - This might need to be called before connection is established.
         *          + As far as a I know this works on linux even after sockets are binded/connected
         * 
         * @param tickrate - The rate in seconds the socket should timeout. Higher tickrate = Higher refresh rate
         */
        void setTimeout(int tickrate) {
            int micro_seconds = 1e+6;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = micro_seconds / tickrate;

            setsockopt(fd(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        }

        /**
         * @brief Send a packet through the socket connection
         * 
         * @param p - The packet of data we want to send
         * @return int - the nmber of bytes we sent
         */
        // TODO Change so this function handles if we sent all of the data or not
        int sendPacket(Packet *p) {
            assert(isConnected | isReceiving);

            int num_bytes_sent = send(socket_fd, p->data(), p->size(), 0);
#ifdef VERBOSE
            std::cout << "Sent packet of size: " << p->size() << std::endl;
#endif
    
            return num_bytes_sent;
        }

        /**
         * @brief Receives a packet from the socket connection
         *  - It receives the raw data and then formats it into a packet
         * @warning - Blocking function
         * @see Packet constructor so see the expected setup for a packet
         * 
         * @return Packet* - The data we received 
         */
        Packet* receive() {
            // assert(isReceiving);

            // Recieve HEAD from connection, add to buffer.
            char headBuffer[HEAD.size()];
            int num_bytes;

            // Recieve initial header
            if (type == Protocol::TCP) {
                // Use tcp
                num_bytes = recv(socket_fd, headBuffer, HEAD.size(), 0);
                if (num_bytes == -1) {
                    if (errno == ECONNRESET) {
                        std::cout << "SOCKET: Connection RESET!!! D:" << std::endl;
                        return new Packet(PackType::DISCONNECT);
                    } else if (errno == EWOULDBLOCK) {
#ifdef VERBOSE
                        std::cout << "SOCKET: Receive timed out" << std::endl;
#endif
                        return nullptr;
                    } else {
                        std::cerr << "SOCKET: read header error: " << strerror(errno) <<  std::endl;
                        return nullptr;
                    }
                }
            } else {
                // Use udp
            }

            if (num_bytes == 0) {
                return nullptr;
            }

            // Grab the first two headers 
            //   - There should always be a SIZE and TYPE header in that order
            Header size_header = Header(headBuffer); 
            Header type_header = Header(&headBuffer[size_header.size()+1]);
            
            // Check to see if there is more data left
            int packet_size_left = std::stoi(size_header.getValue()) - HEAD.size();
#ifdef VERBOSE
            std::cout << "Packet size remaining is " << packet_size_left << std::endl;
#endif
            // Check packet size is normal
            if (packet_size_left < 0) {
                std::cerr << "RECEIVE ERROR: Packet size remaining is negative" << std::endl;
                return nullptr;
            }

            // Check if there is no remaining data in packet
            if (packet_size_left == 0) {
                // Packet is just headers
                return new Packet(size_header, type_header);
            }

            // Get the rest of the data from the packet.
            char dataBuffer[packet_size_left];
            num_bytes = 0;
            if (packet_size_left > 0) {
                // Receive rest of data
                if (type == Protocol::TCP) {
                    // Use tcp
                    num_bytes = recv(socket_fd, dataBuffer, packet_size_left, 0);
                    if (num_bytes == -1) {
                        std::cerr << "CLIENT: read packet error: " << strerror(errno) <<  std::endl;
                    }
                } else {
                    // TODO Use udp
                }
            }

#ifdef VERBOSE
            // For debugging
            std::cout << "Bytes received  " << num_bytes << std::endl;
#endif
            // This buffer should just be the data segments of the packet
            return new Packet(size_header, type_header, dataBuffer, packet_size_left);
        }

        /**
         * @brief Set the filedescriptor of the socket connection
         * 
         * @param fd 
         */
        void setFD(int fd) {
            this->socket_fd = fd;
            std::cout << "client fd = " << this->socket_fd << std::endl;
        }

        /**
         * @brief Get the filedescriptor of the socket 
         * 
         * @return int - the filedescriptor
         */
        int fd() {
            return socket_fd;
        }

        /**
         * @brief Get the IP of the socket connection
         * 
         * @return std::string - the remoteIP
         */
        std::string ip() {
            return remoteIP;
        }

        /**
         * @brief Get the port of the socket
         * 
         * @return std::string - the port
         */
        std::string port() {
            return remotePort;
        }

        /**
         * @brief Return if the socket has succesfully binded/connected or not
         * 
         * @return true 
         * @return false 
         */
        bool isOnline() {
            return isConnected;
        }

        /**
         * @brief Gets the sockaddr struct in IPv4 or IPv6 
         *  The game only assumes IPv4
         * 
         * @param sa 
         * @return void* - the sockaddr
         */
        void *get_in_addr(struct sockaddr *sa)
        {
            if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
            }

            return &(((struct sockaddr_in6*)sa)->sin6_addr);
        }
};

#endif