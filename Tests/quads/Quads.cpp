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
// #include "Tileset.hpp"
// #include "Edge.hpp"
// #include "Quadrant.hpp"
#include "Quads.hpp"
#include "QuadConstants.hpp"

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
    int num, num_lines = 0, num_quads = 0, quad_type = 0, tileset_id = 0;

    if (infile.is_open()) {

        while (std::getline(infile, line))  // calculate the number of lines in the file
            ++num_lines;

        infile.clear();
        infile.seekg(0, infile.beg);    // Return to the beginning of the file

        num_quads = num_lines / 7;
        
        while (!infile.eof()) {

            for (int i = 0; i < num_quads; i++) {
                
                infile >> quad_type;                    
                vector_2d q;

                for (int j = 0; j < MAX_ROW; j++) {     // read the quad into a 2d vector
                    std::vector<int> v;
                    for (int k = 0; k < MAX_COL; k++) {
                        infile >> num;
                        v.push_back(num);
                    }
                    q.push_back(v);
                }

                Tileset t(q, tileset_id++);

                switch (quad_type) {
                    case 4: {
                        quads[0].add(t);
                        auto corners = corner(q);
                        Tileset t3(corners.at(2), tileset_id++);
                        Tileset t4(corners.at(0), tileset_id++);
                        Tileset t6(corners.at(1), tileset_id++);
                        Edge t_t3(t, t3, 2, 0);
                        Edge t3_t4(t3, t4, 2, 0);
                        Edge t4_t6(t4, t6, 2, 0);
                        // t_t3.print_edge();
                        // t3_t4.print_edge();
                        // t4_t6.print_edge();
                        edges.push_back(t_t3);
                        edges.push_back(t3_t4);
                        edges.push_back(t4_t6);
                        quads[2].add(t3);
                        quads[3].add(t4);
                        quads[5].add(t6);
                        break;
                    }
                    case 5: {
                        quads[1].add(t);
                        Tileset t5(flip(t.get_tiles()), tileset_id++);
                        Edge t_t5(t, t5, 2, 0);
                        // t_t5.print_edge();
                        edges.push_back(t_t5);
                        quads[4].add(t5);
                        break;
                    }
                    case 6: {
                        quads[0].add(t);
                        for (int i = 1; i < 6; i++) {
                            Tileset tAll(t.get_tiles(), tileset_id++);
                            Edge t_tAll(t, tAll, 2, 0);
                            // t_tAll.print_edge();
                            edges.push_back(t_tAll);
                            quads[i].add(tAll);
                        }  
                        break;
                    }
                    case 7: {
                        quads[0].add(t);
                        Tileset t3(mirror(t.get_tiles()), tileset_id++);
                        Tileset t4(t.get_tiles(), tileset_id++);
                        Tileset t6(mirror(t.get_tiles()), tileset_id++);
                        Edge t_t3(t, t3, 2, 0);
                        Edge t3_t4(t3, t4, 2, 0);
                        Edge t4_t6(t4, t6, 2, 0);
                        // t_t3.print_edge();
                        // t3_t4.print_edge();
                        // t4_t6.print_edge();
                        edges.push_back(t_t3);
                        edges.push_back(t3_t4);
                        edges.push_back(t4_t6);
                        quads[2].add(t3);
                        quads[3].add(t4);
                        quads[5].add(t6);
                        break;
                    }
                    case 9: {
                        quads[0].add(t);
                        for (int i = 1; i < 3; i++) {
                            Tileset tTop(t.get_tiles(), tileset_id++);
                            Edge t_tTop(t, tTop, 2, 0);
                            // t_tTop.print_edge();
                            edges.push_back(t_tTop);
                            quads[i].add(tTop);
                        }
                        for (int j = 3; j < 6; j++) {
                            Tileset tBottom(flip(t.get_tiles()), tileset_id++);
                            Edge t_tBottom(t, tBottom, 2, 0);
                            // t_tBottom.print_edge();
                            edges.push_back(t_tBottom);
                            quads[j].add(tBottom);
                        }
                        break;
                    }
                    default: {
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
}

Quads::Quads() {
    for (int i = 1; i < 7; i++) {
        Quadrant q(i);
        quads.push_back(q);
    }
}

std::vector<Quadrant> Quads::get_quads() {
    return quads;
}

std::vector<Edge> Quads::get_edges() {
    return edges;
}

void Quads::print_quads() {
    for (int i = 0; i < 6; i++) {
        std::cout << "Quadrant " << i+1 << std::endl;
        quads[i].print_quadrant();
    }
}

void Quads::print_edges() {
    std::cout << "Edges in Quads: " << edges.size() << std::endl;
    for (auto edge : edges) {
        edge.print_edge();
    }
}

int Quads::get_num_tilesets() {
    int num_tilesets = 0;
    for (auto& quad : quads) {
        num_tilesets += quad.size();
    }
    return num_tilesets;
}