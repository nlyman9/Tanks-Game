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

#include <SDL2/SDL.h>

SDL_Window* gWindow;
SDL_Surface* gSurface;
SDL_Renderer* gRenderer;
constexpr int SCREEN_HEIGHT = 720;
constexpr int SCREEN_WIDTH = 1296;
constexpr int TILE_SIZE = 48;
bool mapRecieved = false;
std::vector<int> *unpack(std::vector<char>* packed, std::vector<int> *unPacked, int bits){
	std::vector<bool> workSet;
    //first turn the packed map into a bool array
	for(auto curr : mapPacked){
		for(int i = 0; i < 8; i++){
			workSet.push_back((bool)((curr >> (7 - i)) & 1));
			//std::cout << (int) ((curr >> (7 - i)) & 1);
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
    mapRecieved = true;
    std::cout << unPacked->size() << std::endl;
    return unPacked;
}
void displayMap(std::vector<int>* map){
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
    std::cout << std::endl;
    SDL_RenderPresent(gRenderer);
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

	gWindow = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

int main() {

    if(!initSDL()) {
        std::cout << "Unsuccessful SDL initalization" << std::endl;
        exit(1);
    }

    int status;
    struct addrinfo hints;
    struct addrinfo *serverInfo;
    int sockfd;
    char buffer[105];

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
            nbytes = recv(sockfd, buffer, 500, 0);
            if (nbytes <= 0) {
                printf("Closing connection to server.");
                close(sockfd);
                exit(10);
            } else {
                //recieved data
                //for this test only recieving the map data
                std::vector<char> test(buffer, buffer + (sizeof(buffer)/sizeof(buffer[0])));
                std::cout << test.size() << std::endl;
                std::vector<int> map;
                unpackMap(test, &map);

                int x = 0;
                int y = 0;
                SDL_Rect currentTile;
                std::cout << (int) map.size() << " " << nbytes << "\n";
                for(auto tile : map){
                    std::cout << (int) tile << " ";
                    
                    // Extract row and column from the 1D vector
                    if(x == 27) {
                        y++;
                        x = 0;
                    }

                    currentTile = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    if(tile == 0) {
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                    }
                    if(tile == 1) {
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0x00);
                    }
                    if(tile == 2) {
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0x00);
                    }
                    if(tile == 3) {
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
                    }
                    if(tile == 4) {
                        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                    }
                    SDL_RenderFillRect(gRenderer, &currentTile);
                    x++;
                }
                SDL_RenderPresent(gRenderer);
                std::cout << " \n";
            }
        } else if (fgets(buffer, 100, stdin) != NULL) { // get input from user
            // Check for user input in the terminal 
            //printf("%s", buffer);
            //send data
            send(sockfd, buffer, 100, 0);
        }

    }
}