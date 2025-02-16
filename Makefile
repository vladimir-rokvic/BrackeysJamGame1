CFLAGS:= -Iinclude/ -Wall
LIBS:= -lSDL2 -lSDL2_mixer -lSDL2_image
all:
	g++ main.cpp ./src/*.cpp $(CFLAGS) -o break $(LIBS)
