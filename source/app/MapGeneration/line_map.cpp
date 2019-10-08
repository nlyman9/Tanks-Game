#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "line_map.hpp"

using namespace std;

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;

int room[X_WIDE][Y_HIGH];

int** generateLineMap() {
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
	for(int i = 0; i < 13; i++) {
		random_index = rand() % 24;

		for(int j = 0; j < 24; j++) {
			if(i % 2 != 0) {
				if(j == random_index)
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