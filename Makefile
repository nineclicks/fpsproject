all: project

project: project.cpp game.cpp view.cpp input.cpp vec.cpp
	g++ -g project.cpp game.cpp view.cpp input.cpp vec.cpp -o project -Wall -lX11 -lGL -lGLU -lm

clean:
	rm -f project
	rm -f *.o