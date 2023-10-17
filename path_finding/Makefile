#OBJS specifies which files to compile as part of the project
OBJS = aStart.cpp
OBFILES = aStart.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -O3

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = draw2

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(COMPILER_FLAGS) -c $(OBJS)
	$(CC) $(OBFILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./draw2
 
boids : boids.cpp
	$(CC) $(COMPILER_FLAGS) -c boids.cpp
	$(CC) boids.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

aStar : aStart.cpp
	$(CC) $(COMPILER_FLAGS) -c aStart.cpp
	$(CC) aStart.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2
