#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>

class Client {
    public:
        int sockfd;
        struct sockaddr *clientAddr;
        Client(int fd, sockaddr *addr) : sockfd{fd}, clientAddr{addr} {}
        ~Client() {
            delete &sockfd;
            delete clientAddr;
            printf("Closing client connection %d", sockfd);
        }

};


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main() {
    // Set structs and variables for the internet
    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo, *p;
    struct sockaddr_storage remoteaddr; // client address; for setting new connections
    int listenerfd; 

    socklen_t addr_len = sizeof(struct sockaddr_storage);
    fd_set master;      // Master of file descriptors
    fd_set read_fds;    // Read fd's returned from select
    int fdmax;          // maximym file descriptor number

    char remoteIP[INET_ADDRSTRLEN];
    char buf[100];

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    int yes = 1;

    // Set memory for hints
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_INET;          // Use IPv4 because im lazy
    hints.ai_socktype = SOCK_STREAM;    // Use TCP because it does work for me


    // Set address info for listening address '0.0.0.0'
    if ((status = getaddrinfo("0.0.0.0", "8123", &hints, &serverInfo)) != 0) {
        printf("Failed to get address info.");
        exit(3);
    }

    for(p = serverInfo; p != NULL; p = p->ai_next) {
        listenerfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listenerfd < 0) { 
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listenerfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listenerfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(listenerfd);
            continue;
        }

        break;
    }
    freeaddrinfo(serverInfo);

    // Listen on the socket, and allow a max wait queue of size 20 
    listen(listenerfd, 20);

    // Accept a connection

    // Set timevalue to 2 seconds and 500000 microsecond
    struct timeval tv = {2, 500000};

    // Add listener to master set
    FD_SET(listenerfd, &master);

    fdmax = listenerfd;

    int newfd;
    int nbytes;
    int i, j;
    char buffer[100];
    // Loop of server
    while(1) {
        read_fds = master;
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // Loop through our connections
        for(i = 0; i <= fdmax; i++) {
            // Check if one of the connections is in the set read_fds
            if (FD_ISSET(i, &read_fds)) {
                
                // Check if it is a new connection
                if (i == listenerfd) { // if listenerfd is in set, we have a new connection
                    addr_len = sizeof(remoteaddr);
                    newfd = accept(listenerfd, (struct sockaddr *)&remoteaddr, &addr_len);

                    if (newfd == -1)  { 
                        // Failed to accept
                        perror("accept");
                    } else {
                        // Successfully accepted connection 
                        FD_SET(newfd, &master); // Add new connection to master list
                        if (newfd > fdmax) {
                            fdmax = newfd; // Keep track of max
                        }
                        printf("New connection from %s on socket %d.\n",
                                inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN), 
                                newfd);
                    }
                } else {
                    // Handle data from clients
                    if ((nbytes = (recv(i, buf, 100, 0))) <= 0) {
                        // Either error or closed connection
                        if (nbytes == 0) { // Connection closed
                            printf("Socket %d disconnected.", i);
                        } else {
                            perror("recv");
                        }
                        // remember to close the fd
                        close(i);
                        FD_CLR(i, &master); // Remove from master set
                        
                    } else { 
                        // We have real data from client 
                        for(j = 0; j <= fdmax; j++) {

                            // relay message to connections
                            if (FD_ISSET(j, &master)) {
                                // except the listener and ourselves
                                if (j != listenerfd && j != i) {
                                    
                                    sprintf(buffer, "User %d: %.90s", i, buf);
                                    
                                    if (send(j, buffer, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                } else if (j == listenerfd) {
                                    printf("Client %d: %s\n", i, buf);
                                    fflush(stdout);
                                }
                            }
                        }
                    }
                }
            } 
        }
    }

}