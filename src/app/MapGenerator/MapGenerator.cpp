#include "MapGenerator.hpp"
#include <time.h>
#include <math.h>
#include <iostream>
#include "Edge.hpp"
#include "Graph.hpp"
#include "Map.hpp"
#include "QuadConstants.hpp"
#include "Quadrant.hpp"
#include "Quads.hpp"
#include "Tileset.hpp"

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

std::vector<std::vector<int>> MapGenerator::generateMirrorMap()
{
    std::vector<std::vector<int>> array = generateEmptyMap();

	// generate three random cubes
	int box1_x1 = rand() % 5;
	int box1_y1 = 1;
	int box1_x2 = rand() % 5 + 6;
	int box1_y2 = rand() % 2 + 3;

	int box2_x1 = rand() % 5 + 1;
	int box2_y1 = rand() % 2 + 5;
	int box2_x2 = rand() % 5 + 6;
	int box2_y2 = rand() % 2 + 8;

	int box3_x1 = rand() % 5;
	int box3_y1 = rand() % 2 + 10;
	int box3_x2 = rand() % 5 + 6;
	int box3_y2 = 12;

	// check if box1 is big enough
	if(abs((box1_x1 - box1_x2) * (box1_y1 - box1_y2)) >= 4)
	{
		if(box1_x1 > box1_x1)
		{
			int temp = box1_x1;
			box1_x1 = box1_x2;
			box1_x2 = temp;
		}

		if(box1_y1 > box1_y1)
		{
			int temp = box1_y1;
			box1_y1 = box1_y2;
			box1_y2 = temp;
		}

		for(int i = box1_x1; i < box1_x2; i++)
		{
			for(int j = box1_y1; j < box1_y2; j++)
			{
				array[i][j] = 2;
			}
		}
	}

	// check if box2 is big enough
	if(abs((box2_x1 - box2_x2) * (box2_y1 - box2_y2)) >= 4)
	{
		if(box2_x1 > box2_x1)
		{
			int temp = box2_x1;
			box2_x1 = box2_x2;
			box2_x2 = temp;
		}

		if(box2_y1 > box2_y1)
		{
			int temp = box2_y1;
			box2_y1 = box2_y2;
			box2_y2 = temp;
		}

		for(int i = box2_x1; i < box2_x2; i++)
		{
			for(int j = box2_y1; j < box2_y2; j++)
			{
				array[i][j] = 2;
			}
		}
	}

	// check if box3 is big enough
	if(abs((box3_x1 - box3_x2) * (box3_y1 - box3_y2)) > 4)
	{
		if(box3_x1 > box3_x1)
		{
			int temp = box3_x1;
			box3_x1 = box3_x2;
			box3_x2 = temp;
		}

		if(box3_y1 > box3_y1)
		{
			int temp = box3_y1;
			box3_y1 = box3_y2;
			box3_y2 = temp;
		}

		for(int i = box3_x1; i < box3_x2; i++)
		{
			for(int j = box3_y1; j < box3_y2; j++)
			{
				array[i][j] = 2;
			}
		}
	}

	// copy boxes to opposite side and flip
	for(int i = 12; i < 24; i++)
	{
		for(int j = 0; j < 13; j++)
		{
			array[i][j] = array[23 - i][12 - j];
		}
	}

	return array;
}

