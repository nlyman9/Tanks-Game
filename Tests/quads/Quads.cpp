#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <istream>
#include <map>
#include <iterator>
#include "Quad.hpp"
#include "Quads.hpp"
#include "QuadConstants.hpp"


void Quads::print_quads() {
    for (auto& q : this->quads) {
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
}

vector_4d Quads::get_quads() {
    return quads;
}

vector_2d Quads::flip(vector_2d q) {
    vector_2d q_flip(q.rbegin(), q.rend()); // convert from q2 to q5
    return q_flip;
}

vector_2d Quads::mirror(vector_2d q) {
    vector_2d q_mirror; // convert from q1 to q3
    for (auto& v : q) {
        std::vector<int> vec(v.rbegin(), v.rend());
        q_mirror.push_back(vec);
    }
    return q_mirror;
}

vector_3d Quads::corner(vector_2d q1) {
    
    vector_3d reversed;

    reversed.push_back(flip(q1)); // convert to q4
    reversed.push_back(mirror(flip(q1))); // convert to q6
    reversed.push_back(flip(mirror(flip(q1)))); // convert to q3

    return reversed;
}

void Quads::make_quads() {

    std::ifstream infile ("quadrants.txt");
    std::string line;
    int num, num_lines, num_quads, quad_type = 0, quad_id = 0;

    vector_3d quadrant_1, quadrant_2, quadrant_3, 
                quadrant_4, quadrant_5, quadrant_6, quadrant_all;

    if (infile.is_open()) {

        while (std::getline(infile, line))  // calculate the number of lines in the file
            ++num_lines;

        infile.clear();
        infile.seekg(0, infile.beg);    // Return to the beginning of the file

        num_quads = num_lines / 7;
        
        while (!infile.eof()) {

            for (int i = 0; i < num_quads; i++) {
                
                infile >> quad_type;                    
                vector_2d quad;

                for (int j = 0; j < MAX_ROW; j++) {     // read the quad into a 2d vector
                    std::vector<int> v;
                    for (int k = 0; k < MAX_COL; k++) {
                        infile >> num;
                        v.push_back(num);
                    }
                    quad.push_back(v);
                }

                switch (quad_type) {

                    case 4: {   // corner quads 
                        quadrant_1.push_back(quad);
                        auto corners = corner(quad);
                        quadrant_4.push_back(corners.at(0));
                        quadrant_6.push_back(corners.at(1));
                        quadrant_3.push_back(corners.at(2));
                        // Perhaps create the edges at this stage
                        break;
                    }   
                    case 5: {   // middle quads
                        quadrant_2.push_back(quad);
                        quadrant_5.push_back(flip(quad));
                        quadrant_all.push_back(quad);
                        quadrant_all.push_back(flip(quad));
                        break;
                    }
                    case 6: {   // any quad
                        quadrant_all.push_back(quad);
                        break;
                    }
                    case 7: {   // mirror
                        quadrant_1.push_back(quad);
                        quadrant_4.push_back(quad);
                        quadrant_3.push_back(mirror(quad));
                        quadrant_6.push_back(mirror(quad));
                        break;
                    }
                    case 9: {   // top bottom
                        quadrant_1.push_back(quad);
                        quadrant_2.push_back(quad);
                        quadrant_3.push_back(quad);
                        quadrant_4.push_back(flip(quad));
                        quadrant_5.push_back(flip(quad));
                        quadrant_6.push_back(flip(quad));
                    }
                    default: {  // 6 - any quads
                        // quads.push_back(quad);
                        // std::cout << "Reached default case" << std::endl;
                        break;
                    }
                }

                quad_type = 0;
            }
                
        }
        infile.close();

    } else {
        std::cout << "unable to open file";
    }

    quads.push_back(quadrant_1);
    quads.push_back(quadrant_2);
    quads.push_back(quadrant_3);
    quads.push_back(quadrant_4);
    quads.push_back(quadrant_5);
    quads.push_back(quadrant_6);
    quads.push_back(quadrant_all);
}

Quads::Quads() {
    make_quads();
}

// int main() {
//     Quads q = Quads();
//     q.print_quads();
//     return 0;
// }