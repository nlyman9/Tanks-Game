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

#define NUM_PLAYERS 2
//bools for receive
bool receiveReady = true;
bool receivedData = false;

//bools for send
bool sendReady = false;
bool sendData = false;
int connections = 0;

//receive lock
SDL_SpinLock rlock = 0;
//send lock
SDL_SpinLock slock = 0;

Server *server;

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

    std::string server_ip = std::string(argv[1]);
    char *server_port = argv[2];

    server = new Server(server_ip, std::atoi(server_port));

    // Bind and start listening as server
    server->bind();
    server->listen();

    // SDL_CreateThread(serverThread, "server thread", NULL);

    serverProcess();
    
}

/**
 * @brief The server process - Basically main when a game is being prepared and running
 * 
 * @return int 
 */
int serverProcess() {
    //timeval is passed into select and is used to say when select should timeout
    //setting all the members to 0 tells select it should not block
    struct timeval* timeout = (timeval*) calloc(1, sizeof(struct timeval));
    timeout->tv_sec = 0;
    timeout->tv_usec = 0;

    // Generate map
    auto map = serverMapGen();
    std::cout << "Map size is " << map->size() << std::endl;
    pack(map, &packedMap, 3); //pack map into 3 bits


    // First wait for 2 clients
    while (server->numClients() < 2) {
        if (server->accept()) {
            std::cout << "Server: New client connection accepted" << std::endl;
        }

        std::cout << "Server: Looping - numClients = " << server->numClients() << std::endl;
        sleep(1);
    }

    // Send map to the clients!
    std::cout << "Server: Preparing to send map!" << std::endl;
    Packet *mapPacket = new Packet(PackType::MAP);
    std::cout << "Map size is " << packedMap.size() << std::endl;
    mapPacket->appendData(packedMap);
    server->broadcast(mapPacket);

    server->gameOn = true;

    // Game Loop
    // TODO simulate the game on server's side?
    std::vector<ClientConnection*> *pendingClients;
    while (server->gameOn) {
        std::cout << "\n\nSERVER: GAME - # Clients = " << server->numClients() << std::endl;
        fflush(stdout);

        // Poll clients for pending messages 
        // This function calls receive! Do not call again unless you have a specific reason
        // The polling (select function) currently waits for a specified timeout value 
        //      @see ServerConnection for timeout value
        while ( (pendingClients = server->pollClientsAndReceive()) == nullptr) {
            // Just go back and pollClientsAndReceive()
        }
        std::cout << "SERVER: Going to check mailbox!!!" << std::endl;
        fflush(stdout);

        // Get packages from pending clients
        // pendingClients can be null IF:
        //      + we havent received data but we need to send a keyframe
        Packet *mail;
        if (pendingClients != nullptr) {
            for (auto client : *pendingClients) {
                std::cout << "SERVER: Getting packet from client " << client->id() << std::endl;
                fflush(stdout);
                mail = server->getPacket(client->id());
                if (mail != nullptr) {
                    std::cout << "SERVER: You got mail!" << std::endl;
                    mail->printData();
                    if (mail->getType() == PackType::KEYSTATE) {
                        // If a keystate, Prepare to send that client's keystate to the other clients 
                        server->addPacketFromClientToClients(client->id(), mail);
                    }
                    fflush(stdout);
                } else {
                    std::cout << "SERVER: No mail :(" << std::endl;
                    fflush(stdout);
                }
            }
        }

        // Share data with clients - send data from buffer
        for (auto client : server->clients()) {
            std::cout << "SERVER: TO " << client->id() << ": ";
            client->sendFromBuffer();
        }

        // TODO Send Game States
        // Game states -> slower rate 
        // server->broadcast(GAMESTATE?);
    }

    return 1;
}

/**
 * @brief Generate a map on the server to sennd to clients
 * 
 * @return std::vector<int>* 
 */
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

// get sockaddr, IPv4 or IPv6: 
//  Code from Beej's guide
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}