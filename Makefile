CC = clang++
COMPILER_FLAGS = -Wall -I/usr/include/SDL2
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

Game: Tests/game.cpp
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

clean:
	rm *.o
