OBJ = main.cpp vector2d.hpp vector2d.cpp
all: 
	g++ $(OBJ) -w -lSDL2 -o ray.out
