// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <iostream>
#define PORT 8080 

int main(int argc, char const *argv[]) 
{ 
    bool host = true;
    char* s = new char[24];
    
    std::cin >> s;

    if(strcmp(s, "host") == 0){
        std::cout << "hosting\n" << std::flush;
        host = true;
    }else if(strcmp(s, "client") == 0){
        std::cout << "trying to connect\n" << std::flush;
        host = false;
    }

    //if hosting the server
    if(host){
        int server_fd, ssocket, valread; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 
        char buffer[1024] = {0}; 
        char *hello = "Hello from server\n"; 
        
        // Creating socket file descriptor 
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            std::cout << "socket failed";
            exit(EXIT_FAILURE); 
        } 
        
        // Forcefully attaching socket to the port 8080 
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                    &opt, sizeof(opt))) 
        { 
            std::cout << "setsockopt function failed";
            exit(EXIT_FAILURE); 
        } 

        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( PORT ); 
        
        // Forcefully attaching socket to the port 8080 
        if (bind(server_fd, (struct sockaddr *)&address,  
                                    sizeof(address))<0) 
        { 
            std::cout << "binding failed";
            exit(EXIT_FAILURE); 
        } 
        if (listen(server_fd, 3) < 0) 
        { 
            std::cout << "listen failed";
            exit(EXIT_FAILURE); 
        } 
        if ((ssocket = accept(server_fd, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0) 
        { 
            std::cout << "accept failed";
            exit(EXIT_FAILURE); 
        } 
        valread = read( ssocket , buffer, 1024); 
        std::cout << buffer;
        send(ssocket , hello , strlen(hello) , 0 ); 
        std::cout << "Hello message sent\n"; 
        return 0; 
    }
    //if client
    else{
        int sk = 0, valread; 
        struct sockaddr_in serv_addr; 
        char *hello = "Hello from client\n"; 
        char buffer[1024] = {0}; 
        if ((sk = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        { 
            std::cout << "\nSocket creation error\n"; 
            return -1; 
        } 
    
        serv_addr.sin_family = AF_INET; 
        serv_addr.sin_port = htons(PORT); 
        
        // Convert IPv4 and IPv6 addresses from text to binary form 
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
        { 
            std::cout << "\nInvalid address\n"; 
            return -1; 
        } 
    
        if (connect(sk, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        { 
            std::cout << "\nConnection Failed \n"; 
            return -1; 
        } 
        send(sk , hello , strlen(hello) , 0 ); 
        std::cout << "Hello message sent\n"; 
        valread = read( sk , buffer, 1024); 
        std::cout << buffer ; 
        return 0; 
    }
} 