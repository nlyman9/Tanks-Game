#include "Network.hpp"
class Network{
    public:
    void sender(int fd, std::vector<int>* buffer, size_t t, int flags, int csFlag){
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
                send(fd, buffer->data, t, flags);
                break;
        }
    }
    void sender(int fd, std::vector<char>* buffer, size_t t, int flags, int csFlag){
        switch(csFlag){
            case 0:
                send(fd, buffer->data, t, flags); 
                break;
            case 1:
                send(fd, buffer->data, t, flags); //already compressed and delete
                delete buffer;
                break;
            //case 2: 
            default:
                send(fd, buffer->data, t, flags); 
                break;
        }
    }
    private:
    class packetHeader{
        //to be filled out
        public:
        char header[headersize];
    };
    void createPacket(packetHeader* hdr, char* buffer, int size){
        char packet[size];
        strcat(packet, hdr->header);
        strcat(packet, buffer);
    }
    std::vector<char> *packMap(std::vector<int>* map, std::vector<char>* mapPacked)
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
    void sendMap(int fd, std::vector<int>* map, size_t t, int flags, int csFlag){
        //create a char array to send it in
        std::vector<char>* mapPacked = new std::vector<char>();   
        packMap(map, mapPacked);
        std::cout << (int) mapPacked->size() << "\n";
        send(fd, mapPacked->data, t, 0);
    }
}