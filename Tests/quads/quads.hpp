#ifndef _QUADS_H_
#define _QUADS_H_

#include <vector>

typedef std::vector<int>       vector_1d;
typedef std::vector<vector_1d> vector_2d;
typedef std::vector<vector_2d> vector_3d;
typedef std::vector<vector_3d> vector_4d;

// extern std::vector<std::vector<std::vector<std::vector<int>>>> quads;

void make_quads();
vector_4d get_quads();
void print_quads(const vector_3d);

#endif