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
#include <chrono>
#include "Player.hpp"
#include "Constants.hpp"
#include <math.h>

/**
 * @brief Construct a new Player:: Player object
 * 
 * @param sprite 
 * @param turret 
 * @param x 
 * @param y 
 */
Player::Player(Sprite *sprite, Sprite *turret, float x, float y) {
    setSprite(sprite);
    setPos(x, y);
}

Player::Player(float x, float y) {
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
	
	// Danny: to get the tank to render correctly, I replaced %src with NULL.
    // SDL_Rect src = {0, 0, 20, 20};
	
    SDL_Rect* dst = get_box();
    SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), NULL, dst, theta, NULL, SDL_FLIP_NONE);
}

/**
 * @brief update the player object
 *  Overrides base class OBJECT
 * 
 */
void Player::update() {
    // Move player

    // Rotate player 
    rotatePlayer(theta_v);

    //setPos(getX() + x_vel, getY() + y_vel);
    float updateStep = MS_PER_UPDATE/1000;
    setPos(getX() + (x_vel * updateStep), getY() + (y_vel * updateStep));

    SDL_Rect* overlap;
    SDL_Rect currentPos = {getX(), getY(), TANK_WIDTH + 1.41 * sin((theta * M_PI_4)/180), TANK_HEIGHT + 1.41 * sin((theta * M_PI_4)/180)};

    for(auto obstacle : obstacles) {  
        overlap = check_collision(&currentPos, &obstacle);
        if(overlap != nullptr) {
            //std::cout << overlap->w << ":" << overlap->h << ":" << overlap->x << ":" << overlap->y << std::endl;
            //std::cout << *getBoundingBox() << std::endl << "THETA " << theta << std::endl << std::endl;
            
            setPos(getX() - (x_vel * updateStep) / 2, getY() - (y_vel * updateStep) /2);
            break;
        }

        // BoundingBox *box = getBoundingBox();
            
        // // Collides with top of box
        // if (box->frontRight.x > obstacle.x && box->frontRight.x < obstacle.x + obstacle.w &&
        //     box->frontRight.y > obstacle.y && box->frontRight.y < obstacle.y + obstacle.h ||
        //     box->frontLeft.x > obstacle.x && box->frontLeft.x < obstacle.x + obstacle.w &&
        //     box->frontLeft.y > obstacle.y && box->frontLeft.y < obstacle.y + obstacle.h) {

        //     if (y_vel > 0) {
        //         float y_overlap = std::max((box->frontRight.y - (obstacle.y)), (box->frontLeft.y - (obstacle.y)));
        //         std::cout << box->frontRight.y << " -- " << obstacle.y << " -> " << y_overlap << std::endl;
        //         setX(getX());
        //         setY(getY() - y_overlap);
        //         break;
        //     } else if (y_vel < 0) {
        //         float y_overlap = std::max(((obstacle.y + obstacle.h) - box->frontRight.y), ((obstacle.y + obstacle.h) - box->frontLeft.y));
        //         std::cout << box->frontRight.y << " -- " << obstacle.y << " -> " << y_overlap << std::endl;
        //         setX(getX());
        //         setY(getY() + y_overlap);
        //         break;
        //     }
        // }
        // // Collides with left side of box
        // if (box->frontRight.x > obstacle.x && box->frontRight.x < obstacle.x + obstacle.w/4 &&
        //     box->frontRight.y > obstacle.y && box->frontRight.y < obstacle.y + obstacle.h) {
        //     int norm_vel = abs(y_vel)/y_vel;
        //     if (isnan(norm_vel)) 
        //         norm_vel = 1;

        //     float x_overlap = box->frontRight.x - (obstacle.x * norm_vel);
        //     std::cout << box->frontRight.x << " -- " << obstacle.x << " -> " << x_overlap << std::endl;
        //     setX(getX() - x_overlap);
        //     setY(getY());
        //     break;

        // }
    }

    // Check he isn't moving outside of the map
    if (getX() + TANK_WIDTH > SCREEN_WIDTH - TILE_SIZE - BORDER_GAP)	// Right border
    {
        setX(SCREEN_WIDTH - TILE_SIZE - TANK_WIDTH - BORDER_GAP);
    }
    if (getX() < TILE_SIZE + BORDER_GAP)	// left border
    {
        setX(TILE_SIZE + BORDER_GAP);
    }
    if (getY() < TILE_SIZE)	// Top border
    {
        setY(TILE_SIZE);
    }
    if (getY() + TANK_HEIGHT > SCREEN_HEIGHT - TILE_SIZE)	// bottom border
    {
        setY(SCREEN_HEIGHT - TILE_SIZE - TANK_HEIGHT);
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
bool Player::move(float x, float y) {
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
bool Player::place(float x, float y) {
    return false;
}

/* Player Specific Functions */

bool Player::fire() {
    return false;
}

bool Player::rotatePlayer(float t) {
    theta += t;

    if(theta < 0) {
        theta = 360 + theta;
    }
    theta %= 360;
    return true;
}

bool Player::rotateTurret(float theta) {
    return false;
}

void Player::getEvent(std::chrono::duration<double, std::ratio<1, 1000>> time) {

    delta_velocity = 0;
    x_deltav = 0;
    y_deltav = 0;
    theta_v = 0;

    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    if (keystate[SDL_SCANCODE_W]) {
        delta_velocity += MAX_PLAYER_VELOCITY;
        x_deltav += delta_velocity * cos((theta * M_PI) / 180);
        y_deltav += delta_velocity * sin((theta * M_PI) / 180);
    }
    if (keystate[SDL_SCANCODE_A]) {
        theta_v -= PHI;
    }

    if (keystate[SDL_SCANCODE_S]) {
        delta_velocity -= MAX_PLAYER_VELOCITY;
        x_deltav += delta_velocity * cos((theta * M_PI) / 180);
        y_deltav += delta_velocity * sin((theta * M_PI) / 180);
    }
    if (keystate[SDL_SCANCODE_D]) {
        theta_v += PHI;
    }

    // Set Player's X velocity
    if (x_deltav == 0) {
        // No user-supplied "push", return to rest
        if (x_vel > 0) {
            if (x_vel < (1 * time.count()))
                x_vel = 0;
            else
                x_vel -= (1 * time.count());
        }
        else if (x_vel < 0) {
            if (-x_vel < (1 * time.count()))
                x_vel = 0;
            else 
                x_vel += (1 * time.count());
        }
    } else {
        x_vel = x_deltav; //* time.count();
        //x_vel *= cos((theta * M_PI) / 180);
    }
    
    // Set Player's Y velocity
    if (y_deltav == 0) {
        // No user-supplied "push", return to rest
        if (y_vel > 0) {
            if (y_vel < (1 * time.count()))
                y_vel = 0;
            else
                y_vel -= (1 * time.count());
        }
        else if (y_vel < 0) {
            if (-y_vel < (1 * time.count()))
                y_vel = 0;
            else 
                y_vel += (1 * time.count());
        }

    } else {
        y_vel = y_deltav; //* time.count();
        //y_vel *= sin((theta * M_PI) / 180);
    }


    //Keep for debug purposes
    //std::cout << theta << ":" << x_deltav << ":" << y_deltav << "|" << x_vel << ":" << y_vel << std::endl;

    // Clamp velocities within bounds of -+ MAX_PLAYER_VELOCITY
    if(x_vel > MAX_PLAYER_VELOCITY) 
    {
        x_vel = MAX_PLAYER_VELOCITY;
    }
    if(x_vel < -MAX_PLAYER_VELOCITY) 
    {
        x_vel = -MAX_PLAYER_VELOCITY;
    }

    if(y_vel > MAX_PLAYER_VELOCITY) 
    {
        y_vel = MAX_PLAYER_VELOCITY;
    }
    if(y_vel < -MAX_PLAYER_VELOCITY) 
    {
        y_vel = -MAX_PLAYER_VELOCITY;
    }
}

/**
 * @brief Get the bounding box of the player's tank
 * 
 * The bounding box is based off 4 points: backLeft, backRight, frontLeft, frontRight;
 *  - The front of the tank is the direction the tank is pointing. The back is the opposit.
 *  - The left side is the lefthand side of the direction the the tank is facing. The right is the right hand side.
 * 
 * @return BoundingBox* 
 */
BoundingBox* Player::getBoundingBox() {
    BoundingBox *box = new BoundingBox();
    float sqrt_2 = 1.414213;
    float radius = (TANK_WIDTH * sqrt_2)/2;

    // The player's theta increases clockwise instead of how trig functions naturally increases counter-clockwise
    // So, the the trig functions for backLeft use theta+225 instead of theta+135
    box->backLeft.x = (getX() + (TANK_WIDTH/2)) + radius * cos((theta+225) * M_PI / 180);
    box->backLeft.y = (getY() + (TANK_HEIGHT/2)) + radius * sin((theta+225) * M_PI / 180);

    box->backRight.x = (getX() + (TANK_WIDTH/2)) + radius * cos((theta+135) * M_PI / 180);
    box->backRight.y = (getY() + (TANK_HEIGHT/2)) + radius * sin((theta+135) * M_PI / 180);

    box->frontLeft.x = (getX() + (TANK_WIDTH/2)) + radius * cos((theta+315) * M_PI / 180);
    box->frontLeft.y = (getY() + (TANK_HEIGHT/2)) + radius * sin((theta+315) * M_PI / 180);

    box->frontRight.x = (getX() + (TANK_WIDTH/2)) + radius * cos((theta+45) * M_PI / 180);
    box->frontRight.y = (getY() + (TANK_HEIGHT/2)) + radius * sin((theta+45) * M_PI / 180);

    return box;
}