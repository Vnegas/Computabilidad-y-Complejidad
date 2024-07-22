CC  := gcc
CXX := g++
CFLAGS := -g -c -Wall
CXXFLAGS := -g -c -Wall
LDFLAGS := -g
DEPS := sudoku.hpp main.cpp solver.hpp geneticAlg.hpp
TARGETS := sudoku

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

%: %.o
	$(CXX) $^ $(LDFLAGS) -o $@

sudoku: sudoku.o main.o solver.o geneticAlg.o
	$(CXX) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out sudoku
