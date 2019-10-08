#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

using namespace std;

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;

int room[X_WIDE][Y_HIGH];

int** linesMap () {
	printf("linesmap\n");
	int** room = 0;
	room = new int*[24];

	//initialize random seed
	srand(time(NULL));
	int random_index;
	for(int i = 0; i < 24; i++) {
		random_index = rand() % 24;

		room[i] = new int[13];
		for(int j = 0; j < 13; j++)
		{
			room[i][j] = 0;
		}

		// for(int j = 0; j < X_WIDE; j++) {
		// 	if(i % 2 != 0) {
		// 		if(j == random_index)
		// 			room[j][i] = 0;
		// 		else
		// 			room[j][i] = 2;
		// 	}
		// 	else
		// 		room[j][i] = 0;
		// }
	}
	return room;
}