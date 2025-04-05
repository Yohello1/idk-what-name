all: invert gaussianBlur gaussian

invert: invert.cpp
	g++ $(shell pkg-config --cflags --libs opencv4) invert.cpp -o invert.run

gaussianBlur: gaussianBlur.cpp
	g++ $(shell pkg-config --cflags --libs opencv4) gaussian.cpp -o gaussianBlur.run

gaussian: gaussian.cpp
	g++ $(shell pkg-config --cflags --libs opencv4) -g gaussian.cpp -o gaussian.run



clean:
	rm -rf *.o *.run
