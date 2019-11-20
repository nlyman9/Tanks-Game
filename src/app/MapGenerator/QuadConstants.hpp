#ifndef QUADCONSTANTS_HPP
#define QUADCONSTANTS_HPP

#include <vector>

typedef std::vector<int>       vector_1d;
typedef std::vector<vector_1d> vector_2d;
typedef std::vector<vector_2d> vector_3d;
typedef std::vector<vector_3d> vector_4d;

extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;

extern const int MAX_ROW;
extern const int MAX_COL;

extern const int Q1_MIN_HEIGHT, Q2_MIN_HEIGHT, Q3_MIN_HEIGHT;
extern const int Q1_MAX_HEIGHT, Q2_MAX_HEIGHT, Q3_MAX_HEIGHT;

extern const int Q4_MIN_HEIGHT, Q5_MIN_HEIGHT, Q6_MIN_HEIGHT;
extern const int Q4_MAX_HEIGHT, Q5_MAX_HEIGHT, Q6_MAX_HEIGHT;

extern const int DIV_MIN_WIDTH, DIV_MAX_WIDTH, DIV_HEIGHT;

extern const int Q1_WIDTH_OFFSET, Q4_WIDTH_OFFSET;
extern const int Q2_WIDTH_OFFSET, Q5_WIDTH_OFFSET;
extern const int Q3_WIDTH_OFFSET, Q6_WIDTH_OFFSET;

extern const int Q1_HEIGHT_OFFSET, Q2_HEIGHT_OFFSET, Q3_HEIGHT_OFFSET;
extern const int Q4_HEIGHT_OFFSET, Q5_HEIGHT_OFFSET, Q6_HEIGHT_OFFSET;

extern const int MIN_HEIGHTS[];
extern const int MAX_HEIGHTS[];

extern const int WIDTH_OFFSETS[];
extern const int HEIGHT_OFFSETS[];

#endif