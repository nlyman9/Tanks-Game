#include "MapGenerator.hpp"
#include <time.h>
#include <math.h>

int** MapGenerator::generateMirrorMap() //tile_map is 24x13
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

int** MapGenerator::generateLineMap() {
	// make empty room array
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

	int** room = array;

	//initialize random seed
	srand(time(NULL));
	int random_index;
	int random_index_bonus;
	for(int i = 0; i < 13; i++) {
		random_index = rand() % 24;
		if(rand() % 3 == 0)
		{
			random_index_bonus = rand() % 24;
			if(abs(random_index_bonus - random_index) == 1)
			{
				random_index_bonus = -1;
			}
		}
		else
		{
			random_index_bonus = -1;
		}
		

		for(int j = 0; j < 24; j++) {
			if(i % 2 != 0) {
				if(j == random_index || j == random_index_bonus)
					room[j][i] = 0;
				else
					room[j][i] = 2;
			}
			else
				room[j][i] = 0;
		}
	}
	return room;
}