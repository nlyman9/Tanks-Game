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

#define R_BUFFER_SIZE 152
//bools for receive
bool receiveReady = true;
bool receivedData = false;

//bools for send
bool sendReady = false;
bool sendData = false;

//receive lock
SDL_SpinLock rlock = 0;
//send lock
SDL_SpinLock slock = 0;

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

int serverProcess() {
    //timeval is passed into select and is used to say when select should timeout
    //setting all the members to 0 tells select it should not block
    struct timeval* timeout = (timeval*) calloc(1, sizeof(struct timeval));
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;

    while (true) {
        std::cout << "Server is looping" << std::endl;
        sleep(1);
    }
}

int serverThread(){
    int newfd;
    int nbytes;
    int i, j;
    //timeval is passed into select and is used to say when select should timeout
    //setting all the members to 0 tells select it should not block
    struct timeval* timeout = (timeval*) calloc(1, sizeof(struct timeval));
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;

    while (true) {
        std::cout << "Server is looping" << std::endl;
        sleep(1);
    }


    // Loop of server
    // while (gameOn)
    // {
    //     client_fds = master;
    //     // fcntl(listenerfd, F_SETFL, O_NONBLOCK); // non blocking socket
    //     if (select(fdmax + 1, &client_fds, NULL, NULL, timeout) == -1)
    //     {
    //         std::cout << "SERVER: Select error: " << strerror(errno) << std::endl;
    //         exit(4);
    //     }
    //     // Loop through our connections
    //     for (i = 0; i <= fdmax; i++)
    //     {
    //         // Check if one of the connections is in the set client_fds
    //         if (FD_ISSET(i, &client_fds))
    //         {
    //             SDL_AtomicLock(&slock);
    //             if(sendReady){ 
    //                 //data ready to send
    //                 send(i, sBuffer->data(), sBuffer->size(), 0);
    //             }
    //             SDL_AtomicUnlock(&slock);
    //             // Check if it is a new connection
    //             if (i == listenerfd)
    //             { // if listenerfd is in set, we have a new connection -- map sending is separate from other sending as of now
    //                 addr_len = sizeof(remoteaddr);
    //                 newfd = accept(listenerfd, (struct sockaddr *)&remoteaddr, &addr_len);      
    //                 if (newfd == -1)
    //                 {
    //                     // Failed to accept
    //                     std::cout << "SERVER: failed to accept" << std::endl;
    //                     continue;
    //                 }
    //                 else
    //                 {
    //                     // Successfully accepted connection
    //                     FD_SET(newfd, &master); // Add new connection to master list
    //                     if (newfd > fdmax)
    //                     {
    //                         fdmax = newfd; // Keep track of max
    //                     }
    //                     std::cout << "SERVER: New connection from " << inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN) << " on socket " << newfd << std::endl;
    //                     //new connection so need to send map data here accepted so send data
    //                     std::vector<char> toSend;
    //                     //accessing buffer so lock it
    //                     SDL_AtomicLock(&slock);
    //                     //if(ready){ //if the buffer is ready
    //                     for(int i = 0; i < packedMap.size(); i++){
    //                         sBuffer->push_back(packedMap.at(i));
    //                     }
    //                     int size = sBuffer->size();
    //                     appendHeader(sBuffer, (char) 0);
    //                     appendHeader(sBuffer, (char) size);
    //                     send(newfd, sBuffer->data(), sBuffer->size(), 0);
    //                     sBuffer->clear(); 
    //                     //} //end of if ready to send
    //                     //sBuffer sent so unlock it
    //                     SDL_AtomicUnlock(&slock);
    //                 }
    //             }
    //             else
    //             {   
    //                 if(receiveReady){ //if the receive buffer has been copied to the second buffer, be ready to receive more!
    //                     nbytes = recv(i, rBuffer->data(), R_BUFFER_SIZE, 0);
    //                     receiveReady = false;
    //                 }
    //                 std::cout << "SERVER: received: " << nbytes << " bytes" << std::endl;
    //                 std::cout << "SERVER: Locking receive buffer!" << std::endl;
    //                 SDL_AtomicLock(&rlock);
    //                 /* 
    //                 ALWAYS appends to the rcbuffer and only clears the rcBuffer when data has been used
    //                 this makes packet checking more tedious in the gameloop
    //                 add the size of each packet to the header?
    //                 To do this, we can just appendHeader(size,appendHeader(type, buffer))
    //                 strip header -> get size
    //                 strip header again -> get type
    //                 size should not include header/type so we don't overshoot into the next packet in the queue
    //                 */
                    
    //                 if(!receivedData){
    //                     //if we have used up received data, clear the buffer
    //                     rcBuffer->clear();
    //                 }
    //                 //otherwise just push it to the end of the buffer
    //                 //copy the receive buffer to the double buffer for gloop to use
    //                 for(int i = 0; i < R_BUFFER_SIZE; i++)
    //                     rcBuffer->push_back(rBuffer->at(i));
    //                 rBuffer->clear(); //clear the receive buffer
    //                 rBuffer->resize(R_BUFFER_SIZE);
    //                 receivedData = true; 
    //                 receiveReady = true;

    //                 SDL_AtomicUnlock(&rlock);
    //                 std::cout << "SERVER: rBuffer cleared! Unlocked, rcBuffer contains rBuffer!" << std::endl;
                    
    //                 if (nbytes <= 0)
    //                 {
    //                     // Either error or closed connection
    //                     if (nbytes == 0)
    //                     { // Connection closed
    //                         std::cout << "SERVER: Socket " << i <<" disconnected." << std::endl;
    //                         // remember to close the fd
    //                         close(i);
    //                         FD_CLR(i, &master); // Remove from master set
    //                     }
    //                     else
    //                     {
    //                         //client disconnected... for testing purposes just closing the server
    //                         //so we dont have to in task manager
    //                         std::cout << "SERVER: Recv error server server exiting" << std::endl;
    //                         exit(0);
    //                     }
                        
    //                 }
    //                 else if (nbytes > 0)
    //                 {
    //                     // We have real data from client dont really need this for our purposes
    //                     /*for (j = 0; j <= fdmax; j++)
    //                     {
    //                         // relay message to connections
    //                         if (FD_ISSET(j, &master))
    //                         {
    //                             // except the listener and ourselves
    //                             if (j != listenerfd && j != i)
    //                             {
    //                                 std::cout << "SERVER: passing message through" << std::endl;
    //                                 if (send(j, sBuffer->data(), sBuffer->size(), 0) == -1)
    //                                 {
    //                                     std::cout << "SERVER: Send error" << std::endl;
    //                                 }
    //                             }
    //                             else if (j == listenerfd)
    //                             {
    //                                 fflush(stdout);
    //                             }
    //                         }
    //                     }*/
    //                 }
    //             }
    //         }
    //     }

    //     //end of for loop so clear send buffers
    //     SDL_AtomicLock(&slock);
    //     sBuffer->clear();
    //     sendReady = false;
    //     SDL_AtomicUnlock(&slock);
    // }

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
    // //init buffers
    // //buffer received
    // rBuffer = new std::vector<char>(R_BUFFER_SIZE);
    // //double buffered receive
    // rcBuffer = new std::vector<char>();;
    // //to send buffer
    // sBuffer = new std::vector<char>();
    // //buffer to fill in
    // sfBuffer = new std::vector<char>();

    // Get the server option's
    std::cout << "IP " << argv[1] << std::endl;
    std::cout << "PORT " << argv[2] << std::endl;

    std::vector<int>* map = serverMapGen();

    std::string server_ip = std::string(argv[1]);
    char *server_port = argv[2];
    
    pack(map, &packedMap, 3); //pack map into 3 bits

    std::cout << "MAP PACKED " << argv[2] << std::endl;

    Server *server = new Server(server_ip, std::atoi(server_port));

    server->connect();
    server->listen();
    server->accept();

    // SDL_CreateThread(serverThread, "server thread", NULL);

    serverProcess();
    

    // gameOn = true;
    // SDL_CreateThread(serverThread, "server thread", NULL);

    // //Game stuff here
    // while(gameOn){
    //     //beginning of loop check received data
        
    //     if(receivedData){    
    //         SDL_AtomicLock(&rlock); //lock that rcBuffer up
    //         std::cout << "SERVER: Data received, locking buffer." << std::endl;
    //         //apply rcBuffer

    //         //clear rcBuffer
    //         rcBuffer->clear();

    //         //ready to receive more data
    //         receivedData = false; 
    //         std::cout << "SERVER: Unlocking buffer." << std::endl;
    //         SDL_AtomicUnlock(&rlock);
    //     }
        
    //     //game stuff! fill in sfBuffer as we go



    //     //std::cout << "gloop ended, locking buffer" << std::endl;
    //     //fill in the send buffer from the sfBuffer
    //     //if time condition? -- this will be sending the whole game state
    //     SDL_AtomicLock(&slock);
    //     if(!sendReady){ //make sure the other thread is not sending anything atm
    //         //std::cout << "Copying sf buffer into s buffer" <<  std::endl;
    //         //could add a && i < max send buffer size in case its sending too much
    //         //then instead of clear, delete from 0 to max send buffer size and shift
    //         //but that raises issues more than it solves so lets try to avoid
    //         //and send the rest of the data next loop -- still adding more data 
    //         for(int i = 0; i < sfBuffer->size(); i++)
    //             sBuffer->push_back(sfBuffer->at(i));
    //             //set send to ready
    //             sendReady = true;
    //             sfBuffer->clear(); //clear the to send buffer
    //     }
    //     //unlock
    //     SDL_AtomicUnlock(&slock);
    //     //std::cout << "buffer unlocked" << std::endl;
    // }
}