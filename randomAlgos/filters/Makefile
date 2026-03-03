all: invert gaussianBlur gaussian

invert: invert.cpp
	g++ $(shell pkg-config --cflags --libs opencv4) invert.cpp -o invert.run

gaussianBlur: gaussianBlur.cpp
	g++ $(shell pkg-config --cflags --libs opencv4) gaussianBlur.cpp -o gaussianBlur.run

gaussian: gaussian.cpp
	g++ -O3 -fopenmp $(shell pkg-config --cflags --libs opencv4) -g gaussian.cpp -o gaussian.run

diffGaussian: gaussian.cpp
	acpp -O3 -g  $(shell pkg-config --cflags --libs opencv4) differenceOfGaussians.cpp -o differenceOfGaussians.run
xdog:
	acpp -O3 -g  $(shell pkg-config --cflags --libs opencv4) extendedDOG.cpp -o xdog.run

diffColours: gaussian.cpp
	acpp -g -O0 -lm -lsycl $(shell pkg-config --cflags --libs opencv4) extendedDOG.cpp -o extendedDOG.run


clean:
	rm -rf *.o *.run
