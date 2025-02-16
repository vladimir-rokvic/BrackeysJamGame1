CFLAGS:= -Iinclude/ -I/usr/include/SDL2
LIBS:= -lSDL2 -lSDL2_mixer
all:
	g++ main.cpp ./src/*.cpp $(CFLAGS) -o break $(LIBS)
