/**
 * @file object.hpp
 * @author Networking Team (Adam Alec Jakob)
 * @brief Header for abstract object class for general entities in the game
 * @version 0.1
 * @date 2019-09-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

/**
 * @brief UNDEFINED means the object/type has not been defined/implemented yet
 * 
 * UNDEFINED = bool -> Essentially should be void but it doesn't work.
 */
using UNDEFINED = bool;
using Sprite = UNDEFINED;


#ifndef OBJECT_HPP
#define OBJECT_HPP  
#include <iostream>
#include <SDL2/SDL.h>
class OBJECT {
    private:
        Sprite sprite;
        struct pos {
            int x;
            int y;
        };
    public:

        /**
         * @brief Updates the object at a fixed timestep;
         * 
         * Fixed timesteps allow for updates to be deterministic.
         */
        virtual void update() = 0;

        /**
         * @brief draw the object to the screen; uses float parameter for extrapolation.
         * 
         * @param update_lag (float) {Jakob -> "We can change the name, I can't think of a good one."}
         *  The value used to exptrapolate; should be in range of [0.0, 1.0)
         *  Helps solve the "stuck in the middle" problem of rendering a frame 
         *  in-between two updates.
         */
        virtual void draw(SDL_Renderer *gRenderer, double update_lag) = 0;

        // virtual struct pos* getPos() = 0;

        /**
         * @brief move the object an offset from its current x-y position
         * 
         * @param x - how much to move object's current x position by
         * @param y - how much to move object's current y position by
         * 
         * @return true  - moved object succesfully 
         * @return false - failed to move object 
         */
        virtual bool move(int x, int y) = 0;

        /**
         * @brief place/teleport an object to a 2D location
         * 
         * @param x - set x position
         * @param y - set y position
         * 
         * @return true  - placed player succesfully 
         * @return false - failed to place player 
         */
        virtual bool place(int x, int y) = 0;
        virtual bool check_collision(OBJECT* A, OBJECT* B) = 0;
        // virtual ~OBJECT() = 0; //destructor
};

#endif