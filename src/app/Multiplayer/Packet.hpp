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
        std::vector<std::string> datas;
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

        Packet(Header size, Header type, const char* data) {
            headers.push_back(size);
            headers.push_back(type);

            char *temp_data = strtok((char *)data, " ");
            while (temp_data != nullptr) {
                datas.push_back(temp_data);
                temp_data = strtok(NULL, " ");
            }
        }

        // Setters
        void appendData(std::string d) {
            datas.push_back(d);
        }

        void insetData(std::string d, int index) {
            datas.insert(datas.begin()+index, d);
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
        std::tuple<std::vector<Header>, std::vector<std::string>> getTuple() {
            return std::make_tuple(headers, datas);
        }
        
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

        std::vector<std::string> getData() {
            return datas;
        }

        std::vector<char*>* get() {
            std::vector<char*> *raw = new std::vector<char*>();
            // Set up headers
            for (auto head : headers) {
                // Headers should only have two values so this is okay
                auto temp = head.get()->data();
                raw->push_back(temp[0]);
                raw->push_back(temp[1]);
                delete temp[0];
                delete temp[1];
            }

            for (auto d : datas) {
                raw->push_back((char *) d.data());
            }
            return raw;
        }

        std::string* dataString() {
            std::string *raw_data = new std::string();
            for (auto head : headers) {
                raw_data->append(head.data());
            }

            for (auto d : datas) {
                raw_data->append(d.data());
            }

            return raw_data;
        }

        const char* data() {
            std::string *raw_data = new std::string();
            for (auto head : headers) {
                raw_data->append(head.data());
                raw_data->push_back(' ');
                // std::cout << "HEAD: " << head.data() << " | ";
            }

            for (auto d : datas) {
                raw_data->append(d.data());
                raw_data->push_back(' ');
                // std::cout << "DATA: " << d.data() << std::endl;
            }

            return raw_data->data();
        }

        size_t size() {
            size_t s = 0;

            // Get the size of the headers
            for (auto i = headers.begin(); i != headers.end(); i++) {
                s += i->size() + 1; // + 1 = push_back(' ');
            }

            // Get the size of the data 
            for (auto i = datas.begin(); i != datas.end(); i++) {
                s += i->size() + 1;
            }

            return s;
        }
};

#endif