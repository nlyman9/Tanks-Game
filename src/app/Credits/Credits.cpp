#include <iostream>
#include <string>
#include <vector>
#include "Credits.hpp"
#include "ImageLoader.hpp"

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#endif // __APPLE__

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

	std::vector<SDL_Texture*> creditsImages;
	// Load media
	creditsImages.push_back(loadImage("src/res/images/cs1666_adamibrahim.bmp", render->getRenderer()));	// index 0
	creditsImages.push_back(loadImage("src/res/images/Alec_Img.bmp", render->getRenderer()));	// index 1
	creditsImages.push_back(loadImage("src/res/images/Njl26 Credits Picture.bmp", render->getRenderer()));	// index 2
	creditsImages.push_back(loadImage("src/res/images/AlexClewell_cs1666.bmp", render->getRenderer()));
	creditsImages.push_back(loadImage("src/res/images/brendanmarani_picture.bmp", render->getRenderer()));
	creditsImages.push_back(loadImage("src/res/images/danny_credit.bmp", render->getRenderer()));
	// creditsImages.push_back(loadImage("src/res/images/dtm32.bmp", render->getRenderer()));
	creditsImages.push_back(loadImage("src/res/images/ecm53.bmp", render->getRenderer()));

	for(auto image : creditsImages) {
		SDL_RenderClear(render->getRenderer());
		SDL_RenderCopy(render->getRenderer(), image, NULL, NULL);
		SDL_RenderPresent(render->getRenderer());
		SDL_Delay(3000);
	}

	jakobCredits(loadImage("src/res/images/jakob_img.png", render->getRenderer()), loadImage("src/res/images/jakob_hitmarker.png", render->getRenderer()));

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
	SDL_RenderClear(render->getRenderer());
	SDL_RenderCopy(render->getRenderer(), picture, NULL, NULL);
	SDL_RenderPresent(render->getRenderer());

	SDL_Delay(2500);

	// Create texture of jakob.bmp so I can write other things on it
	SDL_Texture* back = SDL_CreateTexture(render->getRenderer(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (int i = 0; i < 6; i++) {
		// Set target to sdl texture back
		SDL_SetRenderTarget(render->getRenderer(), back);
		// Redraw the first image first.
		SDL_RenderCopy(render->getRenderer(), picture, NULL, NULL);
		
		// Wait 0.5 seconds
		if (i < 5) 
			SDL_Delay(200);  
		else 
			SDL_Delay(750);

		// Render hit marker on background
		SDL_RenderCopy(render->getRenderer(), hitmarker, NULL, &(pos[i]));

		// Render edited texture 'back' to screen.
		SDL_SetRenderTarget(render->getRenderer(), NULL);
		SDL_RenderCopy(render->getRenderer(), back, NULL, NULL);
		SDL_RenderPresent(render->getRenderer());
	}
	SDL_Delay(1500);    

	/* END Jakob's Credit Scene */
}
