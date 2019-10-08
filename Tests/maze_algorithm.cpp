#include <time.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <math.h>

constexpr int X_WIDE = 24;
constexpr int Y_HIGH = 13;
// int maze[X_WIDE][Y_HIGH];
vector<vector<int>> maze(X_WIDE, vector<int> (Y_HIGH, 0));

    /* 2 is wall
       0 is empty space
       3 is exposed but undetermined
       4 is unexposed and undetermined */

void carve(int y, int x) {
    vector<vector<int>> maze(X_WIDE, vector<int> (Y_HIGH, 0));
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
            extra.insert(x+1, y);
        }      
    }
    std::random_shuffle(extra.begin(), extra.end());
    frontier.insert(frontier.end(), extra.begin(), extra.end());
}

void harden(int x, int y) {
    maze[x][y] = 2;
}

bool check(int x, int y, bool nondiagonals) {
    
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
                    if (field[x+1][y-1] == 0) return False;
                if (y < Y_HIGH-1)
                    if (field[x+1][y+1] == 0) return False;
            return True;        
        } else if (edgestate == 2) {
            if (x > 0)
                if (y > 0)
                    if (field[x-1][y-1] == 0) return False;
                if (y < Y_HIGH-1) 
                    if (field[x-1][y+1] == 0) return False;
            return True;
        } else if (edgestate == 4) {
            if (y < Y_HIGH-1)
                if (x > 0)
                    if (field[x-1][y+1] == 0) return False;
                if (x < X_WIDE-1)
                    if (field[x+1][y+1] == 0) return False;
            return True;
        } else if (edgestate == 8) {
            if (y > 0)
                if (x > 0)
                    if (field[x-1][y-1] == 0) return False;
                if (x < X_WIDE-1)
                    if (field[x+1][y-1] == 0) return False
            return True;
        }
        return False;
    } else {
        if (edgestate == 1 || edgestate == 2 || edgestate == 4 || edgestate == 8) return True;
        return False;
    }

}

int main () {

    // for (int i = 0; i < X_WIDE; i++) {
    //     for (int j = 0; j < Y_HIGH; j++) {
    //         maze[x][y] = 0;
    //     }
    // }
    srand(Time(NULL));
    int xchoice = rand() % 24;
    int ychoice = rand() % 13;
    int branchrate = 0;
    int pos = 0;

    carve(ychoice, xchoice);
    
    while(i < frontier.size()) {
        pos = rand();
        pos = pow(exp(-branchrate));

    }
    return 0;
}