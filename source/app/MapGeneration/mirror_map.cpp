// TODO
#include <time.h>
#include <math.h>

#include "mirror.hpp"

int** generateMirrorMap() //tile_map is 24x13
{
    int** array = 0;
    array = new int*[24];

    for(int i = 0; i < 24; i++)
    {
        array[i] = new int[13];
        for(int j = 0; j < 13; j++)
        {
            array[i][j] = 0;
        }
    }

    srand(time(NULL));

    array[13][10] = 2;

    return array;
}