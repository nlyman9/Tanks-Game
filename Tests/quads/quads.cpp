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

constexpr int MAX_ROW = 6;
constexpr int MAX_COL = 8;
std::vector<std::vector<std::vector<std::vector<int>>>> quads;

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

std::vector<std::vector<std::vector<std::vector<int>>>> get_quads() {
    return quads;
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

void write_to(std::vector<std::vector<std::vector<int>>> quadrant) {
    // std::ofstream out("quads_out.bin", std::ios::out | std::ios::binary);
    std::ofstream out("quads_out.txt");
    std::ostream_iterator<int> out_iter(out, " ");
    for (auto& quad : quadrant) { // get the 2d vector
        for (int i = 0; i < MAX_ROW; i++) { // get the vector at [i]
            int* q = quad[i].data();
            out << &q;
        }
    }
    out.close();
}

int main() {

    std::ifstream infile ("quadrants.txt");
    std::string line;
    int num, num_lines, num_quads, quad_type = 0;

    std::vector<std::vector<std::vector<int>>> quads;

    std::vector<std::vector<std::vector<int>>> quadrant_1, quadrant_2, quadrant_3, 
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
                std::vector<std::vector<int>> quad;

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
                        std::cout << "Reached default case" << std::endl;
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
    
    // print_quads(quads);

    std::cout << "Quadrant 1:" << std::endl;
    print_quads(quadrant_1);

    std::cout << "Quadrant 2:" << std::endl;
    print_quads(quadrant_2);

    std::cout << "Quadrant 3:" << std::endl;
    print_quads(quadrant_3);

    std::cout << "Quadrant 4:" << std::endl;
    print_quads(quadrant_4);

    std::cout << "Quadrant 5:" << std::endl;
    print_quads(quadrant_5);

    std::cout << "Quadrant 6:" << std::endl;
    print_quads(quadrant_6);

    std::cout << "Quadrant All" << std::endl;
    print_quads(quadrant_all);

    quads.push_back(quadrant_1);
    quads.push_back(quadrant_2);
    quads.push_back(quadrant_3);
    quads.push_back(quadrant_4);
    quads.push_back(quadrant_5);
    quads.push_back(quadrant_6);
    quads.push_back(quadrant_all);
    
    return 0;
}