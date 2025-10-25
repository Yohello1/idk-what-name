#OBJS specifies which files to compile as part of the project
OBJS = birds.c

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -g `sdl2-config --cflags --libs` -lpthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = draw2

#This is the target that compiles our executable
testDisp : testDisp.cpp
	icpx testDisp.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2


agentRocket : agentRocket.cpp
	icpx -O3 -g agentRocket.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2

agentFollow : agentFollow.cpp
	icpx -std=c++20 -fsycl -O3 -g agentFollow.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o draw2
	./draw2
