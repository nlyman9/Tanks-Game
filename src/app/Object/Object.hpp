/**
 * @file Object.hpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Header for abstract Object class for general entities in the game
 * @version 0.1
 * @date 2019-09-29
 *
 * @copyright Copyright (c) 2019
 *
 */

/**
 * @brief UNDEFINED means the Object/type has not been defined/implemented yet
 *
 * UNDEFINED = bool -> Essentially should be void but it doesn't work.
 */
using UNDEFINED = bool;

#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "Sprite.hpp"
#include "Constants.hpp"


class Point {
     public:
          float x;
          float y;

          friend std::ostream& operator<<(std::ostream& out, const Point &p);
};

inline std::ostream& operator<<(std::ostream& out, const Point &p) {
     out << "[x: " << p.x << " | y: " << p.y << "]";
     return out;
}

class BoundingBox {
     public:
          Point backLeft;
          Point backRight;
          Point frontLeft;
          Point frontRight;

          friend std::ostream& operator<<(std::ostream& out, const BoundingBox &box);
};

inline std::ostream& operator<<(std::ostream& out, const BoundingBox &box) {
     out << "Back Left: " << box.backLeft << "  Front Left : " << box.frontLeft << std::endl;
     out << "Back Right: " << box.backRight << "  Front Right : " << box.frontRight;
     return out;
}


class Object
{
private:
    Sprite sprite;
    float x;
    float y;
    SDL_Rect box;


public:
    Object() {};
    SDL_Rect* check_collision(Object *B);
    SDL_Rect* check_collision(SDL_Rect *B);
    SDL_Rect* check_collision(SDL_Rect* A, SDL_Rect *B);
    SDL_Rect *get_box();
    bool check_bounds();
    void setSprite(Sprite *new_sprite);
    void setTurretSprite(Sprite* new_sprite);

    void setPos(float x, float y);
    void setX(float x);
    void setY(float y);
    float getX();
    float getY();

    virtual BoundingBox* getBoundingBox() = 0;

    void setObstacleLocations(std::vector<SDL_Rect>* obstaclesLocs);

    std::vector<SDL_Rect> obstacles;
    Sprite* getSprite();
    Sprite* getTurretSprite();
    /**
         * @brief Updates the Object at a fixed timestep;
         *
         * Fixed timesteps allow for updates to be deterministic.
         */
    virtual void update() = 0;

    /**
         * @brief draw the Object to the screen; uses float parameter for extrapolation.
         *
         * @param update_lag (float) {Jakob -> "We can change the name, I can't think of a good one."}
         *  The value used to exptrapolate; should be in range of [0.0, 1.0)
         *  Helps solve the "stuck in the middle" problem of rendering a frame
         *  in-between two updates.
         */
    virtual void draw(SDL_Renderer *gRenderer, double update_lag) = 0;

    // virtual struct pos* getPos() = 0;

    /**
         * @brief move the Object an offset from its current x-y position
         *
         * @param x - how much to move Object's current x position by
         * @param y - how much to move Object's current y position by
         *
         * @return true  - moved Object succesfully
         * @return false - failed to move Object
         */
    virtual bool move(float x, float y) = 0;

    /**
         * @brief place/teleport an Object to a 2D location
         *
         * @param x - set x position
         * @param y - set y position
         *
         * @return true  - placed player succesfully
         * @return false - failed to place player
         */
    virtual bool place(float x, float y) = 0;
    ~Object(); //destructor
};

#endif
