#ifndef PACKET_HPP
#define PACKET_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>

#include "Header.hpp"

enum class PackType {
    INIT = 0,
    MAP,        // 1
    KEYSTATE,   // 2
    MAPSTATE,   // 3
    KEYFRAME,   // 4
    NUM_TYPES   // 5
};

class Packet {
    private:
        std::vector<Header> headers;
        std::vector<char> datas;
        size_t packet_size;

    public:
        Packet(PackType type)
        {   
            // The header for size is the first header.
            Header packet_size = Header("SIZE", "00000000");
            headers.push_back(packet_size);

            // The header for the type is the second header.
            Header packet_type = Header("TYPE", std::to_string((int)type));
            headers.push_back(packet_type);

            setPacketSize();
            std::cout << "Initialized packet with  " << headers.size() << " headers - Data: " << this->data() << std::endl;
        }

        Packet(Header size, Header type, const char* data, int data_size) {
            headers.push_back(size);
            headers.push_back(type);
            for (int i = 0; i < data_size; i++) {
                datas.push_back((char) data[i]);
            }
            std::cout << "Appended data = " << std::string(datas.data(), datas.size()) << std::endl;
            
        }

        // Setters
        void appendData(std::string d) {
            for (char c : d) {
                datas.push_back(c);
            }
            setPacketSize();
        }

        void appendData(std::vector<int> d) {
            for (int val : d) {
                datas.push_back(val);
            }
            std::cout << "Appended data = " << std::string(datas.data(), datas.size()) << std::endl;
            setPacketSize();
        }

        void appendData(std::vector<char> d) {
            for (char c : d) {
                datas.push_back(c);
            }
            setPacketSize();
        }

        void setPacketSize() {
            Header& packet_size = headers.at(0);
            std::cout << "Who is this pakcet ??" << packet_size.getHead() << std::endl;
            fflush(stdout);
            // Make sure the header is the size header
            assert(packet_size.getHead().compare("SIZE") == 0);

            std::string temp_size = std::to_string(this->size());
            std::cout << "Size is = " << temp_size << std::endl;

            // The size of a packet may only be of size 
            assert(temp_size.size() <= 8);

            // Pad with leading zeroes 
            std::string size_str = std::string(8 - temp_size.size(), '0').append(temp_size);


            packet_size.setValue(size_str);
            std::cout << "Set size of packet to " << size_str << std::endl;
        }


        // Getters 
        /**
         * @brief Get the Headers object
         * @warning Using a for-each loop causes segfaults, 
         *     use a normal for loop to iterate, or an iterator.
         * 
         * @return std::vector<Header> 
         */
        std::vector<Header> getHeaders() {
            return headers;
        }

        std::vector<char>* getDatas() {
            return new std::vector<char>(datas);
        }

        std::string getDatasString() {
            return std::string(datas.data(), datas.size());
        }

        int getType() {
            return atoi(headers.at(1).getValue().c_str());
        }

        const char* data() {
            std::string *raw_data = new std::string();
            for (auto head : headers) {
                raw_data->append(head.data());
                raw_data->push_back(' ');
                // std::cout << "HEAD: " << head.data() << " | ";
            }
            raw_data->append(std::string(datas.data(), datas.size()));

            return raw_data->data();
        }

        size_t size() {
            size_t s = 0;

            // Get the size of the headers
            for (auto i = headers.begin(); i != headers.end(); i++) {
                s += i->size() + 1; // + 1 = push_back(' ');
            }

            // Get the size of the data 
            s += datas.size();

            return s;
        }
};

#endif