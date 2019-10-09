# CS1666-First-Team

CS 1666 Principles of Computer Game Design and Implementation

Fall 2019

# Set Up
Run `sh setup.sh` to install the following libraries
* SDL2
* SDL2 Image
* SDL2 ttf
* SDL2 Mixer
# Running the Game
1. Run `sh build_game.sh`
2. `./build/bin/Game`
# Running the Game w/out a Shell Script
1. `mkdir build`
2. `cd build`
3. `cmake ../src -DMAC=0`
4. `make`
