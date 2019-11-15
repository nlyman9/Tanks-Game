#ifndef SOCKET_HPP
#define SOCKET_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h> 

#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>
#include <unistd.h>

enum class Protocol {
    UDP,
    TCP
};

class Socket {
    private:
        Protocol type;
        std::string remoteIP;
        std::string remotePort;
        int socket_fd;


        // True after connecSocket() or bind()
        bool isConnected;
        bool isListening;

        // 
        Packet HEAD = Packet(PackType::INIT);

    public:
        Socket(Protocol prot, std::string ip, int port) {
            type = prot;
            remoteIP = ip;
            remotePort = std::to_string(port);
            socket_fd = -1;
            isConnected = false;
            isListening = false;

            std::cout << "Remote IP = " << remoteIP << std::endl;
        };

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

        bool listenSocket(int size) {
            assert(type == Protocol::TCP);

            if (listen(socket_fd, size) == -1) {
                std::cerr << "ERROR: failed to listen: " << errno << std::endl;
                return false;
            }

            isListening = true;
            return true;
        }

        Socket* acceptSocket() {
            assert(type == Protocol::TCP);
            assert(isListening);
            struct sockaddr_storage incomingConnection;
            socklen_t incomingSize = sizeof(incomingConnection);

            // Accept connection
            std::cout << "ACCEPTINGGGG "<< std::endl;
            fflush(stdout);
            int clientFD = accept(socket_fd, (struct sockaddr *)&incomingConnection, &incomingSize);

            if (clientFD < 0) {
                std::cerr << "ERROR: Unable to accept connection!" << std::endl;
                return nullptr;
            }

            // Get IP of client connection
            char clientIP[INET_ADDRSTRLEN];
            std::cout << "CLIEENNNT "<< std::endl;
            fflush(stdout);
            inet_ntop(incomingConnection.ss_family, Socket::get_in_addr((struct sockaddr *)&incomingConnection), clientIP, sizeof(clientIP));

            Socket *client = new Socket(Protocol::TCP, std::string(clientIP), std::atoi(remoteIP.data()));
            client->socket_fd = clientFD;

            std::cout << "REturninnggg "<< std::endl;

            return client;
            
        }

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
                std::cout << "Socket: Waiting for connection..." << std::endl;
                return false;
            }
            std::cout << "CLIENT Socket: Connected ip: " << remoteIP << " Port: " << remotePort << std::endl;

            freeaddrinfo(serverInfo);

            // Connected!
            isConnected = true;
            return true;
        }

        int sendSocket(Packet *p) {
            int num_bytes_sent = send(socket_fd, p->data(), p->size(), 0);
            std::cout << "Sent packet of size: " << p->size() << std::endl;
            free(p);
    
            return num_bytes_sent;
        }

        Packet* receive() {
            assert(isConnected);

            // Recieve from connection, add to buffer.
            char headBuffer[HEAD.size()];
            int num_bytes;

            // Recieve initial header
            if (type == Protocol::TCP) {
                // Use tcp
                num_bytes = recv(socket_fd, headBuffer, HEAD.size(), 0);
                if (num_bytes == -1) {
                    std::cerr << "CLIENT: read header error: " << strerror(errno) <<  std::endl;
                }
            } else {
                // Use udp
            }

            if (num_bytes == 0) {
                return nullptr;
            }
            // else 

            // Grab the first two headers 
            //   - There should always be a SIZE and TYPE header in that order
            Header size_header = Header(headBuffer); 
            std::cout << "THE HEADER IS " << size_header.data() << std::endl;
            std::cout << "THE PACKET SIZE IS " << size_header.getValue() << std::endl;
            std::cout << "THE HEADER SIZE IS " << size_header.size() << std::endl;
            fflush(stdout);

            Header type_header = Header(&headBuffer[size_header.size()+1]);
            // std::cout << "THE HEADER IS " << type_header.data() << std::endl;
            // std::cout << "THE VALUE IS " << type_header.getValue() << std::endl;
            // std::cout << "THE HEADER SIZE IS " << size_header.size() << std::endl;
            
            // Check to see if there is more data left
            int packet_size_left = std::stoi(size_header.getValue()) - HEAD.size();
            // std::cout << "Packet size remaining is " << packet_size_left << std::endl;

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
                    // Use udp
                }
            }

            std::cout << "Bytes received  " << num_bytes << std::endl;
            // This buffer should just be the data segments of the packet
            return new Packet(size_header, type_header, dataBuffer, packet_size_left);
            
        }
        
        int fd() {
            return socket_fd;
        }

        std::string ip() {
            return remoteIP;
        }

        std::string port() {
            return remotePort;
        }

        bool isOnline() {
            return isConnected;
        }

        // get sockaddr, IPv4 or IPv6:
        void *get_in_addr(struct sockaddr *sa)
        {
            if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
            }

            return &(((struct sockaddr_in6*)sa)->sin6_addr);
        }
};

#endif