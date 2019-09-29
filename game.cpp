#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int BOX_WIDTH = 20;
constexpr int BOX_HEIGHT = 20;

constexpr int MAX_SPEED = 5;

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

void display_credits(){
    gTex.clear();
    gTex.push_back(loadImage("images/cs1666_adamibrahim.bmp"));	// index 0
	gTex.push_back(loadImage("images/Alec_Img.bmp"));	// index 1
	gTex.push_back(loadImage("images/Njl26 Credits Picture.bmp"));	// index 2
	gTex.push_back(loadImage("images/AlexClewell_cs1666.bmp"));
	gTex.push_back(loadImage("images/brendanmarani_picture.bmp"));
	gTex.push_back(loadImage("images/CS1666_BenKurzyna.bmp"));
	gTex.push_back(loadImage("images/danny_credit.bmp"));
	gTex.push_back(loadImage("images/dtm32.bmp"));
	gTex.push_back(loadImage("images/ecm53.bmp"));

    for(auto image : gTex){
        SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, image, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(3000);
    }

    jakobCredits(loadImage("images/jakob_img.png"), loadImage("images/jakob_hitmarker.png"));

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

bool init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}
	
	gWindow = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Adding VSync to avoid absurd framerates
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == nullptr) {	
		std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}

	// Set renderer draw/clear color
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
			
	return true;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	// Current position to render the box
	// Start off with it in the middle
	int x_pos = SCREEN_WIDTH/2 - BOX_WIDTH/2;
	int y_pos = SCREEN_HEIGHT/2 - BOX_HEIGHT/2;

	// Current velocity of the box
	// Start off at reset
	int x_vel = 0;
	int y_vel = 0;

	SDL_Event e;
	bool gameon = true;
	while(gameon) {

		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				gameon = false;
			}
			else if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        display_credits();
                        break;
					case SDLK_w:
						y_vel -= 1;
						break;

					case SDLK_a:
						x_vel -= 1;
						break;

					case SDLK_s:
						y_vel += 1;
						break;

					case SDLK_d:
						x_vel += 1;
						break;
				}
			}
			
		}

        //cap movement speed
        if(x_vel > MAX_SPEED){
            x_vel = MAX_SPEED;
        }else if(x_vel < -MAX_SPEED){
            x_vel = -MAX_SPEED;
        }
        if(y_vel > MAX_SPEED){
            y_vel = MAX_SPEED;
        }else if(y_vel < -MAX_SPEED){
            y_vel = -MAX_SPEED;
        }

		// Move box
		x_pos += x_vel;
		y_pos += y_vel;

        //check boundaries
        if(x_pos < 0){
            x_pos = 0;
            x_vel = 0;
        }else if(x_pos > (SCREEN_WIDTH - BOX_WIDTH)){
            x_pos = SCREEN_WIDTH - BOX_WIDTH;
            x_vel = 0;
        }
        if(y_pos < 0){
            y_pos = 0;
            y_vel = 0;
        }else if(y_pos > (SCREEN_HEIGHT - BOX_HEIGHT)){
            y_pos = SCREEN_HEIGHT - BOX_HEIGHT;
            y_vel = 0;
        }
		
		// Draw box
		// Clear black
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(gRenderer);
		
		// Cyan box
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
		SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
		SDL_RenderFillRect(gRenderer, &fillRect);
		
        SDL_RenderPresent(gRenderer);

	}

	// Out of game loop, clean up
	close();
}
