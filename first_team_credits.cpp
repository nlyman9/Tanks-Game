#include <iostream>
#include <vector>
#include <string>

#ifdef __APPLE__ 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#endif // __APPLE__

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

// Function declarations
bool init();
SDL_Texture* loadImage(std::string fname);
void close();
void jakobCredits(SDL_Texture*  picture, SDL_Texture* hitmarker);

// Globals
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
std::vector<SDL_Texture*> gTex;

Mix_Chunk* gSound;

bool init() {
    // Flag what subsystems to initialize
	// For now, just video

    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // Check if we fail to initialize video
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set texture filtering to linear
	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

    gWindow = SDL_CreateWindow("CS1666: The First Team", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	/* Create a renderer for our window
	 * Use hardware acceleration (last arg)
	 * Choose first driver that can provide hardware acceleration
	 *   (second arg, -1)
	 */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	// Initialize PNG loading via SDL_image extension library
	int imgFlags = IMG_INIT_PNG;
	int retFlags = IMG_Init(imgFlags);
	if(retFlags != imgFlags) {
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	// AUDIO
	if (SDL_Init(SDL_INIT_AUDIO) < 0) { // Check if we fail to initialize video
        std::cout << "SDL could not initialize audio! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

	int audFlags = MIX_INIT_MP3;
	retFlags = Mix_Init(audFlags);
	if (audFlags != retFlags) {
		std::cout << "SDL could not initialize MP3! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1)
    {
		std::cout << "SDL could not initialize mixer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;    
    }

	gSound = Mix_LoadWAV( "sounds/jakob_theme.mp3" );

	if (gSound == nullptr) {
		std::cout << "SDL could not load sound file! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}


	return true;
}

SDL_Texture* loadImage(std::string fname) {
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

void close() {
	for (auto i : gTex) {
		SDL_DestroyTexture(i);
		i = nullptr;
	}

	Mix_FreeChunk(gSound);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;
	gSound = nullptr;

	// Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

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
	gTex.push_back(loadImage("images/cs1666_adamibrahim.bmp"));	// index 0
	gTex.push_back(loadImage("images/Alec_Img.bmp"));	// index 1
	gTex.push_back(loadImage("images/Njl26 Credits Picture.bmp"));	// index 2
	gTex.push_back(loadImage("images/AlexClewell_cs1666.bmp"));
	gTex.push_back(loadImage("images/brendanmarani_picture.bmp"));
	gTex.push_back(loadImage("images/CS1666_BenKurzyna.bmp"));
	gTex.push_back(loadImage("images/danny_credit.bmp"));
	gTex.push_back(loadImage("images/dtm32.bmp"));
	gTex.push_back(loadImage("images/ecm53.bmp"));

	for(auto image : gTex) {
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, image, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(3000);
	}

	jakobCredits(loadImage("images/jakob_img.png"), loadImage("images/jakob_hitmarker.png"));

	/* END Jakob's Credit Scene */

	close();
}

void jakobCredits(SDL_Texture*  picture, SDL_Texture* hitmarker) {
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


		// Play audio
		Mix_PlayChannel(-1, gSound, 0); 

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
