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

#include <SDL2/SDL.h>

SDL_Window* gWindow;
SDL_Surface* gSurface;
SDL_Renderer* gRenderer;
constexpr int SCREEN_HEIGHT = 720;
constexpr int SCREEN_WIDTH = 1296;
constexpr int TILE_SIZE = 48;

class Client
{
public:
    int sockfd;
    struct sockaddr *clientAddr;
    Client(int fd, sockaddr *addr) : sockfd{fd}, clientAddr{addr} {}
    ~Client()
    {
        delete &sockfd;
        delete clientAddr;
        printf("Closing client connection %d", sockfd);
    }
};
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
std::vector<char> *packMap(std::vector<int> map, std::vector<char>* mapPacked)
{
    /*
        bring in an array of int representing the map
        the map is 15 tiles downward and 27 tiles across
        each tile can be represented by 3 bits
        that ends up with 405 possible tiles and 1215 bits
        which is 151.875 -> 152 chars
        Rules for each tile
        bit     descr     integer rep
        000 - Passable      -  0
        001 - hole          -  1
        010 - wall/obstacle -  2
        011 - bomb tile     -  3
        100 - destroyed ob  -  4
        101 - UNUSED        -  5
        110 - UNUSED        -  6
        111 - UNUSED        -  7
    */
    /*
        since c++ is not bit addressable, I need to create a bool vector
        for each int in the map array
        I need to isolate the bits and push them in to the workingSet bool array
    */
    int x = 0;
    int y = 0;
    SDL_Rect currentTile;
    std::vector<bool> workingSet;
    for (auto curr : map)
    {
        bool bit2 = (bool)(curr >> 2 & 1);
        bool bit1 = (bool)((curr >> 1) & 1);
        bool bit0 = (bool)((curr) & 1);

        workingSet.push_back(bit2);
        //std::cout << (curr >> 2 & 1);
        workingSet.push_back(bit1);
        //std::cout << (curr >> 1 & 1);
        workingSet.push_back(bit0);
        //std::cout << (curr & 1);

        // Extract row and column from the 1D vector
        if(x == 27) {
            y++;
            x = 0;
        }
        currentTile = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        if(!bit2 && !bit1 && !bit0) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        }
        if(!bit2 && !bit1 && bit0) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0x00);
        }
        if(!bit2 && bit1 && !bit0) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
        }
        if(!bit2 && bit1 && bit0) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
        }
        if(bit2 && !bit1 && !bit0) {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
        }
        SDL_RenderFillRect(gRenderer, &currentTile);
        x++;
	}
    SDL_RenderPresent(gRenderer);

    /*
        working set is a bool vector, but I need to return a char array
        so I need to take every 8 bools and pack them into a char
    */
   	int i = 0;
	char temp = 0;
	for(auto currInSet : workingSet){
		//std::cout << currInSet;
		temp = temp | ((char) currInSet << (7-i));
		if((i == 7) && i != 0){
			mapPacked->push_back(temp);
			temp = 0;
			i = -1;
		}
		i++;
	}
	if(i+1 < 8 && i != 0) 
        mapPacked->push_back(temp); //push back the last temp if the above array did not align
	//std::cout << "Binary Representation of map : \n";
	//for(auto curr : *mapPacked){
	//	std::cout << (int)(curr >> 7 & 1) << (int)(curr >> 6 & 1) << (int)(curr >> 5 & 1) << (int)(curr >> 4 & 1) << (int)(curr >> 3 & 1) << (int)(curr >> 2 & 1) << (int)(curr >> 1 & 1) << (int)(curr & 1) << '\n';
	//}
    return mapPacked;
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

bool initSDL() 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	gWindow = SDL_CreateWindow("Server", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// SEt up rendered with out vsync
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

    return true;
}
void displayMap(std::vector<int>* map){
    std::cout << map->size() << std::endl;
    int i = 0;
    int x = 0;
    int y = 0;
    SDL_Rect currentTile;
    for(i = 0 ; i < map->size() ; i++, x++){
        if(x == 27) {
            y++;
            x = 0;
        }
        currentTile = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        std::cout << map->at(i) << " ";
        switch(map->at(i))
        {   
            case 0: 
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                break;
            case 1:
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0x00);
                break;
            case 2:
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
                break;
            default:
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0x00);
                break;
        }
        SDL_RenderFillRect(gRenderer, &currentTile);
    }
    std::cout  << std::endl;
    SDL_RenderPresent(gRenderer);
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
int main()
{
    if(!initSDL()) {
        std::cout << "Unsuccessful SDL initalization" << std::endl;
        exit(1);
    }

    std::vector<int>* test = new std::vector<int>();
    std::vector<char> test2;
    //create a map that just cycles through the tiles
    for(int i = 0; i < 405; i++) {
        test->push_back(i % 3);
    }
    std::vector<int>* test3 = new std::vector<int>();
    pack(test, &test2, 2); //pack map into 3 bits
    unpack(&test2, test3, 2);
    displayMap(test3);
    // std::cout << "Binary Representation of map : \n";
	//for(auto curr : test2){
	//	std::cout << (int)(curr >> 7 & 1) << (int)(curr >> 6 & 1) << (int)(curr >> 5 & 1) << (int)(curr >> 4 & 1) << (int)(curr >> 3 & 1) << (int)(curr >> 2 & 1) << (int)(curr >> 1 & 1) << (int)(curr & 1) << '\n';
	//}
    //test 2 is our packed map
    std::cout << (int) (*test).size() << " ";
    delete test; //delete the map from memory
    std::cout << (int) test2.size() << "\n";
    // Set structs and variables for the internet
    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo, *p;
    struct sockaddr_storage remoteaddr; // client address; for setting new connections
    int listenerfd;

    socklen_t addr_len = sizeof(struct sockaddr_storage);
    fd_set master;   // Master of file descriptors
    fd_set read_fds; // Read fd's returned from select
    int fdmax;       // maximym file descriptor number

    char remoteIP[INET_ADDRSTRLEN];
    char buf[105];

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
    char buffer[100];
    // Loop of server
    while (1)
    {
        read_fds = master;
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
                        // printf("New connection from %s on socket %d.\n",
                        //        inet_ntop(remoteaddr.ss_family, &remoteaddr, remoteIP, INET_ADDRSTRLEN),
                        //        newfd);
                        //new connection so need to send map data here accepted so send data
                        send(newfd, test2.data(), test2.size(), 0);
                    }
                }
                else
                {
                    // Handle data from clients
                    if ((nbytes = (recv(i, buf, 100, 0))) <= 0)
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

                                    sprintf(buffer, "User %d: %.90s", i, buf);

                                    if (send(j, buffer, nbytes, 0) == -1)
                                    {
                                        perror("send");
                                    }
                                }
                                else if (j == listenerfd)
                                {
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