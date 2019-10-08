// TODO
#include <time.h>
#include <math.h>

#include "mirror.hpp"

// namespace mirror
// {
    int** generate_mirror() //tile_map is 24x13
    {
        int** array = 0;
        array = new int*[13];

        for(int j = 0; j < 13; j++)
        {
            array[j] = new int[24];
            for(int h = 0; h < 24; h++)
            {
                array[j][h] = 0;
            }
        }

        srand(time(NULL));

        array[13][10] = 2;

        return array;
    }
// }