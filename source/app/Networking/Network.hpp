#ifndef __NETWORKING_HPP
#define __NETWORKING_HPP
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
class Network{
    public:
    void sender(int fd, std::vector<int>* buffer, size_t t, int flags, int csFlag);
    void sender(int fd, std::vector<char>* buffer, size_t t, int flags, int csFlag);
    private:
    std::vector<char> *packMap(std::vector<int>* map, std::vector<char>* mapPacked);
    void sendMap(int fd, std::vector<int>* map, size_t t, int flags, int csFlag);
};
#endif