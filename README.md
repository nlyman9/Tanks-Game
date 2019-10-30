# CS1666-First-Team

CS 1666 Principles of Computer Game Design and Implementation

Fall 2019

# Set Up
Run `sh setup.sh` to install the following libraries
* SDL2
* SDL2 Image
* SDL2 ttf
* SDL2 Mixer

# Building the Game w/out a Shell Script
1. `mkdir build`
2. `cd build`
3. Build types:
  *  `cmake ../src -c` for a clean build 
  * `cmake ../src` for a rebuild 
4. `make`

# Build the Game w/ a Shell Script
* Run `sh build_game.sh`

# Running the Game
* `./build/bin/Game`

# Running Multiplayer 

For now, you have to command line arguments when running the game and then select "Multiplayer" in the menu.

## Host

* `./build/bin/Game host <port>`

## Client 

* `./build/bin/Game client <ip> <port>`
For local connections, use ip `127.0.0.1`.
