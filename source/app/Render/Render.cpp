#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Render.hpp"
#include "GameLoop.hpp"
using namespace mainLoop;

GameLoop* gloop;
SDL_Texture* gTileSheet;
SDL_Rect gTileRects[3];
std::vector<SDL_Texture*> gTex;
SDL_Rect cur_out;
SDL_Texture* loadImage(std::string fname);
Render::~Render() {
    gloop->close();
}

int Render::run() {
		
		int c;
		SDL_Rect cur_out;

        SDL_SetRenderDrawColor(gloop->gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gloop->gRenderer);

		//FROM BORDERGAP + TILE SIZE TO GET INTERIOR OF MAP
		//x represents the pixels of the screen, not the tile index anymore
		/*for (int x = BORDER_GAP + TILE_SIZE; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE) {
			for (int y = TILE_SIZE; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE) {
				cur_out = { x, y, TILE_SIZE, TILE_SIZE};
				SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[0], &cur_out);
			}
		}*/

		//just initializes a new array and fills it with whatever(zeros for the default background)
		int block_render[26][15];
		for (int j = 0; j < 27; j++) {
			for(int j = 0; j < 16; j++ ) {
				block_render[i][j] = 0;
			}
		}

		for (int x = BORDER_GAP + TILE_SIZE, i = 0; x < SCREEN_WIDTH - BORDER_GAP - TILE_SIZE; x+=TILE_SIZE, i++) {
			for (int y = TILE_SIZE, j = 0; y < SCREEN_HEIGHT - TILE_SIZE; y+=TILE_SIZE, j++) {
				cur_out = { x, y, TILE_SIZE, TILE_SIZE};
				SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[block_render[i],[j]], &cur_out);
			}
		}
		

		for (int x = )
		
		//GENERATES TOP BORDER
		c = BORDER_GAP;
		while (c < SCREEN_WIDTH - BORDER_GAP) {
			cur_out = {c, 0, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += TILE_SIZE;
		}
		//GENERATES LEFT BORDER
		c = 0;
		while (c < SCREEN_HEIGHT) {
			cur_out = {BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += TILE_SIZE;
		}
		//GENERATES BOTTOM BORDER
		c = BORDER_GAP;
		while (c < SCREEN_WIDTH - BORDER_GAP) {
			cur_out = {c, SCREEN_HEIGHT - TILE_SIZE, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += TILE_SIZE;
		}
		//GENERATES RIGHT BORDER
		c = 0;
		while (c < SCREEN_HEIGHT) {
			cur_out = {SCREEN_WIDTH - TILE_SIZE - BORDER_GAP, c, TILE_SIZE, TILE_SIZE};
			SDL_RenderCopy(gloop->gRenderer, gTileSheet, &gTileRects[2], &cur_out);
			c += TILE_SIZE;
		}
		// Draw box
		SDL_SetRenderDrawColor(gloop->gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_Rect fillRect = {gloop->x_pos, gloop->y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gloop->gRenderer, &fillRect);

		SDL_SetRenderDrawColor(gloop->gRenderer, 0xff, 0x00, 0xff, 0xff);
        SDL_Rect fillRect_obst = {gloop->x_obst_pos, gloop->y_obst_pos, OBST_WIDTH, OBST_HEIGHT};
		SDL_RenderFillRect(gloop->gRenderer, &fillRect_obst);

		SDL_SetRenderDrawColor(gloop->gRenderer, 0xff, 0x00, 0x00, 0xff);
		SDL_Rect enemyRect = {gloop->x_enemy_pos, gloop->y_enemy_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gloop->gRenderer, &enemyRect);
		SDL_RenderPresent(gloop->gRenderer);


}