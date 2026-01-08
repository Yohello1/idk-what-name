# List of object files (matching your tree structure)
OBJS = main.o graphics.o math.o settings.o struct.o spatial.o floaters.o

# Compiler and Executable name
CC = g++
OBJ_NAME = draw2

# Optimization and Compiler Flags
# -flto: Link Time Optimization (cross-file optimization)
# -funroll-loops: Specific optimization for math-heavy loops
CFLAGS = -O3 -march=native -flto -funroll-loops \
         -fassociative-math -fsimd-cost-model=unlimited \
         -mavx2 -mfma -mrecip -ffast-math -fopenmp -Wall

# Linker Flags
# Note: -flto must be present in the linker stage as well
LDFLAGS = -lSDL2 -lSDL2_image `sdl2-config --cflags --libs` -lpthread -fopenmp -flto

# Final Linker Step
# This joins the object files into the final executable
$(OBJ_NAME) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(OBJ_NAME)

# Individual Compilation Step (Pattern Rule)
# Compiles each .cpp file into a .o file independently
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Convenience command: build and run
run: $(OBJ_NAME)
	./$(OBJ_NAME)

# Remove build artifacts
clean:
	rm -f *.o $(OBJ_NAME)
