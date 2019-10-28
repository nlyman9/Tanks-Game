#include "MapGenerator.hpp"
#include <time.h>
#include <math.h>
#include <iostream>

std::vector<std::vector<int>> MapGenerator::generateMirrorMap() //tile_map is 24x13
{
    std::vector<std::vector<int>> array;
    for(int i = 0; i < X_WIDE; i++)
    {
        array.push_back(std::vector<int>(Y_HIGH));
        for(int j = 0; j < Y_HIGH; j++)
        {
            array[i][j] = 0;
        }
    }

    srand(time(NULL));

    array[Y_HIGH][10] = 2;

    return array;
}

std::vector<std::vector<int>> MapGenerator::generateLineMap() {

	std::vector<std::vector<int>> array;
    for(int i = 0; i < X_WIDE; i++)
    {
		array.push_back(std::vector<int>(Y_HIGH));
        for(int j = 0; j < Y_HIGH; j++)
        {
            array[i][j] = 0;
        }
    }

	std::vector<std::vector<int>> room = array;

	//initialize random seed
	srand(time(NULL));
	int random_index;
	int random_index_bonus;
	for(int i = 0; i < Y_HIGH; i++) {
		random_index = rand() % X_WIDE;
		if(rand() % 3 == 0)
		{
			random_index_bonus = rand() % X_WIDE;
			if(abs(random_index_bonus - random_index) == 1)
			{
				random_index_bonus = -1;
			}
		}
		else
		{
			random_index_bonus = -1;
		}
		

		for(int j = 0; j < X_WIDE; j++) {
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

std::vector<std::vector<int>>* MapGenerator::generateMap() {
	for(int j = 0; j < X_WIDE; j++)
	{
		tile_map.push_back(std::vector<int>(Y_HIGH)); 
		for(int h = 0; h < Y_HIGH; h++)
		{	
			tile_map[j][h] = 0;
		}
	}
	// Select map generation technique
	enum map_types { destructible, holes, line, maze, mirror };
	srand(time(NULL));

	// switch(rand() % 4)
	switch(line)
	{
		case destructible:
			tile_map[4][4] = 2;
			break;
		case holes:
			tile_map[1][1] = 2;
			break;
		case line:
			tile_map = generateLineMap();
			break;
		case maze:
			tile_map[6][10] = 2;
			break;
		case mirror:
			tile_map = generateMirrorMap();
			tile_map[14][10] = 2;
			break;
	}

	return &tile_map;
}