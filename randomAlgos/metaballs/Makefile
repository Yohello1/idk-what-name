#OBJS specifies which files to compile as part of the project
OBJS = birds.c

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image `sdl2-config --cflags --libs` -lpthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = draw2

#This is the target that compiles our executable
testDisp : testDisp.cpp
	icpx testDisp.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

metaBalls2D : metaBalls2D.cpp
	icpx metaBalls2D.cpp -O2 $(COMPILER_FLAGS) $(LINKER_FLAGS) -lm -o draw2
	./draw2
