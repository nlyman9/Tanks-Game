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
std::vector<int> *unpackMap(std::vector<char> mapPacked){
    //std::cout << "UNPACKING\n";
	std::vector<bool> workSet;
    //first turn the packed map into a bool array
	for(auto curr : *mapPacked){
		for(int i = 0; i < 8; i++){
			workSet.push_back((curr >> (7 - i)) & 1);
			//std::cout << (int) ((curr >> (7 - i)) & 1);
		}
		//std::cout << '\n';
	}
	std::vector<int> *retVal = new std::vector<int>();
	int tmp = 0;
	i = 0;
	//std::cout << "Converting to int array\n";
    //cycle through the bool array
    //every 3 bools is one int
	for(auto curr : workSet){
		//std::cout << "curr is : " << curr << '\n';
		tmp = (tmp) | (curr << (2-i));
		if(i == 2){
			retVal->push_back(tmp);
			//std::cout << tmp << '\n';
			//for(unsigned int j = 1 << 31; j > 0; j = j/2){
			//	(tmp & j)? std::cout << 1: std::cout << 0;
			//}
			//std::cout << '\n';
			tmp = 0;
			i = -1;
		}
		i++;
	}
    return retVal;
}
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