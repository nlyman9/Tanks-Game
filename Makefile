CPP_FILES = $(wildcard *.cpp)
OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))
CC = clang++
COMPILER_FLAGS = -Wall -I/usr/include/SDL2
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
MACFLAGS = -std=c++11 -Wall -I/Library/Frameworks/SDL2.framework/Headers -I/Library/Frameworks/SDL2_image.framework/Headers -I/Library/Frameworks/SDL2_mixer.framework/Headers -F/Library/Frameworks/ -framework SDL2_image -framework SDL2 -framework SDL2_mixer
UNIXMAIN = main

all: $(OUT_NAMES)

%.o: %.cpp
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

unix: $(CPP_FILES)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(UNIXMAIN)


mac: sdl2_bmp.cpp
	$(CC) $(MACFLAGS) $^ -o $(UNIXMAIN)

clean:
	rm -f *.o mac main
