CXX = g++
CXXFLAGS = -std=c++11 -Wall

main: main.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp

run: main
	./main

.PHONY: clean
clean:
	rm -f main