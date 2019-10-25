#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <istream>

constexpr int MAX_ROW = 6;
constexpr int MAX_COL = 8;

void print_quads(const std::vector<std::vector<std::vector<int>>> q) {
    for(auto& quad : q) {
        for (auto& v : quad) {
            for (auto& num: v) {
                std::cout << num << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> flip(std::vector<std::vector<int>> q) {
    std::vector<std::vector<int>> q_flip(q.rbegin(), q.rend()); // convert from q2 to q5
    return q_flip;
}

std::vector<std::vector<int>> mirror(std::vector<std::vector<int>> q) {
    std::vector<std::vector<int>> q_mirror; // convert from q1 to q3
    for (auto& v : q) {
        std::vector<int> vec(v.rbegin(), v.rend());
        q_mirror.push_back(vec);
    }
    return q_mirror;
}

std::vector<std::vector<std::vector<int>>> corner(std::vector<std::vector<int>> q1) {
    
    std::vector<std::vector<std::vector<int>>> reversed;

    reversed.push_back(flip(q1)); // convert to q4
    reversed.push_back(mirror(flip(q1))); // convert to q6
    reversed.push_back(flip(mirror(flip(q1)))); // convert to q3

    return reversed;
}

int main() {

    std::ifstream infile ("quadrants.txt");
    std::string line;
    int num, num_lines, num_quads, quad_type = 0;

    std::vector<std::vector<std::vector<int>>> quads;
    
    if (infile.is_open()) {

        while (std::getline(infile, line))  // calculate the number of lines in the file
            ++num_lines;
        
        // std::cout << "Number of lines in file: " << num_lines << "\n" << std::endl;

        infile.clear();
        infile.seekg(0, infile.beg);    // Return to the beginning of the file

        num_quads = num_lines / 7;

        // std::cout << "Number of quads in file: " << num_quads << "\n" << std::endl;
        
        while (!infile.eof()) {

            for (int i = 0; i < num_quads; i++) {
                
                infile >> quad_type;                    
                std::vector<std::vector<int>> quad;

                for (int j = 0; j < MAX_ROW; j++) {     // read the quad into a 2d vector
                    std::vector<int> v;
                    for (int i = 0; i < MAX_COL; i++) {
                        infile >> num;
                        v.push_back(num);
                    }
                    quad.push_back(v);
                }

                switch (quad_type) {

                    case 4: {   // corner quads 
                        quads.push_back(quad);
                        auto corners = corner(quad);
                        for (auto& c : corners)
                            quads.push_back(c);
                        break;
                    }   
                    case 5: {   // middle quads
                        quads.push_back(quad);
                        quads.push_back(flip(quad));
                        break;
                    }
                    case 6: {   // any quad
                        quads.push_back(quad);
                        break;
                    }
                    case 7: {   // mirror
                        quads.push_back(quad);
                        quads.push_back(mirror(quad));
                        break;
                    }
                    default: {  // 6 - any quads
                        // quads.push_back(quad);
                        break;
                    }
                }
            }
                
        }
        infile.close();

    } else {
        std::cout << "unable to open file";
    }
    
    print_quads(quads);

    return 0;
}