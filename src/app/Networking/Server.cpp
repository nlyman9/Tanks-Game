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
#include "Constants.hpp"

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

    // First wait for 2 clients
    while (server->numClients() < 2) {
        if (server->accept()) {
            std::cout << "Server: New client connection accepted" << std::endl;
        }

        std::cout << "Server: Looping - numClients = " << server->numClients() << std::endl;
        sleep(1);
    }
    //initialize the server lists
    server->initPlayerAndMailLists();

    // Set player starting positions
    //              { Player 1 , Player 2 }
    int x_pos[2] = {100, SCREEN_WIDTH - 100};
    int y_pos[2] = {50, SCREEN_HEIGHT - 50};
    Player* player1 = new Player(x_pos[0], y_pos[0], false);
    Player* player2 = new Player(x_pos[1], y_pos[1], false);
    server->addPlayer(player1);
    server->addPlayer(player2);
    //set time
    server->setStartTime();
    // Initialize Player Data
    for (int i = 0; i < server->numClients(); i++) {
        // Packet playerID, inital position
        std::vector<char> data;
        // Add Player's ID
        data.push_back(std::to_string(i)[0]); 
        data.push_back(' ');

        // Tell client where player 1 starts
        data.push_back(std::to_string(0)[0]); 
        data.push_back(' ');

        std::string xP1 = std::to_string(x_pos[0]); 
        for (auto x_character : xP1) {
            data.push_back(x_character); // Player 1 x
        }
        data.push_back(' ');

        std::string yP1 = std::to_string(y_pos[0]); 
        for (auto y_character : yP1) {
            data.push_back(y_character); // Player 1 y
        }
        data.push_back(' ');

        // Tell client where player 2 starts
        data.push_back(std::to_string(1)[0]); 
        data.push_back(' ');

        std::string xP2 = std::to_string(x_pos[1]); 
        for (auto x_character : xP2) {
            data.push_back(x_character); // Player 2 x
        }
        data.push_back(' ');

        std::string yP2 = std::to_string(y_pos[1]); 
        for (auto y_character : yP2) {
            data.push_back(y_character); // Player 2 y
        }
        data.push_back(' ');

        std::string startTimeStr = server->getStartTimeStr(); 
        for (auto character : startTimeStr) {
            data.push_back(character); // Time stamp
        }
        data.push_back(' ');

        std::cout << "Server: Preparing to send init data to player " << i << "!" << std::endl;
        Packet *initPacket = new Packet(PackType::INIT);
        std::cout << "Init packet size is " << data.size() << std::endl;
        initPacket->appendData(data);
        server->sendPacketToClient(i ,initPacket);
    }

    // Generate map
    auto map = serverMapGen();
    std::cout << "Map size is " << map->size() << std::endl;
    pack(map, &packedMap, 3); //pack map into 3 bits

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
    server->time_since_last_keyframe = std::chrono::system_clock::now();
    while (server->gameOn) {
#ifdef VERBOSE
        std::cout << "\n\nSERVER: GAME - # Clients = " << server->numClients() << std::endl;
        fflush(stdout);
#endif

        // Poll clients for pending messages 
        // This function calls receive! Do not call again unless you have a specific reason
        // The polling (select function) currently waits for a specified timeout value 
        //      @see ServerConnection for timeout value
        while ( (pendingClients = server->pollClientsAndReceive()) == nullptr) {
            // Just go back and pollClientsAndReceive()
            try{
                if(server->simulate()){
                    //create packet of gamestate and broadcast
                    if((std::chrono::system_clock::now() - server->time_since_last_keyframe) > std::chrono::seconds{1}){
                        server->time_since_last_keyframe - std::chrono::system_clock::now();
                        Packet* gamestatepacket = server->getGamestatePacket();
#ifdef VERBOSE                    
                    std::cout << "SERVER: Sending keyframe - ";
                    gamestatepacket->printData();
#endif
                        server->broadcast(gamestatepacket);
                    }
                }else{
#ifdef VERBOSE
                    std::cout << "Failed to simulate game" << std::endl;
#endif
                    server->reset_lag_time();
                }
            }catch (const std::exception &exc){
                std::cerr << exc.what();
            }
        }
#ifdef VERBOSE
        std::cout << "SERVER: Going to check mailbox!!!" << std::endl;
        fflush(stdout);
#endif

        // Get packages from pending clients
        // pendingClients can be null IF:
        //      + we havent received data but we need to send a keyframe
        Packet *mail;
        if (pendingClients != nullptr) {
            for (auto client : *pendingClients) {
#ifdef VERBOSE
                std::cout << "SERVER: Getting packet from client " << client->id() << std::endl;
                fflush(stdout);
#endif
                mail = server->getPacket(client->id());
                if (mail != nullptr) {
#ifdef VERBOSE
                    std::cout << "SERVER: You got mail!" << std::endl;
                    mail->printData();
#endif
                    if (mail->getType() == PackType::KEYSTATE) {
                        //copy the data of the mail
                        server->setMail(mail, client->id());
                        // If a keystate, Prepare to send that client's keystate to the other clients 
                        server->addPacketFromClientToClients(client->id(), mail);
                    }
                    fflush(stdout);
                } else {
#ifdef VERBOSE
                    std::cout << "SERVER: No mail :(" << std::endl;
                    fflush(stdout);
#endif
                }
            }
        }

        // Share data with clients - send data from buffer
        for (auto client : server->clients()) {
#ifdef VERBOSE
            std::cout << "SERVER: TO " << client->id() << ": ";
#endif
            client->sendFromBuffer();
        }
        //simulate the game
        try{
            if(server->simulate()){
                //create packet of gamestate and broadcast
                if((std::chrono::system_clock::now() - server->time_since_last_keyframe) > std::chrono::seconds{1}){
                    server->time_since_last_keyframe - std::chrono::system_clock::now();
                    Packet* gamestatepacket = server->getGamestatePacket();
                    server->broadcast(gamestatepacket);
#ifdef VERBOSE
                std::cout << "SERVER: Sending keyframe - ";
                gamestatepacket->printData();
#endif
                }
            }else{
#ifdef VERBOSE
                std::cout << "Failed to simulate game" << std::endl;
#endif
            server->reset_lag_time();
            }
        }catch (const std::exception &exc){
            // catch anything thrown within try block that derives from std::exception
            std::cerr << exc.what();
        }
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
    std::vector<SDL_Rect> tileArray;
    int count = 0;
    for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
        for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
            SDL_Rect cur_out = { x, y, TILE_SIZE, TILE_SIZE};
            SDL_Rect hole_tile = { x+5, y+5, TILE_SIZE-5, TILE_SIZE-5 }; //does not work, enemy AI needs update
            if(map[i][j] == 2) {
                tileArray.push_back(cur_out);
                server->getProjectileObstacles().push_back(cur_out);
            } else if(map[i][j] == 1) {
                tileArray.push_back(hole_tile);
            }
        }
    }
    for(int i = 0; i < server->numClients(); i++) {
        server->getPlayer(i)->setObstacleLocations(&tileArray);
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