std::vector<std::vector<int>> MapGenerator::generateLineMap()
{
	std::vector<std::vector<int>> room = generateEmptyMap();
	int random_index;
	int random_index_bonus;

	srand(time(NULL));

	int tile_randomizer = rand() % 2 + 1;

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
					room[j][i] = 1;
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
						if (j + 1 > 23) {
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

	int tile_randomizer = rand() % 2 + 1;

	for(int i = 6; i < 18; i++) {
		for(int j = 3; j < 10; j++) {
			if(i == 11 || i == 12 || j == 6) {
				continue;
			}
			array[i][j] = tile_randomizer;
		}
	}

	return array;
}

std::vector<std::vector<int>> MapGenerator::presetHoleMap()
{
    std::vector<std::vector<int>> array = generateEmptyMap();

	for(int i = 0; i < X_WIDE; i++) {
		if(i != 5 && i != 7) {
			array[i][3] = 1;
		}
	}

	for(int i = 0; i < X_WIDE; i++) {
		if(i != 16 && i != 18) {
			array[i][9] = 1;
		}
	}

	for(int i = 0; i < Y_HIGH; i++) {
		if(i != 8 && i != 10) {
			array[6][i] = 1;
		}
	}

	for(int i = 0; i < Y_HIGH; i++) {
		if(i != 2 && i != 4) {
			array[17][i] = 1;
		}
	}
	return array;
}

std::vector<std::vector<int>> MapGenerator::presetCheckerMap()
{
    std::vector<std::vector<int>> room = generateEmptyMap();
    int pre_array[5] = {2, 6, 11, 16, 20};

	int tile_randomizer = rand() % 2 + 1;

	for(int j = 2; j < Y_HIGH; j+=4) {
		if(j == 2 || j == 10) {
			for(int i = 1; i < X_WIDE; i+=4) {
				room[i][j] = tile_randomizer;
			}
			for(int i = 2; i < X_WIDE; i+=4) {
				room[i][j] = tile_randomizer;
			}
		}
		else if(j == 6) {
			for(int i = 3; i < X_WIDE; i+=4) {
				if(i == 23) {

				}
				else {
					room[i][j] = tile_randomizer;
				}
			}
			for(int i = 4; i < X_WIDE; i+=4) {
				room[i][j] = tile_randomizer;
			}
		}
	}
	return room;
}

std::vector<std::vector<int>> MapGenerator::generateOpenLineMap()
{
	std::vector<std::vector<int>> room = generateEmptyMap();
	int random_index;
	std::vector<int> pre_array = {2, 6, 11, 16, 20};
	srand(time(NULL));

	for(auto i : pre_array) {
		random_index = rand() % (Y_HIGH - 2);
		if(random_index == 0)
		{
			random_index = 1;
		}
		for(int j = 0; j < Y_HIGH; j++) {
			if(random_index == j || random_index == j + 1) {
				room[i][j] = 0;
				room[i + 1][j] = 0;
			} else {
				room[i][j] = 2;
				room[i + 1][j] = 2;
			}
		}
	}

	return room;
}

std::vector<std::vector<int>> MapGenerator::generateQuadrantMap() {
	Quads q;
	q.make_quads();
	Graph g(q.get_edges(), q.get_quads(), q.get_num_tilesets());
	g.populate_edges();
	g.calculate_weights();
	std::vector<Tileset> map_tiles = g.get_tiles_for_map();
	Map m;
	m.make_map(map_tiles);
	std::vector<std::vector<int>> room = generateEmptyMap();
	room = m.get_map();
	return room;
}

std::vector<std::vector<int>>* MapGenerator::generateMap()
{
  	// UPDATE THESE WHEN ADDING NEW MAP TYPES
	int NUM_GEN = 4;
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
		case 4:
			tile_map = generateMirrorMap();
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

	// MAKE TRUE TO PRINT MAP FOR DEBUG PURPOSES
	if(false)
	{
		std::cout << "MapGenerator::generateMap()" << std::endl;
		std::vector<std::vector<int>> transpose;

		for(int i = 0; i < Y_HIGH; i++)
		{
			transpose.push_back(std::vector<int>(X_WIDE));
			for(int j = 0; j < X_WIDE; j++)
			{
				transpose[i][j] = tile_map[j][i];
			}
		}

		for( auto i : transpose )
		{
			for( auto j : i )
				std::cout << j;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// for testing
	tile_map[3][3] = 4;

	return &tile_map;
}