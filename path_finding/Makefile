#OBJS specifies which files to compile as part of the project
OBJS = aStart.cpp
OBFILES = aStart.o

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = 

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

leaderBoid : leaderBoid.cpp
	$(CC) $(COMPILER_FLAGS) -c leaderBoid.cpp
	$(CC) leaderBoid.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

leaderMazeAStar : leaderMazeAStar.cpp
	$(CC) $(COMPILER_FLAGS) -c leaderMazeAStar.cpp
	$(CC) leaderMazeAStar.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

leaderPathFinding : leaderPathFinding.cpp
	$(CC) $(COMPILER_FLAGS) -c leaderPathFinding.cpp
	$(CC) leaderPathFinding.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

ell : ell.cpp
	$(CC) $(COMPILER_FLAGS) -c ell.cpp
	$(CC) ell.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

tea : tea.cpp
	$(CC) $(COMPILER_FLAGS) -O3 -g -c tea.cpp
	$(CC) tea.o $(COMPILER_FLAGS) -O3 $(LINKER_FLAGS) -o draw2
	./draw2

