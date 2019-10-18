/**
 * @file player.cpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Player class implementation
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "Player.hpp"
#include "Constants.hpp"

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param sprite 
 * @param turret 
 * @param x 
 * @param y 
 */
Player::Player(Sprite *sprite, Sprite *turret, int x, int y) {
    setSprite(sprite);
    setPos(x, y);
}

Player::Player(int x, int y) {
    setPos(x, y);
}

/**
 * @brief Destroy the Player:: Player object
 * 
 */
Player::~Player() {}

/**
 * @brief draws the player object 
 *  Overrides base class OBJECT
 * 
 * @param update_lag - the value to extrapolate by
 */
void Player::draw(SDL_Renderer *gRenderer, double update_lag) {
    // Extrapolate the x and y positions 
    // "Solves" stuck in the middle rendering.
    int x_pos = getX() + x_vel * update_lag;
    int y_pos = getY() + y_vel * update_lag;

    // Render to screen (gRenderer)
    // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	// SDL_Rect fillRect = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
	// SDL_RenderFillRect(gRenderer, &fillRect);

    // SDL_Rect pos = {x_pos, y_pos, BOX_WIDTH, BOX_HEIGHT};
    // SDL_RenderCopy(gRenderer, getSprite()->getTexture(), NULL, &pos);
    SDL_Rect src = {0, 0, 20, 20};
    SDL_Rect* dst = get_box();
    SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), &src, dst, theta, NULL, SDL_FLIP_NONE);
}

/**
 * @brief update the player object
 *  Overrides base class OBJECT
 * 
 */
void Player::update() {
    // Move player

    setPos(getX() + x_vel, getY() + y_vel);

    SDL_Rect* overlap;
    SDL_Rect* box = get_box(); // required to update box
    for(auto obstacle : obstacles) {
        overlap = check_collision(&obstacle);
        if(overlap != nullptr) {
            std::cout << overlap->h << " " << overlap->w << " " << theta << std::endl;
            if(abs(theta) > 0 && abs(theta) < 180) {
                setY(getY() - overlap->h - 1);
            } else if(abs(theta) > 180 && abs(theta) < 0) {
                setY(getY() + overlap->h + 1);
            }
            if(abs(theta) > 90 && abs(theta) < 270) {
                setX(getX() - overlap->w - 1);
            } else if(abs(theta) > 270 && abs(theta) < 90) {
                setX(getX() + overlap->w + 1);
            }
        }
    }

    // Check he isn't moving outside of the map
    if (getX() > SCREEN_WIDTH - TILE_SIZE)
    {
        setX(SCREEN_WIDTH - TILE_SIZE);
    }
    if (getX() < TILE_SIZE)
    {
        setX(TILE_SIZE);
    }
    if (getY() < TILE_SIZE)
    {
        setY(TILE_SIZE);
    }
    if (getY() > SCREEN_HEIGHT - TILE_SIZE)
    {
        setY(SCREEN_HEIGHT - TILE_SIZE);
    }
}

/**
 * @brief move the player an offset from its current x-y position
 *  Overrides base class OBJECT
 * 
 * @param x - how much to move player's current x position by
 * @param y - how much to move player's current y position by
 * 
 * @return true  - moved player succesfully 
 * @return false - failed to move player 
 */
bool Player::move(int x, int y) {
    return false;
}

/**
 * @brief place/teleport player to a specified x-y location
 * 
 * @param x - set x position
 * @param y - set y position
 * 
 * @return true  - placed player succesfully 
 * @return false - failed to place player 
 */
bool Player::place(int x, int y) {
    return false;
}

/* Player Specific Functions */

bool Player::fire() {
    return false;
}

bool Player::rotatePlayer(float theta) {
    return false;
}

bool Player::rotateTurret(float theta) {
    return false;
}

// TODO - Change to scancodes?
void Player::getEvent(SDL_Event e) {

    delta_velocity = 0;
    x_deltav = 0;
    y_deltav = 0;

    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_W]) {
        delta_velocity += 1;
        x_deltav += delta_velocity * cos((theta * M_PI) / 180);
        y_deltav += delta_velocity * sin((theta * M_PI) / 180);
    }
    if (keystate[SDL_SCANCODE_A]) {
        theta -= PHI;
    }
    if (keystate[SDL_SCANCODE_S]) {
        delta_velocity -= 1;
        x_deltav += delta_velocity * cos((-theta * M_PI) / 180);
        y_deltav += delta_velocity * sin((-theta * M_PI) / 180);
    }
    if (keystate[SDL_SCANCODE_D]) {
        theta += PHI;
    }

    theta %= 360;

	if (delta_velocity == 0) {
        // No user-supplied "push", return to rest
        if (velocity > 0)
            delta_velocity = -1;
        else if (velocity < 0)
            delta_velocity = 1;
    }

    if (x_deltav == 0) {
        // No user-supplied "push", return to rest
        if (x_vel > 0)
            x_deltav = -1;
        else if (x_vel < 0)
            x_deltav = 1;
    }

    if (y_deltav == 0) {
        // No user-supplied "push", return to rest
        if (y_vel > 0)
            y_deltav = -1;
        else if (y_vel < 0)
            y_deltav = 1;
    }

    // Speed up/slow down
    velocity += delta_velocity;
    x_vel += x_deltav;
    y_vel += y_deltav;

    // Move box
    if (velocity > MAX_VELOCITY)
    {
        velocity = MAX_VELOCITY;
    }
    if (velocity < -MAX_VELOCITY)
    {
        velocity = -MAX_VELOCITY;
    }

    if(x_vel > MAX_VELOCITY) 
    {
        x_vel = MAX_VELOCITY;
    }
    if(x_vel < -MAX_VELOCITY) 
    {
        x_vel = -MAX_VELOCITY;
    }

    if(y_vel > MAX_VELOCITY) 
    {
        y_vel = MAX_VELOCITY;
    }
    if(y_vel < -MAX_VELOCITY) 
    {
        y_vel = -MAX_VELOCITY;
    }
}