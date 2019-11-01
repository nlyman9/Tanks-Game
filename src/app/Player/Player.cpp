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
Player::Player(Sprite *sprite, Sprite *turret, float x, float y, KeyboardController* keyboardController, NetworkController* networkController) : keyController{keyboardController}, netController{networkController} {
    setSprite(sprite);
    setTurretSprite(turret);
    setPos(x, y);
}

Player::Player(float x, float y, KeyboardController* keyboardController, NetworkController* networkController) : keyController{keyboardController}, netController{networkController} {
    setPos(x, y);
}

/**
 * @brief Destroy the Player:: Player object
 *
 */
Player::~Player() {}


/**
 * @brief draws the player object
 *  Overrides base class Object
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
    
    SDL_Rect* dst = get_box();
    SDL_Rect* turret_dst = get_box();

    SDL_RenderCopyEx(gRenderer, getSprite()->getTexture(), NULL, dst, theta, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(gRenderer, getTurretSprite()->getTexture(), NULL, turret_dst, turretTheta, NULL, SDL_FLIP_NONE);
}

/**
 * @brief update the player object
 *  Overrides base class Object
 *
 */
void Player::update() {
    // Move the turret
    // Center the delta x and y by the center of the tank
    float delta_x = mouseX - (getX() + TANK_WIDTH / 2);
    float delta_y = mouseY - (getY() + TANK_HEIGHT / 2);
    float theta_radians = atan2(delta_y, delta_x);
    mouseTheta = (int)(theta_radians * 180 / M_PI); 
    turretTheta = mouseTheta;

    // Move player
    // Rotate player
    rotatePlayer(theta_v);

    float updateStep = MS_PER_UPDATE / 1000;

    // NEW IMPLEMENTATION
    SDL_Rect* overlap;
    SDL_Rect currentPos;

    // first add X velocity
    setX(getX() + (x_vel * updateStep));

    currentPos = {(int)getX(), (int)getY(), TANK_WIDTH, TANK_HEIGHT};

    // correct for collisions
    for(auto obstacle : obstacles) {
        overlap = check_collision(&currentPos, &obstacle);
        if(overlap != nullptr) {
            if(x_vel < 0) {
                setX(floor(getX() + overlap->w));
            } else {
                setX(floor(getX() - overlap->w));
            }
            break;
        }
    }

    // next add Y velocity
    setY(getY() + (y_vel * updateStep));

    currentPos = {(int)getX(), (int)getY(), TANK_WIDTH, TANK_HEIGHT};

    // correct for collisions
    for(auto obstacle : obstacles) {
        overlap = check_collision(&currentPos, &obstacle);
        if(overlap != nullptr) {
            if(y_vel < 0) {
                setY(floor(getY() + overlap->h));
            } else {
                setY(floor(getY() - overlap->h));
            }
            break;
        }
    }

    // Check he isn't moving outside of the map
    if (getX() + TANK_WIDTH > SCREEN_WIDTH - TILE_SIZE - 16)
    {
        setX(SCREEN_WIDTH - TILE_SIZE - 16 - TANK_WIDTH);
    }
    if (getX() < TILE_SIZE + 16)
    {
        setX(TILE_SIZE + 16);
    }
    if (getY() < TILE_SIZE)
    {
        setY(TILE_SIZE);
    }
    if (getY() + TANK_HEIGHT > SCREEN_HEIGHT - TILE_SIZE)
    {
        setY(SCREEN_HEIGHT - TILE_SIZE - TANK_HEIGHT);
    }
}

/**
 * @brief move the player an offset from its current x-y position
 *  Overrides base class Object
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

bool Player::getFire() {
    return fire;
}

bool Player::setFire(bool fire) {
    this->fire = fire;
    return true;
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

int Player::getTheta() {
    return theta;
}

int Player::getTurretTheta() {
    return turretTheta;
}

const Uint8* Player::getEvent(std::chrono::duration<double, std::ratio<1, 1000>> time, SDL_Event* e) {

    delta_velocity = 0;
    x_deltav = 0;
    y_deltav = 0;
    theta_v = 0;
    fire = false;

    const Uint8* keystate; 
    if(keyController != nullptr) {
        keystate = keyController->pollEvent();
    } else {
        keystate = netController->pollEvent();
    }

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

    if(e->type == SDL_MOUSEBUTTONDOWN) {
  		Uint32 current_time = SDL_GetTicks();

  		if (current_time > fire_last_time + 3000) {
  			fire = true;
  			fire_last_time = current_time;
  		}
  	}

    if(theta < 0) {
        theta = 360 + theta;
    }
    theta %= 360;

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

    // Mouse Motion Handling
    if(e->type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    return keystate;
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
