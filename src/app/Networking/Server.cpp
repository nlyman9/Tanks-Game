/*
    Originally created by Jakob - Modified by Adam 
    Modifications: 
    test to create a map on a server then send it over as bit data
    and render on client
*/
#if __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "Server.hpp"
#include "MapGenerator.hpp"
#include "Network.hpp"

bool readyToSend = false;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

std::vector<int>* serverMapGen(){
    // Randomly generate map
    std::vector<int>* map1D = new std::vector<int>();
    MapGenerator* mapGen = new MapGenerator();    
	std::vector<std::vector<int>>* tile_map = mapGen->generateMap();
    std::vector<std::vector<int>>& map = *tile_map;

    // Convert the 2D map into a 1D vector for the map packer
    for(auto row : map) {
        for(auto val : row) {
            map1D->push_back(val);
        }
    }
    
    return map1D;
}

int serverThread(void* data){
    int newfd;
    int nbytes;
    int i, j;
    //timeval is passed into select and is used to say when select should timeout
    //setting all the members to 0 tells select it should not block
    struct timeval* timeout = (timeval*) calloc(1, sizeof(struct timeval));
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;
    // Loop of server
    while (gameOn)
    {
        client_fds = master;
        // fcntl(listenerfd, F_SETFL, O_NONBLOCK); // non blocking socket
        if (select(fdmax + 1, &client_fds, NULL, NULL, timeout) == -1)
        {
            std::cout << "SERVER: Select error: " << strerror(errno) << std::endl;
            exit(4);
        }
        // Loop through our connections
        for (i = 0; i <= fdmax; i++)
        {
            // Check if one of the connections is in the set client_fds
            if (FD_ISSET(i, &client_fds))
            {
                // Check if it is a new connection
                if (i == listenerfd)
                { // if listenerfd is in set, we have a new connection
                    addr_len = sizeof(remoteaddr);
                    newfd = accept(listenerfd, (struct sockaddr *)&remoteaddr, &addr_len);      
                    if (newfd == -1)
                    {
                        // Failed to accept
                        std::cout << "SERVER: failed to accept" << std::endl;
                        continue;
                    }
                    else
                    {
                        // Successfully accepted connection
                        FD_SET(newfd, &master); // Add new connection to master list
                        if (newfd > fdmax)
                        {
                            fdmax = newfd; // Keep track of max
                        }
                        std::cout << "SERVER: New connection from " << inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN) << " on socket " << newfd << std::endl;
                        
                        //new connection so need to send map data here accepted so send data
                        for(int i = 0; i < packedMap.size(); i++){
                            sBuffer->push_back(packedMap.at(i));
                        }
                        
                        // Appender the map header and send the map
                        appendHeader(sBuffer, (char) 0);
                        send(newfd, sBuffer->data(), sBuffer->size(), 0);
                        sBuffer->clear();
                    }
                }
                else
                {   
                    nbytes = recv(i, rBuffer->data(), rBuffer->size(), 0);
                    std::cout << "SERVER: received: " << nbytes << " bytes" << std::endl;
                    // Handle data from clients
                    if (nbytes < 0)
                    {
                        // Either error or closed connection
                        if (nbytes == 0)
                        { // Connection closed
                            std::cout << "SERVER: Socket " << i <<" disconnected." << std::endl;
                            // remember to close the fd
                            close(i);
                            FD_CLR(i, &master); // Remove from master set
                        }
                        else
                        {
                            //client disconnected... for testing purposes just closing the server
                            //so we dont have to in task manager
                            std::cout << "SERVER: Recv error server server exiting" << std::endl;
                            exit(0);
                        }
                        
                    }
                    else if (nbytes > 0)
                    {
                        // We have real data from client
                        for (j = 0; j <= fdmax; j++)
                        {
                            // relay message to connections
                            if (FD_ISSET(j, &master))
                            {
                                // except the listener and ourselves
                                if (j != listenerfd && j != i)
                                {
                                    std::cout << "SERVER: passing message through" << std::endl;
                                    if (send(j, sBuffer->data(), sBuffer->size(), 0) == -1)
                                    {
                                        std::cout << "SERVER: Send error" << std::endl;
                                    }
                                }
                                else if (j == listenerfd)
                                {
                                    fflush(stdout);
                                }
                            }
                        }
                    } else {
                        std::cout << "SERVER: Recieved zero bytes" << std::endl;
                    }
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Main for Server process
 * 
 * @param argc - Number of arguments passed
 * @param argv - Argument values
 *     - Only one value should be passed. The option's arguments.
 * @return int 
 */
int main(int argc, char* argv[])
{
    //init buffers
    //buffer received
    rBuffer = new std::vector<char>(152);
    //to send buffer
    sBuffer = new std::vector<char>();
    //buffer to fill in
    sfBuffer = new std::vector<char>(152);

    // Get the server option's
    std::cout << "IP " << argv[1] << std::endl;
    std::cout << "PORT " << argv[2] << std::endl;

    std::vector<int>* map = serverMapGen();

    char *server_ip = argv[1];
    char *server_port = argv[2];
    
    pack(map, &packedMap, 3); //pack map into 3 bits
    
    // Set structs and variables for the internet
    addr_len = sizeof(struct sockaddr_storage);

    char remoteIP[INET_ADDRSTRLEN];

    FD_ZERO(&master);
    FD_ZERO(&client_fds);

    int yes = 1;

    // Set memory for hints
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;       // Use IPv4 because im lazy
    hints.ai_socktype = SOCK_STREAM; // Use TCP because it does work for me

    // Set address info for listening address '0.0.0.0'
    if ((status = getaddrinfo(server_ip, server_port, &hints, &serverInfo)) != 0)
    {
        std::cout << "Failed to get address info." << std::endl;
        exit(3);
    }

    for (p = serverInfo; p != NULL; p = p->ai_next)
    {
        listenerfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        //set the socket to non-blocking
        //fcntl(listenerfd, F_SETFL, O_NONBLOCK);
        if (listenerfd < 0)
        {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(listenerfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listenerfd, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listenerfd);
            continue;
        }

        break;
    }
    
    freeaddrinfo(serverInfo);

    // Listen on the socket, and allow a max wait queue of size 20
    listen(listenerfd, 20);

    // Add listener to master set
    FD_SET(listenerfd, &master);

    fdmax = listenerfd;
    std::cout << "Creating server thread" << std::endl;
    gameOn = true;
    SDL_CreateThread(serverThread, "server thread", NULL);

    //Game stuff here
    while(gameOn){
        
    }
}