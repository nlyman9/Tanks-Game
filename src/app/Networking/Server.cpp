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

bool readyToSend = false;

std::vector<char>* pack(std::vector<int>* x, std::vector<char>* packed, int bits)
{   
    std::vector<bool> workingSet;
    for (auto curr : *x)
    {
        int i;
        for(i = 0; i < bits; i++){
            workingSet.push_back((bool) (curr >> (bits - i - 1) & 1));
        }
    }
    int i = 0;
    char temp = 0;
    for(auto currInSet : workingSet){
        temp = temp | ((char) currInSet << (7-i));
        if(i == 7){
            packed->push_back(temp);
            temp = 0;
            i = -1;
        }
        i++;
    }
    if(i != 0) 
        packed->push_back(temp); 

    return packed;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

std::vector<int> *unpack(std::vector<char>* packed, std::vector<int> *unPacked, int bits){
	std::vector<bool> workSet;
    int i;
	for(auto curr : *packed){
		for(i = 0; i < 8; i++){
			workSet.push_back((curr >> (7 - i)) & 1);
		}
	}
    i = 0;
	int tmp = 0;
	for(auto curr : workSet){
		tmp = (tmp) | (curr << (bits - 1 - i));
		if(i == (bits - 1)){
			unPacked->push_back(tmp);
			tmp = 0;
			i = -1;
		}
		i++;
	}
    std::cout << unPacked->size() << std::endl;
    return unPacked;
}

std::vector<int>* serverMapGen(){
    std::vector<int>* retval = new std::vector<int>();
    //create a map that just cycles through the tiles
    MapGenerator* mapGen = new MapGenerator();    
	int** tile_map = mapGen->generateMap();

    return retval;

}

int sendThread( void* data){

    int newfd;
    int nbytes;
    int i, j;
    // Loop of server
    while (gameOn)
    {
        std::cout << "server looping" << std::endl;
        sleep(1);
        /*read_fds = master;
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
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
                        perror("accept");
                    }
                    else
                    {
                        // Successfully accepted connection
                        FD_SET(newfd, &master); // Add new connection to master list
                        if (newfd > fdmax)
                        {
                            fdmax = newfd; // Keep track of max
                        }
                        printf("New connection from %s on socket %d.\n",
                               inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN),
                               newfd);
                        //new connection so need to send map data here accepted so send data
                        //send(newfd, test2.data(), test2.size(), 0);
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
                            printf("Socket %d disconnected.", i);
                        }
                        else
                        {
                            perror("recv");
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

                                    //sprintf(buffer, "User %d: %.90s", i, buf);

                                    //if (send(j, buffer, nbytes, 0) == -1)
                                    //{
                                    //    perror("send");
                                    //}
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
        }*/
    }
}

int main()
{

    std::cout << "running server." << std::endl;
    std::vector<int>* test = serverMapGen();
    std::vector<char> test2;
    
    std::vector<int>* test3 = new std::vector<int>();
    pack(test, &test2, 2); //pack map into 3 bits
    unpack(&test2, test3, 2);
    //displayMap(test3);
    // std::cout << "Binary Representation of map : \n";
	//for(auto curr : test2){
	//	std::cout << (int)(curr >> 7 & 1) << (int)(curr >> 6 & 1) << (int)(curr >> 5 & 1) << (int)(curr >> 4 & 1) << (int)(curr >> 3 & 1) << (int)(curr >> 2 & 1) << (int)(curr >> 1 & 1) << (int)(curr & 1) << '\n';
	//}
    //test 2 is our packed map
    delete test; //delete the map from memory
    std::cout << (int) test2.size() << "\n";
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
    if ((status = getaddrinfo("0.0.0.0", "8123", &hints, &serverInfo)) != 0)
    {
        printf("Failed to get address info.");
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

    // Accept a connection

    // Set timevalue to 2 seconds and 500000 microsecond
    struct timeval tv = {2, 500000};

    // Add listener to master set
    FD_SET(listenerfd, &master);

    fdmax = listenerfd;

    int newfd;
    int nbytes;
    int i, j;
    char* tsBuffer = (char*) calloc(152, sizeof(char)); 
    std::vector<char>* workingBuffer;
    std::cout << "Creating send thread" << std::endl;
    gameOn = true;
    SDL_CreateThread(sendThread, "send thread", (void*) tsBuffer);

    //Game stuff here
    while(gameOn){
        
    }
}