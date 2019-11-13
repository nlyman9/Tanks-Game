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
            Header *init = new Header("TYPE", std::to_string((int)type));
            headers.push_back(*init);
            std::cout << "Initialized packet with # " << headers.size() << " - Data: " << init->data() << std::endl;
        }

        // Setters
        void appendData(std::string d) {
            datas.push_back(d);
        }

        void insetData(std::string d, int index) {
            datas.insert(datas.begin()+index, d);
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
                std::cout << "HEAD: " << head.data() << std::endl;
            }

            for (auto d : datas) {
                raw_data->append(d.data());
                std::cout << "DATA: " << d.data() << std::endl;
            }

            return raw_data->data();
        }

        size_t size() {
            return sizeof(headers) + sizeof(datas);
        }
};

#endif