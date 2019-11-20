#include <iostream>
#include <vector>
#include <string>

#include "Credits.hpp"
#include <vector>

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif // __APPLE__

SDL_Texture* Credits::loadImage(std::string fname) {
	SDL_Texture* newText = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newText = SDL_CreateTextureFromSurface(gRenderer, startSurf);
	if (newText == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newText;
}

void Credits::close() {
	for (auto i : gTex) {
		SDL_DestroyTexture(i);
		i = nullptr;
	}


	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int Credits::playCredits() {

	// Jakob's Credit Scene
	#ifdef __APPLE__
	// Add a fake event handler so mac will open the window.
	bool quit = false;                                      
	SDL_Event e;                                            
	if (!quit) {                                         
		while (SDL_PollEvent(&e)) {                         
			if (e.type == SDL_QUIT) {                       
				quit = true;                                
			}                                               
		}                                            
	}  
	#endif // __APPLE__

	// Load media
	gTex.push_back(loadImage("src/res/images/cs1666_adamibrahim.bmp"));	// index 0
	gTex.push_back(loadImage("src/res/images/Alec_Img.bmp"));	// index 1
	gTex.push_back(loadImage("src/res/images/Njl26 Credits Picture.bmp"));	// index 2
	gTex.push_back(loadImage("src/res/images/AlexClewell_cs1666.bmp"));
	gTex.push_back(loadImage("src/res/images/brendanmarani_picture.bmp"));
	gTex.push_back(loadImage("src/res/images/CS1666_BenKurzyna.bmp"));
	gTex.push_back(loadImage("src/res/images/danny_credit.bmp"));
	gTex.push_back(loadImage("src/res/images/dtm32.bmp"));
	gTex.push_back(loadImage("src/res/images/ecm53.bmp"));

	for(auto image : gTex) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, image, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(3000);
	}

	jakobCredits(loadImage("src/res/images/jakob_img.png"), loadImage("src/res/images/jakob_hitmarker.png"));

	/* END Jakob's Credit Scene */

	close();
}

void Credits::jakobCredits(SDL_Texture*  picture, SDL_Texture* hitmarker) {
	// Position of hitmarkers
	std::vector<SDL_Rect> pos; 
	pos.push_back({320, 250, 60, 80});
	pos.push_back({760, 250, 60, 120});
	pos.push_back({500, 375, 600, 100});
	pos.push_back({300, 500, 60, 80});
	pos.push_back({1120, 365, 100, 120});
	pos.push_back({0, 0, 1280, 720});

	/* CODE */
	// Render jakob.bmp first
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, picture, NULL, NULL);
	SDL_RenderPresent(gRenderer);

	SDL_Delay(2500);

	// Create texture of jakob.bmp so I can write other things on it
	SDL_Texture* back = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (int i = 0; i < 6; i++) {
		// Set target to sdl texture back
		SDL_SetRenderTarget(gRenderer, back);
		// Redraw the first image first.
		SDL_RenderCopy(gRenderer, picture, NULL, NULL);
		
		// Wait 0.5 seconds
		if (i < 5) 
			SDL_Delay(200);  
		else 
			SDL_Delay(750);

		// Render hit marker on background
		SDL_RenderCopy(gRenderer, hitmarker, NULL, &(pos[i]));

		// Render edited texture 'back' to screen.
		SDL_SetRenderTarget(gRenderer, NULL);
		SDL_RenderCopy(gRenderer, back, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}
	SDL_Delay(1500);    

	/* END Jakob's Credit Scene */
}
