/*
    Originally created by Jakob - Modified by Adam 
    Modifications: 
    test to create a map on a server then send it over as bit data
    and render on client
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
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "Server.hpp"
#include "MapGenerator.hpp"
#include "Network.hpp"
#include "GameLoop.hpp"

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

int sendThread(void* data){
    int newfd;
    int nbytes;
    int i, j;
    // Loop of server
    while (gameOn)
    {
        std::cout << "server looping" << std::endl;
        sleep(1);
        read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cout << "Select error" << std::endl;
            exit(4);
        }
        // Loop through our connections
        for (i = 0; i <= fdmax; i++)
        {
            // Check if one of the connections is in the set read_fds
            if (FD_ISSET(i, &read_fds))
            {
                // Check if it is a new connection
                if (i == listenerfd)
                { // if listenerfd is in set, we have a new connection
                    addr_len = sizeof(remoteaddr);
                    newfd = accept(listenerfd, (struct sockaddr *)&remoteaddr, &addr_len);      
                    if (newfd == -1)
                    {
                        // Failed to accept
                        std::cout << "Accept error" << std::endl;
                    }
                    else
                    {
                        // Successfully accepted connection
                        FD_SET(newfd, &master); // Add new connection to master list
                        if (newfd > fdmax)
                        {
                            fdmax = newfd; // Keep track of max
                        }
                        std::cout << "New connection from" << inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN) << " on socket " << newfd << std::endl;
                        //new connection so need to send map data here accepted so send data
                        send(newfd, packedMap.data(), packedMap.size(), 0);
                    }
                }
                else
                {
                    sendBuffer[0] = 'x';
                    if (FD_ISSET(i, &master)){
                        send(i, sendBuffer, 8, 0);
                    }
                    // Handle data from clients
                    if ((nbytes = (recv(i, recvBuffer, 100, 0))) <= 0)
                    {
                        // Either error or closed connection
                        if (nbytes == 0)
                        { // Connection closed
                            std::cout << "Socket " << i <<" disconnected." << std::endl;
                        }
                        else
                        {
                            std::cout << "Recv error" << std::endl;
                        }
                        // remember to close the fd
                        close(i);
                        FD_CLR(i, &master); // Remove from master set
                    }
                    else
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

                                    if (send(j, tsBuffer, nbytes, 0) == -1)
                                    {
                                       std::cout << "Send error" << std::endl;
                                    }
                                }
                                else if (j == listenerfd)
                                {
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
    // Get the server option's
    std::cout << "IP " << argv[1] << std::endl;
    std::cout << "PORT " << argv[2] << std::endl;

    std::vector<int>* map = serverMapGen();

    char *server_ip = argv[1];
    char *server_port = argv[2];
    
    Network* net = new Network();
    
    net->pack(map, &packedMap, 3); //pack map into 3 bits
    
    // Set structs and variables for the internet
    addr_len = sizeof(struct sockaddr_storage);

    char remoteIP[INET_ADDRSTRLEN];
    char buf[152];

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

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

    char* tsBuffer = (char*) calloc(152, sizeof(char)); 
    std::cout << "Creating send thread" << std::endl;
    gameOn = true;
    SDL_CreateThread(sendThread, "send thread", (void*) tsBuffer);

    //Game stuff here
    while(gameOn){
        
    }
}