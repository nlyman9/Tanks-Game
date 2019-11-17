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

/**
 * @brief The packet class is the abstraction used to send data between client-server connections
 * - Contains a vector of headers for the vector
 * - A vector of chars for the data - the body of the packet 
 */
class Packet {
    private:
        std::vector<Header> headers;
        std::vector<char> body;
    public:
        /**
         * @brief Construct a new Packet object
         *  - Each packet requires two heades, 
         *      1. A size header (which should always be first)
         *          + The size header only supports 8 digits (characters), so the size of the packet must fit in 8 digits.
         *          + Thats like 100 MB? 
         *      2. A type header (which should always be seconn)   
         *          + It is based off the enum PackType, so parse with PackType
         * 
         * @param type - The type of packet defined by the PackType enum
         */
        Packet(PackType type)
        {   
            // The header for size is the first header. The size must fit in 8 digits long
            Header packet_size = Header("SIZE", "00000000");
            headers.push_back(packet_size);

            // The header for the type is the second header.
            Header packet_type = Header("TYPE", std::to_string((int)type));
            headers.push_back(packet_type);

            setPacketSize(); // Calculate the size header
        }

        /**
         * @brief Construct a new Packet object with the two necessary headers
         *
         * @param size - The the size
         * @param type - The type header
         */
        Packet(Header size, Header type) {
            headers.push_back(size);
            headers.push_back(type);
        }

        /**
         * @brief Construct a new Packet object with all the necessary data
         *  - Used mainly when we receive data and want to reconstruct the data into a packet
         * 
         * @param size - The size of the packet
         * @param type - The type of the packet
         * @param datas - The data char 
         * @param data_size - The size of the data so null terminators don't truncate the rest of the data
         */
        Packet(Header size, Header type, const char* data, int data_size) {
            headers.push_back(size);
            headers.push_back(type);
            for (int i = 0; i < data_size; i++) {
                this->body.push_back((char) data[i]);
            }
            std::cout << "Appended data = " << std::endl;
            this->printData();
        }

        /**
         * @brief Appends a string of data 
         * 
         * @param str - the string of data to append 
         */
        void appendData(std::string str) {
            for (char c : str) {
                this->body.push_back(c);
            }
            setPacketSize(); // Recalculates the size of the packet after the append
        }

        /**
         * @brief Appends a vector of integers 
         * 
         * @param nums - the numbers to append
         */
        void appendData(std::vector<int> nums) {
            for (int val : nums) {
                this->body.push_back(val);
            }
            std::cout << "Appended data = " << std::string(body.data(), body.size()) << std::endl;
            setPacketSize();
        }
        /**
         * @brief Appnd chars to the data 
         * 
         * @param chars - the chars to append
         */
        void appendData(std::vector<char> chars) {
            for (char c : chars) {
                this->body.push_back(c);
            }
            setPacketSize();
        }

        /**
         * @brief Set the Packet Size by updating the size header
         * 
         */
        void setPacketSize() {
            Header& packet_size = headers.at(0);

            // Make sure the header is the size header
            assert(packet_size.getType().compare("SIZE") == 0);

            // Assert that the size of the packet fits into 8 digits (characters) long 
            std::string temp_size = std::to_string(this->size());
            assert(temp_size.size() <= 8);

            // Pad the size with leading zeroes 
            std::string size_str = std::string(8 - temp_size.size(), '0').append(temp_size);

            packet_size.setValue(size_str);
            std::cout << "Set size of packet to " << size_str << std::endl;
        }


        // Getters 
        /**
         * @brief Get the Headers object
         * @warning Using a for-each loop may cause segfaults, 
         *     use a normal for loop to iterate, or an iterator.
         * 
         * @return std::vector<Header> 
         */
        std::vector<Header> getHeaders() {
            return headers;
        }

        /**
         * @brief Get the Datas object
         * 
         * @return std::vector<char>* 
         */
        std::vector<char>* getDatas() {
            return new std::vector<char>(this->body);
        }

        /**
         * @brief Get the data in the form of a string
         * 
         * @return std::string 
         */
        std::string getDatasString() {
            return std::string(body.data(), body.size());
        }

        /**
         * @brief Get the the type of the packet
         *  - Returns the 2nd header in the packet.
         *  @see the constructor to see how Packets should be formatted 
         * 
         * @return PackType - returns the type of packet
         */
        PackType getType() {
            return PackType(atoi(headers.at(1).getValue().c_str()));
        }

        /**
         * @brief Gets the data for the packet to send over network
         * 
         * @return const char* - The data of the whole packet ready to be sent
         */
        const char* data() {
            std::string *raw_data = new std::string();
            // Get the headers
            for (auto head : this->headers) {
                raw_data->append(head.data());
                raw_data->push_back(' ');
            }
            // Append the body of the packet
            raw_data->append(std::string(body.data(), body.size()));
            return raw_data->data();
        }

        /**
         * @brief Prints all the values in the char vector datas
         *    Useful for debugging 
         */
        void printData() {
            for (auto d : this->body) {
                printf(" %d ", d);
            }
            printf("\n");
        }

        /**
         * @brief Gets the size of the packet for network functionality
         *  - Not the size stored locally. 
         *  - Should only be used in tandem with Packet::data()
         * 
         * @return size_t 
         */
        size_t size() {
            size_t s = 0;

            // Get the size of the headers
            for (auto i = headers.begin(); i != headers.end(); i++) {
                s += i->size() + 1; // + 1 = push_back(' ');
            }

            // Get the size of the body  
            s += body.size();

            return s;
        }
};

#endif