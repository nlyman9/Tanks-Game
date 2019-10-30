#include "MapGenerator.hpp"
#include <time.h>
#include <math.h>
#include <iostream>

std::vector<std::vector<int>> MapGenerator::generateEmptyMap()
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

	return array;
}

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

std::vector<std::vector<int>> MapGenerator::generateLineMap()
{
	std::vector<std::vector<int>> room = generateEmptyMap();
	int random_index;
	int random_index_bonus;

	srand(time(NULL));

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
			else {
				room[j][i] = 0;
			}
		}
	}
	return room;
}

std::vector<std::vector<int>> MapGenerator::generateHMazeMap()
{
    std::vector<std::vector<int>> array = generateEmptyMap();

	srand(time(NULL));

    for(int i = 0; i < Y_HIGH; i++)
    {
		if(i < 3 || i > 9) {
			for(int j = 0; j < X_WIDE; j++) {
				array[j][i] = 0;
			}
		} else {
			for(int j = 0; j < X_WIDE; j++) {
				array[j][i] = 2;
			}
		}
    }

	for(int i = 3; i < 11; i++) {
		std::vector<int> set;

		if(i % 2 == 0) {
			set.push_back(0);

			for(int j = 0; j < X_WIDE; j++) {
				if(rand() % 10 > 3 && j < X_WIDE - 1) { // carve right
					set.push_back(j+1);
				} else {
					int set_size = set.size();
					bool can_go_north = false;
					std::vector<int> north_set;

					for(int k = 0; k < set_size; k++) {
						if(array[set.at(k)][i-2] == 0)
						{
							north_set.push_back(set.at(k));
							can_go_north = true;
						}
					}

					if(!can_go_north) { // add another cell to ensure passage north
						if(j+1 > 23) {
							array[j][i] = 0;
							array[j-1][i] = 0;
							continue;
						} else {
							set.push_back(j+1);
							north_set.push_back(j+1);
							set_size = set.size();
							j++;
						}
					}

					int path_north = rand() % north_set.size();

					for(int k = 0; k < set_size; k++) { // cave cells from set
						int col = set.at(k);
						array[col][i] = 0;
						if(col == north_set.at(path_north)) { // carve north from set
							array[col][i-1] = 0;
						}
					}

					set.clear();
					set.push_back(j+2);
					j++;
				}
			}
		}
    }

    return array;
}

std::vector<std::vector<int>> MapGenerator::presetCenterCubes()
{
    std::vector<std::vector<int>> array = generateEmptyMap();

	for(int i = 6; i < 18; i++) {
		for(int j = 3; j < 10; j++) {
			if(i == 11 || i == 12 || j == 6) {
				continue;
			}
			array[i][j] = 2;
		}
	}

	return array;
}

std::vector<std::vector<int>> MapGenerator::presetCheckerMap()
{
    std::vector<std::vector<int>> room = generateEmptyMap();
    int pre_array[5] = {2, 6, 11, 16, 20};

	for(int j = 2; j < Y_HIGH; j+=4) {
		if(j == 2 || j == 10) {
			for(int i = 1; i < X_WIDE; i+=4) {
				room[i][j] = 2;
			}
			for(int i = 2; i < X_WIDE; i+=4) {
				room[i][j] = 2;
			}
		}
		else if(j == 6) {
			for(int i = 3; i < X_WIDE; i+=4) {
				if(i == 23) {

				}
				else {
					room[i][j] = 2;
				}
			}
			for(int i = 4; i < X_WIDE; i+=4) {
				room[i][j] = 2;
			}
		}
	}
	return room;
}

std::vector<std::vector<int>> MapGenerator::generateOpenLineMap()
{
	std::vector<std::vector<int>> room = generateEmptyMap();
	int random_index;
	int pre_array[5] = {2, 6, 11, 16, 20};
	srand(time(NULL));

	for(int i = 0; i < (sizeof(pre_array)/sizeof(pre_array[0])); i++) {
		random_index = rand() % (Y_HIGH - 2);

		for(int j = 0; j < Y_HIGH; j++) {
			if(random_index == j || random_index == j + 1) {
				room[pre_array[i]][j] = 0;
				room[pre_array[i] + 1][j] = 0;
			} else {
				room[pre_array[i]][j] = 2;
				room[pre_array[i] + 1][j] = 2;
			}

		}
	}
	return room;
}

std::vector<std::vector<int>>* MapGenerator::generateMap()
{
	// UPDATE THESE WHEN ADDING NEW MAP TYPES
	int NUM_GEN = 3;
	int NUM_PRE = 3;
	
	// init tile map
	tile_map = generateEmptyMap();

	srand(time(NULL));

	switch(rand() % (int) ceil(NUM_GEN * 1.2))
	{
		case 1:
			tile_map = generateLineMap();
			break;
		case 2:
			tile_map = generateHMazeMap();
			break;
		case 3:
			tile_map = generateOpenLineMap();
			break;
		default:
			switch(rand() % NUM_PRE)
			{
				case 0:
					tile_map = generateEmptyMap();
					break;
				case 1:
					tile_map = presetCenterCubes();
					break;
				case 2:
					tile_map = presetCheckerMap();
					break;
			}
	}

	return &tile_map;
}