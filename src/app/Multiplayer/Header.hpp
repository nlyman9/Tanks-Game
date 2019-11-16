#ifndef HEADER_HPP
#define HEADER_HPP


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

/**
 * @brief This class essentially all the things we would need from a header
 *  - The type string is the ID/Type of header
 *  - The value is the data associated with the ID
 *  - Think similar to a key-value pair
 */
class Header {
    private:
        std::string type, value; 
    public:
        /**
         * @brief Construct a new Header object
         *  Think like key-value pairs 
         * 
         * @param type A string to classify the type of header
         * @param value A string containing the value of the header type
         */
        Header(std::string type, std::string value) {
            this->type = type;
            this->value = value;
        };

        /**
         * @brief Construct a new Header object with no value
         *  - value is set to null
         * 
         * @param header A string to classify the type of header
         */
        Header(std::string type) {
            this->type = type;
            this->value = nullptr;
        }

        /**
         * @brief Construct a new Header object using a char array  
         * 
         * @param data Uses a c-string (null-terminated) to set the type and value (in that order)
         *  - The header and value are seperated by a space.
         */
        Header(const char* data) {
            type = strtok((char *)data, " ");
            value = strtok(NULL, " ");
        }

        ~Header() {}

        /**
         * @brief Set the Value object
         * 
         * @param val - string containing the value 
         */
        void setValue(std::string val) {
            value = val;
        }

        /**
         * @brief Get the Type of the header
         * 
         * @return std::string 
         */
        std::string getType() {
            return type;
        }

        /**
         * @brief Get the value of the header
         * 
         * @return std::string 
         */
        std::string getValue() {
            return value;
        }

        /**
         * @brief Get the header in the form of a tuple
         * 
         * @return std::tuple<std::string, std::string> 
         */
        std::tuple<std::string, std::string> getTuple() {
            return std::make_tuple(type, value);
        }

        /**
         * @brief Get the header in the form of a vector<string>
         * 
         * @return std::vector<std::string>* 
         */
        std::vector<std::string>* getVector() {
            std::vector<std::string> *ret = new std::vector<std::string>();
            ret->push_back(type);
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
            raw->push_back((char *) type.data());
            raw->push_back((char *) value.data());
            return raw;
        } 

        /**
         * @brief Get the data of the type-value pair in a string 
         *  - Remember to free when used
         * 
         * @return std::string* 
         */
        std::string* dataString() {
            std::string *data = new std::string();
            data->append(type);
            data->append(value);

            return data;
        }

        /**
         * @brief Get the data of the type-value pair in a char array (delimited by a space)
         *  - Used for network fuctionality send()/recv()
         * 
         * @return const char* 
         */
        const char* data() {
            std::string *d = new std::string();
            d->append(type.c_str());
            d->push_back(' ');
            d->append(value.c_str());

            return d->data();
        }
        
        /**
         * @brief Returns the size used for the network functions using data()
         * - NOT the size stored locally
         * - Should only be used in tandem with data()
         * 
         * @return size_t - size in bytes
         */
        size_t size() {
            // Plus 1 for the data() fucntion
            return type.size() + 1 + value.size();
        }


        friend std::ostream& operator <<(std::ostream &out, const Header head);
};

#endif