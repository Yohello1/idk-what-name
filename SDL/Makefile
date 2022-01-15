#OBJS specifies which files to compile as part of the project
OBJS = voroni.cpp 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -O3

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -g `sdl2-config --cflags --libs`  -lpthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = draw2

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./draw2
 
