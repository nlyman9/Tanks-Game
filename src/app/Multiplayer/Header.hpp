#ifndef HEADER_HPP
#define HEADER_HPP


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <tuple>

/**
 * @brief This class essentially simulates all the things we would need from a header
 *  - The header string is the ID/Type of header
 *  - The value is the data associated with the ID
 */
class Header {
    private:
        std::string header, value; 
    public:
        Header(std::string header, std::string value) {
            header = header;
            value = value;
        };

        Header(std::string header) {
            header = header;
            value = nullptr;
        }

        ~Header() {
            delete this;
        }

        void setValue(std::string val) {
            value = val;
        }

        std::string getHead() {
            return header;
        }

        std::string getValue() {
            return value;
        }

        /**
         * @brief Get the header in the form of a tuple
         * 
         * @return std::tuple<std::string, std::string> 
         */
        std::tuple<std::string, std::string> getTuple() {
            return std::make_tuple(header, value);
        }

        /**
         * @brief Get the header in the form of a vector<string>
         * 
         * @return std::vector<std::string>* 
         */
        std::vector<std::string>* getVector() {
            std::vector<std::string> *ret = new std::vector<std::string>();
            ret->push_back(header);
            ret->push_back(value);
            return ret;
        }

        /**
         * @brief Get the header in the form of a vector of char pointers
         * 
         * @return std::vector<char*>* 
         */
        std::vector<char*>* get() {
            std::vector<char*> *raw = new std::vector<char*>();
            raw->push_back((char *) header.data());
            raw->push_back((char *) value.data());
            return raw;
        } 

        std::string* dataString() {
            std::string *data = new std::string();
            data->append(header);
            data->append(value);

            return data;
        }

        const char* data() {
            std::string *data = new std::string();
            data->append(header);
            data->append(value);

            return data->data();
        }

        size_t size() {
            return header.size() + value.size();
        }


        friend std::ostream& operator <<(std::ostream &out, const Header head);
};

#endif