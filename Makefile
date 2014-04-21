toCompile = mminfinity_queue.o main.o

CXX = g++

CXXFLAGS = -g -Wall -std=c++0x -O3 -fopenmp#-fPIC

all: mminfQueue

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

mminfQueue: $(toCompile)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) *.o 
