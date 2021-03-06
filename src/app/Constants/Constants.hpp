// Lists of constants
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int BORDER_GAP = 16;
const int OBST_WIDTH = 70;
const int OBST_HEIGHT = 40;
const int TANK_WIDTH = 30;
const int TANK_HEIGHT = 30;
const int PROJECTILE_WIDTH = 15;
const int PROJECTILE_HEIGHT = 6;
const int EXPLOSION_WIDTH = 30;
const int EXPLOSION_HEIGHT = 30;
const int EXPLOSION_FRAMES = 6;
const int MAX_VELOCITY = 2;
const int MAX_PLAYER_VELOCITY = MAX_VELOCITY * 60;
const int TILE_SIZE = 48;
const int FPS = 60;
const double MS_PER_UPDATE = 1000/(double)FPS;  // 1000 MS divided by FPS
const int NUM_OBSTACLES = 312;
const float PHI = 3;
const int TURRET_PHI = 6;
const int CROSSHAIR_SIZE = 30;
const int TIMER_LENGTH = 300;
const int BOMB_HEIGHT = 22;
const int BOMB_WIDTH = 17;
#endif
