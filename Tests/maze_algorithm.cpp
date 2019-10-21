#include <time.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <tuple>

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;
// int maze[X_WIDE][Y_HIGH];
std::vector<std::vector<int>> maze(X_WIDE, std::vector<int> (Y_HIGH, 0));

    /* 2 is wall
       0 is empty space
       3 is exposed but undetermined
       4 is unexposed and undetermined */

void carve(int y, int x) {
    std::vector<int> frontier;
    std::vector<tuple<int, int>> extra;
    std::vector<std::vector<int>> maze(X_WIDE, std::vector<int> (Y_HIGH, 0));
    maze[x][y] = 0;
    if (y > 0) {
        if (maze[x][y-1] == 4) {
            maze[x][y-1] = 3;
            extra.insert(x, y- 1);
        }      
    }
    if (y > Y_HIGH - 1) {
        if (maze[x][y-1] == 4) {
            maze[x][y-1] = 3;
            extra.insert(x, y+1);
        }      
    }
    if (x > 0) {
        if (maze[x][y-1] == 4) {
            maze[x][y-1] = 3;
            extra.insert(x-1, y);
        }      
    }
    if (x > X_WIDE - 1) {
        if (maze[x][y-1] == 4) {
            maze[x][y-1] = 3;
            extra.push_back(std::make_tuple(x+1, y));
        }      
    }
    std::random_shuffle(extra.begin(), extra.end());
    frontier.insert(frontier.end(), extra.begin(), extra.end());
}

void harden(int x, int y) {
    maze[x][y] = 2;
}

bool check(int x, int y, bool nondiagonals) {

    std::vector<std::vector<int>> field;
    
    int edgestate = 0;
    if (x > 0)
        if (field[x-1][y] == 0) edgestate += 1;
    if (x < X_WIDE-1)
        if (field[x+1][y] == 0) edgestate += 2;
    if (y > 0)
        if (field[x][y-1] == 0) edgestate += 4;
    if (y < Y_HIGH-1)
        if (field[x][y+1] == 0) edgestate += 8;

    if (nondiagonals) {
        
        if (edgestate == 1) {
            if (x < X_WIDE-1)
                if (y > 0)
                    if (field[x+1][y-1] == 0) return false;
                if (y < Y_HIGH-1)
                    if (field[x+1][y+1] == 0) return false;
            return true;        
        } else if (edgestate == 2) {
            if (x > 0)
                if (y > 0)
                    if (field[x-1][y-1] == 0) return false;
                if (y < Y_HIGH-1) 
                    if (field[x-1][y+1] == 0) return false;
            return true;
        } else if (edgestate == 4) {
            if (y < Y_HIGH-1)
                if (x > 0)
                    if (field[x-1][y+1] == 0) return false;
                if (x < X_WIDE-1)
                    if (field[x+1][y+1] == 0) return false;
            return true;
        } else if (edgestate == 8) {
            if (y > 0)
                if (x > 0)
                    if (field[x-1][y-1] == 0) return false;
                if (x < X_WIDE-1)
                    if (field[x+1][y-1] == 0) return false;
            return true;
        }
        return false;
    } else {
        if (edgestate == 1 || edgestate == 2 || edgestate == 4 || edgestate == 8) return true;
        return false;
    }

}

int main () {

    // for (int i = 0; i < X_WIDE; i++) {
    //     for (int j = 0; j < Y_HIGH; j++) {
    //         maze[x][y] = 0;
    //     }
    // }
    std::vector<int> frontier;
    srand(time(NULL));
    int xchoice = rand() % 24;
    int ychoice = rand() % 13;
    int branchrate = 0;
    int choice = 0;
    int pos = 0;

    carve(ychoice, xchoice);
    int i = 0;
    while(i < frontier.size()) {
        pos = rand();
        pos = pow(pos, exp(-branchrate));
        choice = frontier[pos*frontier.size()];
        // if (check())

    }
    return 0;
}