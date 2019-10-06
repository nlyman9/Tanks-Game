/*
    Originally created by Jakob - Modified by Adam 
    Modifications:
    test to create a map on a server then pack it and send it over as bit data
    and unpack render on client
*/
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

int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int sockfd;
    char buffer[100];

    fd_set master;      // Master of file descriptors
    fd_set read_fds;    // Read fd's returned from select
    int fdmax;          // maximym file descriptor number

    // Innitialize sets to zero
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;          // IPv4
    hints.ai_socktype = SOCK_STREAM;    // TCP

    // Get address info of server
    if ((status = getaddrinfo("127.0.0.1", "8123", &hints, &serverInfo)) != 0) {
        perror("failed to get address info");
        exit(4);
    }

    // Create a socket based on server info 
    sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    // Connec to server
    if (connect(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
        perror("failed to connect");
        exit(1);
    }

    // Add server fd to master
    FD_SET(sockfd, &master);
    // Add stdin fd to master
    FD_SET(STDIN_FILENO, &master);

    fdmax = sockfd;

    int i, nbytes;
    while(1) {
        read_fds = master;

        // Check for any response from serrver
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // If from server
        if (FD_ISSET(sockfd, &read_fds)) {
            nbytes = recv(sockfd, buffer, 100, 0);
            if (nbytes <= 0) {
                printf("Closing connection to server.");
                close(sockfd);
                exit(10);
            } else {
                printf("%s", buffer);
            }
        } else if (fgets(buffer, 100, stdin) != NULL) { // get input from user
            // Check for user input in the terminal 
            //printf("%s", buffer);
            send(sockfd, buffer, 100, 0);
        }

    }
}