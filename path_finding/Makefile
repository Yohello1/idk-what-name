#OBJS specifies which files to compile as part of the project
OBJS = boids.cpp
OBFILES = boids.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -g -Wall 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = draw2

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) -c -g $(OBJS)
	$(CC) $(OBFILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
#	./draw2
 
