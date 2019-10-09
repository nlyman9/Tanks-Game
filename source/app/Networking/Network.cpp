#include "Network.hpp"

void Network::sender(int fd, std::vector<int>* buffer, size_t t, int flags, int csFlag){
    switch(csFlag){ //can decide how to handle this -- bitwise?
        case 0:
            sendMap(fd, buffer, t, flags, csFlag); //dont delete map
            break;
        case 1:
            sendMap(fd, buffer, t, flags, csFlag); //delete buffer
            delete buffer;
            break;
        //case 2: 
        default:
            send(fd, buffer, t, flags);
            break;
    }
}
void Network::sender(int fd, std::vector<char>* buffer, size_t t, int flags, int csFlag){
    switch(csFlag){
        case 0:
            send(fd, buffer, t, flags); 
            break;
        case 1:
            send(fd, buffer, t, flags); //already compressed and delete
            delete buffer;
            break;
        //case 2: 
        default:
            send(fd, buffer, t, flags); 
            break;
    }
}

//class packetHeader{
    //to be filled out
    //public:
    //char header[headersize];
    //private:
    //int headersize = tbd
//};
//void createPacket(packetHeader* hdr, char* buffer, char* packet){
//    strcat(packet, hdr->header);
//    strcat(packet, buffer);
//}
//idea: change to n - bit compression
//functions ideas: packInt : packChar - pack a vector of ints or chars
//change it to take in N bits you want to pack to?

std::vector<char>* Network::packMap(std::vector<int>* map, std::vector<char>* mapPacked)
{

    std::vector<bool> workingSet;
    for (auto curr : *map)
    {
        workingSet.push_back((bool)(curr >> 2 & 1));
        workingSet.push_back((bool)((curr >> 1) & 1));
        workingSet.push_back((bool)((curr) & 1));
    }
    int i = 0;
    char temp = 0;
    for(auto currInSet : workingSet){
        temp = temp | ((char) currInSet << (7-i));
        if((i == 7) && i != 0){
            mapPacked->push_back(temp);
            temp = 0;
            i = -1;
        }
        i++;
    }
    if(i+1 < 8 && i != 0) 
        mapPacked->push_back(temp); 

    return mapPacked;
}
void Network::sendMap(int fd, std::vector<int>* map, size_t t, int flags, int csFlag){
    //create a char array to send it in
    std::vector<char>* mapPacked = new std::vector<char>();   
    packMap(map, mapPacked);
    std::cout << (int) mapPacked->size() << "\n";
    //createPacket(some header, mapPacked->data, some packet)
    //send(fd, some packet, t, 0)
    send(fd, mapPacked, t, flags);
}
/*
Function Ideas:

int? initNetworkClient(){} call this before game loop to initialize client - connect to server
int? initNetworkMain(){} call this in the main server ?? or make this the main server code ?? or make a seperate main server code ??

bool? checkPacket(){} bring in the buffer and sort out if data was recieved and the packet was sent in its entirety
void resolvePacket(){} reads the packet header and splits/decides what to do with the data

Ideas for packet header:

type - tells what type of data is sent - 0 keystroke data 1 absolute data 2 delta data  - 2 bits?
data - what is the data for? playerN? movement? position? map? -- could also use this to resolve the data type - ints? chars? vector?
compression type - is the data compressed? 0 - no compression 1 - 1bit per field 2 - 2 bits per field etc
size - the size of the packet
*/