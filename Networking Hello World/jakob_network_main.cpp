#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>

int main() {
    int status; //What is the status of host
    struct addrinfo hints;
    struct addrinfo *serverInfo;    // The resolved information from getting the adress infor
    int socketfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 and IPv6 are fine
    hints.ai_socktype = SOCK_STREAM; // Use TCP connection

    // Do a lookup of the server
    getaddrinfo("www.google.com", "80", &hints, &serverInfo);

    //Usinng the resturned serverInfo, attempt to open connection (socket)
    //server->ai_family is defined by what the server wants to use, we left it unspecified.
    //serverInfo->ai_socktype is TCP; we set in hints
    socketfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    if(socketfd == -1) { // failed to open socket
        printf("Failed to open socket: Address = %s ", serverInfo->ai_addr->sa_data);
    }
    else {
        printf("Connection was successful on address (%s)", serverInfo->ai_family);
    }
    


}