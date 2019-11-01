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
//constexpr int headersize = 20;
void sender(int fd, std::vector<int>* buffer, size_t t, int flags, int csFlag);
void sender(int fd, std::vector<char>* buffer, size_t t, int flags, int csFlag);
std::vector<char>* pack(std::vector<int>* x, std::vector<char>* packed, int bits);
std::vector<int> *unpack(std::vector<char>* packed, std::vector<int> *unPacked, int bits);
std::vector<int> *unpack(std::vector<char>* packed, std::vector<int> *unPacked, int bits, int numbers);
std::vector<int> *unpackMap(std::vector<char> mapPacked, std::vector<int> *map);
std::vector<char>* packMap(std::vector<int>* map, std::vector<char>* mapPacked);
void sendMap(int fd, std::vector<int>* map, size_t t, int flags, int csFlag);
void appendHeader(std::vector<char>* packet, char toAppend);
int stripHeader(std::vector<char>* packet);
#endif