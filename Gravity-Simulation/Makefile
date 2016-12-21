CXXFLAGS = -std=c++14 -Wall -Wextra

all: Universe Body Interface main
	$(CXX) $(CXXFLAGS) Universe.o Body.o Interface.o main.o -lsfml-graphics -lsfml-window -lsfml-system -o gravity

debug: CXXFLAGS += -g
debug: all

release: CXXFLAGS += -O3
release: all

exec: all
	./gravity

clean:
	rm *.o gravity

Universe: Universe.cpp
	$(CXX) $(CXXFLAGS) -c Universe.cpp -o Universe.o

Body: Body.cpp
	$(CXX) $(CXXFLAGS) -c Body.cpp -o Body.o

Interface: Interface.cpp
	$(CXX) $(CXXFLAGS) -c Interface.cpp -o Interface.o

main: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o
