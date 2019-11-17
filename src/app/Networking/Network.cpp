#include "Network.hpp"

std::vector<char>* pack(std::vector<int>* x, std::vector<char>* packed, int bits)
{   
    std::vector<bool> workingSet;
    // std::cout << "Packing: ";
    for (auto curr : *x)
    {
        std::cout << curr;
        int i;
        for(i = 0; i < bits; i++){
            workingSet.push_back((bool) (curr >> (bits - i - 1) & 1));
        }
    }
    std::cout << std::endl;
    int i = 0;
    char temp = 0;
    // std::cout << "Result: ";
    for(auto currInSet : workingSet){
        temp = temp | ((char) currInSet << (7-i));
        if(i == 7){
            packed->push_back(temp);
            // printf(" %d ", temp);
            temp = 0;
            i = -1;
        }
        i++;
    }
    if(i != 0) 
        packed->push_back(temp); 

    return packed;
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
    // std::cout << "Result: ";
	for(auto curr : workSet){
		tmp = (tmp) | (curr << (bits - 1 - i));
		if(i == (bits - 1)){
			unPacked->push_back(tmp);
            // printf("%d", tmp);
			tmp = 0;
			i = -1;
		}
		i++;
	}
    return unPacked;
}

int stripHeader(std::vector<char>* packet){
    int toRet = (int) packet->at(0);
    packet->erase(packet->begin());
    std::cout << "packet header: " << toRet << std::endl;
    return toRet;
}
void appendHeader(std::vector<char>* packet, char toAppend){
    packet->insert(packet->begin(), toAppend);
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