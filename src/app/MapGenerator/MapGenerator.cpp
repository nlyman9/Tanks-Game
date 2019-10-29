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
						set.push_back(j+1);
						north_set.push_back(j+1);
						set_size++;
						j++;
					}

					int path_north = rand() % north_set.size();

					// printf("set:");
					for(int k = 0; k < set_size; k++) { // cave cells from set
						int col = set.at(k);
						array[col][i] = 0;
						// printf("%d:", col);
						if(col == north_set.at(path_north)) { // carve north from set
							array[col][i-1] = 0;
						}
					}
					// printf("north:%d\n", north_set.at(path_north));

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

/*std::vector<std::vector<int>> MapGenerator::generateCheckerMap()
{
    std::vector<std::vector<int>> room = generateEmptyMap();
	int random_index;
	int random_index_bonus;

    for(int i = 2; i < Y_HIGH; i++) {
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
}*/

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
			}
			else
				room[pre_array[i]][j] = 1;
				room[pre_array[i] + 1][j] = 1;
		}
	}
	return room;
}

std::vector<std::vector<int>>* MapGenerator::generateMap()
{
	// init tile map
	tile_map = generateEmptyMap();

	// Select map generation technique
	enum map_types { destructible, holes, line, maze, mirror, hmaze };
	srand(time(NULL));

	tile_map = generateOpenLineMap();

	/*switch(rand() % 3) // update this line when adding more generated maps
	{
		case 1:
			tile_map = generateLineMap();
			break;
		case 2:
			tile_map = generateHMazeMap();
			break;
		default:
			switch(rand() % 2) // update this line when adding new preset maps
			{
				case 0:
					tile_map = generateEmptyMap();
					break;
				case 1:
					tile_map = presetCenterCubes();
					break;
			}
	}*/

	return &tile_map;
}