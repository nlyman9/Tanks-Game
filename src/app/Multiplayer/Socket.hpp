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

    public:
        Socket(Protocol type, std::string remoteIP, int port) {
            type = type;
            remoteIP = remoteIP;
            remotePort = std::to_string(port);
            socket_fd = -1;
            isConnected = false;
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
            std::cout << "Socket: Connecting to " << remoteIP << " on port " << remotePort << std::endl;
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
                std::cerr << "Socket: Failed to bind to " << remoteIP << " | " << serverInfo->ai_addr << " on port " << remotePort  << std::endl;
                close(socket_fd);
                exit(5);
            }

            freeaddrinfo(serverInfo);

            isConnected = true;
            return true;
        }

        bool connectSocket() {
            // We should only connect each socket once.
            assert(!isConnected);

            struct addrinfo hints;
            struct addrinfo *serverInfo;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET; // IPv4
            if (type == Protocol::TCP)
                hints.ai_socktype = SOCK_STREAM; // TCP
            else 
                hints.ai_socktype = SOCK_DGRAM; // UDP
            
            // Get address info of destination
            std::cout << "Socket: Connecting to " << remoteIP << " on port " << remotePort << std::endl;
            if (getaddrinfo(remoteIP.c_str(), remotePort.c_str(), &hints, &serverInfo) != 0) {
                std::cout << "Socket: Failed to get address info of " << remoteIP << " on port " << remotePort  << std::endl;
                exit(4);
            }

            // Create socket with remote destination
            socket_fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

            // Connect to remote destination
            while (connect(socket_fd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
                std::cout << "Socket: Waiting for connection..." << std::endl;
                sleep(1);
            }
            std::cout << "Socket: Connected ip: " << remoteIP << " Port: " << remotePort << std::endl;

            freeaddrinfo(serverInfo);

            // Connected!
            isConnected = true;
            return true;
        }
        
        int fd() {
            return socket_fd;
        }

        bool isOnline() {
            return isConnected;
        }
        
};

#endif