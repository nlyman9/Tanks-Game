CPP_FILES = $(wildcard *.cpp)
OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))
CC = clang++
LINKER_FLAGS = -lSDL2

all: $(OUT_NAMES)

%.o: %.cpp
	$(CC) $^ $(LINKER_FLAGS) -o $@

clean:
	rm *.o