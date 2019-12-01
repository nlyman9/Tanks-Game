#include "Bomb.hpp"

Bomb::Bomb(SDL_Rect* box, int theta, Sprite* bombBlack, Sprite* bombRed, Sprite* explosion) :
    blackBomb(bombBlack),
    redBomb(bombRed),
    bombExplosion(explosion) 
{
    startTime = std::chrono::steady_clock::now();

    // Calculate position of bomb center
    int bombCenterX = box->x + box->w / 2 + ((box->w / 2 + BOMB_DROP_PADDING) * cos((theta + 180) * M_PI / 180));
    int bombCenterY = box->y + box->h / 2 + ((box->h / 2 + BOMB_DROP_PADDING) * sin((theta + 180) * M_PI / 180));
    
    // Get top left corner of bomb box
    int x = bombCenterX - BOMB_WIDTH / 2;
    int y = bombCenterY - BOMB_HEIGHT / 2;

    // Set the position of the bomb to behind the tank
    setPos(x, y);

    // Populate the vector of ns intervals to change the bomb color
    flipIntervals.push_back(1000000000);
    flipIntervals.push_back(1750000000);
    flipIntervals.push_back(2250000000);
    flipIntervals.push_back(2450000000);
    flipIntervals.push_back(2550000000);
    flipIntervals.push_back(2600000000);
    flipIntervals.push_back(2650000000);
    flipIntervals.push_back(2700000000);
    flipIntervals.push_back(2750000000);
    flipIntervals.push_back(2800000000);
    flipIntervals.push_back(2850000000);
    flipIntervals.push_back(2900000000);
    flipIntervals.push_back(2950000000);
}

Bomb::Bomb(SDL_Rect* box, int theta) :
    blackBomb(nullptr),
    redBomb(nullptr),
    bombExplosion(nullptr) 
{
    startTime = std::chrono::steady_clock::now();

    // Calculate position of bomb center
    int bombCenterX = box->x + box->w / 2 + ((box->w / 2 + BOMB_DROP_PADDING) * cos((theta + 180) * M_PI / 180));
    int bombCenterY = box->y + box->h / 2 + ((box->h / 2 + BOMB_DROP_PADDING) * sin((theta + 180) * M_PI / 180));
    
    // Get top left corner of bomb box
    int x = bombCenterX - BOMB_WIDTH / 2;
    int y = bombCenterY - BOMB_HEIGHT / 2;

    // Set the position of the bomb to behind the tank
    setPos(x, y);

    // Populate the vector of ns intervals to change the bomb color
    flipIntervals.push_back(1000000000);
    flipIntervals.push_back(1750000000);
    flipIntervals.push_back(2250000000);
    flipIntervals.push_back(2450000000);
    flipIntervals.push_back(2550000000);
    flipIntervals.push_back(2600000000);
    flipIntervals.push_back(2650000000);
    flipIntervals.push_back(2700000000);
    flipIntervals.push_back(2750000000);
    flipIntervals.push_back(2800000000);
    flipIntervals.push_back(2850000000);
    flipIntervals.push_back(2900000000);
    flipIntervals.push_back(2950000000);
}

Bomb::~Bomb() {}

void Bomb::draw(SDL_Renderer *gRenderer, double update_lag) {
    SDL_Rect* dst = get_box();

	if(!exploding) {
		dst->w = BOMB_WIDTH;
		dst->h = BOMB_HEIGHT;

        // Change bomb color
        if(flipColor) {
            setSprite(blackBomb);
        } else {
            setSprite(redBomb);
        }
        // Render the bomb
		SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), NULL, dst, 0.0, NULL, SDL_FLIP_NONE);
	}
	else 
	{
        setSprite(bombExplosion);
		Uint32 current_time = SDL_GetTicks();
		dst->w = EXPLOSION_WIDTH;
		dst->h = EXPLOSION_WIDTH;

		if(frame == 0 && anim_last_time == 0) {
			anim_last_time = SDL_GetTicks();
		}

		if(current_time > anim_last_time + 100) {
			frame++;
			anim_last_time = SDL_GetTicks();
		}

		if(frame < 6) {
			SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), getSprite()->getFrame(frame), dst, 0.0, NULL, SDL_FLIP_NONE);
		} else {
			finished = true;
      		exploding = false;
        }
	}
}

void Bomb::update() {
    currTime = std::chrono::steady_clock::now();
    auto duration = currTime - startTime;

    // Bomb Explodes after 3 seconds
    if(!finished) {
        if(duration.count() >= COUNT_DOWN_NSEC) {
            exploding = true;
        }
    }

    // Flip flag for bomb colorat increasing rate until explosion
    if(duration.count() >= flipIntervals.at(flipCounter)) {
        if(flipCounter < flipIntervals.size() - 1) {
            flipCounter++;
        }
        flipColor = !flipColor;
    }
}