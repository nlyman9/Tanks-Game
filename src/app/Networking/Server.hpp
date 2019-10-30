#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <chrono>
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
#include <SDL2/SDL_thread.h>

char* tsBuffer;
//buffer to fill in
std::vector<char>* fBuffer;
//receive buffer
char* sendBuffer;
char* recvBuffer;
int status;
struct addrinfo hints;
struct addrinfo *serverInfo, *p;
struct sockaddr_storage remoteaddr;
char remoteIP[INET_ADDRSTRLEN];
int listenerfd;
int sockfd;
fd_set master;      // Master of file descriptors
fd_set read_fds;    // Read fd's returned from select
int fdmax;          // maximym file descriptor number
int nbytes;
socklen_t addr_len;
std::vector<char> packedMap;

bool gameOn;
 
#endif