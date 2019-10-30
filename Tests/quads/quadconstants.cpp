#include "quadconstants.hpp"

const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 13;

const int MAX_ROW = 6;
const int MAX_COL = 8;

// const int Q1_MIN_WIDTH = 0, Q4_MIN_WIDTH = 0;
// const int Q1_MAX_WIDTH = 7, Q4_MAX_WIDTH = 7;

// const int Q2_MIN_WIDTH = 8, Q5_MIN_WIDTH = 8;
// const int Q2_MAX_WIDTH = 15, Q5_MAX_WIDTH = 15;

// const int Q3_MIN_WIDTH = 16, Q6_MIN_WIDTH = 16;
// const int Q3_MAX_WIDTH = 23, Q6_MAX_WIDTH = 23;

const int Q1_MIN_HEIGHT = 0, Q2_MIN_HEIGHT = 0, Q3_MIN_HEIGHT = 0;
const int Q1_MAX_HEIGHT = 5, Q2_MAX_HEIGHT = 5, Q3_MAX_HEIGHT = 5;

const int Q4_MIN_HEIGHT = 7, Q5_MIN_HEIGHT = 7, Q6_MIN_HEIGHT = 7;
const int Q4_MAX_HEIGHT = 12, Q5_MAX_HEIGHT = 12, Q6_MAX_HEIGHT = 12;

const int DIV_MIN_WIDTH = 0, DIV_MAX_WIDTH = 23, DIV_HEIGHT = 6;

const int Q1_WIDTH_OFFSET = 0, Q4_WIDTH_OFFSET = 0;
const int Q2_WIDTH_OFFSET = 8, Q5_WIDTH_OFFSET = 8;
const int Q3_WIDTH_OFFSET = 16, Q6_WIDTH_OFFSET = 16;

const int Q1_HEIGHT_OFFSET = 0, Q2_HEIGHT_OFFSET = 0, Q3_HEIGHT_OFFSET = 0;
const int Q4_HEIGHT_OFFSET = 7, Q5_HEIGHT_OFFSET = 7, Q6_HEIGHT_OFFSET = 7;

// const int MIN_WIDTHS[]  = { Q1_MIN_WIDTH, Q2_MIN_WIDTH, Q3_MIN_WIDTH, Q4_MIN_WIDTH, Q5_MIN_WIDTH, Q6_MIN_WIDTH };
// const int MAX_WIDTHS[]  = { Q1_MAX_WIDTH, Q2_MAX_WIDTH, Q3_MAX_WIDTH, Q4_MAX_WIDTH, Q5_MIN_WIDTH, Q6_MAX_WIDTH };
const int MIN_HEIGHTS[] = { Q1_MIN_HEIGHT, Q2_MIN_HEIGHT, Q3_MIN_HEIGHT, Q4_MIN_HEIGHT, Q5_MIN_HEIGHT, Q6_MIN_HEIGHT };
const int MAX_HEIGHTS[] = { Q1_MAX_HEIGHT, Q2_MAX_HEIGHT, Q3_MAX_HEIGHT, Q4_MAX_HEIGHT, Q5_MAX_HEIGHT, Q6_MAX_HEIGHT };

const int WIDTH_OFFSETS[] = { Q1_WIDTH_OFFSET, Q2_WIDTH_OFFSET, Q3_WIDTH_OFFSET, Q4_WIDTH_OFFSET, Q5_WIDTH_OFFSET, Q6_WIDTH_OFFSET };
const int HEIGHT_OFFSETS[] = { Q1_HEIGHT_OFFSET, Q2_HEIGHT_OFFSET, Q3_HEIGHT_OFFSET, Q4_HEIGHT_OFFSET, Q5_HEIGHT_OFFSET, Q6_HEIGHT_OFFSET };