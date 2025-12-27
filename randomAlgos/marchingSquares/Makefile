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


grid : grid.cpp
	g++ grid.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2


bias : bias.cpp
	g++ bias.cpp -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2


moving : moving.cpp
	g++ moving.cpp -O2 $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

quad : quad.cpp
	g++ quad.cpp -g -O2 $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2


spatial : spatial.cpp
	g++ spatial.cpp -g -O2 $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2


area : area.cpp
	g++ area.cpp -O3 -fassociative-math -fsimd-cost-model=unlimited -mavx2 -mfma -mrecip -ffast-math -omp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2
