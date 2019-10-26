#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

// using namespace std;

int main() {
    std::ifstream InputStream ("quadrants");
    // InputStream.open("quadrants.txt");
    std::string output;
    if (InputStream.is_open()) {
        while (std::getline(InputStream, output)) {
            std::cout << output << std::endl;
        }
        InputStream.close();
    } else {
        std::cout << "unable to open file";
    }
    
    return 0;
}